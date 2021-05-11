/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Gr8_AdditiveSynthAudioProcessor::Gr8_AdditiveSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

Gr8_AdditiveSynthAudioProcessor::~Gr8_AdditiveSynthAudioProcessor()
{
}

//==============================================================================
const juce::String Gr8_AdditiveSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Gr8_AdditiveSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Gr8_AdditiveSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Gr8_AdditiveSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Gr8_AdditiveSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Gr8_AdditiveSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Gr8_AdditiveSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Gr8_AdditiveSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Gr8_AdditiveSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void Gr8_AdditiveSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Gr8_AdditiveSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
     //********************************************************************************************//
    //3) Initialize the field of the structure FMData data

    for (int i = 0; i < TOT_VOICES; i++) {
        voices[i] = SynthVoice();
    }

    for (int i = 0; i < TOT_HARMONICS; i++) {
        oscFreqRatio[i] = 1.0;
        oscGains[i] = 0.0;
    }

    oscGains[0] = 1.0;
    masterGain = 0.5;

    initWaveShape();

    SynthVoice voice = voices[firstFreeVoice];
    voice.setFreq(440.0);
    voice.setPhase(INITIAL_PHASE);
    voice.activate();
    //********************************************************************************************//
}

void Gr8_AdditiveSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Gr8_AdditiveSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//Returns the lowest index to a free voice
void Gr8_AdditiveSynthAudioProcessor::updateFirstFreeVoice(int index) {
    if (numCurrentlyPlaying < TOT_VOICES) {
        while (index < TOT_VOICES && voices[index].isActive()) {
            index++;
        }
    }
}

//Returns the highest index to an active voice
void Gr8_AdditiveSynthAudioProcessor::updateLastActiveVoice(int index) {
    if (numCurrentlyPlaying > 0) {
        while (index >= 0 && !voices[index].isActive()) {
            index--;
        }
        lastActiveVoice = index;
    }
    else {
        lastActiveVoice = -1;
    }
}

//Returns the index to the voice playing the input frequency (-1 if none)
int Gr8_AdditiveSynthAudioProcessor::getVoiceIndex(float freq) {
    for (int i = 0; i < TOT_VOICES; i++) {
        if (voices[i].getFreq() == freq) {
            return i;
        }
    }

    return -1;
}

void Gr8_AdditiveSynthAudioProcessor::initWaveShape()
{
    for (int i = 0; i < SAMPLE_RATE; i++) {
        waveShape[i] = (float)sin((double) (2*M_PI * i / SAMPLE_RATE));
    }
}

void Gr8_AdditiveSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    
    //********************************************************************************************//
    // 4) implement the actual synthesis: first retrieve the note value from the NoteOn messages
    juce::MidiMessage m;
    int time;
    int index;
    SynthVoice voice;
    
    for (juce::MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);) {

        if (m.isNoteOn()) {

            if (numCurrentlyPlaying < TOT_VOICES) {
                voice = voices[firstFreeVoice];
                voice.setFreq(m.getMidiNoteInHertz(m.getNoteNumber()));
                voice.setPhase(INITIAL_PHASE);
                numCurrentlyPlaying++;

                if (firstFreeVoice > lastActiveVoice) {
                    lastActiveVoice = firstFreeVoice;
                }

                updateFirstFreeVoice(firstFreeVoice);
                voice.activate();
            }
            
        }
        else if (m.isNoteOff())
        {
            index = getVoiceIndex(m.getMidiNoteInHertz(m.getNoteNumber()));
            voice = voices[index];
            voice.setState(ADSRState::RELEASE);
            voice.deactivate();
            numCurrentlyPlaying--;

            if (index < firstFreeVoice) {
                firstFreeVoice = index;
            }
            if (index == lastActiveVoice) {
                updateLastActiveVoice(lastActiveVoice);
            }
        }
        else if (m.isAftertouch())
        {
        }
        else if (m.isPitchWheel())
        {
        }
    }
    
    // one we have the carrier frequency we can simply apply the formula (remember the Oscillator example)
    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);
    int numSamples = buffer.getNumSamples();
    float output;
    
    for (int i = 0; i < numSamples; ++i)
    {   
        output = 0.0;

        for (int j = 0; j < TOT_VOICES; j++) {
            voice = voices[j];
            if (voice.isActive()) {
                output += voice.computeCurrentOutputValue(oscGains, oscFreqRatio, waveShape);
            }
        }
        
        output = output * masterGain;
        /*
        channelDataL[i] = amp * (float) sin ((double) phase + mod);
        channelDataR[i] = amp * (float) sin ((double) phase + mod);
        */

        channelDataL[i] = output;
        channelDataR[i] = output;
        
    }
    //********************************************************************************************//
}

void Gr8_AdditiveSynthAudioProcessor::setMasterGain(float val) {
    masterGain = val;
}

void Gr8_AdditiveSynthAudioProcessor::setOscGain(int index, float val) {
    oscGains[index] = val;
}

void Gr8_AdditiveSynthAudioProcessor::setOscFreqRatio(int index, float val) {
    oscFreqRatio[index] = val;
}

//==============================================================================
bool Gr8_AdditiveSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Gr8_AdditiveSynthAudioProcessor::createEditor()
{
    return new Gr8_AdditiveSynthAudioProcessorEditor (*this);
}

//==============================================================================
void Gr8_AdditiveSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Gr8_AdditiveSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Gr8_AdditiveSynthAudioProcessor();
}

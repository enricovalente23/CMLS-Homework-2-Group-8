/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AddSynthAudioProcessor::AddSynthAudioProcessor()
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

AddSynthAudioProcessor::~AddSynthAudioProcessor()
{
}

//==============================================================================
const juce::String AddSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AddSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AddSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AddSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AddSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AddSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AddSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AddSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AddSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void AddSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AddSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
     //********************************************************************************************//
    //3) Initialize the field of the structure FMData data
    amp = 1.0; 
    phase = 2.0;

    for (int i = 0; i < TOT_VOICES; i++) {
        car_freq[i] = 0.0;
        activeVoices[i] = false;
    }

    for (int i = 0; i < TOT_HARMONICS; i++) {
        oscFreqRatio[i] = 1.0;
        oscGains[i] = 0.0;
    }
    
    mod_freq = 0.0;
    mod_phase = 1.0;
    mod_index = 0.0;
    //********************************************************************************************//

}

void AddSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AddSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
void AddSynthAudioProcessor::updateFirstFreeVoice(int index) {
    if (numCurrentlyPlaying < TOT_VOICES) {
        while (index < TOT_VOICES && activeVoices[index]) {
            index++;
        }
    }
}

//Returns the highest index to an active voice
void AddSynthAudioProcessor::updateLastActiveVoice(int index) {
    if (numCurrentlyPlaying > 0) {
        while (index >= 0 && !activeVoices[index]) {
            index--;
        }
        lastActiveVoice = index;
    }
    else {
        lastActiveVoice = -1;
    }
}

//Returns the index to the voice playing the input frequency (-1 if none)
int AddSynthAudioProcessor::getVoiceIndex(float freq) {
    for (int i = 0; i < TOT_VOICES; i++) {
        if (car_freq[i] == freq) {
            return i;
        }
    }

    return -1;
}

float AddSynthAudioProcessor::computeVoiceValue(int index) {
    float output = 0.0;
    for (int i = 0; i < TOT_HARMONICS; i++) {
        output+= amp * (float)sin((double)phase);

    }
    phase += (float)(M_PI * 2. * (((double)car_freq[index] / (double)SAMPLE_RATE)));
    if (phase >= M_PI * 2.) phase -= M_PI * 2.;
}

void AddSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    // 4) implement the actual FM synthesis: first retrieve the note value from the NoteOn messages
    float mod;
    juce::MidiMessage m;
    int time;
    int index;
    
    for (juce::MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);) {

        if (m.isNoteOn()) {

            if (numCurrentlyPlaying < TOT_VOICES) {
                amp = 0.1;
                car_freq[firstFreeVoice] = m.getMidiNoteInHertz(m.getNoteNumber());

                if (firstFreeVoice > lastActiveVoice) {
                    lastActiveVoice = firstFreeVoice;
                }

                oscGains[firstFreeVoice] = 0.1;
                activeVoices[firstFreeVoice] = true;
                numCurrentlyPlaying++;
                updateFirstFreeVoice(firstFreeVoice);
            }
            
        }
        else if (m.isNoteOff())
        {
            amp = 0;
            index = getVoiceIndex(m.getMidiNoteInHertz(m.getNoteNumber()));
            activeVoices[index] = false;
            oscGains[index] = 0;
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
        mod =  mod_index * (float) sin((double) mod_phase);

        output = 0.0;

        for (int j = 0; j < TOT_VOICES; j++) {
            if (activeVoices[i]) {
                output += computeVoiceValue(i);
            }
        }
        
        channelDataL[i] = amp * (float) sin ((double) phase + mod);
        channelDataR[i] = amp * (float) sin ((double) phase + mod);
        
        phase +=  (float) ( M_PI * 2. *( ((double) car_freq[firstFreeVoice]  / (double) SAMPLE_RATE)));
        if( phase >= M_PI * 2. ) phase -= M_PI * 2.;
        
        mod_phase += (float) ( M_PI * 2. * ((double) mod_freq / (double) SAMPLE_RATE) );
        if( mod_phase >= M_PI * 2. ) mod_phase -= M_PI * 2.;    
        
    }
    //********************************************************************************************//

}

void AddSynthAudioProcessor::setModFreq(float val) {
    mod_freq = val;
}

void AddSynthAudioProcessor::setModIndex(float val) {
    mod_index = val;
}

void AddSynthAudioProcessor::setMasterGain(float val) {
    masterGain = val;
}


//==============================================================================
bool AddSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AddSynthAudioProcessor::createEditor()
{
    return new AddSynthAudioProcessorEditor (*this);
}

//==============================================================================
void AddSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AddSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AddSynthAudioProcessor();
}

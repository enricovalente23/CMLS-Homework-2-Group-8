/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
uSynthAudioProcessor::uSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

uSynthAudioProcessor::~uSynthAudioProcessor()
{
}

//==============================================================================
const juce::String uSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool uSynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool uSynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool uSynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double uSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int uSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int uSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void uSynthAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String uSynthAudioProcessor::getProgramName(int index)
{
    return {};
}

void uSynthAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void uSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
     //********************************************************************************************//
    //3) Initialize the field of the structure FMData data
    amp = 0.1;
    amp1Parameter = 0.1;
    amp2Parameter = 0.1;
    amp3Parameter = 0.1;

    phase = 2.0;
    phase1 = 2.0;
    phase2 = 2.0;
    phase3 = 2.0;

    car_freq = 0.0;
    car_freq1 = 0.0;
    car_freq2 = 0.0;
    car_freq3 = 0.0;


    //Jit
    numCurrentlyPlaying = 0;
    firstFreeVoice = 0;
    lastActiveVoice = NO_ACTIVE;

    for (int i = 0; i < TOT_HARMONICS; i++) {
        freqRatio[i] = (float) (i+1);
        amps[i] = 0.5;
    }

    for (int i = 0; i < TOT_VOICES; i++) {
        SynthVoice voice;
        voice.initialize((float)car_freq);
        voice.deactivate();
        voices[i] = voice;
    }

    initWaveShape();
    //********************************************************************************************//

}

void uSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool uSynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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

void uSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        // ..do something to the data...
    }

    //********************************************************************************************//
    // 4) implement the actual FM synthesis: first retrieve the note value from the NoteOn messages
    juce::MidiMessage m;
    int time;
    float flagMidi;
    adsr.setSampleRate(static_cast<double> SAMPLE_RATE);

    //Jit
    int index;
    SynthVoice voice;
    //End Jit

    for (juce::MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
    {
        if (m.isNoteOn())
        {
            //Umut
            amp = 0.1;
            flagMidi = 1;
            AttackParameter = 1;
            DecayParameter = 1;
            SustainParameter = 1;
            ReleaseParameter = 1;

            adsr.setParameters({
                AttackParameter,
                DecayParameter,
                SustainParameter,
                ReleaseParameter
                });
            adsr.noteOn();

            //Don't delete this
            car_freq = m.getMidiNoteInHertz(m.getNoteNumber());

            //Jit
            if (numCurrentlyPlaying < TOT_VOICES) {
                voice = voices[firstFreeVoice];
                voice.initialize((float)car_freq);
                numCurrentlyPlaying++;

                if (firstFreeVoice > lastActiveVoice) {
                    lastActiveVoice = firstFreeVoice;
                }

                updateFirstFreeVoice(firstFreeVoice);
            }
            //End Jit
        }
        else if (m.isNoteOff())
        {
            //Umut
            amp = 0;
            adsr.noteOff();

            //Jit
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
            //End Jit
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
    float total = 0;

    for (int i = 0; i < numSamples; ++i)
    {
        car_freq1 = freq1Parameter * car_freq;
        car_freq2 = freq2Parameter * car_freq;
        car_freq3 = freq3Parameter * car_freq;

        total = (amp * (float)sin((double)phase) + amp1Parameter * (float)sin((double)phase1) + amp2Parameter * (float)sin((double)phase2) + amp3Parameter * (float)sin((double)phase3)) * MasterParameter * adsr.getNextSample();

        channelDataL[i] = total;
        channelDataR[i] = total;

        phase += (float)(M_PI * 2. * (((double)car_freq / (double)SAMPLE_RATE)));
        phase1 += (float)(M_PI * 2. * (((double)car_freq1 / (double)SAMPLE_RATE)));
        phase2 += (float)(M_PI * 2. * (((double)car_freq2 / (double)SAMPLE_RATE)));
        phase3 += (float)(M_PI * 2. * (((double)car_freq3 / (double)SAMPLE_RATE)));

        if (phase >= M_PI * 2.) phase -= M_PI * 2.;
        if (phase1 >= M_PI * 2.) phase1 -= M_PI * 2.;
        if (phase2 >= M_PI * 2.) phase2 -= M_PI * 2.;
        if (phase3 >= M_PI * 2.) phase3 -= M_PI * 2.;

        //Jit
        //float output = 0.0;
        //for (int j = 0; j < TOT_VOICES; j++) {
        //    if (voices[j].isActive()) {
        //        output += voices[j].computeCurrentOutputValue(amps, freqRatio, waveShape);
        //    }
        //}

        //output *= masterGain;//Using amp[0] until masterGain is implemented

        //if (output > 1.0) {
        //    output = 1.0;
        //} else if (output < -1.0) {
        //    output = -1.0;        
        //}

        //channelDataL[i] = output;
        //channelDataR[i] = output;
        //End Jit
    }
    //********************************************************************************************//

}

void uSynthAudioProcessor::setFreq1(float val)
{
    freq1Parameter = val;
}

void uSynthAudioProcessor::setFreq2(float val)
{
    freq2Parameter = val;
}

void uSynthAudioProcessor::setFreq3(float val)
{
    freq3Parameter = val;
}

void uSynthAudioProcessor::setMaster(float val)
{
    MasterParameter = val;
}

void uSynthAudioProcessor::setAmp0(float val)
{
    amp0Parameter = val;
}

void uSynthAudioProcessor::setAmp1(float val)
{
    amp1Parameter = val;
}

void uSynthAudioProcessor::setAmp2(float val)
{
    amp2Parameter = val;
}

void uSynthAudioProcessor::setAmp3(float val)
{
    amp3Parameter = val;
}
void uSynthAudioProcessor::setAttack(float val)
{
    AttackParameter = val;
}

void uSynthAudioProcessor::setDecay(float val)
{
    DecayParameter = val;
}

void uSynthAudioProcessor::setSustain(float val)
{
    SustainParameter = val;
}

void uSynthAudioProcessor::setRelease(float val)
{
    ReleaseParameter = val;
}

//Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit 
//Returns the lowest index to a free voice

void uSynthAudioProcessor::setMasterGain(float value) {
    masterGain = value;
}
void uSynthAudioProcessor::updateFirstFreeVoice(int index) {
    if (numCurrentlyPlaying < TOT_VOICES) {
        while (index < TOT_VOICES && voices[index].isActive()) {
            index++;
        }
        firstFreeVoice = index;
    }
}

//Returns the highest index to an active voice
void uSynthAudioProcessor::updateLastActiveVoice(int index) {
    if (numCurrentlyPlaying > 0) {
        while (index >= 0 && !voices[index].isActive()) {
            index--;
        }
        lastActiveVoice = index;
    }
    else {
        lastActiveVoice = NO_ACTIVE;
    }
}

//Returns the index to the voice playing the input frequency (-1 if none)
int uSynthAudioProcessor::getVoiceIndex(float freq) {
    for (int i = 0; i < TOT_VOICES; i++) {
        if (voices[i].getFreq() == freq) {
            return i;
        }
    }

    return -1;
}

void uSynthAudioProcessor::initWaveShape()
{
    for (int i = 0; i < SAMPLE_RATE; i++) {
        waveShape[i] = (float)sin((double)(2 * M_PI * i / SAMPLE_RATE));
    }
}

void uSynthAudioProcessor::setAmps(int index, float value) {
    amps[index] = value;
}
void uSynthAudioProcessor::setFreqRatios(int index, float value) {
    freqRatio[index] = value;
}

//==============================================================================
bool uSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* uSynthAudioProcessor::createEditor()
{
    return new uSynthAudioProcessorEditor(*this);
}

//==============================================================================
void uSynthAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void uSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new uSynthAudioProcessor();
}

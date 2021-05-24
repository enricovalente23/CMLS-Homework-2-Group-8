/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
//==============================================================================
AdditiveSynthAudioProcessor::AdditiveSynthAudioProcessor()
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

AdditiveSynthAudioProcessor::~AdditiveSynthAudioProcessor()
{
}

//==============================================================================
const juce::String AdditiveSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AdditiveSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AdditiveSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AdditiveSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AdditiveSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AdditiveSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AdditiveSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AdditiveSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AdditiveSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void AdditiveSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AdditiveSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
     //********************************************************************************************//
    //3) Initialize the field of the structure Additive synthesis data
    
    //Initilize the level for each oscillator:
    amp0Parameter = 0.1;
    amp1Parameter = 0.1;
    amp2Parameter = 0.1;
    amp3Parameter = 0.1;
    
    //Initilize the phase component for each oscillator
    phase = 0.0;
    phase1 = 0.0;
    phase2 = 0.0;
    phase3 = 0.0;
    
    //Initilize the freq component for each oscillator
    car_freq = 0.0;
    car_freq1= 0.0;
    car_freq2= 0.0;
    car_freq3= 0.0;
    tremolo_amp = 0.5; //tremolo_level; //strenght of the tremolo from 0 to 1
    tremolo_vel = 5;//how fast the tremolo should be the higher the faster
    panning_parameter=0.2;  //start at the middle of L and R channels.
    //********************************************************************************************//

}

void AdditiveSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AdditiveSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AdditiveSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //********************************************************************************************//
    // 4) implement the actual FM synthesis: first retrieve the note value from the NoteOn messages
    juce::MidiMessage m;
    int time;
    adsr.setSampleRate(static_cast<double> SAMPLE_RATE);
    float tremolo; //container for tremolo effect array

    for (juce::MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);)
    {
        if (m.isNoteOn())
        {
            //When a note is played, initilize the adsr parameters
            adsr.setParameters ({
                AttackParameter,
                DecayParameter,
                SustainParameter,
                ReleaseParameter
            });
            
            adsr.noteOn();
            //Also get the note the midi is playing
            car_freq = m.getMidiNoteInHertz(m.getNoteNumber());
        }
        else if (m.isNoteOff())
        {
            //When the note is not playing anymore, turn off the adsr by setting it release to go off
            adsr.noteOff();
        }
        else if (m.isAftertouch())
        {
        }
        else if (m.isPitchWheel())
        {
            tremolo_amp = (float) (m.getPitchWheelValue()); //Dividing by the conversion offset from 14bit to float
            tremolo_amp = tremolo_amp/16383;
        }
    }
    //get write pointers for L and R channels
    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);
    int numSamples = buffer.getNumSamples();
    float total=0;
    for (int i = 0; i < numSamples; ++i)
    {   
        car_freq1 =  freq1Parameter * car_freq;
        car_freq2 =  freq2Parameter * car_freq;
        car_freq3 =  freq3Parameter * car_freq;
        
        tremolo = (1 + tremolo_amp*sin(2* (float) M_PI*i*(float)((float)tremolo_vel/(float)SAMPLE_RATE)));
        
        total = (amp0Parameter * (float) sin ((double) phase) + amp1Parameter * (float) sin ((double) phase1) + amp2Parameter * (float) sin ((double) phase2) + amp3Parameter * (float) sin ((double) phase3)) * MasterParameter * adsr.getNextSample() * tremolo;
        
        channelDataL[i] = total * (1-panning_parameter);
        channelDataR[i] = total * panning_parameter;
        
        // Update the phases in each buffer sample.
        phase +=  (float) ( M_PI * 2. *( ((double) car_freq  / (double) SAMPLE_RATE)));
        phase1 +=  (float) ( M_PI * 2. *( ((double) car_freq1  / (double) SAMPLE_RATE)));
        phase2 +=  (float) ( M_PI * 2. *( ((double) car_freq2  / (double) SAMPLE_RATE)));
        phase3 +=  (float) ( M_PI * 2. *( ((double) car_freq3  / (double) SAMPLE_RATE)));
        
        //Resetting the phases when they go above 2*Pi
        if( phase >= M_PI * 2. ) phase -= M_PI * 2.;
        if( phase1 >= M_PI * 2. ) phase1 -= M_PI * 2.;
        if( phase2 >= M_PI * 2. ) phase2 -= M_PI * 2.;
        if( phase3 >= M_PI * 2. ) phase3 -= M_PI * 2.;
    }
}


    //Set each parameter coming from the input knobs/bars:
 //********************************************************************************************//
void AdditiveSynthAudioProcessor::setFreq1(float val)
{
    freq1Parameter = val;
}

void AdditiveSynthAudioProcessor::setFreq2(float val)
{
    freq2Parameter = val;
}

void AdditiveSynthAudioProcessor::setFreq3(float val)
{
    freq3Parameter = val;
}

void AdditiveSynthAudioProcessor::setMaster(float val)
{
    MasterParameter = val * val;
}

void AdditiveSynthAudioProcessor::setAmp0(float val)
{
    amp0Parameter = val * val;
}

void AdditiveSynthAudioProcessor::setAmp1(float val)
{
    amp1Parameter = val * val;
}

void AdditiveSynthAudioProcessor::setAmp2(float val)
{
    amp2Parameter = val * val;
}

void AdditiveSynthAudioProcessor::setAmp3(float val)
{
    amp3Parameter = val * val;
}
void AdditiveSynthAudioProcessor::setAttack(float val)
{
    AttackParameter = val;
}

void AdditiveSynthAudioProcessor::setDecay(float val)
{
    DecayParameter = val;
}

void AdditiveSynthAudioProcessor::setSustain(float val)
{
    SustainParameter = val;
}

void AdditiveSynthAudioProcessor::setRelease(float val)
{
    ReleaseParameter = val;
}

void AdditiveSynthAudioProcessor::setTremoloVel(float val)
{
    tremolo_vel = val;
}

void AdditiveSynthAudioProcessor::setPanning(float val)
{
    panning_parameter= val;
}

//==============================================================================
bool AdditiveSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AdditiveSynthAudioProcessor::createEditor()
{
    return new AdditiveSynthAudioProcessorEditor (*this);
}

//==============================================================================
void AdditiveSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AdditiveSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AdditiveSynthAudioProcessor();
}

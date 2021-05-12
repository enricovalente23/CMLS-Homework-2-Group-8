/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//********************************************************************************************//
// 1) define some global parameters

#define SAMPLE_RATE   (44100)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

//********************************************************************************************//
//==============================================================================
/**
*/
class AdditiveSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AdditiveSynthAudioProcessor();
    ~AdditiveSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    void setFreq1(float val);
    void setFreq2(float val);
    void setFreq3(float val);
    void setMaster(float val);
    void setAmp0(float val);
    void setAmp1(float val);
    void setAmp2(float val);
    void setAmp3(float val);
    void setAttack(float val);
    void setDecay(float val);
    void setSustain(float val);
    void setRelease(float val);
    void setTremoloVel(float val);
    void setPanning(float val);
    
private:
    //==============================================================================
    //********************************************************************************************//
    // 2) add to the Processor class the variables we need for the Additive synth
    float mod_phase;
    float mod_freq;
    int mod_index;
    
    float phase;
    float phase1;
    float phase2;
    float phase3;
    
    float amp;
    float MasterParameter;
    float amp0Parameter;
    float amp1Parameter;
    float amp2Parameter;
    float amp3Parameter;

    float car_freq;    
    float car_freq1;
    float car_freq2;
    float car_freq3;
    
    float freq1Parameter; //multiplication factors for each additive part
    float freq2Parameter;
    float freq3Parameter;
    float tremolo_param;
    
    float AttackParameter;  //input parameters from the gui
    float DecayParameter;
    float SustainParameter;
    float ReleaseParameter;
    float tremolo_amp;
    float tremolo_vel;
    float panning_parameter;
    
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
//float ADSR::Parameters::attack = 0.1f //to change a parameter

    //********************************************************************************************//
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdditiveSynthAudioProcessor)
};

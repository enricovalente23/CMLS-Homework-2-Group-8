/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//********************************************************************************************//
// 1) define some global parameters

#define SAMPLE_RATE   44100

#ifndef M_PI
#define M_PI  3.14159265
#endif
#define TOT_HARMONICS 4
#define TOT_VOICES 16


//********************************************************************************************//
//==============================================================================
/**
*/
class AddSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AddSynthAudioProcessor();
    ~AddSynthAudioProcessor() override;

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
    
    void setCarrierFreq(float val);
    void setMasterGain(float val);
    void setOscGain(int index, float val);
    void setOscFreqRatio(int index, float val);
    void setModFreq(float val);
    void setModIndex(float val);
    void updateFirstFreeVoice(int index);
    void updateLastActiveVoice(int index);
    int getVoiceIndex(float freq);
    float computeVoiceValue(int index);

private:
    //==============================================================================
    //********************************************************************************************//
    // 2) add to the Processor class the variables we need for the FM synth
    float mod_phase;
    float mod_freq;
    int mod_index;
    
    float phase;
    float amp;
    
    int attack;
    int decay;
    int sustain;
    int release;

    float masterGain;
    float oscGains[TOT_HARMONICS];
    float oscFreqRatio[TOT_HARMONICS];

    bool activeVoices[TOT_VOICES];
    float car_freq[TOT_VOICES];
    float phases[TOT_VOICES];

    int numCurrentlyPlaying = 0;
    int firstFreeVoice = 0;
    int lastActiveVoice = -1;

    //********************************************************************************************//
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AddSynthAudioProcessor)
};
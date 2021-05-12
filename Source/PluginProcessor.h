/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"
//********************************************************************************************//
// 1) define some global parameters

#define SAMPLE_RATE   (44100)
#define TOT_VOICES 16
#define NO_ACTIVE -1

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

//********************************************************************************************//
//==============================================================================
/**
*/
class uSynthAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    uSynthAudioProcessor();
    ~uSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //OUR CODE

    /*void setFreq1(float val);
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
    void setRelease(float val);*/


    //Jit
    void setMasterGain(float value);
    void setAmps(int index, float value);
    void setFreqRatios(int index, float value);
    void updateFirstFreeVoice(int index);
    void updateLastActiveVoice(int index);
    int getVoiceIndex(float freq);
    void initWaveShape();
    void setAdsrParam(int index, float value);
    //End Jit

private:
    //==============================================================================
    //********************************************************************************************//
    // 2) add to the Processor class the variables we need for the FM synth
    //float mod_phase;
    //float mod_freq;
    //int mod_index;

    //float phase;
    //float phase1;
    //float phase2;
    //float phase3;

    //float amp;
    //float MasterParameter;
    //float amp0Parameter;
    //float amp1Parameter;
    //float amp2Parameter;
    //float amp3Parameter;

    //float car_freq;
    //float car_freq1;
    //float car_freq2;
    //float car_freq3;

    //float freq1Parameter; //just as an example //multiplication factors for each additive part
    //float freq2Parameter;  //just as an example
    //float freq3Parameter; //just as an example

    //float AttackParameter;
    //float DecayParameter;
    //float SustainParameter;
    //float ReleaseParameter;
    //juce::ADSR adsr;
    //juce::ADSR::Parameters adsrParams;

    //Jit
    SynthVoice voices[TOT_VOICES];
    float masterGain;
    float amps[TOT_HARMONICS];
    float freqRatio[TOT_HARMONICS];
    float waveShape[SAMPLE_RATE];

    float adsrParam[DIM_ADSR];

    int numCurrentlyPlaying;
    int firstFreeVoice;
    int lastActiveVoice;

    //********************************************************************************************//

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(uSynthAudioProcessor)
};

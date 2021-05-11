/*
  ==============================================================================

    SynthVoice.h
    Created: 11 May 2021 6:54:41pm
    Author:  jiten

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define SAMPLE_RATE   44100
#define INITIAL_FREQ 0.0
#define INITIAL_PHASE 0
#define INITIAL_TIME 0.0
#define TOT_HARMONICS 4

#ifndef M_PI
#define M_PI  3.14159265
#endif

#pragma once

enum class ADSRState {
    ATTACK = 0,
    DECAY,
    SUSTAIN,
    RELEASE
};

class SynthVoice {
public:
    SynthVoice();

    bool isActive();
    ADSRState getState();
    float getPhase();
    float getFreq();
    float getActiveTime();

    void setFreq(float value);
    void activate();
    void deactivate();
    void setPhase(int value);
    void setState(ADSRState newState);

    float computeCurrentOutputValue(float* oscGains, float* oscFreqRatio, float* waveShape);
    int adjustPhase(int value);
    void updatePhase();
    float computeHarmonicOutput(float gain, float freqRatio, float* waveShape);
    void initialize(float freq);

private:

    bool active;
    ADSRState state;
    int phase;
    float freq;
    float activeTime;
};
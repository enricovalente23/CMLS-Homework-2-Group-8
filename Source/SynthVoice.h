/*
  ==============================================================================

    SynthVoice.h
    Created: 10 May 2021 7:59:15pm
    Author:  jiten

  ==============================================================================
*/

#include <JuceHeader.h>

#define SAMPLE_RATE   44100
#define INITIAL_FREQ 0.0
#define INITIAL_PHASE 0.0
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
    void setPhase(float value);
    void setState(ADSRState newState);

    float computeCurrentOutputValue(float* oscGains, float *oscFreqRatio);
    float adjustPhase(float value);
    void updatePhase();
    float computeHarmonicOutput(float gain, float freqRatio);

private:

    bool active;
    ADSRState state;
    float phase;
    float freq;
    float activeTime;
};
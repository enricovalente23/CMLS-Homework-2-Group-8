/*
  ==============================================================================

    SynthVoice.cpp
    Created: 11 May 2021 6:54:41pm
    Author:  jiten

  ==============================================================================
*/

#include "SynthVoice.h"

//CONSTRUCTOR
SynthVoice::SynthVoice() {
    active = false;
    freq = INITIAL_FREQ;
    phase = INITIAL_PHASE;
    activeTime = INITIAL_TIME;
    state = ADSRState::ATTACK;
    releaseGain = INITIAL_RELEASE_GAIN;
}

/*
    GETTERS
*/

bool SynthVoice::isActive() {
    return active;
}

ADSRState SynthVoice::getState() {
    return state;
}

float SynthVoice::getPhase() {
    return phase;
}

float SynthVoice::getFreq() {
    return freq;
}

float SynthVoice::getActiveTime() {
    return activeTime;
}

/*
    SETTERS
*/

void SynthVoice::setFreq(float value) {
    freq = value;
}

void SynthVoice::activate() {
    active = true;
    setState(ADSRState::ATTACK);
}

void SynthVoice::deactivate() {
    active = false;
}

void SynthVoice::setPhase(int value) {
    phase = value;
}

void SynthVoice::setState(ADSRState newState) {
    state = newState;
}

/*
    FUNCTIONS
*/

void SynthVoice::initialize(float freq) {
    setFreq(freq);
    setPhase(INITIAL_PHASE);
    setState(ADSRState::ATTACK);
    activate();
}

float SynthVoice::computeCurrentOutputValue(float* oscGains, float* oscFreqRatio, float* waveShape, float *adsr) {
    float output = 0.0;

    for (int i = 0; i < TOT_HARMONICS; i++) {
        output = output + computeHarmonicOutput(oscGains[i], oscFreqRatio[i], waveShape);
    }

    updatePhase();

    return output * computeGain(adsr);
}

float SynthVoice::computeGain(float* adsr) {
    
    float gain = INITIAL_RELEASE_GAIN;
    float attackTime = adsr[0];
    float decayTime = adsr[1];
    float sustainGain = adsr[2];
    float releaseTime = adsr[3];

    switch (state) {

    case ADSRState::ATTACK:
        gain = (activeTime / SAMPLE_RATE) / attackTime;
        if (gain >= 1.0) {
            setState(ADSRState::DECAY);
            activeTime = INITIAL_TIME;
            gain = 1.0;
        } else {
            activeTime++;
        }
        releaseGain = gain;
        break;

    case ADSRState::DECAY:
        gain = 1 - ((activeTime / SAMPLE_RATE) / decayTime)*(1 - sustainGain);
        if (gain <= sustainGain) {
            setState(ADSRState::SUSTAIN);
        } else {
            gain = sustainGain;
            activeTime++;
        }
        releaseGain = gain;
        break;

    case ADSRState::SUSTAIN:
        gain = sustainGain;
        break;

    case ADSRState::RELEASE:
        gain = (1 - (activeTime / SAMPLE_RATE) / releaseTime)*releaseGain;
        if (gain <= INITIAL_RELEASE_GAIN) {
            deactivate();
        } else {
            activeTime++;
        }
        break;
    }

    releaseGain = gain;

    return gain;
}

int SynthVoice::adjustPhase(int value) {
    return value % SAMPLE_RATE;
}

void SynthVoice::updatePhase() {
    if (phase + 1 == SAMPLE_RATE) {
        phase = INITIAL_PHASE;
    }
    else {
        phase++;
    }
}

float SynthVoice::computeHarmonicOutput(float gain, float freqRatio, float* waveShape) {
    return gain * waveShape[adjustPhase((int)(phase * freq * freqRatio))];
}

void SynthVoice::release() {
    setState(ADSRState::RELEASE);
}
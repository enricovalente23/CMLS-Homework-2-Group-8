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
    activate();
}

float SynthVoice::computeCurrentOutputValue(float* oscGains, float* oscFreqRatio, float* waveShape) {
    float output = 0.0;

    for (int i = 0; i < TOT_HARMONICS; i++) {
        output = output + computeHarmonicOutput(oscGains[i], oscFreqRatio[i], waveShape);
    }

    updatePhase();

    return output;
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
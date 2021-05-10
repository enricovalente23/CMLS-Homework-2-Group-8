/*
  ==============================================================================

    SynthVoice.cpp
    Created: 10 May 2021 7:59:27pm
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
}

void SynthVoice::deactivate() {
    active = false;
}
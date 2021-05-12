/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class uSynthAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    uSynthAudioProcessorEditor(uSynthAudioProcessor&);
    ~uSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    uSynthAudioProcessor& audioProcessor;

    juce::Slider Master;
    juce::Label MasterLabel;

    juce::Slider modFreq1;
    juce::Label modFreq1Label;

    juce::Slider modFreq2;
    juce::Label modFreq2Label;

    juce::Slider modFreq3;
    juce::Label modFreq3Label;

    juce::Slider ampFreq0;
    juce::Label ampFreq0Label;

    juce::Slider ampFreq1;
    juce::Label ampFreq1Label;

    juce::Slider ampFreq2;
    juce::Label ampFreq2Label;

    juce::Slider ampFreq3;
    juce::Label ampFreq3Label;


    juce::Slider Attack;
    juce::Label AttackLabel;

    juce::Slider Decay;
    juce::Label DecayLabel;

    juce::Slider Sustain;
    juce::Label SustainLabel;

    juce::Slider Release;
    juce::Label ReleaseLabel;

    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(uSynthAudioProcessorEditor)
};

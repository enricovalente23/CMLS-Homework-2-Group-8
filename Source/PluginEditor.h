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
class Gr8_AdditiveSynthAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    Gr8_AdditiveSynthAudioProcessorEditor (Gr8_AdditiveSynthAudioProcessor&);
    ~Gr8_AdditiveSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Gr8_AdditiveSynthAudioProcessor& audioProcessor;

    juce::Slider masterGain;
    juce::Label masterGainLabel;

    juce::Slider gainOne;
    juce::Label gainLabelOne;

    juce::Slider gainTwo;
    juce::Label gainLabelTwo;

    juce::Slider gainThree;
    juce::Label gainLabelThree;

    juce::Slider freqOne;
    juce::Label freqLabelOne;

    juce::Slider freqTwo;
    juce::Label freqLabelTwo;

    juce::Slider freqThree;
    juce::Label freqLabelThree;

    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gr8_AdditiveSynthAudioProcessorEditor)
};

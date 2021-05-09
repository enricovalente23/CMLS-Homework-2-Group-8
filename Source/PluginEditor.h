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
class FMSynthAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    FMSynthAudioProcessorEditor (AddSynthAudioProcessor&);
    ~FMSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AddSynthAudioProcessor& audioProcessor;
    
    juce::Slider modFreq;
    juce::Label modFreqLabel;
    
    juce::Slider modIndex;
    juce::Label modIndexLabel;

    juce::Slider masterGain;
    juce::Label masterGainLabel;
    
    void sliderValueChanged( juce::Slider * slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthAudioProcessorEditor)
};

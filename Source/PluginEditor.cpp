/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AddSynthAudioProcessorEditor::AddSynthAudioProcessorEditor (AddSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    
    masterGain.setRange(0.0, 1.0, 0.005);
    masterGain.setSliderStyle(juce::Slider::LinearVertical);
    masterGain.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 100, 20);
    masterGain.addListener(this);
    masterGainLabel.setText("Master", juce::dontSendNotification);

    addAndMakeVisible(masterGain);
    addAndMakeVisible(masterGainLabel);
}

AddSynthAudioProcessorEditor::~AddSynthAudioProcessorEditor()
{
}

//==============================================================================
void AddSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AddSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    masterGainLabel.setBounds(10, 10, 100, 20);
    masterGain.setBounds(10, 30, 100, 100);

}


void AddSynthAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
   if (slider == &masterGain) {
       audioProcessor.setMasterGain(masterGain.getValue());
   }
}
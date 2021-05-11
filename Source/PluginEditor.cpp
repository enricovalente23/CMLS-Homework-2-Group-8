/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Gr8_AdditiveSynthAudioProcessorEditor::Gr8_AdditiveSynthAudioProcessorEditor (Gr8_AdditiveSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1000, 600);

    masterGain.setRange(0.0, 1.0, 0.005);
    masterGain.setSliderStyle(juce::Slider::Rotary);
    masterGain.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 100, 20);
    masterGainLabel.setText("Master", juce::dontSendNotification);
    masterGain.addListener(this);

    gainOne.setRange(0.0, 1.0, 0.005);
    gainOne.setSliderStyle(juce::Slider::LinearBarVertical);
    gainOne.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    gainLabelOne.setText("Osc2 Gain", juce::dontSendNotification);
    gainOne.addListener(this);

    gainTwo.setRange(0.0, 1.0, 0.005);
    gainTwo.setSliderStyle(juce::Slider::LinearBarVertical);
    gainTwo.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    gainLabelTwo.setText("Osc3 Gain", juce::dontSendNotification);
    gainTwo.addListener(this);

    gainThree.setRange(0.0, 1.0, 0.005);
    gainThree.setSliderStyle(juce::Slider::LinearBarVertical);
    gainThree.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    gainLabelThree.setText("Osc4 Gain", juce::dontSendNotification);
    gainThree.addListener(this);

    freqOne.setRange(0.0, 1.0, 0.005);
    freqOne.setSliderStyle(juce::Slider::Rotary);
    freqOne.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    freqLabelOne.setText("Osc2 Freq", juce::dontSendNotification);
    freqOne.addListener(this);

    freqTwo.setRange(0.0, 1.0, 0.005);
    freqTwo.setSliderStyle(juce::Slider::Rotary);
    freqTwo.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    freqLabelTwo.setText("Osc3 Freq", juce::dontSendNotification);
    freqTwo.addListener(this);

    freqThree.setRange(0.0, 1.0, 0.005);
    freqThree.setSliderStyle(juce::Slider::Rotary);
    freqThree.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    freqLabelThree.setText("Osc4 Freq", juce::dontSendNotification);
    freqThree.addListener(this);

    masterGain.setValue(1.0);
    gainOne.setValue(0.5);
    gainTwo.setValue(0.5);
    gainThree.setValue(0.5);
    freqOne.setValue(0.5);
    freqTwo.setValue(0.5);
    freqThree.setValue(0.5);

    addAndMakeVisible(masterGain);
    addAndMakeVisible(masterGainLabel);

    addAndMakeVisible(gainOne);
    addAndMakeVisible(gainLabelOne);
    addAndMakeVisible(gainTwo);
    addAndMakeVisible(gainLabelTwo);
    addAndMakeVisible(gainThree);
    addAndMakeVisible(gainLabelThree);

    addAndMakeVisible(freqOne);
    addAndMakeVisible(freqLabelOne);
    addAndMakeVisible(freqTwo);
    addAndMakeVisible(freqLabelTwo);
    addAndMakeVisible(freqThree);
    addAndMakeVisible(freqLabelThree);
}

Gr8_AdditiveSynthAudioProcessorEditor::~Gr8_AdditiveSynthAudioProcessorEditor()
{
}

//==============================================================================
void Gr8_AdditiveSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void Gr8_AdditiveSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int x;
    int y;

    masterGainLabel.setBounds(10, 10, 100, 50);
    masterGain.setBounds(10, 60, 100, 200);

    gainOne.setBounds(110, 10, 100, 200);
    gainLabelOne.setBounds(110, 210, 100, 100);

    gainTwo.setBounds(210, 10, 100, 200);
    gainLabelTwo.setBounds(210, 210, 100, 100);

    gainThree.setBounds(310, 10, 100, 200);
    gainLabelThree.setBounds(310, 210, 100, 100);
}

void Gr8_AdditiveSynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &masterGain) {
        audioProcessor.setMasterGain(masterGain.getValue());
    } else if (slider == &gainOne) {
        audioProcessor.setOscGain(1, gainOne.getValue());
    } else if (slider == &gainTwo) {
        audioProcessor.setOscGain(2, gainTwo.getValue());
    } else if (slider == &gainThree) {
        audioProcessor.setOscGain(3, gainThree.getValue());
    }
}
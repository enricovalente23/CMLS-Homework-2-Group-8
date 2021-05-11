/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
uSynthAudioProcessorEditor::uSynthAudioProcessorEditor(uSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(600, 400);

    modFreq1.setRange(0.25, 4.0, 0.05);
    modFreq1.setSliderStyle(juce::Slider::Rotary);
    modFreq1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    modFreq1.addListener(this);

    modFreq2.setRange(0.25, 4.0, 0.05);
    modFreq2.setSliderStyle(juce::Slider::Rotary);
    modFreq2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    modFreq2.addListener(this);

    modFreq3.setRange(0.25, 4.0, 0.05);
    modFreq3.setSliderStyle(juce::Slider::Rotary);
    modFreq3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    modFreq3.addListener(this);

    modFreq1Label.setText("1st Frequency Component", juce::dontSendNotification);
    modFreq2Label.setText("2nd Frequency Component", juce::dontSendNotification);
    modFreq3Label.setText("3rd Frequency Component", juce::dontSendNotification);

    ampFreq1.setRange(0.0, 1.0, 0.05);
    ampFreq1.setSliderStyle(juce::Slider::LinearVertical);
    ampFreq1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq1.addListener(this);

    ampFreq2.setRange(0.0, 1.0, 0.05);
    ampFreq2.setSliderStyle(juce::Slider::LinearVertical);
    ampFreq2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq2.addListener(this);

    ampFreq3.setRange(0.0, 1.0, 0.05);
    ampFreq3.setSliderStyle(juce::Slider::LinearVertical);
    ampFreq3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq3.addListener(this);

    ampFreq1Label.setText("1st Amp component", juce::dontSendNotification);
    ampFreq2Label.setText("2nd Amp component", juce::dontSendNotification);
    ampFreq3Label.setText("3rd Amp component", juce::dontSendNotification);

    addAndMakeVisible(ampFreq1);
    addAndMakeVisible(ampFreq1Label);
    addAndMakeVisible(ampFreq2);
    addAndMakeVisible(ampFreq2Label);
    addAndMakeVisible(ampFreq3);
    addAndMakeVisible(ampFreq3Label);

    addAndMakeVisible(modFreq1);
    addAndMakeVisible(modFreq1Label);
    addAndMakeVisible(modFreq2);
    addAndMakeVisible(modFreq2Label);
    addAndMakeVisible(modFreq3);
    addAndMakeVisible(modFreq3Label);
}

uSynthAudioProcessorEditor::~uSynthAudioProcessorEditor()
{
}

//==============================================================================
void uSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void uSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    modFreq1Label.setBounds(10, 10, 70, 20);
    modFreq1.setBounds(10, 40, 70, 70);

    modFreq2Label.setBounds(90, 10, 70, 20);
    modFreq2.setBounds(90, 40, 70, 70);
    modFreq3Label.setBounds(170, 10, 70, 20);
    modFreq3.setBounds(170, 40, 70, 70);

    ampFreq1Label.setBounds(10, 110, 20, 20);
    ampFreq1.setBounds(10, 140, 20, 70);

    ampFreq2Label.setBounds(90, 110, 20, 20);
    ampFreq2.setBounds(90, 140, 20, 70);

    ampFreq3Label.setBounds(170, 110, 20, 20);
    ampFreq3.setBounds(170, 140, 20, 70);
}


void uSynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &modFreq1)
    {
        //audioProcessor.setFreq1(modFreq1.getValue());
        audioProcessor.setFreqRatios(1, modFreq1.getValue());
    }
    else if (slider == &modFreq2)
    {
        //audioProcessor.setFreq2(modFreq2.getValue());
        audioProcessor.setFreqRatios(2, modFreq2.getValue());
    }
    else if (slider == &modFreq3)
    {
        //audioProcessor.setFreq3(modFreq3.getValue());
        audioProcessor.setFreqRatios(3, modFreq3.getValue());
    }
    else if (slider == &ampFreq1)
    {
        //audioProcessor.setAmp1(ampFreq1.getValue());
        audioProcessor.setAmps(1, ampFreq1.getValue());
    }
    else if (slider == &ampFreq2)
    {
        //audioProcessor.setAmp2(ampFreq2.getValue());
        audioProcessor.setAmps(2, ampFreq2.getValue());
    }
    else if (slider == &ampFreq3)
    {
        //audioProcessor.setAmp3(ampFreq3.getValue());
        audioProcessor.setAmps(3, ampFreq3.getValue());
    }
}

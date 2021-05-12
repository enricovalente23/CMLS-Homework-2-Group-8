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
    setSize(1200, 400);

    modFreq1.setRange(0.0, 3.0, 0.1);
    modFreq1.setSliderStyle(juce::Slider::Rotary);
    modFreq1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    modFreq1.addListener(this);

    modFreq2.setRange(0.0, 3.0, 0.1);
    modFreq2.setSliderStyle(juce::Slider::Rotary);
    modFreq2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    modFreq2.addListener(this);

    modFreq3.setRange(0.0, 3.0, 0.1);
    modFreq3.setSliderStyle(juce::Slider::Rotary);
    modFreq3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    modFreq3.addListener(this);

    modFreq1Label.setText("Freq 1", juce::dontSendNotification);
    modFreq1Label.setJustificationType(juce::Justification::centred);
    modFreq2Label.setText("Freq 2", juce::dontSendNotification);
    modFreq2Label.setJustificationType(juce::Justification::centred);
    modFreq3Label.setText("Freq 3", juce::dontSendNotification);
    modFreq3Label.setJustificationType(juce::Justification::centred);


    Master.setRange(0.0, 0.1, 0.01);
    Master.setSliderStyle(juce::Slider::LinearVertical);
    Master.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    Master.addListener(this);

    ampFreq0.setRange(0.0, 0.1, 0.01);
    ampFreq0.setSliderStyle(juce::Slider::LinearVertical);
    ampFreq0.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq0.addListener(this);

    ampFreq1.setRange(0.0, 0.1, 0.01);
    ampFreq1.setSliderStyle(juce::Slider::LinearVertical);
    ampFreq1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq1.addListener(this);

    ampFreq2.setRange(0.0, 0.1, 0.01);
    ampFreq2.setSliderStyle(juce::Slider::LinearVertical);
    ampFreq2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq2.addListener(this);

    ampFreq3.setRange(0.0, 0.1, 0.01);
    ampFreq3.setSliderStyle(juce::Slider::LinearVertical);
    ampFreq3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq3.addListener(this);

    MasterLabel.setText("Master", juce::dontSendNotification);
    MasterLabel.setJustificationType(juce::Justification::centred);
    /*ampFreq0Label.setText("Gain 0", juce::dontSendNotification);
     ampFreq0Label.setJustificationType(juce::Justification::centred);*/
    ampFreq1Label.setText("Gain 1", juce::dontSendNotification);
    ampFreq3Label.setJustificationType(juce::Justification::centred);
    ampFreq2Label.setText("Gain 2", juce::dontSendNotification);
    ampFreq2Label.setJustificationType(juce::Justification::centred);
    ampFreq3Label.setText("Gain 3", juce::dontSendNotification);
    ampFreq3Label.setJustificationType(juce::Justification::centred);


    addAndMakeVisible(Master);
    addAndMakeVisible(MasterLabel);
    addAndMakeVisible(ampFreq0);
    /*addAndMakeVisible(ampFreq0Label);*/
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
    auto w = getWidth();
    auto h = getHeight();
    auto margin = 10;

    MasterLabel.setBounds(w * 0.0f / 9.0f, 0, w / 4.0f, 15);
    /*ampFreq0Label.setBounds(w * 1.0f / 9.0f, 0, w / 4.0f, 15);*/

    /*
     AttackLabel.setBounds(w * 5.0f / 9.0f, 0, w / 4.0f, 15);
     DecayLabel.setBounds(w * 6.0f / 9.0f, 0, w / 4.0f, 15);
     SustainLabel.setBounds(w * 7.0f / 9.0f, 0, w / 4.0f, 15);
     ReleaseLabel.setBounds(w * 8.0f / 9.0f, 0, w / 4.0f, 15);
     */


     /// Labels Freq Oscillators
    auto LabelW = w * (1 / 9.0f);
    auto LabelH = 15;
    /*ampFreq1Label.setBounds (w * (1 / 9.0f), 0, LabelW, LabelH);*/
    modFreq1Label.setBounds(w * (2 / 9.0f), 0, LabelW, LabelH);
    modFreq2Label.setBounds(w * (3 / 9.0f), 0, LabelW, LabelH);
    modFreq3Label.setBounds(w * (4 / 9.0f), 0, LabelW, LabelH);

    /// Frequency sliders on the first row
    auto scale = 0.5f;
    auto sw = scale * w * (1 / 9.0f);
    auto off = (1 - scale) * w * (1 / 9.0f) * 0.5;
    auto sYf = LabelH + margin;
    modFreq1.setBounds(off + w * (2 / 9.0f), sYf, sw, sw);
    modFreq2.setBounds(off + w * (3 / 9.0f), sYf, sw, sw);
    modFreq3.setBounds(off + w * (4 / 9.0f), sYf, sw, sw);

    /// Labels Gain
    auto slg = sYf + sw + 2 * margin;

    ampFreq1Label.setBounds(w * (2 / 9.0f), slg, LabelW, LabelH);
    ampFreq2Label.setBounds(w * (3 / 9.0f), slg, LabelW, LabelH);
    ampFreq3Label.setBounds(w * (4 / 9.0f), slg, LabelW, LabelH);

    auto sYg = slg + LabelH + margin;
    auto sh = h - sYg;
    sw = w * (1 / 9.0f);

    /// Gain sliders on the second row
    ampFreq0.setBounds(w * (1 / 9.0f), sYf, sw, h - sYf);
    ampFreq1.setBounds(w * (2 / 9.0f), sYg, sw, sh);
    ampFreq2.setBounds(w * (3 / 9.0f), sYg, sw, sh);
    ampFreq3.setBounds(w * (4 / 9.0f), sYg, sw, sh);


    Master.setBounds(w * 0.0f / 9.0f, 15, w / 9.0f, h - 15);
    /*ampFreq0.setBounds(w * 1.0f / 9.0f, 15, w / 4.0f, h - 15);*/

    /*
     ampFreq1.setBounds(w * 2.0f / 9.0f, 15, w / 4.0f, h - 15);
     ampFreq2.setBounds(w * 3.0f / 9.0f, 15, w / 4.0f, h - 15);
     ampFreq3.setBounds(w * 4.0f / 9.0f, 15, w / 4.0f, h - 15);
     */

     /*
      Attack.setBounds(w * 5.0f / 9.0f, 15, w / 4.0f, h - 15);
      Decay.setBounds(w * 6.0f / 9.0f, 15, w / 4.0f, h - 15);
      Sustain.setBounds(w * 7.0f / 9.0f, 15, w / 4.0f, h - 15);
      Release.setBounds(w * 8.0f / 9.0f, 15, w / 4.0f, h - 15);
      */




      /*
       // This is generally where you'll want to lay out the positions of any
       // subcomponents in your editor..
       modFreq1Label.setBounds(10,10,70,20);
       modFreq1.setBounds(10,40,70,70);
       modFreq2Label.setBounds(90,10,70,20);
       modFreq2.setBounds(90,40,70,70);
       modFreq3Label.setBounds(170,10,70,20);
       modFreq3.setBounds(170,40,70,70);

       MasterLabel.setBounds(10, 10, 70, 20);
       Master.setBounds(10, 40, 70, 70);

       //ampFreq0Label.setBounds(10, 110, 20, 20);
       ampFreq0.setBounds(10, 140, 20, 70);
       ampFreq1Label.setBounds(10,110,20,20);
       ampFreq1.setBounds(10,140,20,70);
       ampFreq2Label.setBounds(90,110,20,20);
       ampFreq2.setBounds(90,140,20,70);
       ampFreq3Label.setBounds(170,110,20,20);
       ampFreq3.setBounds(170,140,20,70);
       */
}


void uSynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &modFreq1)
    {
        audioProcessor.setFreq1(modFreq1.getValue());
    }
    else if (slider == &modFreq2)
    {
        audioProcessor.setFreq2(modFreq2.getValue());
    }
    else if (slider == &modFreq3)
    {
        audioProcessor.setFreq3(modFreq3.getValue());
    }
    else if (slider == &ampFreq0)
    {
        audioProcessor.setAmp0(ampFreq0.getValue());
    }
    else if (slider == &ampFreq1)
    {
        audioProcessor.setAmp1(ampFreq1.getValue());
    }
    else if (slider == &ampFreq2)
    {
        audioProcessor.setAmp2(ampFreq2.getValue());
    }
    else if (slider == &ampFreq3)
    {
        audioProcessor.setAmp3(ampFreq3.getValue());
    }
    else if (slider == &Master)
    {
        audioProcessor.setMaster(Master.getValue());
    }
    else if (slider == &Attack)
    {
        audioProcessor.setAttack(Attack.getValue());
    }
    else if (slider == &Decay)
    {
        audioProcessor.setDecay(Decay.getValue());
    }
    else if (slider == &Sustain)
    {
        audioProcessor.setSustain(Sustain.getValue());
    }
    else if (slider == &Release)
    {
        audioProcessor.setRelease(Release.getValue());
    }


    //Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit Jit 
    if (slider == &modFreq1)
    {
        audioProcessor.setFreqRatios(1, modFreq1.getValue());
    }
    else if (slider == &modFreq2)
    {
        audioProcessor.setFreqRatios(2, modFreq2.getValue());
    }
    else if (slider == &modFreq3)
    {
        audioProcessor.setFreqRatios(3, modFreq3.getValue());
    }
    else if (slider == &ampFreq0)
    {
        audioProcessor.setAmps(0, ampFreq0.getValue());
    }
    else if (slider == &ampFreq1)
    {
        audioProcessor.setAmps(1, ampFreq1.getValue());
    }
    else if (slider == &ampFreq2)
    {
        audioProcessor.setAmps(2, ampFreq2.getValue());
    }
    else if (slider == &ampFreq3)
    {
        audioProcessor.setAmps(3, ampFreq3.getValue());
    }
    else if (slider == &Master)
    {
        audioProcessor.setMasterGain(Master.getValue());
    }
    else if (slider == &Attack)
    {
        audioProcessor.setAttack(Attack.getValue());
    }
    else if (slider == &Decay)
    {
        audioProcessor.setDecay(Decay.getValue());
    }
    else if (slider == &Sustain)
    {
        audioProcessor.setSustain(Sustain.getValue());
    }
    else if (slider == &Release)
    {
        audioProcessor.setRelease(Release.getValue());
    }
}

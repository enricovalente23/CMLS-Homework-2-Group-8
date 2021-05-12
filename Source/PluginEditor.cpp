/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AdditiveSynthAudioProcessorEditor::AdditiveSynthAudioProcessorEditor (AdditiveSynthAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1700, 600);
    
    modFreq1.setRange(0.25,4.0,0.05);
    modFreq1.setSliderStyle(juce::Slider::Rotary);
    modFreq1.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    modFreq1.addListener(this);
    
    modFreq2.setRange(0.25, 4.0, 0.05);
    modFreq2.setSliderStyle(juce::Slider::Rotary);
    modFreq2.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    modFreq2.addListener(this);
    
    modFreq3.setRange(0.25, 4.0, 0.05);
    modFreq3.setSliderStyle(juce::Slider::Rotary);
    modFreq3.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    modFreq3.addListener(this);
    
    modFreq1Label.setText("Freq 1",juce::dontSendNotification);
    modFreq1Label.setJustificationType(juce::Justification::centred);
    modFreq2Label.setText("Freq 2", juce::dontSendNotification);
    modFreq2Label.setJustificationType(juce::Justification::centred);
    modFreq3Label.setText("Freq 3",juce::dontSendNotification);
    modFreq3Label.setJustificationType(juce::Justification::centred);
    
    
    
    Master.setRange(0.0, 1.0, 0.05);
    Master.setSliderStyle(juce::Slider::LinearVertical);
    Master.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    Master.addListener(this);
    
    Panning.setRange(0.0, 1.0, 0.05);
    Panning.setSliderStyle(juce::Slider::Rotary);
    Panning.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    Panning.addListener(this);
    
    TremoloVel.setRange(1.0, 10.0, 0.1);
    TremoloVel.setSliderStyle(juce::Slider::Rotary);
    TremoloVel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    TremoloVel.addListener(this);
    
    
    ampFreq0.setRange(0.0, 1.0, 0.05);
    ampFreq0.setSliderStyle(juce::Slider::LinearVertical);
    ampFreq0.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq0.addListener(this);
    
    ampFreq1.setRange(0.0, 1.0, 0.05);
    ampFreq1.setSliderStyle(juce::Slider::LinearVertical );
    ampFreq1.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq1.addListener(this);
    
    ampFreq2.setRange(0.0, 1.0, 0.05);
    ampFreq2.setSliderStyle(juce::Slider::LinearVertical );
    ampFreq2.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq2.addListener(this);
    
    ampFreq3.setRange(0.0, 1.0, 0.05);
    ampFreq3.setSliderStyle(juce::Slider::LinearVertical);
    ampFreq3.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    ampFreq3.addListener(this);
    
    
    Attack.setRange(0.1, 3.0, 0.05);
    Attack.setSliderStyle(juce::Slider::LinearVertical);
    Attack.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    Attack.addListener(this);
    
    Decay.setRange(0.1, 3.0, 0.05);
    Decay.setSliderStyle(juce::Slider::LinearVertical);
    Decay.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    Decay.addListener(this);
    
    Sustain.setRange(0.1, 3.0, 0.05);
    Sustain.setSliderStyle(juce::Slider::LinearVertical);
    Sustain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    Sustain.addListener(this);
    
    Release.setRange(0.1, 3.0, 0.05);
    Release.setSliderStyle(juce::Slider::LinearVertical);
    Release.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    Release.addListener(this);
    
    
    
    MasterLabel.setText("Master", juce::dontSendNotification);
    MasterLabel.setJustificationType(juce::Justification::centred);
    ampFreq0Label.setText("Gain 0", juce::dontSendNotification);
    ampFreq0Label.setJustificationType(juce::Justification::centred);
    PanningLabel.setText("Panning", juce::dontSendNotification);
    PanningLabel.setJustificationType(juce::Justification::centred);
    TremoloVelLabel.setText("Tremolo Vel", juce::dontSendNotification);
    TremoloVelLabel.setJustificationType(juce::Justification::centred);
    
    
    ampFreq1Label.setText("Gain 1",juce::dontSendNotification);
    ampFreq1Label.setJustificationType(juce::Justification::centred);
    ampFreq2Label.setText("Gain 2", juce::dontSendNotification);
    ampFreq2Label.setJustificationType(juce::Justification::centred);
    ampFreq3Label.setText("Gain 3", juce::dontSendNotification);
    ampFreq3Label.setJustificationType(juce::Justification::centred);
    
    
    AttackLabel.setText("Attack", juce::dontSendNotification);
    AttackLabel.setJustificationType(juce::Justification::centred);
    DecayLabel.setText("Decay", juce::dontSendNotification);
    DecayLabel.setJustificationType(juce::Justification::centred);
    SustainLabel.setText("Sustain", juce::dontSendNotification);
    SustainLabel.setJustificationType(juce::Justification::centred);
    ReleaseLabel.setText("Release", juce::dontSendNotification);
    ReleaseLabel.setJustificationType(juce::Justification::centred);
    
    
    addAndMakeVisible(Master);
    addAndMakeVisible(MasterLabel);
    addAndMakeVisible(ampFreq0);
    addAndMakeVisible(ampFreq0Label);
    addAndMakeVisible(Panning);
    addAndMakeVisible(PanningLabel);
    addAndMakeVisible(TremoloVel);
    addAndMakeVisible(TremoloVelLabel);
    
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
    
    addAndMakeVisible(Attack);
    addAndMakeVisible(AttackLabel);
    addAndMakeVisible(Decay);
    addAndMakeVisible(DecayLabel);
    addAndMakeVisible(Sustain);
    addAndMakeVisible(SustainLabel);
    addAndMakeVisible(Release);
    addAndMakeVisible(ReleaseLabel);
}

AdditiveSynthAudioProcessorEditor::~AdditiveSynthAudioProcessorEditor()
{
}

//==============================================================================
void AdditiveSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    auto backgroundColourId = juce::Colours::orange;
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.setColour (juce::Colours::white);
}

void AdditiveSynthAudioProcessorEditor::resized()
{
    auto w = getWidth();
    auto h = getHeight();
    auto margin = 10;
    
    
    /// Labels Freq Oscillators
    auto LabelW = w * (1 / 9.0f);
    auto LabelH = 15;
    
    
    /*MasterLabel.setBounds(w * 0.0f / 9.0f, 0, w / 9.0f, 15);*/
    
    /*ampFreq0Label.setBounds(w * 1.0f / 9.0f, 0, w / 4.0f, 15);*/
    ampFreq0Label.setBounds(w * (1 / 9.0f), 0, LabelW, LabelH);
    
    
    
    
    PanningLabel.setBounds(w * (0 / 9.0f), 0, LabelW, LabelH);
    TremoloVelLabel.setBounds(w * (1 / 9.0f), 0, LabelW, LabelH);
    modFreq1Label.setBounds (w * (2 / 9.0f), 0, LabelW, LabelH);
    modFreq2Label.setBounds (w * (3 / 9.0f), 0, LabelW, LabelH);
    modFreq3Label.setBounds (w * (4 / 9.0f), 0, LabelW, LabelH);
    
    
    /// Labels ADSR
    AttackLabel.setBounds(w * 5.0f / 9.0f, 0, w / 9.0f, 15);
    DecayLabel.setBounds(w * 6.0f / 9.0f, 0, w / 9.0f, 15);
    SustainLabel.setBounds(w * 7.0f / 9.0f, 0, w / 9.0f, 15);
    ReleaseLabel.setBounds(w * 8.0f / 9.0f, 0, w / 9.0f, 15);
    
    
    /// Frequency and tremolo knobs above gain sliders
    auto scale = 0.5f;
    auto sw =  scale * w * (1 / 9.0f);
    auto off = (1 - scale) * w * (1 / 9.0f) * 0.5;
    auto sYf = LabelH + margin;
    Panning.setBounds(off + w * (0 / 9.0f), sYf, sw, sw);
    TremoloVel.setBounds(off + w * (1 / 9.0f), sYf, sw, sw);
    modFreq1.setBounds (off + w * (2 / 9.0f), sYf, sw, sw);
    modFreq2.setBounds (off + w * (3 / 9.0f), sYf, sw, sw);
    modFreq3.setBounds (off + w * (4 / 9.0f), sYf, sw, sw);
    
    /// Labels Gain
    auto slg = sYf + sw + 2 * margin;
    
    MasterLabel.setBounds(w * (0 / 9.0f), slg, LabelW, LabelH);
    ampFreq0Label.setBounds(w * (1 / 9.0f), slg, LabelW, LabelH);
    ampFreq1Label.setBounds (w * (2 / 9.0f), slg, LabelW, LabelH);
    ampFreq2Label.setBounds (w * (3 / 9.0f), slg, LabelW, LabelH);
    ampFreq3Label.setBounds (w * (4 / 9.0f), slg, LabelW, LabelH);
    
    auto sYg = slg + LabelH +margin;
    auto sh = h - sYg;
    sw = w * (1 / 9.0f);
    
    /// Gain sliders below frequency sliders
    
    /*ampFreq0.setBounds (w * (1 / 9.0f), sYf, sw, h - sYf);*/
    
    Master.setBounds(w * (0 / 9.0f), sYg, sw, sh);
    ampFreq0.setBounds(w * (1 / 9.0f), sYg, sw, sh);
    ampFreq1.setBounds (w * (2 / 9.0f), sYg, sw, sh);
    ampFreq2.setBounds (w * (3 / 9.0f), sYg, sw, sh);
    ampFreq3.setBounds (w * (4 / 9.0f), sYg, sw, sh);
    
    
    /*Master.setBounds(w * 0.0f / 9.0f, 15, w / 9.0f, h - 15);*/
    
    /*ampFreq0.setBounds(w * 1.0f / 9.0f, 15, w / 9.0f, h - 15);*/
    
    Attack.setBounds(w * 5.0f / 9.0f, 15, w / 9.0f, h - 15);
    Decay.setBounds(w * 6.0f / 9.0f, 15, w / 9.0f, h - 15);
    Sustain.setBounds(w * 7.0f / 9.0f, 15, w / 9.0f, h - 15);
    Release.setBounds(w * 8.0f / 9.0f, 15, w / 9.0f, h - 15);
    
}


void AdditiveSynthAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
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
    else if (slider == &ampFreq0)
    {
        audioProcessor.setAmp0(ampFreq0.getValue());
    }
    else if (slider == &Panning)
    {
        audioProcessor.setPanning(Panning.getValue());
    }
    else if (slider == &TremoloVel)
    {
        audioProcessor.setTremoloVel(TremoloVel.getValue());
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

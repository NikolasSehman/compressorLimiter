/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HighLowFilterAudioProcessorEditor::HighLowFilterAudioProcessorEditor (HighLowFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 1050);
    //
        ratioSlider.addListener(this);
        ratioSlider.setBounds(75, 60, 150, 150);
        ratioSlider.setRange(1.f, 10.f,0.1f);
        ratioSlider.setValue(*audioProcessor.ratio);
        ratioSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 25);
        ratioSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        ratioSlider.setLookAndFeel(&ratioLookandFeel);
        addAndMakeVisible(ratioSlider);
        
        
        attackSlider.addListener(this);
        attackSlider.setBounds(100, 280, 100, 100);
        attackSlider.setRange(10.f, 1000.f,1.f);
        attackSlider.setValue(*audioProcessor.attack);
        attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 25);
        attackSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        attackSlider.setLookAndFeel(&attackReleaseLookandFeel);
        addAndMakeVisible(attackSlider);
        
        thresholdSlider.addListener(this);
        thresholdSlider.setBounds(25, 425, 250, 250);
        thresholdSlider.setRange(-60.f, 0.f,0.5f);
        thresholdSlider.setValue(*audioProcessor.threshold);
        thresholdSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 25);
        thresholdSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        thresholdSlider.setLookAndFeel(&ThresholdLookandFeel);
        addAndMakeVisible(thresholdSlider);
        
        
        
        releaseSlider.addListener(this);
        releaseSlider.setBounds(100, 725, 100, 100);
        releaseSlider.setRange(10.f, 1000.f,1.f);
        releaseSlider.setValue(*audioProcessor.release);
        releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 25);
        releaseSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        releaseSlider.setLookAndFeel(&attackReleaseLookandFeel);
        addAndMakeVisible(releaseSlider);
        
        makeupGainSlider.addListener(this);
        makeupGainSlider.setBounds(75, 875, 150, 150);
        makeupGainSlider.setRange(0.f, 18.f,0.1f);
        makeupGainSlider.setValue(*audioProcessor.makeupGain);
        makeupGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 25);
        makeupGainSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        makeupGainSlider.setLookAndFeel(&MakeupGainLookandFeel);
        addAndMakeVisible(makeupGainSlider);
    
    
    limiterSlider.addListener(this);
    limiterSlider.setBounds(225, 700, 50, 300);
    limiterSlider.setRange(-18.f, 0.f, 0.1f);
    limiterSlider.setValue(*audioProcessor.limiterCeiling);
    limiterSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 25);
    limiterSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    limiterSlider.setLookAndFeel(&limiterLookandFeel);
    addAndMakeVisible(limiterSlider);
    
//    mixSlider.addListener(this);
//    mixSlider.setBounds(225, 60, 50, 300);
//    mixSlider.setRange(0.f, 1.f, 0.01f);
//    mixSlider.setValue(0.5f);
//    mixSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
//    mixSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
//    mixSlider.setLookAndFeel(&mixLookandFeel);
//    addAndMakeVisible(mixSlider);
    
    
    
    
    
    
    
    
    ThresholdLookandFeel.setColour(Slider::rotarySliderFillColourId, Colours::maroon);
    ratioLookandFeel.setColour(Slider::rotarySliderOutlineColourId, Colours::maroon);
    MakeupGainLookandFeel.setColour(Slider::rotarySliderOutlineColourId, Colours::maroon);
    attackReleaseLookandFeel.setColour(Slider::rotarySliderOutlineColourId, Colours::maroon);
    
    ThresholdLookandFeel.setColour(Slider::rotarySliderOutlineColourId, Colours::white);
    ratioLookandFeel.setColour(Slider::rotarySliderFillColourId, Colours::white);
    MakeupGainLookandFeel.setColour(Slider::rotarySliderFillColourId, Colours::white);
    attackReleaseLookandFeel.setColour(Slider::rotarySliderFillColourId, Colours::white);
    
    limiterLookandFeel.setColour(Slider::trackColourId, Colours::maroon);
    
    //mixLookandFeel.setColour(Slider::trackColourId, Colours::purple);
    
}

HighLowFilterAudioProcessorEditor::~HighLowFilterAudioProcessorEditor()
{
}

//==============================================================================
void HighLowFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll(Colours::black);

        g.setColour (juce::Colours::greenyellow);
        g.setFont (15.0f);
        //g.drawFittedText ("Wombatduke INC", getLocalBounds(), juce::Justification::centred, 1);
        g.setColour(juce::Colours::white);
        g.drawFittedText("Threshold",  100,    400,     100, 25, juce::Justification::centred, 1);
        g.drawFittedText("Ratio - (dB)",   100, 30,    100, 25, juce::Justification::centred, 1);
        g.drawFittedText("Release - (dB)",       100,    700,    100, 25, juce::Justification::centred, 1);
        g.drawFittedText("Attack - (ms)",      100,    250,    100, 25, juce::Justification::centred, 1);
        g.drawFittedText("MakeUp Gain",     100,    850,   100, 25, juce::Justification::centred, 1);
        g.drawFittedText("Limiter", 200, 675, 100,25, juce::Justification::centred,1);
       // g.drawFittedText("Wet", 200, 30, 100, 25, juce::Justification::centred,1);
       // g.drawFittedText("Dry", 200, 375, 100, 25, juce::Justification::centred,1);
        //g.drawFittedText("MakeUp Gain - (ms)", 730,    75,    100, 25, juce::Justification::centred, 1);
}

void HighLowFilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
void HighLowFilterAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
  if (slider == &thresholdSlider)
       *audioProcessor.threshold = thresholdSlider.getValue();
    if (slider == &ratioSlider)
        *audioProcessor.ratio = ratioSlider.getValue();
    if (slider == &attackSlider)
        *audioProcessor.attack = attackSlider.getValue();
    if (slider == &releaseSlider)
        *audioProcessor.release = releaseSlider.getValue();
    if (slider == &makeupGainSlider)
        *audioProcessor.makeupGain = makeupGainSlider.getValue();
    if (slider == &limiterSlider)
        *audioProcessor.limiterCeiling = limiterSlider.getValue();
//    if (slider == &mixSlider)
//        std::cout<<mixSlider.getValue()<<std::endl;
//        audioProcessor.mix = mixSlider.getValue();
}

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
class HighLowFilterAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            public juce::Slider::Listener
                                            
{
public:
    HighLowFilterAudioProcessorEditor (HighLowFilterAudioProcessor&);
    ~HighLowFilterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged (Slider* slider) override;
    
private:
    
        Slider thresholdSlider;
        Slider ratioSlider;
        Slider attackSlider;
        Slider releaseSlider;
        Slider makeupGainSlider;
    
    
        Slider limiterSlider;
    
//    Slider mixSlider;
    
    
    LookAndFeel_V4 ratioLookandFeel;
    LookAndFeel_V4 ThresholdLookandFeel;
    LookAndFeel_V4 MakeupGainLookandFeel;
    LookAndFeel_V4 attackReleaseLookandFeel;
    
    LookAndFeel_V4 limiterLookandFeel;
    
//    LookAndFeel_V4 mixLookandFeel;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HighLowFilterAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HighLowFilterAudioProcessorEditor)
};

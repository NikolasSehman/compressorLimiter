/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class HighLowFilterAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    HighLowFilterAudioProcessor();
    ~HighLowFilterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    AudioParameterFloat * threshold;
    AudioParameterFloat * ratio;
    AudioParameterFloat * attack;
    AudioParameterFloat * makeupGain;
    AudioParameterFloat * release;
    
    AudioParameterFloat * limiterCeiling;
    
    //AudioParameterFloat mix {.5f};
   // float mInputGain = nullptr;
private:
    
    dsp::Compressor<float> Compressor;
    dsp::Gain<float> gain;
    dsp::Limiter<float> limiter;
    dsp::ProcessSpec spec;
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HighLowFilterAudioProcessor)
};

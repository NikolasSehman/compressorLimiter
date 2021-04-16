/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HighLowFilterAudioProcessor::HighLowFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(threshold = new AudioParameterFloat("Threshold", "Threshold", -60.f,0.f,0.f));
    addParameter( ratio = new AudioParameterFloat("Ratio", "Ratio", 1.f, 10.f, 1.f));
    addParameter(attack = new AudioParameterFloat("Attack","Attack",10.f, 1000.f, 100.f));
    addParameter(release = new AudioParameterFloat("Release", "Release", 10.f, 1000.f, 100.f));
    addParameter(makeupGain = new AudioParameterFloat("MakeUpGain", "MakeUpGain", 0.f, 18.f, 0.f));
    addParameter(limiterCeiling = new AudioParameterFloat("LimiterThreshold", "Limiter", -18.f, 0.f, 0.f));
}

HighLowFilterAudioProcessor::~HighLowFilterAudioProcessor()
{
}

//==============================================================================
const juce::String HighLowFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HighLowFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HighLowFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HighLowFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HighLowFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HighLowFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HighLowFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HighLowFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HighLowFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void HighLowFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HighLowFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;
    
    Compressor.prepare(spec);
    Compressor.reset();
    
    gain.prepare(spec);
    gain.reset();
    
    limiter.prepare(spec);
    limiter.reset();
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void HighLowFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HighLowFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HighLowFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

//
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
    //}
    
    //convert buffer to audio block
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> dry (buffer);
    
    //do things to the audio bock
    //block.multiplyBy(.125f);
    Compressor.setAttack(*attack);
    Compressor.setRelease(*release);
    Compressor.setRatio(*ratio);
    Compressor.setThreshold(*threshold);
    gain.setGainDecibels(*makeupGain);
    
    Compressor.process(dsp::ProcessContextReplacing<float> (block));
    gain.process(dsp::ProcessContextReplacing<float> (block));
    
    limiter.setThreshold(*limiterCeiling);
    limiter.setRelease(12.f);
    limiter.process(dsp::ProcessContextReplacing<float>(block));
    
    //Wet/Dry mix
//    gain.setGainLinear(mix);
//    gain.process(dsp::ProcessContextReplacing<float>(block));
//
//    gain.setGainLinear(1-mix);
//    gain.process(dsp::ProcessContextReplacing<float>(dry));
//    block.replaceWithSumOf(block, dry);
    
    //convert block back to buffer
    block.copyTo(buffer);
    
}

//==============================================================================
bool HighLowFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HighLowFilterAudioProcessor::createEditor()
{
    return new HighLowFilterAudioProcessorEditor (*this);
}

//==============================================================================
void HighLowFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    std::unique_ptr<XmlElement> xml (new XmlElement("MyCompressorParameters"));
    xml->setAttribute("Threshold", (double) *threshold);
    xml->setAttribute("Ratio", (double) *ratio);
    xml->setAttribute("Attack", (double) *attack);
    xml->setAttribute("Release", (double) *release);
    xml->setAttribute("MakeUpGain", (double) *makeupGain);
    xml->setAttribute("LimiterThreshold", (double) *limiterCeiling);
    
    copyXmlToBinary(*xml, destData);
}

void HighLowFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> xml (getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr){
        if(xml->hasTagName("MyCompressorParameters")){
            *threshold = xml -> getDoubleAttribute("Threshold", 0.f);
            *ratio = xml -> getDoubleAttribute("Ratio", 1.f);
            *attack = xml -> getDoubleAttribute("Attack", 0.f);
            *release = xml -> getDoubleAttribute("Release", 0.f);
            *makeupGain = xml -> getDoubleAttribute("MakeUpGain", 0.f);
            *limiterCeiling = xml -> getDoubleAttribute("LimiterThreshold", 0.f);
        }
    }
    
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HighLowFilterAudioProcessor();
}

#include "PluginProcessor.h"
#include "PluginEditor.h"

LVOXAudioProcessor::LVOXAudioProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "LVOX_PARAMS", createParameterLayout()),
      presetManager (apvts),
      dspChain (apvts)
{
}

LVOXAudioProcessor::~LVOXAudioProcessor() {}

void LVOXAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels = (juce::uint32) getTotalNumOutputChannels();
    dspChain.prepare (spec);
}

void LVOXAudioProcessor::releaseResources()
{
    dspChain.reset();
}

bool LVOXAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void LVOXAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Measure input level
    float inLevel = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        inLevel = std::max (inLevel, buffer.getMagnitude (ch, 0, buffer.getNumSamples()));
    inputLevel.store (inLevel);

    // Get host BPM
    if (auto* playHead = getPlayHead())
    {
        if (auto pos = playHead->getPosition())
        {
            if (auto bpm = pos->getBpm())
                dspChain.setHostBPM (*bpm);
        }
    }

    // Process DSP chain
    dspChain.process (buffer);

    // Measure output level
    float outLevel = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        outLevel = std::max (outLevel, buffer.getMagnitude (ch, 0, buffer.getNumSamples()));
    outputLevel.store (outLevel);
}

juce::AudioProcessorEditor* LVOXAudioProcessor::createEditor()
{
    return new LVOXAudioProcessorEditor (*this);
}

void LVOXAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void LVOXAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LVOXAudioProcessor();
}

/*
 * PluginProcessor.cpp — Tape Decay JUCE plugin
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

TapeDecayProcessor::TapeDecayProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
TapeDecayProcessor::createParameterLayout()
{
    return {
        std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID("decay", 1),
            "Decay",
            juce::NormalisableRange<float>(0.0f, 0.99f, 0.01f),
            0.1f)
    };
}

void TapeDecayProcessor::prepareToPlay(double, int)
{
}

void TapeDecayProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                       juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const float decay = apvts.getRawParameterValue("decay")->load();
    const float scale = 1.0f / (1.0f - decay);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        float* samples = buffer.getWritePointer(ch);
        const int numSamples = buffer.getNumSamples();

        for (int i = 0; i < numSamples; ++i)
        {
            float s = samples[i];

            if (s > 0.0f)
            {
                s -= decay;
                if (s < 0.0f) s = 0.0f;
            }
            else if (s < 0.0f)
            {
                s += decay;
                if (s > 0.0f) s = 0.0f;
            }

            samples[i] = s * scale;
        }
    }
}

juce::AudioProcessorEditor* TapeDecayProcessor::createEditor()
{
    return new TapeDecayEditor(*this);
}

void TapeDecayProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary(*xml, destData);
}

void TapeDecayProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TapeDecayProcessor();
}

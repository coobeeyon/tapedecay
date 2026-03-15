/*
 * PluginEditor.h — Tape Decay JUCE plugin GUI
 */

#pragma once
#include "PluginProcessor.h"

class TapeDecayEditor : public juce::AudioProcessorEditor
{
public:
    explicit TapeDecayEditor(TapeDecayProcessor&);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    TapeDecayProcessor& processor;

    juce::Slider decaySlider;
    juce::Label  decayLabel;
    juce::AudioProcessorValueTreeState::SliderAttachment decayAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TapeDecayEditor)
};

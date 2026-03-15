/*
 * PluginEditor.cpp — Tape Decay JUCE plugin GUI
 */

#include "PluginEditor.h"

TapeDecayEditor::TapeDecayEditor(TapeDecayProcessor& p)
    : AudioProcessorEditor(&p),
      processor(p),
      decayAttachment(p.apvts, "decay", decaySlider)
{
    decaySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(decaySlider);

    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(decayLabel);

    setSize(200, 200);
}

void TapeDecayEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a2e));

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("Tape Decay", getLocalBounds().removeFromTop(36),
               juce::Justification::centred);
}

void TapeDecayEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    area.removeFromTop(30);

    decayLabel.setBounds(area.removeFromTop(20));
    decaySlider.setBounds(area);
}

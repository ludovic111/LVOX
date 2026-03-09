#include "EQPanel.h"

namespace
{
    struct BandParamIDs
    {
        const char* freq;
        const char* gain;
        const char* q;
        const char* type;
        const char* active;
    };

    const BandParamIDs bandParams[4] =
    {
        { ParamIDs::eqBand1Freq, ParamIDs::eqBand1Gain, ParamIDs::eqBand1Q, ParamIDs::eqBand1Type, ParamIDs::eqBand1Active },
        { ParamIDs::eqBand2Freq, ParamIDs::eqBand2Gain, ParamIDs::eqBand2Q, ParamIDs::eqBand2Type, ParamIDs::eqBand2Active },
        { ParamIDs::eqBand3Freq, ParamIDs::eqBand3Gain, ParamIDs::eqBand3Q, ParamIDs::eqBand3Type, ParamIDs::eqBand3Active },
        { ParamIDs::eqBand4Freq, ParamIDs::eqBand4Gain, ParamIDs::eqBand4Q, ParamIDs::eqBand4Type, ParamIDs::eqBand4Active },
    };
}

EQPanel::EQPanel (juce::AudioProcessorValueTreeState& apvts)
    : ModulePanel (apvts, "Parametric EQ", ParamIDs::eqBypass)
{
    for (int i = 0; i < 4; ++i)
    {
        auto* band = bands.add (new BandControls (juce::String (i + 1)));

        addAndMakeVisible (band->freqKnob);
        addAndMakeVisible (band->gainKnob);
        addAndMakeVisible (band->qKnob);
        addAndMakeVisible (band->typeBox);
        addAndMakeVisible (band->activeToggle);

        band->freqKnob.attach (apvts, bandParams[i].freq);
        band->gainKnob.attach (apvts, bandParams[i].gain);
        band->qKnob.attach    (apvts, bandParams[i].q);

        band->typeAttachment   = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, bandParams[i].type, band->typeBox);
        band->activeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>   (apvts, bandParams[i].active, band->activeToggle);
    }
}

void EQPanel::layoutControls (juce::Rectangle<int> area)
{
    auto bandWidth = area.getWidth() / 4;

    for (int i = 0; i < 4; ++i)
    {
        auto bandArea = area.removeFromLeft (bandWidth);
        auto* band = bands[i];

        juce::FlexBox fb;
        fb.flexDirection  = juce::FlexBox::Direction::column;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignItems     = juce::FlexBox::AlignItems::center;

        fb.items.add (juce::FlexItem (band->activeToggle).withWidth (60).withHeight (24));
        fb.items.add (juce::FlexItem (band->typeBox)     .withWidth (100).withHeight (24));
        fb.items.add (juce::FlexItem (band->freqKnob)    .withWidth (80).withHeight (90));
        fb.items.add (juce::FlexItem (band->gainKnob)    .withWidth (80).withHeight (90));
        fb.items.add (juce::FlexItem (band->qKnob)       .withWidth (80).withHeight (90));

        fb.performLayout (bandArea);
    }
}

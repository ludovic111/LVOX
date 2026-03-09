#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "../../Parameters.h"
#include "../Helpers/ColourPalette.h"

class EQCurveComponent : public juce::Component, public juce::Timer
{
public:
    EQCurveComponent (juce::AudioProcessorValueTreeState& apvts)
        : apvts (apvts)
    {
        startTimerHz (30);
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced (1.0f);

        // Background
        g.setColour (juce::Colour (0xff0d1117));
        g.fillRoundedRectangle (bounds, 4.0f);

        // Grid lines
        g.setColour (juce::Colours::white.withAlpha (0.06f));
        // Horizontal: 0dB line
        float zeroY = bounds.getY() + bounds.getHeight() * 0.5f;
        g.drawHorizontalLine ((int) zeroY, bounds.getX(), bounds.getRight());
        // +/- 6, 12 dB lines
        for (float db : { -12.0f, -6.0f, 6.0f, 12.0f })
        {
            float y = dbToY (db, bounds);
            g.drawHorizontalLine ((int) y, bounds.getX(), bounds.getRight());
        }
        // Vertical frequency lines
        for (float freq : { 100.0f, 1000.0f, 10000.0f })
        {
            float x = freqToX (freq, bounds);
            g.drawVerticalLine ((int) x, bounds.getY(), bounds.getBottom());
        }

        // Compute combined magnitude response
        juce::Path curvePath;
        bool pathStarted = false;

        const int numPoints = (int) bounds.getWidth();
        for (int px = 0; px < numPoints; ++px)
        {
            float x = bounds.getX() + (float) px;
            float freq = xToFreq (x, bounds);
            float totalDB = 0.0f;

            for (int band = 0; band < 4; ++band)
            {
                if (!bandActive[band] || std::abs (bandGain[band]) < 0.01f)
                    continue;

                auto coeffs = makeCoefficients (band);
                if (coeffs == nullptr)
                    continue;

                double mag = coeffs->getMagnitudeForFrequency (freq, 44100.0);
                totalDB += (float) juce::Decibels::gainToDecibels (mag);
            }

            float y = dbToY (totalDB, bounds);
            if (!pathStarted)
            {
                curvePath.startNewSubPath (x, y);
                pathStarted = true;
            }
            else
            {
                curvePath.lineTo (x, y);
            }
        }

        if (pathStarted)
        {
            // Filled area under curve
            juce::Path fillPath (curvePath);
            fillPath.lineTo (bounds.getRight(), zeroY);
            fillPath.lineTo (bounds.getX(), zeroY);
            fillPath.closeSubPath();

            g.setColour (FrutigerColours::aquaDark.withAlpha (0.15f));
            g.fillPath (fillPath);

            // Curve line
            g.setColour (FrutigerColours::aquaLight);
            g.strokePath (curvePath, juce::PathStrokeType (1.5f));
        }

        // Frequency labels
        g.setColour (juce::Colours::white.withAlpha (0.3f));
        g.setFont (9.0f);
        for (auto [freq, label] : std::initializer_list<std::pair<float, const char*>> {
            { 100.0f, "100" }, { 1000.0f, "1k" }, { 10000.0f, "10k" } })
        {
            float x = freqToX (freq, bounds);
            g.drawText (label, juce::Rectangle<float> (x - 15.0f, bounds.getBottom() - 12.0f, 30.0f, 12.0f),
                        juce::Justification::centred);
        }
    }

    void timerCallback() override
    {
        struct BandIDs { const char* freq; const char* gain; const char* q; const char* type; const char* active; };
        static const BandIDs ids[4] = {
            { ParamIDs::eqBand1Freq, ParamIDs::eqBand1Gain, ParamIDs::eqBand1Q, ParamIDs::eqBand1Type, ParamIDs::eqBand1Active },
            { ParamIDs::eqBand2Freq, ParamIDs::eqBand2Gain, ParamIDs::eqBand2Q, ParamIDs::eqBand2Type, ParamIDs::eqBand2Active },
            { ParamIDs::eqBand3Freq, ParamIDs::eqBand3Gain, ParamIDs::eqBand3Q, ParamIDs::eqBand3Type, ParamIDs::eqBand3Active },
            { ParamIDs::eqBand4Freq, ParamIDs::eqBand4Gain, ParamIDs::eqBand4Q, ParamIDs::eqBand4Type, ParamIDs::eqBand4Active },
        };

        bool changed = false;
        for (int i = 0; i < 4; ++i)
        {
            float f = *apvts.getRawParameterValue (ids[i].freq);
            float g = *apvts.getRawParameterValue (ids[i].gain);
            float q = *apvts.getRawParameterValue (ids[i].q);
            int   t = (int) *apvts.getRawParameterValue (ids[i].type);
            bool  a = *apvts.getRawParameterValue (ids[i].active) > 0.5f;

            if (f != bandFreq[i] || g != bandGain[i] || q != bandQ[i] || t != bandType[i] || a != bandActive[i])
            {
                bandFreq[i] = f;
                bandGain[i] = g;
                bandQ[i] = q;
                bandType[i] = t;
                bandActive[i] = a;
                changed = true;
            }
        }

        if (changed)
            repaint();
    }

private:
    juce::AudioProcessorValueTreeState& apvts;

    float bandFreq[4]  = { 100.0f, 500.0f, 3000.0f, 8000.0f };
    float bandGain[4]  = { 0.0f, 0.0f, 0.0f, 0.0f };
    float bandQ[4]     = { 1.0f, 1.0f, 1.0f, 1.0f };
    int   bandType[4]  = { 1, 0, 0, 2 };
    bool  bandActive[4] = { true, true, true, true };

    juce::dsp::IIR::Coefficients<float>::Ptr makeCoefficients (int band) const
    {
        float freq = bandFreq[band];
        float gain = bandGain[band];
        float q    = bandQ[band];
        int   type = bandType[band];
        float linearGain = juce::Decibels::decibelsToGain (gain);

        switch (type)
        {
            case 0: return juce::dsp::IIR::Coefficients<float>::makePeakFilter (44100.0, freq, q, linearGain);
            case 1: return juce::dsp::IIR::Coefficients<float>::makeLowShelf (44100.0, freq, q, linearGain);
            case 2: return juce::dsp::IIR::Coefficients<float>::makeHighShelf (44100.0, freq, q, linearGain);
            default: return nullptr;
        }
    }

    float freqToX (float freq, juce::Rectangle<float> bounds) const
    {
        float norm = (std::log (freq / 20.0f)) / (std::log (20000.0f / 20.0f));
        return bounds.getX() + norm * bounds.getWidth();
    }

    float xToFreq (float x, juce::Rectangle<float> bounds) const
    {
        float norm = (x - bounds.getX()) / bounds.getWidth();
        return 20.0f * std::pow (20000.0f / 20.0f, norm);
    }

    float dbToY (float db, juce::Rectangle<float> bounds) const
    {
        float maxDB = 18.0f;
        float norm = 0.5f - (db / (2.0f * maxDB));
        return bounds.getY() + norm * bounds.getHeight();
    }
};

/*
 * File:   msmax.cpp
 * Author: Geoscience Australia
 *
 */

#define SEISCOMP_COMPONENT MSmax

#include "msmax.h"

#include <seiscomp3/core/plugin.h>
#include <seiscomp3/logging/log.h>

#include <seiscomp3/math/filter/butterworth.h>

#include <math.h>

ADD_SC_PLUGIN(
	"MSvmax magnitude, J. Ristau method",
	"Derived from MLh magnitude & Sc3 Tutorial methods, J.Salichon, GNS Science New Zealand, J.Becker, Gempa",
	0, 0, 1
);

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// MSmax Amplitude

Amplitude_MSmax::Amplitude_MSmax()
  : Seiscomp::Processing::MagnitudeProcessor(hasDistance(false),
    periodStep(3)
{}

void Amplitude_MSmax::setHint(
    Seiscomp::Processing::WaveformProcessor::ProcessingHint hint,
    double value
) {
    if (hint ==
            Seiscomp::Processing::WaveformProcessor::ProcessingHint::Distance)
    {
        eventDistance = value;
        hasDistance = true;
    }
    /*
     DO NOT DELETE. It is very important to call the parent function.
     AmplitudeProcessor::setHint() is the function which triggers the whole
     amplitude processor process. If the parent setHint() is not called, the
     whole processor will not work correctly.
     */
    AmplitudeProcessor::setHint(hint, value);
}

bool Amplitude_MSmax::computeAmplitude(const Seiscomp::DoubleArray &data,
    size_t i1, size_t i2,
    size_t si1, size_t si2,
    double offset,
    AmplitudeIndex *dt,
    AmplitudeValue *amplitude,
    double *period, double *snr)
{

}

double Amplitude_MSmax::calculateAmp(const Seiscomp::DoubleArray &wave)
{
    double maxMag, maxAmp;
    Seiscomp::Math::Filtering::IIR::ButterworthBandpass<double> *filter = NULL;

    for (int time = 8; time <= 25; time += periodStep)
    {
        double cornerFreq = cornerFrequency(time, eventDistance);
        double highCornerFreq, lowCornerFreq;
        highLowCornerFreq(cornerFreq, time, highCornerFreq, lowCornerFreq);

        // Create copy of waveform data.
        Seiscomp::DoubleArray waveCopy = wave.copy(
                Seiscomp::Array::DataType::DOUBLE);

        // Apply the Butterworth filter.
        filter = new Seiscomp::Math::Filtering::IIR::ButterworthBandpass<double>(
                3, lowCornerFreq, highCornerFreq);
        filter->apply(waveCopy);

        // Get the amp.
        waveCopy.r

        // Run the actual formula.

        // Check if it's the new maximum.

        delete filter;
        filter = NULL;
    }
}

double Amplitude_MSmax::cornerFrequency(double period, double distance)
{

}

void Amplitude_MSmax::highLowCornerFreq(
    double cornerFreq,
    double period,
    double &high,
    double &low)
{

}

// End MSmax Amplitude
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// MSmax Magnitude

Magnitude_MSmax::Magnitude_MSmax()
{

}

Magnitude_MSmax::~Magnitude_MSmax()
{

}

bool Magnitude_MSmax::setup(const Seiscomp::Processing::Settings &settings)
{
    // No setup is required in this processor.
    return true;
}

std::string Magnitude_MSmax::amplitudeType() const
{
    return "MSmax";
}

Seiscomp::Processing::MagnitudeProcessor::Status Magnitude_MSmax::computeMagnitude(
      double amplitude,   // in millimetres
      double period,      // in seconds
      double delta,       // in degrees
      double depth,       // in kilometres
      double &value)
{
    if (period < 8 || period > 25)
    {
        SEISCOMP_DEBUG(
                "MSmax magnitude: Event period of %f falls outside of 8 to 25 seconds range.", period);
        return PeriodOutOfRange;
    }

    double cornerFreq = cornerFrequency(period, delta);
    double highCorner, lowCorner;
    highLowCornerFreq(cornerFreq, period, highCorner, lowCorner);

    // Apply the Butterworth bandpass filter to the amplitude.
    Seiscomp::Math::Filtering::IIR::ButterworthBandpass<double> *p_filter = (
        new Seiscomp::Math::Filtering::IIR::ButterworthBandpass<double>(
            3, lowCorner, highCorner, amplitude, true));
    // Is the amplitude the sample rate?
    double msvmaxAmp;  // Bandpass here.
    p_filter->apply()

    value = (log(msvmaxAmp) + (0.5 * (log(sin(delta)))) +
                (0.0031 * (pow((20.0 / period), 1.8)) * delta) -
                (0.66 * (log(20.0 / period))) - log(cornerFreq) - 0.43);

    value = 4.8;
    return OK;
}

double Magnitude_MSmax::cornerFrequency(double period, double delta)
{
    return 0.6 / (period * sqrt(delta));
}

void Magnitude_MSmax::highLowCornerFreq(
            double cornerFreq,
            double period,
            double &high,
            double &low)
{
    high = 1.0 / (period + cornerFreq);
    low = 1.0 / (period - cornerFreq);
}

// End MSmax Magnitude
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// Register the classes as their respective amplitude and magnitude processor.
REGISTER_AMPLITUDEPROCESSOR(Amplitude_MSmax, GA_MSVMAX_AUS_AMP_TYPE);
REGISTER_MAGNITUDEPROCESSOR(Magnitude_MSmax, GA_MSVMAX_AUS_MAG_TYPE);

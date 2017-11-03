/*
 * File:   msmax.h
 * Author: Geoscience Australia
 */

#ifndef __MSMAX_PLUGIN_H__
#define __MSMAX_PLUGIN_H__

#define GA_MSVMAX_AUS_AMP_TYPE "MSmax"
#define GA_MSVMAX_AUS_MAG_TYPE "MSmax"

#include <seiscomp3/processing/amplitudeprocessor.h>
#include <seiscomp3/processing/magnitudeprocessor.h>

/*
 Calculates the MSmax amplitude. The magnitude calculations are performed in
 this class.
 */
class Amplitude_MSmax : public Seiscomp::Processing::AmplitudeProcessor
{
public:

    /*#########################################################################
                                                PUBLIC METHODS
    #########################################################################*/

    // Constructor. Returns a new Magnitude_MSvmax object.
    Amplitude_MSmax();

    // Destructor
    virtual ~Amplitude_MSmax();

    /*
    Hints that are sent throughout the Seiscomp system. This method is used to
    get the distance to the epicentre.
    @param hint: The type of hint the incoming hint is.
    @param value: The value of the hint.
    */
    virtual void setHint(
        Seiscomp::Processing::WaveformProcessor::ProcessingHint hint,
        double value);

    /*
    Computes the amplitude of data in the range[i1, i2].
    Input:
    @param data: the waveform data.
    @param offset: the computed noise offset.
    @param i1: start index in data (trigger + config.signalBegin).
    @param i2: end index in data (trigger + config.signalEnd).
    @param si1: start index of the amplitude search window.
    @param si2: end index of the amplitude search window.
    NOTE: si1 and si2 are guaranteed to be in range [i1,i2] if
    the default AmplitudeProcessor::process method is called (especially
    when re-implemented).
    Output:
    @param dt: the picked data index (can be a subindex if required)
    the dt.begin and dt.end are the begin/end of the timewindow
    in samples relativ to the picked index. dt.begin and dt.end
    do not need to be in order, they are ordered afterwards
    automatically. The default values for begin/end are 0.
    @param amplitude: the picked amplitude value with optional uncertainties.
    @param period: the period in samples and not seconds (-1 if not calculated).
    @param snr: signal-to-noise ratio.
    */
    virtual bool computeAmplitude(const Seiscomp::DoubleArray &data,
                                  size_t i1, size_t i2,
                                  size_t si1, size_t si2,
                                  double offset,
                                  AmplitudeIndex *dt,
                                  AmplitudeValue *amplitude,
                                  double *period, double *snr);

protected:

    /*#########################################################################
                                       PROTECTED MEMBER VARIABLES
    #########################################################################*/

    /*
    Whether or not distance to epicentre has been calculated. The
    AmplitudeProcessor is not always successful in being able to calculate
    the distance to epicentre for an event. This variable is used to
    determine whether the processor  has or has not calculated it.
    */
    bool        hasDistance;

    /*
     The delta distance, in degrees, of the event origin
    */
    bool        eventDistance;

    /*
    The amount to step by when checking for a new value.
    */
    int         periodStep;

    /*#########################################################################
                                            PROTECTED METHODS
    #########################################################################*/

    /*
    Returns the amplitude value that is used the calculate the  max magnitude
    (as per the calculation). This is done by performing the magnitude
    calculation.
    @param wave: The waveform data.
    @returns: The amplitude associated with the max magnitude.
    */
    double calculateAmp(const Seiscomp::DoubleArray &wave);

    /*
    Calculates the corner filter frequency value for the magnitude calculation.
    @param period: (in seconds).
    @param distance: (in degrees).
    @returns: corner filter frequency.
    */
    double cornerFrequency(double period, double distance);

    /*
    Calculates the high and low corner frequencies for the Butterworth
    bandpass filter that will be applied to the wave.
    @param cornerFreq: The corner filter frequency value.
    @param period: (in seconds).
    @param high: The resultant high corner frequency.
    @param low: The resultant low corner frequency.
     */
    void highLowCornerFreq(
        double cornerFreq,
        double period,
        double &high,
        double &low);
};

/*
Calculates the MSmax magnitude.
*/
class Magnitude_MSmax : public Seiscomp::Processing::MagnitudeProcessor
{
    public:

        /*#####################################################################
                                             PUBLIC METHODS
        #####################################################################*/

        // Constructor. Returns a new Magnitude_MSvmax object.
        Magnitude_MSmax();

        // Configures the plugin to get all the relevant information it
        // requires to perform the calculations.
        bool setup(const Seiscomp::Processing::Settings &settings);

        // Sets the amplitude type that is being used in the calculation.
        // This method is used to specify what amplitude from scamp is used
        // as the amplitude value passed into the computeMagnitude method for
        // this magnitude processor. To set the amplitude type you want, return
        // the name of the amplitude type from scamp that you want to use.
        std::string amplitudeType() const;

        // Calculates the MSvmax magnitude with the given parameters.
        // @param amplitude: Amplitude of the seismic event (in millimetres).
        // @param period: (in seconds).
        // @param delta: (in degrees).
        // @param depth: the depth of the epicentre of the event (in kms).
        // @param value: The result of the calculation.
        Seiscomp::Processing::MagnitudeProcessor::Status computeMagnitude(
		      double amplitude,   // in millimetres
		      double period,      // in seconds
		      double delta,       // in degrees
		      double depth,       // in kilometres
		      double &value);

    protected:

        /*#####################################################################
                                             PROTECTED METHODS
        #####################################################################*/

        // Calculates the corner filter frequency value for the magnitude calculation.
        // @param period: (in seconds).
        // @param delta: (in degrees).
        // @returns: corner filter frequency.
        double cornerFrequency(double period, double delta);

        // Calculates the high and low corner frequencies for the Butterworth
        // bandpass filter that will be applied to the surface wave amplitude.
        // @param cornerFreq: The corner filter frequency value.
        // @param period: (in seconds).
        // @param high: The resultant high corner frequency.
        // @param low: The resultant low corner frequency.
        void highLowCornerFreq(
            double cornerFreq,
            double period,
            double &high,
            double &low);
};

#endif /* __MSMAX_PLUGIN_H__ */

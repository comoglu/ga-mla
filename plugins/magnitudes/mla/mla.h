/*
 * File:   mla.h
 */

#ifndef __MLA_PLUGIN_H__
#define __MLA_PLUGIN_H__

#define GA_ML_AUS_AMP_TYPE "MLa"
#define GA_ML_AUS_MAG_TYPE "MLa"

#include <seiscomp3/processing/amplitudes/MLv.h>
#include <seiscomp3/processing/magnitudeprocessor.h>
#include <seiscomp3/core/plugin.h>
#if SC_API_VERSION < SC_API_VERSION_CHECK(12,0,0)
#include <seiscomp3/geo/geofeatureset.h>
#else
#include <seiscomp3/geo/featureset.h>
#endif

#include <string>
#include <map>

/*
Calculates the MLa amplitude. This amplitude value is used by the MLa magnitude
processor. The amplitude value calculated in this amplitude is the same as the
MLv, except that the returned amplitude is the zero-to-peak value, instead of
the peak-to-peak value of the MLv. The other difference to MLv is that the
max distance is set to 11 degrees instead of 8.
*/
class Amplitude_MLA : public Seiscomp::Processing::AmplitudeProcessor_MLv
{
	DECLARE_SC_CLASS(Amplitude_MLA);
public:

    /*
    Constructor. Extends the base class behaviour by changing the
    maximum distance to 11 degrees.
    */
    explicit Amplitude_MLA(const std::string& type=GA_ML_AUS_AMP_TYPE);

    /*
    Constructor. Extends the base class behaviour by changing the
    maximum distance to 11 degrees.
    */
    Amplitude_MLA(const Seiscomp::Core::Time& trigger, const std::string& type=GA_ML_AUS_AMP_TYPE);

    /*
    Returns the capabilities of the processor. This will be NoCapability.
    @returns: Capability of processor (NoCapability).
    */
    virtual int capabilities() const;

    /*
    Creates the parameter options associated with the capability.
    @param cap: The capability to create parameters for.
    @returns: The list of parameters.
    */
    virtual Seiscomp::Processing::AmplitudeProcessor::IDList
        capabilityParameters(Capability cap) const;

    /*
    Sets the value of a parameter.
    @param cap: The capability to set a parameter for.
    @param value: The value of the capability.
    @returns: Whether a parameter value has been set.
    */
    virtual bool setParameter(Capability cap, const std::string &value);

protected:

    /*
    Computes the amplitude of data in the range[i1, i2].
    Input parameters:
    @param data: the waveform data
    @param offset: the computed noise offset
    @param i1: start index in data (trigger + config.signalBegin)
    @param i2: end index in data (trigger + config.signalEnd)
    @param si1: start index of the amplitude search window
    @param si2: end index of the amplitude search window

    NOTE: si1 and si2 are guaranteed to be in range [i1,i2] if
    the default AmplitudeProcessor::process method is called (especially
    when re-implemented).

    Output parameters:
    @param dt: the picked data index (can be a subindex if required)
        the dt.begin and dt.end are the begin/end of the timewindow
        in samples relativ to the picked index. dt.begin and dt.end
        do not need to be in order, they are ordered afterwards
        automatically. The default values for begin/end are 0.
    @param amplitude: the picked amplitude value with optional uncertainties.
    @param period: the period in samples and not seconds (-1 if not calculated)
    @param snr: signal-to-noise ratio
    */
    bool computeAmplitude(const Seiscomp::DoubleArray &data,
            size_t i1, size_t i2,
            size_t si1, size_t si2,
            double offset,
            AmplitudeIndex *dt, AmplitudeValue *amplitude,
            double *period, double *snr);
};

/*
Calculates the MLa  magnitude. There are 3 geographical
regions defined for this magnitude type: West, East, and South. Each region has
a different formula for calculating the magnitude type associated with it.
The formula used is the one which corresponds with which region the source
information is located within. The region extents are defined by a .bna file.
*/
class Magnitude_MLA : public Seiscomp::Processing::MagnitudeProcessor
{
    // Typedef the member function pointer for the different formulas of magnitude
    // calculation.
    typedef Seiscomp::Processing::MagnitudeProcessor::Status (Magnitude_MLA::*MagCalc)(
        double amplitude,   // in millimetres
        double period,      // in seconds
        double delta,       // in degrees
        double depth,       // in kilometres
        double &value);

    public:

        /*#####################################################################
                                            PUBLIC METHODS
        #####################################################################*/

        // Constructor. Return a new Magnitude_MLA object.
        Magnitude_MLA();

        // Destructor. Cleans up all data created on the heap.
        virtual ~Magnitude_MLA();

        // Configures the plugin to get all the relevant information it
        // requires to perform the calculations.
        bool setup(const Seiscomp::Processing::Settings &settings);

        // Sets the amplitude type that is being used in the calculation.
        // This method is used to specify what amplitude from scamp is used
        // as the amplitude value passed into the computeMagnitude method for
        // this magnitude processor. To set the amplitude type you want, return
        // the name of the amplitude type from scamp that you want to use.
        std::string amplitudeType() const;

        // Calculates the ml magnitude with the given parameters.
        // @param amplitude: Amplitude of the seismic event (in millimetres).
        // @param period: (in seconds).
        // @param delta: (in degrees).
        // @param depth: the depth of the epicentre of the event (in kms).
        // @param hypocenter The optional origin which describes the hypocenter.
        // @param receiver The sensor location meta-data of the receiver.
        // @param value: The result of the calculation.
        Seiscomp::Processing::MagnitudeProcessor::Status computeMagnitude(
              double amplitudeValue,   // in millimetres
#if SC_API_VERSION >= SC_API_VERSION_CHECK(12,0,0)
              const std::string &unit,
#endif
              double period,      // in seconds
#if SC_API_VERSION >= SC_API_VERSION_CHECK(12,0,0)
              double snr,
#endif
              double delta,       // in degrees
              double depth,       // in kilometres
              const Seiscomp::DataModel::Origin *hypocenter,
              const Seiscomp::DataModel::SensorLocation *receiver,
#if SC_API_VERSION >= SC_API_VERSION_CHECK(12,0,0)
              const Seiscomp::DataModel::Amplitude *amplitude,
#endif
              double &value);

        /*#####################################################################
                                            STATIC METHODS
        #####################################################################*/

        /*
        Calculates the distance (the R component of the formula). The distance
         is calculated as SQRT(delta^2, depth^2).

        @param delta: (in degrees).
        @param depth: the depth of the epicentre of the event (in kms).
        @returns The distance (R component) of the calculation formula.
        */
        static double distance(double delta, double depth);

    private:

        /*#####################################################################
                                    PRIVATE MEMBER VARIABLES
        #####################################################################*/

        /*
        The dataset representing the geographical regions which are used to
        determine which formula to use.
        */
        Seiscomp::Geo::GeoFeatureSet        *m_regions;

        /*
        The category for all the GeoFeatures in the m_regions GeoFeatureSet
        from the bna file.
        */
        Seiscomp::Geo::Category                 *m_fileCat;

        /*
        Maps the name of the region to the member function which is used to
        calculate the magnitude for that region.
        */
        std::map<std::string, MagCalc>     m_regionToCalcMap;

        /*#####################################################################
                                                PRIVATE METHODS
        #####################################################################*/

        /*
        Initialises the std::map which ties a region to the member function
        which performs the calculation for that region.
        */
        void setupRegionToCalc();

        /*
        Calculates the ml magnitude for the west region (Western Australia).
        The formula for this region is as follows:
        log10(A)+1.137log10(R)+0.000657*R+0.66

        Where A is the amplitude from vertical component as a maximum
        displacement in millimetres of Wood-Anderson instrument and R is the
        distance in kilometres.

        @param amplitude: Amplitude of the seismic event (in millimetres).
        @param period: (in seconds).
        @param delta: (in degrees).
        @param depth: the depth of the epicentre of the event (in kms).
        @param value: The result of the calculation.*/
        Seiscomp::Processing::MagnitudeProcessor::Status computeMagWest(
		      double amplitude,   // in millimetres
		      double period,      // in seconds
		      double delta,       // in degrees
		      double depth,       // in kilometres
		      double &value);

        /*
        Calculates the ml magnitude for the east region (Eastern Australia).
        The formula for this region is as follows:
        log10(A)+1.34log10(R/100)+0.00055*(R-100)+3.13

        Where A is the amplitude from vertical component as a maximum
        displacement in millimetres of Wood-Anderson instrument and R is the
        distance in kilometres.

        @param amplitude: Amplitude of the seismic event (in millimetres).
        @param period: (in seconds).
        @param delta: (in degrees).
        @param depth: the depth of the epicentre of the event (in kms).
        @param value: The result of the calculation
        */
        Seiscomp::Processing::MagnitudeProcessor::Status computeMagEast(
		      double amplitude,   // in millimetres
		      double period,      // in seconds
		      double delta,       // in degrees
		      double depth,       // in kilometres
		      double &value);

        /*
        Calculates the ml magnitude for the south region (Flinders Ranges).
        The formula for this region is as follows:
        log10(A)+1.1log10(R)+0.0013*R+0.7

        Where A is the amplitude from vertical component as a maximum
        displacement in millimetres of Wood-Anderson instrument and R is the
        distance in kilometres.

        @param amplitude: Amplitude of the seismic event (in millimetres).
        @param period: (in seconds).
        @param delta: (in degrees).
        @param depth: the depth of the epicentre of the event (in kms).
        @param value: The result of the calculation.
        */
        Seiscomp::Processing::MagnitudeProcessor::Status computeMagSouth(
		      double amplitude,   // in millimetres
		      double period,      // in seconds
		      double delta,       // in degrees
		      double depth,       // in kilometres
		      double &value);

};

#endif /* __MLA_PLUGIN_H__ */

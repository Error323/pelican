#ifndef ZENITHCALIBRATER_H
#define ZENITHCALIBRATER_H

/**
 * @file ZenithCalibrater.h
 */

#include "pelican/modules/AbstractModule.h"
#include "pelican/data/Source.h"
#include "pelican/data/CorrectedVisibilityData.h"
#include "pelican/utility/constants.h"
#include <vector>

namespace pelican {

extern "C" {
/// Prototype for LAPACK function zheev():
/// Find all the eigenvalues and vectors of Hermitian matrix A.
void zheev_(const char* jobz, const char* uplo, int* n, complex_t* A, int* lda,
        double* w, complex_t* work, int* lwork, double* rwork, int* info);
}

class ConfigNode;
class VisibilityData;
class ModelVisibilityData;

/**
 * @class ZenithCalibrater
 *
 * @brief
 * Provides basic calibration routines for whole-sky views.
 *
 * @details
 */

class ZenithCalibrater : public AbstractModule
{
    public:
        /// Module constructor.
        ZenithCalibrater(const ConfigNode& config);

        /// Module destructor.
        ~ZenithCalibrater();

        /// Runs the module.
        void run(VisibilityData* rawVis, ModelVisibilityData* modelVis,
                CorrectedVisibilityData* correctedVis);

    private:
        /// Calibration loop.
        void _calibrate(const unsigned nAnt, const unsigned nEigenvaluesUsed,
                const unsigned nIterations, const double tolerance, int lWork,
                complex_t *work, double* rWork, complex_t* Vz, complex_t* Dz);

        /// Computes the complex gains.
        void _computeComplexGains(
                const unsigned nAnt,
                const unsigned /* nEigenvaluesUsed */,
                complex_t* Dz,
                complex_t* modelVis,
                complex_t* Vz,
                complex_t* work,
                int lWork,
                double* rWork,
                complex_t* gains
        );

        /// Builds the corrected visibility set using the complex gains.
        void _buildCorrectedVisibilities(const unsigned nAnt,
                const complex_t* rawVis, const complex_t* antennaGain,
                const complex_t* sigma_n, complex_t* correctedVis);

        /// Sets the diagonals of a square matrix.
        void _setDiagonals(const unsigned size, const complex_t* diagonals,
                complex_t* matrix);

        /// Extract and check data blobs from the data hash
        void _checkDataBlobs(VisibilityData* rawVis,
                ModelVisibilityData* modelVis,
                CorrectedVisibilityData* correctedVis);

        /// Grab configuration options from the config node
        void _getConfiguration(const ConfigNode& config);

    private:
        unsigned _nEigenvaluesUsed;
        double _tolerance;
        unsigned _nIterations;

        std::vector<unsigned> _channels;
        Polarisation _polarisation;

        int _freqRefChannel;                ///< Frequency reference channel
        double _freqRef;                    ///< Reference frequency
        double _freqDelta;                  ///< Frequency delta
};

} // namespace pelican

#endif // ZENITHCALIBRATER_H

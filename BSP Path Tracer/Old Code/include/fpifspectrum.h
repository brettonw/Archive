//
// Floating Point Image Format (FPIF) Reference Implementation (RI)
// spectrum header file.
//

#ifndef FPIF_SPECTRAL_LOCUS_RI_H
#define	FPIF_SPECTRAL_LOCUS_RI_H

#ifndef FPIF_RI_H
#include "fpif.h"
#endif /* !FPIF_RI_H */

class FPIFSpectralLocus {
  public:
    static FPIFFloat	getMinWavelength();
    static FPIFFloat	getMaxWavelength();
    static void		getChromaticity(FPIFFloat wavelengthInNanometers,
				FPIFFloat& x, FPIFFloat& y);
    static FPIFFloat	getXChromaticity(FPIFFloat wavelengthInNanometers);
    static FPIFFloat	getYChromaticity(FPIFFloat wavelengthInNanometers);
    static FPIFFloat	getMinXChromaticity(FPIFFloat yChromaticity);
    static FPIFFloat	getMaxXChromaticity(FPIFFloat yChromaticity);
    static FPIFBoolean	isPhysical(FPIFFloat x, FPIFFloat y);

  private:
    static FPIFSize	getIndex(FPIFFloat);
    static FPIFFloat	getFraction(FPIFFloat);
    static FPIFFloat	interpolateX(FPIFFloat, FPIFSize);

  private:
    static const FPIFFloat locus[][2];
};

#endif /* !FPIF_SPECTRAL_LOCUS_RI_H */

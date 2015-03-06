/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 * Generated by MATLAB(R) 8.3 and the Signal Processing Toolbox 6.21.
 * Generated on: 26-Feb-2015 11:35:04
 */

/*
 * Discrete-Time IIR Filter (real)
 * -------------------------------
 * Filter Structure    : Direct-Form II, Second-Order Sections
 * Number of Sections  : 5
 * Stable              : Yes
 * Linear Phase        : No
 */

/* General type conversion for MATLAB generated C-code  */
//#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * C:\Program Files\MATLAB\R2014a\extern\include\tmwtypes.h 
 */


namespace filterFromMatlab2 {

#define MWSPT_NSEC 11
    const int NL[MWSPT_NSEC] = {1, 3, 1};
    double NUM[MWSPT_NSEC][3] = {
            {
                    0.3031909682064,                 0,                 0
            },
            {
                    1,   -1.999976735989,                 1
            },
            {
                    0.9935198868402,                 0,                 0
            },
            {
                    1,   -1.999990672948,                 1
            },
            {
                    3.227886792779,                 0,                 0
            },
            {
                    1,                -1,                 0
            },
            {
                    1,                 0,                 0
            }
    };
    const int DL[MWSPT_NSEC] = {1, 3, 1};
    double DEN[MWSPT_NSEC][3] = {
            {
                    1,                 0,                 0
            },
            {
                    1,   -1.997760792981,   0.9979200382502
            },
            {
                    1,                 0,                 0
            },
            {
                    1,   -1.986862824199,   0.9872074565499
            },
            {
                    1,                 0,                 0
            },
            {
                    1,   -0.959436644467,                 0
            },
            {
                    1,                 0,                 0
            }
    };


}

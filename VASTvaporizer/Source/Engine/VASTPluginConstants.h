/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

// includes for the project
#include <string>
#include <vector>
#include <math.h>

typedef signed int MYUINT; //CHECK FOR PORTABILITY
#define myFloat float      /* float or float, to set the resolution of the FFT, etc. (the resulting wavetables are always float) */

// More #defines for MacOS/XCode
//CHTS due to OS X 10.6 compatibility
/*
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
*/

//from wavetable
#define C_WAVE_TABLE_SIZE		2048 //2048; //256 works ok
#define C_WAV_FORMAT_WT_SIZE	2048 //serum format has always 2048 samples
#define C_MAX_NUM_POSITIONS		256
#define C_MAX_NUM_FREQS			128 //was 32 
#define C_WTFXTYPE_FM			29

//from m_set
#define C_MAX_SOFTFADE			2000 // 1:20 sec
#define C_OVERSAMPLING_RATIO	4 // not more here due to FIR filter design
#define C_MAX_SAMPLE_RATE		192000 // to prevent dynamic buffer creation
#define C_MAX_BUFFER_SIZE		32768 //8192 //in VSTHost
#define C_MAX_PARALLEL_OSC		24 //per Bank

#define C_MAX_EDITOR_THREADS	1 //parallel threads maximum

#define C_MAX_POLY				32
#define C_MAX_NUM_KEYS			128

#define C_MAX_SAMPLER_VIEWPORT_MARKERS 50

#define C_F127SQUARED			16129.0f

#define M_TWELFTH_ROOT_TWO 1.0594630943592952645618252949463

#ifdef M_PI
	#undef  M_PI	// we want to use our own version of PI, and we don't want the redefine warnings showing up all over the place
#endif
#define M_PI	   3.1415926535897932384626433832795 //redefined here, keep!	
#ifndef M_2PI
	#define M_2PI	   6.283185307179586476925286766559
#endif
#ifdef M_LN2
	#undef  M_LN2	// we want to use our own version 
#endif
#define M_LN2	   0.69314718055994530942 //redefined here, keep!

#ifndef M_SQRT2
	#define M_SQRT2    1.41421356237309504880
#endif
#ifndef M_E
	#define M_E        2.71828182845904523536
#endif
#define M_MODMATRIX_NOT_SET 99999.0
#define M_MODMATRIX_MAX_MIDINOTES 255
#define M_MODMATRIX_MAX_DESTINATIONS 220 //check overflow
#define M_MODMATRIX_MAX_SOURCES 30 //check overflow
#define M_MODMATRIX_MAX_SLOTS 16 //check overflow

#define M_CENTS_LUT_SIZE 1000

#define OUTPUT_MIN_PLUS          1.175494351e-07         /* min positive output */
#define OUTPUT_MIN_MINUS        -1.175494351e-07         /* min negative output */

// a few more constants from student suggestions
//#define  pi 3.1415926535897932384626433832795
#define  sqrt2over2  0.707106781186547524401 // same as M_SQRT1_2

// constants for dealing with overflow or underflow
#define FLT_EPSILON_PLUS      1.192092896e-07         /* smallest such that 1.0+FLT_EPSILON != 1.0 */
#define FLT_EPSILON_MINUS    -1.192092896e-07         /* smallest such that 1.0-FLT_EPSILON != 1.0 */
#define FLT_MIN_PLUS          1.175494351e-38         /* min positive value */
#define FLT_MIN_MINUS        -1.175494351e-38         /* min negative value */

// basic enums
enum {intData, floatData, UINTData, nonData};
// oscillator usage
enum {OSCbankA, OSCbankB, OSCbankC, OSCbankD};

enum OscillatorEditMode { SelectMode, DrawMode, DrawModeFree, OscillatorEditMode_NUM };
enum OscillatorGridMode { NoGrid, Grid4x4, Grid8x8, Grid16x16, Grid32x32, Grid64x64, Grid128x128, OscillatorGridMode_NUM };
enum FreqEditMode { SingleBin, EverySecond, EveryThird, Pile, DragShift, DragStretch, FreqEditMode_NUM };
enum BinMode { ClipBin, NormalizeAll, BinMode_NUM };

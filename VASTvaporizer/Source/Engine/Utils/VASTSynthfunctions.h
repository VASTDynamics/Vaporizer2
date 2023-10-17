/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

// #include "VASTEngineHeader.h" // circular!
#include "../VASTPluginConstants.h"
#include "../VASTEngineHeader.h"
#include "time.h"
#include "VASTLookuptables.h"
#include <cmath>
#include <string>
#include <stdio.h>

#define CONVEX_LIMIT 0.00398107
#define CONCAVE_LIMIT 0.99601893
#define ARC4RANDOMMAX 4294967295 // (2^32 - 1)

#define EXTRACT_BITS(the_val, bits_start, bits_len) ((the_val >> (bits_start - 1)) & ((1 << bits_len) - 1))

enum fu_type
{
	fut_none = 0,
	fut_vastiir6,
	fut_vastsvf,
	fut_vastldf,
	fut_vastkrg12,
	fut_vastcomb,
	fut_vastscream,
	fut_lp12,
	fut_lp24,
	fut_lpmoog,
	fut_hp12,
	fut_hp24,
	fut_bp12,
	fut_br12,
	fut_comb,
	fut_SNH,
	// fut_comb_neg,
	// fut_apf,
	n_fu_type,
};

enum fu_subtype
{
	st_SVF = 0,
	st_vastsvf_lp,
	st_vastsvf_hp,
	st_vastsvf_bp,
	st_vastldf_LPF12,
	st_vastldf_HPF12,
	st_vastldf_LPF24,
	st_vastldf_HPF24,
	st_vastkrg12,
	st_vastkrg12sat,
	st_moog6,
	st_moog12,
	st_moog18,
	st_moog24,
	st_Rough,
	st_Smooth,
	st_Medium, // disabled
	st_iir_bp,
	st_iir_lp,
	st_iir_ap,
	st_iir_hp,
	st_iir_hs,
	st_iir_ls,
	st_iir_no,
	st_iir_pk
};

enum ws_type
{
	wst_none = 0,
	wst_tanh,
	wst_hard,
	wst_asym,
	wst_sinus,
	wst_digi,
	n_ws_type,
};

enum fb_configuration
{
	fb_serial = 0,
	fb_serial2,
	fb_serial3,
	fb_dual,
	fb_dual2,
	fb_stereo,
	fb_ring,
	fb_wide,
	n_fb_configuration,
};

const float c_driftfilter = 0.00001f;
const float c_driftm = 1.f / sqrt(c_driftfilter);
const float c_randdriftfilter_slow = 0.000001f;									 
const float c_randdriftm_slow = 1.f / sqrt(c_randdriftfilter_slow);
const float c_randdriftfilter_fast = 0.001f;
const float c_randdriftm_fast = 1.f / sqrt(c_randdriftfilter_fast);

const int n_cm_coeffs = 8; 
const int n_filter_registers = 5;
const int BLOCK_SIZE = 32;
const int MAX_FILTER_OVERSAMPLING = 4;
const int BLOCK_SIZE_OS = MAX_FILTER_OVERSAMPLING * BLOCK_SIZE;
const float BLOCK_SIZE_OS_INV = 1.f / BLOCK_SIZE_OS;
const int BLOCK_SIZE_QUAD = BLOCK_SIZE >> 2;
const int FIRipol_M = 256;
const int FIRipol_M_bits = 8;
const int FIRipol_N = 12;
const int FIRoffset = FIRipol_N >> 1;
const int FIRipolI16_N = 8;
const int FIRoffsetI16 = FIRipolI16_N >> 1;
const int MAX_FB_COMB = 2048; // must be 2^n
//const int MAX_FB_COMB = 65536; // must be 2^n

inline float i2f_binary_cast(int i)
{
	float* f = (float*)&i;
	return *f;
};

const __m128 m128_mask_signbit = _mm_set1_ps(i2f_binary_cast(0x80000000));
const __m128 m128_mask_absval = _mm_set1_ps(i2f_binary_cast(0x7fffffff));
const __m128 m128_zero = _mm_set1_ps(0.0f);
const __m128 m128_half = _mm_set1_ps(0.5f);
const __m128 m128_one = _mm_set1_ps(1.0f);
const __m128 m128_two = _mm_set1_ps(2.0f);
const __m128 m128_four = _mm_set1_ps(4.0f);
const __m128 m128_1234 = _mm_set_ps(1.f, 2.f, 3.f, 4.f);
const __m128 m128_0123 = _mm_set_ps(0.f, 1.f, 2.f, 3.f);
const __m128 m128_hundred = _mm_set1_ps(100.f);

//for ladder filter
const __m128 m128_minus1 = _mm_set1_ps(-1.f);
const __m128 m128_minus4 = _mm_set1_ps(-4.f);
const __m128 m128_ldfc1 = _mm_set1_ps(0.76923076923f);
const __m128 m128_ldfc2 = _mm_set1_ps(0.23076923076f);

//basic_dsp.h
inline float limit_range(float x, float low, float high)
{
	float result;
	_mm_store_ss(&result,
		_mm_min_ss(_mm_max_ss(_mm_load_ss(&x), _mm_load_ss(&low)), _mm_load_ss(&high)));
	return result;
}

inline __m128 sum_ps_to_ss(__m128 x)
{
	/*__m128 a = _mm_add_ss(x,_mm_shuffle_ps(x,x,_MM_SHUFFLE(0,0,0,1)));
	__m128 b =
	_mm_add_ss(_mm_shuffle_ps(x,x,_MM_SHUFFLE(0,0,0,2)),_mm_shuffle_ps(x,x,_MM_SHUFFLE(0,0,0,3)));
	return _mm_add_ss(a,b);*/
	__m128 a = _mm_add_ps(x, _mm_movehl_ps(x, x));
	return _mm_add_ss(a, _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 1)));
}

inline __m128 max_ps_to_ss(__m128 x)
{
	__m128 a = _mm_max_ss(x, _mm_shuffle_ps(x, x, _MM_SHUFFLE(0, 0, 0, 1)));
	__m128 b = _mm_max_ss(_mm_shuffle_ps(x, x, _MM_SHUFFLE(0, 0, 0, 2)),
		_mm_shuffle_ps(x, x, _MM_SHUFFLE(0, 0, 0, 3)));
	return _mm_max_ss(a, b);
}

inline __m128 hardclip_ss(__m128 x)
{
	const __m128 x_min = _mm_set_ss(-1.0f);
	const __m128 x_max = _mm_set_ss(1.0f);
	return _mm_max_ss(_mm_min_ss(x, x_max), x_min);
}

inline float rcp(float x)
{
	_mm_store_ss(&x, _mm_rcp_ss(_mm_load_ss(&x)));
	return x;
}

inline __m128d hardclip8_sd(__m128d x)
{
	const __m128d x_min = _mm_set_sd(-7.0f);
	const __m128d x_max = _mm_set_sd(8.0f);
	return _mm_max_sd(_mm_min_sd(x, x_max), x_min);
}

inline __m128 hardclip_ps(__m128 x)
{
	const __m128 x_min = _mm_set1_ps(-1.0f);
	const __m128 x_max = _mm_set1_ps(1.0f);
	return _mm_max_ps(_mm_min_ps(x, x_max), x_min);
}

inline float saturate(float f)
{
	__m128 x = _mm_load_ss(&f);
	const __m128 x_min = _mm_set_ss(-1.0f);
	const __m128 x_max = _mm_set_ss(1.0f);
	x = _mm_max_ss(_mm_min_ss(x, x_max), x_min);
	_mm_store_ss(&f, x);
	return f;
}

inline __m128 softclip_ss(__m128 in)
{
	// y = x - (4/27)*x^3,  x € [-1.5 .. 1.5]
	const __m128 a = _mm_set_ss(-4.f / 27.f);

	const __m128 x_min = _mm_set_ss(-1.5f);
	const __m128 x_max = _mm_set_ss(1.5f);

	__m128 x = _mm_max_ss(_mm_min_ss(in, x_max), x_min);
	__m128 xx = _mm_mul_ss(x, x);
	__m128 t = _mm_mul_ss(x, a);
	t = _mm_mul_ss(t, xx);
	t = _mm_add_ss(t, x);

	return t;
}

inline void snap_to_zero_ps(__m128 &x)
{
	const __m128 x_min = _mm_set1_ps(-1.0e-8f);
	const __m128 x_max = _mm_set1_ps(1.0e-8f);
	__m128 mask1 = _mm_cmpge_ps(x, x_max); // ? 0xffffffff : 0
	__m128 mask2 = _mm_cmple_ps(x, x_min); // ? 0xffffffff : 0
	mask1 = _mm_or_ps(mask1, mask2); // 0xffffffff where x > xmax || x < xmin
	x = _mm_and_ps(mask1, x);
}

inline __m128 softclip_ps(__m128 in)
{
	// y = x - (4/27)*x^3,  x € [-1.5 .. 1.5]
	const __m128 a = _mm_set1_ps(-4.f / 27.f);

	const __m128 x_min = _mm_set1_ps(-1.5f);
	const __m128 x_max = _mm_set1_ps(1.5f);

	__m128 x = _mm_max_ps(_mm_min_ps(in, x_max), x_min);
	__m128 xx = _mm_mul_ps(x, x);
	__m128 t = _mm_mul_ps(x, a);
	t = _mm_mul_ps(t, xx);
	t = _mm_add_ps(t, x);

	return t;
}

inline __m128 tanh7_ps(__m128 x)
{
	const __m128 a = _mm_set1_ps(-1.f / 3.f);
	const __m128 b = _mm_set1_ps(2.f / 15.f);
	const __m128 c = _mm_set1_ps(-17.f / 315.f);
	const __m128 one = _mm_set1_ps(1.f);
	__m128 xx = _mm_mul_ps(x, x);
	__m128 y = _mm_add_ps(one, _mm_mul_ps(a, xx));
	__m128 x4 = _mm_mul_ps(xx, xx);
	y = _mm_add_ps(y, _mm_mul_ps(b, x4));
	x4 = _mm_mul_ps(x4, xx);
	y = _mm_add_ps(y, _mm_mul_ps(c, x4));
	return _mm_mul_ps(y, x);
}

inline __m128 tanh7_ss(__m128 x)
{
	const __m128 a = _mm_set1_ps(-1.f / 3.f);
	const __m128 b = _mm_set1_ps(2.f / 15.f);
	const __m128 c = _mm_set1_ps(-17.f / 315.f);
	const __m128 one = _mm_set1_ps(1.f);
	__m128 xx = _mm_mul_ss(x, x);
	__m128 y = _mm_add_ss(one, _mm_mul_ss(a, xx));
	__m128 x4 = _mm_mul_ss(xx, xx);
	y = _mm_add_ss(y, _mm_mul_ss(b, x4));
	x4 = _mm_mul_ss(x4, xx);
	y = _mm_add_ss(y, _mm_mul_ss(c, x4));
	return _mm_mul_ss(y, x);
}

inline __m128 abs_ps(__m128 x)
{
	return _mm_and_ps(x, m128_mask_absval);
}

inline __m128 softclip8_ps(__m128 in)
{
	const __m128 a = _mm_set1_ps(-0.00028935185185f);

	const __m128 x_min = _mm_set1_ps(-12.f);
	const __m128 x_max = _mm_set1_ps(12.f);

	__m128 x = _mm_max_ps(_mm_min_ps(in, x_max), x_min);
	__m128 xx = _mm_mul_ps(x, x);
	__m128 t = _mm_mul_ps(x, a);
	t = _mm_mul_ps(t, xx);
	t = _mm_add_ps(t, x);
	return t;
}

inline double tanh7_double(double x)
{
	const double a = -1 / 3, b = 2 / 15, c = -17 / 315;
	// return tanh(x);
	double xs = x * x;
	double y = 1 + xs * a + xs * xs * b + xs * xs * xs * c;
	// double y = 1 + xs*(a + xs*(b + xs*c));
	// t = xs*c;
	// t += b
	// t *= xs;
	// t += a;
	// t *= xs;
	// t += 1;
	// t *= x;

	return y * x;
}

inline float amp_to_linear(float x)
{
	x = std::max(0.f, x);
	return x * x * x;
}
inline float linear_to_amp(float x)
{
	return powf(limit_range(x, 0.0000000001f, 1.f), 1.f / 3.f);
}
inline float amp_to_db(float x)
{
	return limit_range((float)(6.f * 3.f * log(x) / log(2.f)), -192.f, 96.f);
}
inline float db_to_amp(float x)
{
	return limit_range(powf((10.f / 3.f), 0.05f * x), 0.f, 1.f);
}

inline double sincf(double x)
{
	if (x == 0)
		return 1;
	return (sin(M_PI * x)) / (M_PI * x);
}

inline double sinc(double x)
{
	if (fabs(x) < 0.0000000000000000000001)
		return 1.0;
	return (sin(x) / x);
}

inline double blackman(int i, int n)
{
	// if (i>=n) return 0;
	return (0.42 - 0.5 * cos(2 * M_PI * i / (n - 1)) + 0.08 * cos(4 * M_PI * i / (n - 1)));
}

inline double symmetric_blackman(double i, int n)
{
	// if (i>=n) return 0;
	i -= (n / 2);
	return (0.42 - 0.5 * cos(2 * M_PI * i / (n)) + 0.08 * cos(4 * M_PI * i / (n)));
}

inline double blackman(double i, int n)
{
	// if (i>=n) return 0;
	return (0.42 - 0.5 * cos(2 * M_PI * i / (n - 1)) + 0.08 * cos(4 * M_PI * i / (n - 1)));
}

inline double blackman_harris(int i, int n)
{
	// if (i>=n) return 0;
	return (0.35875 - 0.48829 * cos(2 * M_PI * i / (n - 1)) + 0.14128 * cos(4 * M_PI * i / (n - 1)) -
		0.01168 * cos(6 * M_PI * i / (n - 1)));
}

inline double symmetric_blackman_harris(double i, int n)
{
	// if (i>=n) return 0;
	i -= (n / 2);
	// return (0.42 - 0.5*cos(2*M_PI*i/(n)) + 0.08*cos(4*M_PI*i/(n)));
	return (0.35875 - 0.48829 * cos(2 * M_PI * i / (n)) + 0.14128 * cos(4 * M_PI * i / (n - 1)) -
		0.01168 * cos(6 * M_PI * i / (n)));
}

inline double blackman_harris(double i, int n)
{
	// if (i>=n) return 0;
	return (0.35875 - 0.48829 * cos(2 * M_PI * i / (n - 1)) + 0.14128 * cos(4 * M_PI * i / (n - 1)) -
		0.01168 * cos(6 * M_PI * i / (n - 1)));
}

inline double softclip_double(double in)
{
	const double c0 = (4.0 / 27.0);
	double x = (in > -1.5f) ? in : -1.5;
	x = (x < 1.5f) ? x : 1.5;
	double ax = c0 * x;
	double xx = x * x;
	return x - ax * xx;
}

inline double softclip4_double(double in)
{
	double x = (in > -6.0) ? in : -6.0;
	x = (x < 6.0) ? x : 6.0;
	double ax = 0.0023148148148 * x; // 1.0 / 27*16
	double xx = x * x;
	return x - ax * xx;
}

inline double softclip8_double(double in)
{
	double x = (in > -12.0) ? in : -12.0;
	x = (x < 12.0) ? x : 12.0;
	double ax = 0.00028935185185 * x; // 1.0 / 27*128
	double xx = x * x;
	return x - ax * xx;
}

inline double softclip2_double(double in)
{
	double x = (in > -3.0) ? in : -3.0;
	x = (x < 3.0) ? x : 3.0;
	double ax = 0.185185185185185185 * x; // 1.0 / 27*2
	double xx = x * x;
	return x - ax * xx;
}

inline float megapanL(float pos) // valid range -2 .. 2 (> +- 1 is inverted phase)
{
	if (pos > 2.f)
		pos = 2.f;
	if (pos < -2.f)
		pos = -2.f;
	return (1 - 0.75f * pos - 0.25f * pos * pos);
}

inline float megapanR(float pos)
{
	if (pos > 2.f)
		pos = 2.f;
	if (pos < -2.f)
		pos = -2.f;
	return (1 + 0.75f * pos - 0.25f * pos * pos);
}

//portable_instrinsics.h
#if LINUX
#include <immintrin.h>
#endif

#define vFloat __m128

#define vZero _mm_setzero_ps()

#define vAdd _mm_add_ps
#define vSub _mm_sub_ps
#define vMul _mm_mul_ps
#define vMAdd(a, b, c) _mm_add_ps(_mm_mul_ps(a, b), c)
#define vNMSub(a, b, c) _mm_sub_ps(c, _mm_mul_ps(a, b))

#define vAnd _mm_and_ps
#define vOr _mm_or_ps

#define vCmpGE _mm_cmpge_ps

#define vMax _mm_max_ps
#define vMin _mm_min_ps

#define vLoad _mm_load_ps

inline vFloat vLoad1(float f)
{
	return _mm_load1_ps(&f);
}

inline vFloat vSqrtFast(vFloat v)
{
	return _mm_rcp_ps(_mm_rsqrt_ps(v));
}

inline float vSum(vFloat x)
{
	__m128 a = _mm_add_ps(x, _mm_movehl_ps(x, x));
	a = _mm_add_ss(a, _mm_shuffle_ps(a, a, _MM_SHUFFLE(0, 0, 0, 1)));
	float f;
	_mm_store_ss(&f, a);

	return f;
}

inline void set1f(__m128& m, int i, float f)
{
	*((float*)&m + i) = f;
}

inline float get1f(__m128 m, int i)
{
	return *((float*)&m + i);
}

//CHVAST SURGE


//VAST for chunk save files
/*
inline float strToFloat(char* myString) {
	uint32_t num;
//	sscanf_s(myString, "%x", &num); //slow
	char * p;
	num = strtol(myString, &p, 16);
	if (*p != 0) {
		jassertfalse; //not a hex number - todo error handling
	}
	return *((float*)&num);
}
*/

inline float hexStrToFloat(char* myString) {
	uint32_t num = 0;
	while ((*myString >= '0' && *myString <= '9') || (*myString >= 'a' && *myString <= 'f')) {
		if (*myString >= '0' && *myString <= '9')
			num = (num * 16) + (*myString - '0');
		else
			if (*myString >= 'a' && *myString <= 'f')
				num = (num * 16) + (10 + *myString - 'a');
			else
				jassertfalse; //not a hex number - todo error handling
		++myString;
	}
	return *((float*)&num);
}

inline String floatToHexStr(float value) { //slow!!!
	char arr[9];
	snprintf(arr, 9, "%08x", *(int *)&value);
	return String(arr);
}

static char hextab[] = { '0', '1', '2', '3', '4', '5', '6', '7',
'8', '9' ,'a', 'b', 'c', 'd', 'e', 'f' };

//The function that performs the conversion. Accepts a buffer with "enough space" 
//and populates it with a string of hexadecimal digits. Returns the length in digits
inline String uintToHexStr(unsigned int num)
{
	char buff[9];
	int len = 0, k = 0;
	do//for every 4 bits
	{
		//get the equivalent hex digit
		buff[len] = hextab[num & 0xF];
		len++;
		num >>= 4;
	} while (num != 0);
	//since we get the digits in the wrong order reverse the digits in the buffer
	for (; k<len / 2; k++)
	{//xor swapping
		buff[k] ^= buff[len - k - 1];
		buff[len - k - 1] ^= buff[k];
		buff[k] ^= buff[len - k - 1];
	}
	//null terminate the buffer and return the length in digits
	while (len < 8) {
		buff[len] = '0';
		len++;
	}
	buff[len] = '\0';
	return String(buff);
}

inline double cubicBezierApproximation(double x, double curvy) { //x and curvy 0..1
	//control points 2 and 3 are the same and p2x = 1- p2y
	//https://math.stackexchange.com/questions/26846/is-there-an-explicit-form-for-cubic-b%C3%A9zier-curves	
	// scale axis
	jassert((curvy >= 0.f) && (curvy <= 1.f));
	jassert((x >= 0.f) && (x <= 1.f));
	if (x < 0.f) x = 0.f;
	if (x > 1.f) x = 1.f;
	if (approximatelyEqual(curvy, 0.5))	
		return x; //perfopt
	else if (curvy <= 0.000001)
		return 0.0 ; //CHECK
	else if (curvy >= 0.999999)
		//return (x > 0.001f) ? 1.0 : 0.0; //CHECK
		return 1.0; //CHECK
	else {
		double xscale1 = 2.0 * x  * (1.0 - x) * curvy + x * x;
		double xscale2 = 2.0 * xscale1  * (1.0 - xscale1) * curvy + xscale1 * xscale1;
		double xscale3 = 2.0 * xscale2  * (1.0 - xscale2) * curvy + xscale2 * xscale2;
		double xscale4 = 2.0 * xscale3  * (1.0 - xscale3) * curvy + xscale3 * xscale3;
		xscale4 = 2.0 * xscale4  * (1.0 - xscale4) * curvy + xscale4 * xscale4;
		jassert((xscale4 >= 0.f) && (xscale4 <= 1.f));
		return xscale4;
	}
	return x; //default
}

// returns "plain" or cooked version of fVar
inline float cookVSTGUIVariable(float fMin, float fMax, float fVar)
{
	return (fMax - fMin)*fVar + fMin;
}

// returns normalized version of fVar
inline float convertToVSTGUIVariable(float fMin, float fMax, float fVar)
{
	float fDiff = fMax - fMin;
	float fRawData = (fVar - fMin) / fDiff;
	return fRawData;
}

static inline float fastlog2(float x)
{
	union { float f; unsigned int i; } vx = { x };
	union { unsigned int i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
	float y = vx.i;
	y *= 1.1920928955078125e-7f;

	return y - 124.22551499f
		- 1.498030302f * mx.f
		- 1.72587999f / (0.3520887068f + mx.f);
}

static inline float fastpow2(float p)
{
	float offset = (p < 0) ? 1.0f : 0.0f;
	float clipp = (p < -126) ? -126.0f : p;
	int w = clipp;
	float z = clipp - w + offset;
	union { unsigned int i; float f; } v = { static_cast<unsigned int> ((1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z)) };

	return v.f;
}

inline double fastPrecisePow(double a, double b) { //TS copied from somewhere
	if ((a < 0.0) || (b < 0.0)) return powf(a, b);

	// calculate approximation with fraction of the exponent
	int e = (int)b;
	union {
		double d;
		int x[2];
	} u = { a };
	u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
	u.x[0] = 0;

	// exponentiation by squaring with the exponent's integer part
	// double r = u.d makes everything much slower, not sure why
	double r = 1.0;
	while (e) {
		if (e & 1) {
			r *= a;
		}
		a *= a;
		e >>= 1;
	}

	return r * u.d;
}

inline bool checkFloatBufferZero(AudioSampleBuffer& buffer) {
	for (int i = 0; i < buffer.getNumSamples(); i++) {
		if (buffer.getSample(0, i) != 0.0f) return false;
		if (buffer.getSample(1, i) != 0.0f) return false;
	}
	return true;
}

static inline float fastexp(float p)
{
	return fastpow2(1.442695040f * p);
}

static inline float fasttan(float x)
{
	static const float halfpi = 1.5707963267948966f;
	return sin(x) / sin(x + halfpi);
}

static inline float fasttanh(float p)
{
	//  return -1.0f + 2.0f / (1.0f + fastexp (-2.0f * p));
	return p / (fabs(2 * p) + 3 / (2 + 2 * p * 2 * p));
}

inline double fastPow(double a, double b) {
	union {
		double d;
		int x[2];
	} u = { a };
	u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
	u.x[0] = 0;
	return u.d;
}

inline double semitonesBetweenFrequencies(double dStartFrequency, double dEndFrequency)
{
	return fastlog2(dEndFrequency / dStartFrequency)*12.0;
	//	return Log2(dEndFrequency/dStartFrequency)*12.0;
}

/* pitchShiftMultiplier()

returns a multiplier for a given pitch shift in semitones
to shift octaves,     call pitchShiftMultiplier(octaveValue*12.0);
to shift semitones,   call pitchShiftMultiplier(semitonesValue);
to shift cents,       call pitchShiftMultiplier(centsValue/100.0);
*/
inline double pitchShiftMultiplier(double dPitchShiftSemitones)
{
	if (dPitchShiftSemitones == 0)
		return 1.0;

	// 2^(N/12)
	//	return fastPow(2.0, dPitchShiftSemitones/12.0);
	return pow(2.0, dPitchShiftSemitones / 12.0);
}

/* paramToTimeCents()

converts a time parameter (secs) to TimeCents
See the MMA MIDI DLS Level 1 or 2 Spec

*/
inline double paramToTimeCents(double dParam)
{
	return 1200.0*65536.0*fastlog2(dParam);
}

/* timeCentsToParam()

converts a timeCents to a time (sec) value
See the MMA MIDI DLS Level 1 or 2 Spec

*/
inline double timeCentsToParam(double dTimeCents)
{
	//return fastpow2(dTimeCents/(1200.0*65536.0));
	// return fastpow2(dTimeCents/(1200.0*65536.0));
	return pow(2.0, dTimeCents / (1200.0*65536.0));
}

/* capChargeTransform()

calculates the expDecayTransform of the input

dValue = the unipolar (0 -> 1) value to convert

returns a value from 0 up to 1.0
*/
inline double capChargeTransform(double dValue)
{
	if (dValue == 0.0)
		return 0.0;

	return 1.0 - exp(-6.0*dValue);
}

/* capDischargeTransform()

calculates the capDischargeTransform of the input

dValue = the unipolar (0 -> 1) value to convert
*/
inline double capDischargeTransform(double dValue)
{
	if (dValue == 1.0)
		return 0.0;

	return exp(-6.0*dValue);
}


/* linearIn_dBTransform()

calculates the linearIn_dBTransform of the input

dValue = the unipolar (0 -> 1) value to convert
*/
inline double linearIn_dB_AttackTransform(double dValue)
{
	if (dValue == 0.0)
		return 0.0;

	return pow(10.0, ((1.0 - dValue)*-96.0) / 20.0);
}

/* linearIn_dBTransform()

calculates the linearIn_dBTransform of the input

dValue = the unipolar (0 -> 1) value to convert
*/
inline double linearIn_dB_DecayTransform(double dValue)
{
	if (dValue == 1.0)
		return 0.0;

	return pow(10.0, (dValue*-96.0) / 20.0);
}


/* convexTransform()

calculates the convexTransform of the input

dValue = the unipolar (0 -> 1) value to convert
*/
inline double convexTransform(double dValue)
{
	if (dValue <= CONVEX_LIMIT)
		return 0.0;

	return 1.0 + (5.0 / 12.0)*log10(dValue);
}

/* convexInvertedTransform()

calculates the convexInvertedTransform of the input

dValue = the unipolar (0 -> 1) value to convert
*/
inline double convexInvertedTransform(double dValue)
{
	if (dValue >= CONCAVE_LIMIT)
		return 0.0;

	return 1.0 + (5.0 / 12.0)*log10(1.0 - dValue);
}

/* concaveTransform()

calculates the concaveTransform of the input

dValue = the unipolar (0 -> 1) value to convert
*/
inline double concaveTransform(double dValue)
{
	if (dValue >= CONCAVE_LIMIT)
		return 1.0;

	return -(5.0 / 12.0)*log10(1.0 - dValue);
}

/* concaveInvertedTransform()

calculates the concaveInvertedTransform of the input

dValue = the unipolar (0 -> 1) value to convert
*/
inline double concaveInvertedTransform(double dValue)
{
	if (dValue <= CONVEX_LIMIT)
		return 1.0;

	return -(5.0 / 12.0)*log10(dValue);
}

/* unipolarToBipolar()

calculates the bipolar (-1 -> +1) value from a unipolar (0 -> 1) value

dValue = the value to convert
*/
inline double unipolarToBipolar(double dValue)
{
	return 2.0*dValue - 1.0;
}

/* midiToBipolar()

calculates the bipolar (-1 -> +1) value from a unipolar MIDI (0 -> 127) value

MYUINT uMIDIValue = the MIDI value to convert
*/
inline double midiToBipolar(MYUINT uMIDIValue)
{
	return 2.0*(double)uMIDIValue / 127.0 - 1.0;
}

/* midiToPanValue()

calculates the pan value (-1 -> +1) value from a unipolar MIDI (0 -> 127) value

MYUINT uMIDIValue = the MIDI value to convert
*/
inline double midiToPanValue(MYUINT uMIDIValue)
{
	// see MMA DLS Level 2 Spec; controls are asymmetrical
	if (uMIDIValue == 64)
		return 0.0;
	else if (uMIDIValue <= 1) // 0 or 1
		return -1.0;

	return 2.0*(double)uMIDIValue / 127.0 - 1.0;
}

/* bipolarToUnipolar()

calculates the unipolar (0 -> 1) value from a bipolar (-1 -> +1) value

dValue = the value to convert
*/
inline double bipolarToUnipolar(double dValue)
{
	return 0.5*dValue + 0.5;
}

/* midiToUnipolar()

calculates the unipolar (0 -> 1) value from a MIDI (0 -> 127) value

dValue = the value to convert
*/
inline double midiToUnipolar(MYUINT uMIDIValue)
{
	return (double)uMIDIValue / 127.0;
}

/* unipolarToMIDI()

calculates the MIDI (0 -> 127) value from a unipolar (0 -> 1) value

dValue = the value to convert
*/
inline MYUINT unipolarToMIDI(float fUnipolarValue)
{
	return fUnipolarValue*127.0;
}

/* calculatePanValues()

calculates the left and right pan values

dPanMod = bipolar (-1 -> 1) pan modulation value

returns are via pass-by-reference mechanism
*/
inline void calculatePanValues(double dPanMod, double& dLeftPanValue, double& dRightPanValue)
{
	//dLeftPanValue = fastcos((pi/4.0)*(dPanMod + 1.0));
	//dRightPanValue = fastsin((pi/4.0)*(dPanMod + 1.0));

	dLeftPanValue = cos((M_PI / 4.0)*(dPanMod + 1.0));
	dRightPanValue = sin((M_PI / 4.0)*(dPanMod + 1.0));

	dLeftPanValue = fmax(dLeftPanValue, (double)0.0);
	dLeftPanValue = fmin(dLeftPanValue, (double)1.0);
	dRightPanValue = fmax(dRightPanValue, (double)0.0);
	dRightPanValue = fmin(dRightPanValue, (double)1.0);
}

/* calcValueVoltOctaveExp()

returns the volt/octave exponential value

dLowLimit = low limit of control
dHighLimit = high limit of control
dControlValue = on the range of 0 -> 1
*/
inline double calcValueVoltOctaveExp(double dLowLimit, double dHighLimit, double dControlValue)
{
	double dOctaves = fastlog2(dHighLimit / dLowLimit);
	if (dLowLimit == 0)
		return dControlValue;

	// exp control
	return dLowLimit* powf(2.0, dControlValue*dOctaves);
}

/* calcInverseValueVoltOctaveExp()

returns a value between 0 -> 1 representing the volt/octave control location

dLowLimit = low limit of control
dHighLimit = high limit of control
dControlValue = value between low and high limit
*/
inline double calcInverseValueVoltOctaveExp(double dLowLimit, double dHighLimit, double dControlValue)
{
	double dOctaves = fastlog2(dHighLimit / dLowLimit);
	if (dLowLimit == 0)
		return dControlValue;

	return fastlog2(dControlValue / dLowLimit) / dOctaves;
}

/* use std::log2 instead
inline double log2(double n)
{
	// log(n)/log(2) is log2.  
	return log(n) / log((double)2);
}
*/

/* calcModulatedValueExp()

returns the new exponentially modulated control value

dLowLimit = low limit of control
dHighLimit = high limit of control
dControlValue = current value of control
dModValue = moudulator: on the range of 0 -> 1
bExpUserControl = type of control on GUI; TRUE if user is adjusting a volt/octave control FALSE if linear
*/
inline double calcModulatedValueVoltOctaveExp(double dLowLimit, double dHighLimit, double dControlValue,
	double dModValue, bool bExpUserControl)
{
	dModValue = bipolarToUnipolar(dModValue);

	//	double dOctaves = fastlog2(dHighLimit/dLowLimit);
	double dOctaves = log2(dHighLimit / dLowLimit);

	// location of control relative to 50% point
	double dControlOffset = bExpUserControl ? calcInverseValueVoltOctaveExp(dLowLimit, dHighLimit, dControlValue) - 0.5 : ((dControlValue - dLowLimit) / (dHighLimit - dLowLimit)) - 0.5;

	// exp control
	//	double dExpControlOffset = dLowLimit*fastpow2(dControlOffset*dOctaves);
	double dExpControlOffset = dLowLimit*pow(2.0, dControlOffset*dOctaves);

	// mod exp control
	double dModulatedValueExp = dExpControlOffset*pow(2.0, dModValue*dOctaves);

	// bound
	dModulatedValueExp = fmin(dModulatedValueExp, dHighLimit);
	dModulatedValueExp = fmax(dModulatedValueExp, dLowLimit);

	// return it
	return dModulatedValueExp;
}

/* calcModulatedValueLin()

returns the new linearly modulated control value

dLowLimit = low limit of control
dHighLimit = high limit of control
dControlValue = current value of control
dModValue = moudulator: on the range of 0 -> 1
*/
inline double calcModulatedValueLin(double dLowLimit, double dHighLimit, double dControlValue, double dModValue)
{
	// convert mod value to bipolar
	double dModulator = unipolarToBipolar(dModValue)*(dHighLimit - dLowLimit) / 2.0;// + dLowLimit; 

	double dModulatedValue = dControlValue + dModulator;

	// bound
	dModulatedValue = fmin(dModulatedValue, dHighLimit);
	dModulatedValue = fmax(dModulatedValue, dLowLimit);

	// return it
	return dModulatedValue;
}

inline double doWhiteNoise()
{
	float fNoise = 0.0;

#if defined _WINDOWS || defined _WINDLL || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 36)
	// fNoise is 0 -> 32767.0
	fNoise = (float)rand();

	// normalize and make bipolar
	fNoise = 2.0*(fNoise / 32767.0) - 1.0;
#else
	// fNoise is 0 -> ARC4RANDOMMAX
	fNoise = (float)arc4random();

	// normalize and make bipolar
	fNoise = 2.0*(fNoise / ARC4RANDOMMAX) - 1.0;
#endif

	return fNoise;
}

inline double doPNSequence(MYUINT& uPNRegister)
{
	// get the bits
	MYUINT b0 = EXTRACT_BITS(uPNRegister, 1, 1); // 1 = b0 is FIRST bit from right
	MYUINT b1 = EXTRACT_BITS(uPNRegister, 2, 1); // 1 = b1 is SECOND bit from right
	MYUINT b27 = EXTRACT_BITS(uPNRegister, 28, 1); // 28 = b27 is 28th bit from right
	MYUINT b28 = EXTRACT_BITS(uPNRegister, 29, 1); // 29 = b28 is 29th bit from right

	// form the XOR
	MYUINT b31 = b0^b1^b27^b28;

	// form the mask to OR with the register to load b31
	if (b31 == 1)
		b31 = 0x10000000;

	// shift one bit to right
	uPNRegister >>= 1;

	// set the b31 bit
	uPNRegister |= b31;

	// convert the output into a floating point number, scaled by experimentation
	// to a range of o to +2.0
	float fOut = (float)(uPNRegister) / ((pow((float)2.0, (float)32.0)) / 16.0);

	// shift down to form a result from -1.0 to +1.0
	fOut -= 1.0;

	return fOut;
}

//const double B = 4.0 / (float)M_PI;
//const double Cval = -4.0 / ((float)M_PI*(float)M_PI);
//const double P = 0.225;
// http://devmaster.net/posts/9648/fast-and-accurate-sine-cosine
// input is -pi to +pi
/*
inline double parabolicSine(double x, bool bHighPrecision = true)
{
	double y = B * x + Cval * x * fabs(x);

	if (bHighPrecision)
		y = P * (y * fabs(y) - y) + y;

	return y;
}

const double D = 5.0*(float)M_PI*(float)M_PI;
inline double BhaskaraISine(double x)
{
	double d = fabs(x);
	double sgn = d == 0 ? 1.0 : x / fabs(x);
	return 16.0*x*((float)M_PI - sgn*x) / (D - sgn*4.0*x*((float)M_PI - sgn*x));
}

inline void RotatePoint45CW(double& x1, double& y1)
{
	double x = x1;
	double y = y1;
	x1 = (x + y) / 1.41421356;
	y1 = (y - x) / 1.41421356;
}

inline void RotatePoint45CCW(double& x1, double& y1)
{
	double x = x1;
	double y = y1;
	x1 = (x - y) / 1.41421356;
	y1 = (x + y) / 1.41421356;
}

inline void calculateVectorMixValues(double dOriginX, double dOriginY,
	double dPointX, double dPointY,
	double& dAmag, double& dBmag, double& dCmag, double& dDmag,
	double& dACMix, double& dBDMix, int nCellsPerSide, bool bJoystickCoords = true)
{
	double x = dPointX;
	double y = dPointY;

	double dUnitScalar = 1.41421356*nCellsPerSide; // ncells*sqrt(2)	

	if (!bJoystickCoords)
	{
		//RotatePoint(x, y, +45.0);
		RotatePoint45CCW(x, y);

		x /= dUnitScalar;
		y /= dUnitScalar;
	}

	// these are 0->1 unipolar
	dACMix = (x + 1.0) / 2.0;
	dBDMix = (y + 1.0) / 2.0;

	if (bJoystickCoords)
	{
		dPointX *= dUnitScalar;
		dPointY *= dUnitScalar;
	}

	dPointX += dOriginX;
	dPointY += dOriginY;

	// actual surface is -45 degrees from joystick axes
	if (bJoystickCoords)
		//RotatePoint(dPointX, dPointY, -45.0);
		RotatePoint45CW(dPointX, dPointY);

	dPointX *= 127;
	dPointY *= 127;

	if (dPointX > 127)   
		dPointX = 127;
	else if (dPointX < -128)
		dPointX = -128;

	if (dPointY > 127)   
		dPointY = 127;
	else if (dPointY < -128)
		dPointY = -128;

	dPointX += 127;
	dPointY += 127;

	if (dPointX == 127 && dPointY == 127)
	{
		dAmag = 0.25;
		dBmag = 0.25;
		dCmag = 0.25;
		dDmag = 0.25;
		return;
	}

	// the Korg/Sequential Circuits VS Equations
	dBmag = dPointX*dPointY / 645;   
	dCmag = dPointX*(255 - dPointY) / 645; 
	dDmag = (255 - dPointX)*(255 - dPointY) / 645;
	dAmag = 100.0 - dBmag - dCmag - dDmag;

	// convert from percent
	dAmag /= 100.0;
	dBmag /= 100.0;
	dCmag /= 100.0;
	dDmag /= 100.0;

	// limit to 1.0 from rounding/truncation
	dAmag = fmin(1.0, dAmag);
	dBmag = fmin(1.0, dBmag);
	dCmag = fmin(1.0, dCmag);
	dDmag = fmin(1.0, dDmag);

	dAmag = fmax(0.0, dAmag);
	dBmag = fmax(0.0, dBmag);
	dCmag = fmax(0.0, dCmag);
	dDmag = fmax(0.0, dDmag);
}

inline void calculateVectorJoystickValues(double dPointX, double dPointY,
	double& dAmag, double& dBmag, double& dCmag, double& dDmag,
	double& dACMix, double& dBDMix)
{
	// these are 0->1 unipolar
	dACMix = (dPointX + 1.0) / 2.0;
	dBDMix = (dPointY + 1.0) / 2.0;

	dPointX = 127.0*unipolarToBipolar(dPointX);
	dPointY = 127.0*unipolarToBipolar(dPointY);

	if (dPointX > 127)   
		dPointX = 127;
	else if (dPointX < -128)
		dPointX = -128;

	if (dPointY > 127)   
		dPointY = 127;
	else if (dPointY < -128)
		dPointY = -128;

	dPointX += 127;
	dPointY += 127;

	if (dPointX == 127 && dPointY == 127)
	{
		dAmag = 0.25;
		dBmag = 0.25;
		dCmag = 0.25;
		dDmag = 0.25;
		return;
	}

	// the Korg/Sequential Circuits VS Equations
	dBmag = dPointX*dPointY / 645;   
	dCmag = dPointX*(255 - dPointY) / 645; 
	dDmag = (255 - dPointX)*(255 - dPointY) / 645;
	dAmag = 100.0 - dBmag - dCmag - dDmag;

	// convert from percent
	dAmag /= 100.0;
	dBmag /= 100.0;
	dCmag /= 100.0;
	dDmag /= 100.0;

	// limit to 1.0 from rounding/truncation
	dAmag = fmin(1.0, dAmag);
	dBmag = fmin(1.0, dBmag);
	dCmag = fmin(1.0, dCmag);
	dDmag = fmin(1.0, dDmag);

	dAmag = fmax(0.0, dAmag);
	dBmag = fmax(0.0, dBmag);
	dCmag = fmax(0.0, dCmag);
	dDmag = fmax(0.0, dDmag);
}

// return cell
inline int translateRotorPointToGridCell(double& dOriginX, double& dOriginY,
	double& dRotorX, double& dRotorY,
	int nCellsPerSide,
	bool bJoystickCoords = true)
{
	double dUnitScalar = 1.41421356*nCellsPerSide; // ncells*sqrt(2)	

	dRotorX *= dUnitScalar;
	dRotorY *= dUnitScalar;

	dOriginX *= dUnitScalar;
	dOriginY *= dUnitScalar;

	dRotorX += dOriginX;
	dRotorY += dOriginY;

	// actual surface is -45 degrees from joystick axes
	if (bJoystickCoords)
		// RotatePoint(dRotorX, dRotorY, -45.0);
		RotatePoint45CW(dRotorX, dRotorY);

	double yL = -nCellsPerSide + 2.0*(nCellsPerSide - 1);
	double yH = nCellsPerSide;

	double xL = -nCellsPerSide;
	double xH = nCellsPerSide - 2.0*(nCellsPerSide - 1);

	dRotorX = fmax(dRotorX, (double)-nCellsPerSide);
	dRotorX = fmin(dRotorX, (double)nCellsPerSide);

	dRotorY = fmax(dRotorY, (double)-nCellsPerSide);
	dRotorY = fmin(dRotorY, (double)nCellsPerSide);

	int row = 0;
	int cell = 0;

	// find row
	for (int i = 0; i<nCellsPerSide; i++)
	{
		if (dRotorY >= yL && dRotorY <= yH)
		{
			row = i;
			break;
		}
		yL -= 2.0;
		yH -= 2.0;
	}

	// find col
	for (int i = 0; i<nCellsPerSide; i++)
	{
		if (dRotorX >= xL && dRotorX <= xH)
		{
			cell = row*nCellsPerSide + i;
			break;
		}
		xL += 2.0;
		xH += 2.0;
	}

	// now set origin and translated point
	dOriginX = (xL + xH) / 2.0;
	dOriginY = (yL + yH) / 2.0;

	dRotorX -= dOriginX;
	dRotorY -= dOriginY;

	return cell;
}
*/

/*
// 2-sample PolyBLEP
inline double doPolyBLEP_2(double dModulo, double dInc, double dHeight, bool bRisingEdge, double &dStepsUntilWrap)
{
	// --- return value
	double dPolyBLEP = 0.0;

	// --- LEFT side of discontinuity
	//	   -1 < t < 0
	if (dModulo > 1.0 - dInc)
	{
		// --- calculate distance
		double t = (dModulo - 1.0) / dInc;
		dStepsUntilWrap = (1.0 - dModulo) / dInc; // steps until wrap

		// --- calculate residual
		dPolyBLEP = dHeight*(t*t + 2.0*t + 1.0);
	}
	// --- RIGHT side of discontinuity
	//     0 <= t < 1 
	else if (dModulo < dInc)
	{
		// --- calculate distance
		double t = dModulo / dInc;
		dStepsUntilWrap = (1.0 - dModulo) / dInc; // steps until wrap

		// --- calculate residual
		dPolyBLEP = dHeight*(2.0*t - t*t - 1.0);
	}

	// --- subtract for falling, add for rising edge
	if (!bRisingEdge)
		dPolyBLEP *= -1.0;

	return dPolyBLEP;
}
*/

/*
// --- N Sample BLEP (uPointsPerSide = N)
inline double doBLEP_N(const double* pBLEPTable, double dTableLen, double dModulo, double dInc, double dHeight,
	bool bRisingEdge, double dPointsPerSide, bool bInterpolate = false)
{
	// return value
	double dBLEP = 0.0;

	// t = the distance from the discontinuity
	double t = 0.0;

	// --- find the center of table (discontinuity location)
	double dTableCenter = dTableLen / 2.0 - 1;

	// LEFT side of edge
	// -1 < t < 0
	for (int i = 1; i <= (MYUINT)dPointsPerSide; i++)
	{
		if (dModulo > 1.0 - (double)i*dInc)
		{
			// --- calculate distance
			t = (dModulo - 1.0) / (dPointsPerSide*dInc);

			// --- get index
			float fIndex = (1.0 + t)*dTableCenter;

			// --- truncation
			if (!bInterpolate)
			{
				dBLEP = pBLEPTable[(int)fIndex];
			}
			else
			{
				float fIndex = (1.0 + t)*dTableCenter;
				float frac = fIndex - int(fIndex);
				dBLEP = dLinTerp(0, 1, pBLEPTable[(int)fIndex], pBLEPTable[(int)fIndex + 1], frac);
			}

			// --- subtract for falling, add for rising edge
			if (!bRisingEdge)
				dBLEP *= -1.0;

			return dHeight*dBLEP;
		}
	}

	// RIGHT side of discontinuity
	// 0 <= t < 1 
	for (int i = 1; i <= (MYUINT)dPointsPerSide; i++)
	{
		if (dModulo < (double)i*dInc)
		{
			// calculate distance
			t = dModulo / (dPointsPerSide*dInc);

			// --- get index
			float fIndex = t*dTableCenter + (dTableCenter + 1.0);

			// truncation
			if (bInterpolate)
			{
				dBLEP = pBLEPTable[(int)fIndex];
			}
			else
			{
				float frac = fIndex - int(fIndex);
				if ((int)fIndex + 1 >= dTableLen)
					dBLEP = dLinTerp(0, 1, pBLEPTable[(int)fIndex], pBLEPTable[0], frac);
				else
					dBLEP = dLinTerp(0, 1, pBLEPTable[(int)fIndex], pBLEPTable[(int)fIndex + 1], frac);
			}

			// subtract for falling, add for rising edge
			if (!bRisingEdge)
				dBLEP *= -1.0;

			return dHeight*dBLEP;
		}
	}

	return 0.0;
}
*/

/*
//CHANGE TS
// --- N Sample BLEP (uPointsPerSide = N)
inline double doBLEP_N_Now(const double* pBLEPTable, double dTableLen, double dModulo, double dInc, double dHeight,
	bool bRisingEdge, double dPointsPerSide, bool bInterpolate, int syncPoint, double syncSteps)
{
	// return value
	double dBLEP = 0.0;

	// t = the distance from the discontinuity
	double t = 0.0;

	// --- find the center of table (discontinuity location)
	double dTableCenter = dTableLen / 2.0 - 1;

	// LEFT side of edge
	// -1 < t < 0
	if (syncPoint <= (MYUINT)dPointsPerSide) {
		//		if (dModulo > 1.0 - (double)i*dInc)
		//		{
		// --- calculate distance
		float lWrapModulo = dModulo + (syncSteps - (syncPoint - 1.0)) * dInc; // here it will be synced!
		t = (dModulo - lWrapModulo) / (dPointsPerSide*dInc);

		// --- get index
		float fIndex = (1.0 + t)*dTableCenter;

		// --- truncation
		if (bInterpolate)
		{
			dBLEP = pBLEPTable[(int)fIndex];
		}
		else
		{
			float fIndex = (1.0 + t)*dTableCenter;
			float frac = fIndex - int(fIndex);
			dBLEP = dLinTerp(0, 1, pBLEPTable[(int)fIndex], pBLEPTable[(int)fIndex + 1], frac);
		}

		// --- subtract for falling, add for rising edge
		if (!bRisingEdge)
			dBLEP *= -1.0;

		return dHeight*dBLEP;
		//		}
	}

	// RIGHT side of discontinuity
	// 0 <= t < 1 
	if (syncPoint > (MYUINT)dPointsPerSide) {
		//	if (dModulo < (double)i*dInc)
		//	{
		// calculate distance

		float lWrapModulo = dModulo + (syncSteps - (syncPoint - 1.0)) * dInc; // here it will be synced!
		t = (dModulo - lWrapModulo) / (dPointsPerSide*dInc);

		// --- get index
		//float fIndex = t*dTableCenter + (dTableCenter + 1.0);
		float fIndex = t*dTableCenter + (dTableCenter + 1.0);

		// truncation
		if (bInterpolate)
		{
			dBLEP = pBLEPTable[(int)fIndex];
		}
		else
		{
			float frac = fIndex - int(fIndex);
			if ((int)fIndex + 1 >= dTableLen)
				dBLEP = dLinTerp(0, 1, pBLEPTable[(int)fIndex], pBLEPTable[0], frac);
			else
				dBLEP = dLinTerp(0, 1, pBLEPTable[(int)fIndex], pBLEPTable[(int)fIndex + 1], frac);
		}

		// subtract for falling, add for rising edge
		if (!bRisingEdge)
			dBLEP *= -1.0;

		return dHeight*dBLEP;

		//}
	}
	return 0.0;
}

//CHANGE TS
*/
//

/*
inline double tanh_table(double dX)
{
	if (dX < -1.0) return -1.0;
	if (dX > 1.0) return 1.0;
	if (dX == 0) return 0.0;
	return dTanhTable[(int)(bipolarToUnipolar(dX)*4095.0)];

}

*/

//MiNBLEP code from experimentalscene.com
// MinBLEP Generation Code
// By Daniel Werner
// This Code Is Public Domain

// SINC Function
inline float SINC(float x)
{
	float pix;

	if (x == 0.0f)
		return 1.0f;
	else
	{
		pix = M_PI * x;
		return sinf(pix) / pix;
	}
}

// Generate Blackman Window
inline void BlackmanWindow(int n, float *w)
{
	int m = n - 1;
	int i;
	float f1, f2, fm;

	fm = (float)m;
	for (i = 0; i <= m; i++)
	{
		f1 = (2.0f * M_PI * (float)i) / fm;
		f2 = 2.0f * f1;
		w[i] = 0.42f - (0.5f * cosf(f1)) + (0.08f * cosf(f2));
	}
}

// Discrete Fourier Transform
inline void DFT(int n, float *realTime, float *imagTime, float *realFreq, float *imagFreq)
{
	int k, i;
	float sr, si, p;

	for (k = 0; k < n; k++)
	{
		realFreq[k] = 0.0f;
		imagFreq[k] = 0.0f;
	}

	for (k = 0; k < n; k++)
		for (i = 0; i < n; i++)
		{
			p = (2.0f * M_PI * (float)(k * i)) / n;
			sr = cosf(p);
			si = -sinf(p);
			realFreq[k] += (realTime[i] * sr) - (imagTime[i] * si);
			imagFreq[k] += (realTime[i] * si) + (imagTime[i] * sr);
		}
}

// Inverse Discrete Fourier Transform
inline void InverseDFT(int n, float *realTime, float *imagTime, float *realFreq, float *imagFreq)
{
	int k, i;
	float sr, si, p;

	for (k = 0; k < n; k++)
	{
		realTime[k] = 0.0f;
		imagTime[k] = 0.0f;
	}

	for (k = 0; k < n; k++)
	{
		for (i = 0; i < n; i++)
		{
			p = (2.0f * M_PI * (float)(k * i)) / n;
			sr = cosf(p);
			si = -sinf(p);
			realTime[k] += (realFreq[i] * sr) + (imagFreq[i] * si);
			imagTime[k] += (realFreq[i] * si) - (imagFreq[i] * sr);
		}
		realTime[k] /= n;
		imagTime[k] /= n;
	}
}

// Complex Absolute Value
inline float cabs(float x, float y)
{
	return sqrtf((x * x) + (y * y));
}

// Complex Exponential
inline void cexp(float x, float y, float *zx, float *zy)
{
	float expx;

	expx = expf(x);
	*zx = expx * cosf(y);
	*zy = expx * sinf(y);
}

// Compute Real Cepstrum Of Signal
inline void RealCepstrum(int n, float *signal, float *realCepstrum)
{
	float *realTime, *imagTime, *realFreq, *imagFreq;
	int i;

	realTime = new float[n];
	imagTime = new float[n];
	realFreq = new float[n];
	imagFreq = new float[n];

	// Compose Complex FFT Input

	for (i = 0; i < n; i++)
	{
		realTime[i] = signal[i];
		imagTime[i] = 0.0f;
	}

	// Perform DFT

	DFT(n, realTime, imagTime, realFreq, imagFreq);

	// Calculate Log Of Absolute Value

	for (i = 0; i < n; i++)
	{
		realFreq[i] = logf(cabs(realFreq[i], imagFreq[i]));
		imagFreq[i] = 0.0f;
	}

	// Perform Inverse FFT

	InverseDFT(n, realTime, imagTime, realFreq, imagFreq);

	// Output Real Part Of FFT
	for (i = 0; i < n; i++)
		realCepstrum[i] = realTime[i];

	delete realTime;
	delete imagTime;
	delete realFreq;
	delete imagFreq;
}

// Compute Minimum Phase Reconstruction Of Signal
inline void MinimumPhase(int n, float *realCepstrum, float *minimumPhase)
{
	int i, nd2;
	float *realTime, *imagTime, *realFreq, *imagFreq;

	nd2 = n / 2;
	realTime = new float[n];
	imagTime = new float[n];
	realFreq = new float[n];
	imagFreq = new float[n];

	if ((n % 2) == 1)
	{
		realTime[0] = realCepstrum[0];
		for (i = 1; i < nd2; i++)
			realTime[i] = 2.0f * realCepstrum[i];
		for (i = nd2; i < n; i++)
			realTime[i] = 0.0f;
	}
	else
	{
		realTime[0] = realCepstrum[0];
		for (i = 1; i < nd2; i++)
			realTime[i] = 2.0f * realCepstrum[i];
		realTime[nd2] = realCepstrum[nd2];
		for (i = nd2 + 1; i < n; i++)
			realTime[i] = 0.0f;
	}

	for (i = 0; i < n; i++)
		imagTime[i] = 0.0f;

	DFT(n, realTime, imagTime, realFreq, imagFreq);

	for (i = 0; i < n; i++)
		cexp(realFreq[i], imagFreq[i], &realFreq[i], &imagFreq[i]);

	InverseDFT(n, realTime, imagTime, realFreq, imagFreq);

	for (i = 0; i < n; i++)
		minimumPhase[i] = realTime[i];

	delete realTime;
	delete imagTime;
	delete realFreq;
	delete imagFreq;
}

/*
Function:	lagrpol() implements n-order Lagrange Interpolation

Inputs:		double* x	Pointer to an array containing the x-coordinates of the input values
double* y	Pointer to an array containing the y-coordinates of the input values
int n		The order of the interpolator, this is also the length of the x,y input arrays
double xbar	The x-coorinates whose y-value we want to interpolate

Returns		The interpolated value y at xbar. xbar ideally is between the middle two values in the input array,
but can be anywhere within the limits, which is needed for interpolating the first few or last few samples
in a table with a fixed size.
*/
inline double lagrpol(double* x, double* y, int n, double xbar)
{
	int i, j;
	double fx = 0.0;
	double l = 1.0;
	for (i = 0; i<n; i++)
	{
		l = 1.0;
		for (j = 0; j<n; j++)
		{
			if (j != i)
				l *= (xbar - x[j]) / (x[i] - x[j]);
		}
		fx += l * y[i];
	}
	return (fx);
}

inline float dLinTerp(float x1, float x2, float y1, float y2, float x)
{
	float denom = x2 - x1;
	if (denom == 0)
		return y1; // should not ever happen

				   // calculate decimal position of x
	float dx = (x - x1) / (x2 - x1);

	// use weighted sum method of interpolating
	float result = dx * y2 + (1 - dx)*y1;

	return result;

}


inline bool normalizeBuffer(double* pInputBuffer, MYUINT uBufferSize)
{
	double fMax = 0;

	for (MYUINT j = 0; j<uBufferSize; j++)
	{
		if ((fabs(pInputBuffer[j])) > fMax)
			fMax = fabs(pInputBuffer[j]);
	}

	if (fMax > 0)
	{
		for (MYUINT j = 0; j<uBufferSize; j++)
			pInputBuffer[j] = pInputBuffer[j] / fMax;
	}

	return true;
}

//#endif

// Helper Functions ------------------------------------------------------------- //
// These log/antilog control functions are plotted at this website:
// http://old.fooplot.com/index.php?&type0=0&type1=0&type2=0&type3=0&type4=0&y0=x&y1=%2810%5Ex%20-%201%29/9&y2=log%289*x%2B1%29&y3=log%289*%281-x%29%2B1%29&y4=%2810%5E%281-x%29%20-%201%29/9&r0=&r1=&r2=&r3=&r4=&px0=&px1=&px2=&px3=&px4=&py0=&py1=&py2=&py3=&py4=&smin0=0&smin1=0&smin2=0&smin3=0&smin4=0&smax0=2pi&smax1=2pi&smax2=2pi&smax3=2pi&smax4=2pi&thetamin0=0&thetamin1=0&thetamin2=0&thetamin3=0&thetamin4=0&thetamax0=2pi&thetamax1=2pi&thetamax2=2pi&thetamax3=2pi&thetamax4=2pi&ipw=1&ixmin=0&ixmax=1&iymin=0&iymax=1&igx=0.1&igy=0.1&igl=1&igs=0&iax=1&ila=1&xmin=0&xmax=1&ymin=0&ymax=1
//
// calcLogControl: accepts a float variable from 0.0 to 1.0
// returns a log version from 0.0 to 1.0
/* y = log10(9x + 1)
|
1.0 |					*
|		*
|	*
| *
|*
|*
0.0 ------------------------
0.0					1.0
*/
inline float calcLogControl(float fVar)
{
	return log10(9.0*fVar + 1.0);
}
// ----------------------------------------------------------------------------- //


// calcAntiLogControl: accepts a float variable from 0.0 to 1.0
// returns an anti-log version from 0.0 to 1.0
/* y = (10^x - 1)/9
|
1.0 |					*
|					*
|				  *
|				*
|		*
|*
0.0 ------------------------
0.0					1.0
*/
inline float calcAntiLogControl(float fVar)
{
	return fVar = (pow((float)10.0, fVar) - 1.0) / 9.0;
}
// ----------------------------------------------------------------------------- //

// calcInverseLogControl:accepts a float variable from 0.0 t0 1.0
// returns an anti-log version from 1.0 to 0.0
/* y = log(9*(1-x)+1)
|
1.0 |*
|			*
|				*
|				  *
|					*
|					*
0.0 ------------------------
0.0					1.0
*/
inline float calcInverseLogControl(float fVar)
{
	return log10(9.0*(1.0 - fVar) + 1.0);
}
// ----------------------------------------------------------------------------- //

// calcInverseAntiLogControl: accepts a float variable from 0.0 t0 1.0
// returns an anti-log version from 1.0 to 0.0
/* y = (10^(1-x) - 1)/9
|
1.0 |*
|*
| *
|	*
|		*
|					*
0.0 ------------------------
0.0					1.0
*/
inline float calcInverseAntiLogControl(float fVar)
{
	return (pow((float)10.0, (float)(1.0 - fVar)) - 1.0) / 9.0;
}
// ----------------------------------------------------------------------------- //


// Helpers for advanced users who make their own GUI
inline float calcDisplayVariable(float fMin, float fMax, float fVar)
{
	return (fMax - fMin)*fVar + fMin;
}

// 0->1
inline float calcSliderVariable(float fMin, float fMax, float fVar)
{
	float fDiff = fMax - fMin;
	float fCookedData = (fVar - fMin) / fDiff;
	return fCookedData;
}

//
// String Helpers: these convert to and from strings
//
// user must delete char array when done!
inline char* MYUINTToString(long value)
{
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wformat"
	char* text = new char[33];
	ltoa(value, text, 10);
	return text;
//#pragma clang diagnostic pop
}

inline MYUINT stringToMYUINT(char* p)
{
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wformat"
	return atol(p);
//#pragma clang diagnostic pop
}

// user must delete char array when done!
inline char* intToString(long value)
{
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wformat"
	char* text = new char[33];
	itoa(value, text, 10);
	return text;
//#pragma clang diagnostic pop
}

inline double stringToDouble(char* p)
{
	return atof(p);
}

inline double stringToFloat(char* p)
{
	return atof(p);
}

inline int stringToInt(char* p)
{
	return atol(p);
}

// user must delete char array when done!
inline char* floatToString(float value, int nSigDigits)
{
	char* text = new char[64];
	if (nSigDigits > 32)
		nSigDigits = 32;
	// gcvt (value, nSigDigits, text);
	sprintf(text, "%.*f", nSigDigits, value);

	return text;
}

// user must delete char array when done!
inline char* doubleToString(double value, int nSigDigits)
{
	char* text = new char[64];
	if (nSigDigits > 32)
		nSigDigits = 32;
	// gcvt (value, nSigDigits, text);
	sprintf(text, "%.*f", nSigDigits, value);
	return text;
}


inline char* addStrings(char* pString1, char* pString2)
{
	int n = strlen(pString1);
	int m = strlen(pString2);

	char* p = new char[n + m + 1];
	strcpy(p, pString1);

	return strncat(p, pString2, m);
}


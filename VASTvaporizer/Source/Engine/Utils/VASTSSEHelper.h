/*
VAST Dynamics Audio Software (TM)
*/
#pragma once
#include "../VASTEngineHeader.h"
#ifdef __aarch64__ //arm64
	#include "../../sse2neon.h"
#else
	#include "immintrin.h"
#endif

//http://www.juce.com/forum/topic/simple-sse-wrapper

//http://felix.abecassis.me/2011/09/cpp-getting-started-with-sse/

//http://www.dsharlet.com/2011/12/05/writing-maintainable-simd-intrinsics-using-c-templates/

//http://stackoverflow.com/questions/10102357/sse-much-slower-than-regular-function


union vec4 {
	struct { float x, y, z, w; };
	__m128 sse{ 0.f, 0.f, 0.f, 0.f };

	inline vec4(void) {}
	inline vec4(__m128 val) { sse = val; }
	inline void horizontal_sum() {
		sse = _mm_hadd_ps(sse, sse);
		sse = _mm_hadd_ps(sse, sse);
	}
	inline void operator=(float a) { sse = _mm_load1_ps(&a); }
	inline void operator=(float *a) { sse = _mm_load_ps(a); }
	inline vec4(float *a) { (*this) = a; }
	inline vec4(float a) { (*this) = a; }
};

inline vec4 operator+(const vec4 &a, const vec4 &b) { vec4 result; result.sse = _mm_add_ps(a.sse, b.sse); return result; }
inline vec4 operator-(const vec4 &a, const vec4 &b) { vec4 result; result.sse = _mm_sub_ps(a.sse, b.sse); return result; }
inline vec4 operator*(const vec4 &a, const vec4 &b) { vec4 result; result.sse = _mm_mul_ps(a.sse, b.sse); return result; }
inline vec4 operator/(const vec4 &a, const vec4 &b) { vec4 result; result.sse = _mm_div_ps(a.sse, b.sse); return result; }
inline vec4 operator++(const vec4 &a)
{
	vec4 B;
	vec4 result;
	B.x = 1.0f; B.y = 1.0f; B.z = 1.0f; B.w = 1.0f;
	result.sse = _mm_add_ps(a.sse, B.sse);
	return result;
}


/*
_declspec(align(16)) class vec4 {
public:
inline vec4(void) {}
inline vec4(__m128 val) { v = val; }

__m128 v;    // SSE 4 x float vector

inline void operator=(float *a) { v = _mm_load_ps(a); }
inline vec4(float *a) { (*this) = a; }
inline vec4(float a) { (*this) = a; }

inline void operator=(float a) { v = _mm_load1_ps(&a); }
inline void horizontal_sum() {
v = _mm_hadd_ps(v, v);
v = _mm_hadd_ps(v, v);
}
inline float vectorGetByIndex(unsigned int i) {
return v.m128_f32[i];
}
};
*/


/*

#ifndef __SseHelper__h
#define __SseHelper__h

#include "immintrin.h"

// SSE vector.
//#if JUCE_WINDOWS
_declspec(align(16)) class vec2
//#else
//__attribute__((aligned(16))) class vec2
//#endif
{
public:
typedef float T;
enum { N = 2 };

__m128d v;

vec2() { }
vec2(float x) : v(_mm_set1_pd(x)) { }
vec2(float p1, float p2) : v(_mm_set_pd(p1, p2)) { }
vec2(float *px) : v(_mm_load_pd(px)) { }
vec2(__m128d v) : v(v) { }
};

inline vec2 operator + (vec2 &l, vec2 &r)
{
return vec2(_mm_add_ps(l.v, r.v));
}
inline vec4 operator - (const vec4 &l, const vec4 &r)
{
return vec4(_mm_sub_ps(l.v, r.v));
}
inline vec4 operator * (const vec4 &l, const vec4 &r)
{
return vec4(_mm_mul_ps(l.v, r.v));
}
inline vec4 operator / (const vec4 &l, const vec4 &r)
{
return vec4(_mm_div_ps(l.v, r.v));
}

_declspec(align(16)) class vec4
{
public:
__m128 v;
vec4(__m128 v) : v(v) { }
};

inline vec4 operator + (vec4 &l, vec4 &r)
{
return vec4(_mm_add_ps(l.v, r.v));
}
inline vec4 operator - (const vec4 &l, const vec4 &r)
{
return vec4(_mm_sub_ps(l.v, r.v));
}
inline vec4 operator * (const vec4 &l, const vec4 &r)
{
return vec4(_mm_mul_ps(l.v, r.v));
}
inline vec4 operator / (const vec4 &l, const vec4 &r)
{
return vec4(_mm_div_ps(l.v, r.v));
}

#endif

*/


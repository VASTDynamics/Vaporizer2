/*
VAST Dynamics Software
*/

#ifndef VASTENGINEHEADER_H_INCLUDED
#define VASTENGINEHEADER_H_INCLUDED

#include <assert.h>
#include <math.h>
//#include <stdlib.h> // for memory leak detection
#ifndef _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#endif
#ifndef _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS
#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS
#endif
// For WIN vs MacOS
// XCode requires these be defined for compatibility
#if defined _WINDOWS || defined _WINDLL
    #include <windows.h>
	typedef __int64 Long64_t; //Portable signed long integer 8 bytes
	typedef unsigned __int64 ULong64_t; //Portable unsigned long integer 8 bytes

#else // MacOS
	typedef unsigned int        UINT;
	typedef unsigned long       DWORD;
	typedef unsigned char		UCHAR;
	typedef unsigned char       BYTE;

	typedef long long Long64_t; //Portable signed long integer 8 bytes
	typedef unsigned long long ULong64_t;//Portable unsigned long integer 8 byte
#endif							 
										 
// platform independent assert
#ifdef _WINDOWS
	#ifndef v_ANALYZER_NORETURN
		#define v_ANALYZER_NORETURN
	#endif
	#define vMACRO_WITH_FORCED_SEMICOLON(x) \
		   __pragma(warning(push)) \
		   __pragma(warning(disable:4127)) \
		   do { x } while (false) \
		   __pragma(warning(pop))
	#define vassertfalse              vMACRO_WITH_FORCED_SEMICOLON (if (IsDebuggerPresent()) __debugbreak(); v_ANALYZER_NORETURN)
	#define vassert(expression)       vMACRO_WITH_FORCED_SEMICOLON (if (! (expression)) jassertfalse;)
#else
	#define vassertfalse   // do nothing     
    #define vassert(expression)  // do nothing
#endif						   

//portable UNUSED macro
#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

#include <float.h>
//#define MAXFLOAT FLT_MAX //redefined if set here
#ifndef _WINDOWS
    #define MAXUINT 0xFFFFFFFF
#endif

#include "JuceHeader.h"
	#ifdef _DEBUG   
		#ifdef _WINDOWS
			#ifndef DBG_NEW      
				#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
				#define new DBG_NEW  
			#endif  // _DEBUG

			#define _CRTDBG_MAP_ALLOC
			#include <crtdbg.h>
		#endif
	#endif  // _DEBUG
#endif  // VASTENGINEHEADER_H_INCLUDED

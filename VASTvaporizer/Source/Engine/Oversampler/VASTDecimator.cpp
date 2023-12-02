/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTDecimator.h"
#include <stdio.h>

#ifdef __aarch64__ //arm64
    #include "../../sse2neon.h"
#else
    #if defined _MACOSX || defined JUCE_LINUX
        #include <pmmintrin.h>
        #include <cstddef>
    #else
        #include "../Utils/VASTSSEHelper.h"
        #include "immintrin.h"
    #endif
#endif

#ifdef DEBUG
	#define _CRTDBG_MAP_ALLOC
#endif

CDecimator::CDecimator(void)
{
	//FACTOR4
#if defined _MACOSX || defined JUCE_LINUX
	m_pIRBuffer = new float[C_FACTOR4_IR_LENGTH];
#else
	m_pIRBuffer = (float*)_aligned_malloc(C_FACTOR4_IR_LENGTH*sizeof(float), 16);
#endif
	// flush buffer
	memset(m_pIRBuffer, 0, C_FACTOR4_IR_LENGTH*sizeof(float));

#if defined _MACOSX || defined JUCE_LINUX
	m_pLeftInputBuffer = new float[C_FACTOR4_IR_LENGTH * 2];
	m_pRightInputBuffer = new float[C_FACTOR4_IR_LENGTH * 2];
#else
	m_pLeftInputBuffer = (float*)_aligned_malloc(C_FACTOR4_IR_LENGTH * 2 * sizeof(float), 16);
	m_pRightInputBuffer = (float*)_aligned_malloc(C_FACTOR4_IR_LENGTH * 2 * sizeof(float), 16);
#endif

	// flush x buffers
	memset(m_pLeftInputBuffer, 0, C_FACTOR4_IR_LENGTH * 2 * sizeof(float));
	memset(m_pRightInputBuffer, 0, C_FACTOR4_IR_LENGTH * 2 * sizeof(float));

	// reset all indices
	m_nReadIndexDL = 0;
	m_nReadIndexH = 0;
	m_nWriteIndex = 0;

	//FACTOR2
	M = 6;
	steep = true;
	factor2DecimatorLoadCoefficients();
	reset();
}


CDecimator::~CDecimator(void)
{
	// free up our input buffers
#if defined _MACOSX || defined JUCE_LINUX
	if (m_pLeftInputBuffer) delete[] m_pLeftInputBuffer;
	if (m_pRightInputBuffer) delete[] m_pRightInputBuffer;
	if (m_pIRBuffer) delete[] m_pIRBuffer;
#else
	if (m_pLeftInputBuffer) _aligned_free(m_pLeftInputBuffer);
	if (m_pRightInputBuffer) _aligned_free(m_pRightInputBuffer);
	if (m_pIRBuffer) _aligned_free(m_pIRBuffer);
#endif
}

void CDecimator::init(int L, int FIRLen, float *pIRBuffer)
{
	// load it
	for (int i = 0; i<C_FACTOR4_IR_LENGTH; i++)
	{
		m_pIRBuffer[i] = pIRBuffer[i];
	}

	// flush x buffers
	memset(m_pLeftInputBuffer, 0, C_FACTOR4_IR_LENGTH * 2 * sizeof(float));
	memset(m_pRightInputBuffer, 0, C_FACTOR4_IR_LENGTH * 2 * sizeof(float));

	// reset all indices
	m_nReadIndexDL = 0;
	m_nReadIndexH = 0;
	m_nWriteIndex = 0;
}

void CDecimator::reset()
{
	//FACTOR 4
	// flush x buffers
	memset(m_pLeftInputBuffer, 0, C_FACTOR4_IR_LENGTH*2*sizeof(float));
	memset(m_pRightInputBuffer, 0, C_FACTOR4_IR_LENGTH*2*sizeof(float));

	// reset all indices
	m_nReadIndexDL = 0;
	m_nReadIndexH = 0;
	m_nWriteIndex = 0;

	//FACTOR 2
	factor2DecimatorReset();
}

/*DECIMATOR FOR 2-TIMES OVERSAMPLING*/

const unsigned int hr_BLOCK_SIZE = 256;
alignas(16) const __m128 half = _mm_set_ps1(0.5f);

void CDecimator::decimateSamples2(float* pLeftDeciBuffer, float* pRightDeciBuffer, int numSamples, float* outL, float* outR)
{
	jassert((numSamples % 2) == 0);
	jassert(numSamples < hr_BLOCK_SIZE * 4);

    alignas(16) __m128 L [hr_BLOCK_SIZE];
    alignas(16) __m128 R [hr_BLOCK_SIZE];
    for (int i = 0; i < (numSamples / 4) + 1; i++) {
        L[i] = _mm_loadu_ps(pLeftDeciBuffer + i * 4);
        R[i] = _mm_loadu_ps(pRightDeciBuffer + i * 4);
    }
    
	alignas(16) __m128 o[hr_BLOCK_SIZE];
	// fill the buffer with interleaved stereo samples
	for (int k = 0; k < numSamples; k += 4)	{
		//[o3,o2,o1,o0] = [L0,L0,R0,R0]
		o[k] = _mm_shuffle_ps(L[k >> 2], R[k >> 2], _MM_SHUFFLE(0, 0, 0, 0));
		o[k + 1] = _mm_shuffle_ps(L[k >> 2], R[k >> 2], _MM_SHUFFLE(1, 1, 1, 1));
		o[k + 2] = _mm_shuffle_ps(L[k >> 2], R[k >> 2], _MM_SHUFFLE(2, 2, 2, 2));
		o[k + 3] = _mm_shuffle_ps(L[k >> 2], R[k >> 2], _MM_SHUFFLE(3, 3, 3, 3));
	}

	// process filters
	for (int j = 0; j < M; j++)
	{
		alignas(16) __m128 tx0 = vx0[j];
		alignas(16) __m128 tx1 = vx1[j];
		alignas(16) __m128 tx2 = vx2[j];
		alignas(16) __m128 ty0 = vy0[j];
		alignas(16) __m128 ty1 = vy1[j];
		alignas(16) __m128 ty2 = vy2[j];
		alignas(16) __m128 ta = va[j];

		for (int k = 0; k < numSamples; k += 2)
		{
			// shuffle inputs
			tx2 = tx1;
			tx1 = tx0;
			tx0 = o[k];
			// shuffle outputs
			ty2 = ty1;
			ty1 = ty0;
			// allpass filter 1
			ty0 = _mm_add_ps(tx2, _mm_mul_ps(_mm_sub_ps(tx0, ty2), ta));
			o[k] = ty0;

			// shuffle inputs
			tx2 = tx1;
			tx1 = tx0;
			tx0 = o[k + 1];
			// shuffle outputs
			ty2 = ty1;
			ty1 = ty0;
			// allpass filter 1
			ty0 = _mm_add_ps(tx2, _mm_mul_ps(_mm_sub_ps(tx0, ty2), ta));
			o[k + 1] = ty0;
		}
		vx0[j] = tx0;
		vx1[j] = tx1;
		vx2[j] = tx2;
		vy0[j] = ty0;
		vy1[j] = ty1;
		vy2[j] = ty2;
	}

	alignas(16) __m128 aR = _mm_setzero_ps();
	alignas(16) __m128 bR = _mm_setzero_ps();
	alignas(16) __m128 cR = _mm_setzero_ps();
	alignas(16) __m128 dR = _mm_setzero_ps();
    
	for (int k = 0; k < numSamples; k += 8)
	{
		/*	const double output=(filter_a.process(input)+oldout)*0.5;
		oldout=filter_b.process(input);*/

		alignas(16) __m128 tL0 = _mm_shuffle_ps(o[k], o[k], _MM_SHUFFLE(1, 1, 1, 1));
		alignas(16) __m128 tR0 = _mm_shuffle_ps(o[k], o[k], _MM_SHUFFLE(3, 3, 3, 3));
		alignas(16) __m128 aL = _mm_add_ss(tL0, o[k + 1]);
		aR = _mm_movehl_ps(aR, o[k + 1]);
		aR = _mm_add_ss(aR, tR0);

		tL0 = _mm_shuffle_ps(o[k + 2], o[k + 2], _MM_SHUFFLE(1, 1, 1, 1));
		tR0 = _mm_shuffle_ps(o[k + 2], o[k + 2], _MM_SHUFFLE(3, 3, 3, 3));
		alignas(16) __m128 bL = _mm_add_ss(tL0, o[k + 3]);
		bR = _mm_movehl_ps(aR, o[k + 3]);
		bR = _mm_add_ss(bR, tR0);

		tL0 = _mm_shuffle_ps(o[k + 4], o[k + 4], _MM_SHUFFLE(1, 1, 1, 1));
		tR0 = _mm_shuffle_ps(o[k + 4], o[k + 4], _MM_SHUFFLE(3, 3, 3, 3));
		alignas(16) __m128 cL = _mm_add_ss(tL0, o[k + 5]);
		cR = _mm_movehl_ps(cR, o[k + 5]);
		cR = _mm_add_ss(cR, tR0);

		tL0 = _mm_shuffle_ps(o[k + 6], o[k + 6], _MM_SHUFFLE(1, 1, 1, 1));
		tR0 = _mm_shuffle_ps(o[k + 6], o[k + 6], _MM_SHUFFLE(3, 3, 3, 3));
		alignas(16) __m128 dL = _mm_add_ss(tL0, o[k + 7]);
		dR = _mm_movehl_ps(dR, o[k + 7]);
		dR = _mm_add_ss(dR, tR0);

		aL = _mm_movelh_ps(aL, bL);
		cL = _mm_movelh_ps(cL, dL);
		aR = _mm_movelh_ps(aR, bR);
		cR = _mm_movelh_ps(cR, dR);
        
		L[k >> 3] = _mm_shuffle_ps(aL, cL, _MM_SHUFFLE(2, 0, 2, 0));
		R[k >> 3] = _mm_shuffle_ps(aR, cR, _MM_SHUFFLE(2, 0, 2, 0));

		// optional: *=0.5;
		L[k >> 3] = _mm_mul_ps(L[k >> 3], half);
		R[k >> 3] = _mm_mul_ps(R[k >> 3], half);
        
        //CHECK for performance
        if (outL)
            _mm_storeu_ps(outL + k / 2, L[k / 8]);
        else
            _mm_storeu_ps(pLeftDeciBuffer + k / 2, L[k / 8]);
        if (outR)
           _mm_storeu_ps(outR + k / 2, R[k / 8]);
        else
            _mm_storeu_ps(pRightDeciBuffer + k / 2, R[k / 8]);
	}
}

void CDecimator::factor2DecimatorReset()
{
	for (int i = 0; i < M; i++)
	{
		vx0[i] = _mm_setzero_ps();
		vx1[i] = _mm_setzero_ps();
		vx2[i] = _mm_setzero_ps();
		vy0[i] = _mm_setzero_ps();
		vy1[i] = _mm_setzero_ps();
		vy2[i] = _mm_setzero_ps();
	}
	oldout = _mm_setzero_ps();
}

void CDecimator::factor2DecimatorSetCoefficients(float* cA, float* cB)
{
	for (int i = 0; i < M; i++)
	{
		// va[i] = _mm_set_ps(cA[i],cB[i],cA[i],cB[i]);
		va[i] = _mm_set_ps(cB[i], cA[i], cB[i], cA[i]);
	}
}

void CDecimator::factor2DecimatorLoadCoefficients()
{
	for (int i = 0; i < M; i++)
	{
		va[i] = _mm_setzero_ps();
	}

	int order = M << 1;
	if (steep)
	{
		if (order == 12) // rejection=104dB, transition band=0.01
		{
			float a_coefficients[6] = { 0.036681502163648017f, 0.2746317593794541f,
									   0.56109896978791948f,  0.769741833862266f,
									   0.8922608180038789f,   0.962094548378084f };

			float b_coefficients[6] = { 0.13654762463195771f, 0.42313861743656667f,
									   0.6775400499741616f,  0.839889624849638f,
									   0.9315419599631839f,  0.9878163707328971f };

			factor2DecimatorSetCoefficients(a_coefficients, b_coefficients);
		}
		else if (order == 10) // rejection=86dB, transition band=0.01
		{
			float a_coefficients[5] = { 0.051457617441190984f, 0.35978656070567017f,
									   0.6725475931034693f, 0.8590884928249939f, 0.9540209867860787f };

			float b_coefficients[5] = { 0.18621906251989334f, 0.529951372847964f, 0.7810257527489514f,
									   0.9141815687605308f, 0.985475023014907f };

			factor2DecimatorSetCoefficients(a_coefficients, b_coefficients);
		}
		else if (order == 8) // rejection=69dB, transition band=0.01
		{
			float a_coefficients[4] = { 0.07711507983241622f, 0.4820706250610472f, 0.7968204713315797f,
									   0.9412514277740471f };

			float b_coefficients[4] = { 0.2659685265210946f, 0.6651041532634957f, 0.8841015085506159f,
									   0.9820054141886075f };

			factor2DecimatorSetCoefficients(a_coefficients, b_coefficients);
		}
		else if (order == 6) // rejection=51dB, transition band=0.01
		{
			float a_coefficients[3] = { 0.1271414136264853f, 0.6528245886369117f, 0.9176942834328115f };

			float b_coefficients[3] = { 0.40056789819445626f, 0.8204163891923343f, 0.9763114515836773f };

			factor2DecimatorSetCoefficients(a_coefficients, b_coefficients);
		}
		else if (order == 4) // rejection=53dB,transition band=0.05
		{
			float a_coefficients[2] = { 0.12073211751675449f, 0.6632020224193995f };

			float b_coefficients[2] = { 0.3903621872345006f, 0.890786832653497f };

			factor2DecimatorSetCoefficients(a_coefficients, b_coefficients);
		}

		else // order=2, rejection=36dB, transition band=0.1
		{
			float a_coefficients = 0.23647102099689224f;
			float b_coefficients = 0.7145421497126001f;

			factor2DecimatorSetCoefficients(&a_coefficients, &b_coefficients);
		}
	}
	else // softer slopes, more attenuation and less stopband ripple
	{
		if (order == 12) // rejection=150dB, transition band=0.05
		{
			float a_coefficients[6] = { 0.01677466677723562f, 0.13902148819717805f,
									   0.3325011117394731f,  0.53766105314488f,
									   0.7214184024215805f,  0.8821858402078155f };

			float b_coefficients[6] = { 0.06501319274445962f, 0.23094129990840923f,
									   0.4364942348420355f,  0.6329609551399348f,
									   0.80378086794111226f, 0.9599687404800694f };

			factor2DecimatorSetCoefficients(a_coefficients, b_coefficients);
		}
		else if (order == 10) // rejection=133dB, transition band=0.05
		{
			float a_coefficients[5] = { 0.02366831419883467f, 0.18989476227180174f,
									   0.43157318062118555f, 0.6632020224193995f, 0.860015542499582f };

			float b_coefficients[5] = { 0.09056555904993387f, 0.3078575723749043f, 0.5516782402507934f,
									   0.7652146863779808f, 0.95247728378667541f };

			factor2DecimatorSetCoefficients(a_coefficients, b_coefficients);
		}
		else if (order == 8) // rejection=106dB, transition band=0.05
		{
			float a_coefficients[4] = { 0.03583278843106211f, 0.2720401433964576f, 0.5720571972357003f,
									   0.827124761997324f };

			float b_coefficients[4] = { 0.1340901419430669f, 0.4243248712718685f, 0.7062921421386394f,
									   0.9415030941737551f };

			factor2DecimatorSetCoefficients(a_coefficients, b_coefficients);
		}
		else if (order == 6) // rejection=80dB, transition band=0.05
		{
			float a_coefficients[3] = { 0.06029739095712437f, 0.4125907203610563f, 0.7727156537429234f };

			float b_coefficients[3] = { 0.21597144456092948f, 0.6043586264658363f, 0.9238861386532906f };

			factor2DecimatorSetCoefficients(a_coefficients, b_coefficients);
		}
		else if (order == 4) // rejection=70dB,transition band=0.1
		{
			float a_coefficients[2] = { 0.07986642623635751f, 0.5453536510711322f };

			float b_coefficients[2] = { 0.28382934487410993f, 0.8344118914807379f };

			factor2DecimatorSetCoefficients(a_coefficients, b_coefficients);
		}

		else // order=2, rejection=36dB, transition band=0.1
		{
			float a_coefficients = 0.23647102099689224f;
			float b_coefficients = 0.7145421497126001f;

			factor2DecimatorSetCoefficients(&a_coefficients, &b_coefficients);
		}
	}
}

/*DECIMATOR FOR 4-TIMES OVERSAMPLING*/

/* L-Point Decimator
Input: Left and Right Input Buffers with L samples per buffer
Output:	Left and Right Input Samples ynL, ynR

This function loops L times, decimating the outputs and returning the LAST output calculated

CALLER SUPPLIES INPUT BUFFERS!
*/
void CDecimator::decimateSamples4(float* pLeftDeciBuffer, float* pRightDeciBuffer, float& ynL, float& ynR)
{
	if (!pLeftDeciBuffer || !pRightDeciBuffer)
		return;

	for (int i = 0; i<C_FACTOR4_OVERSAMPLE; i++)
	{
		float temp_ynL = 0;
		float temp_ynR = 0;

		// decimate next sample
		//
		// ynL and ynR are valid (and returned) after the last call to decimateNextOutputSample()
		this->decimateNextOutputSample4(pLeftDeciBuffer[i], pRightDeciBuffer[i], temp_ynL, temp_ynR, i);

		if (i == 0)
		{
			ynL = temp_ynL;
			ynR = temp_ynR;
		}
	}
}

bool CDecimator::decimateNextOutputSample4(float xnL, float xnR, float& fLeftOutput, float& fRightOutput, int i)
{
	// read inputs
	//
	// left
	m_pLeftInputBuffer[C_FACTOR4_IR_LENGTH - m_nWriteIndex - 1] = xnL;
	m_pLeftInputBuffer[2*C_FACTOR4_IR_LENGTH - m_nWriteIndex - 1] = xnL; //make a vecorizable structure - store it right from the beginning like this

	// right
	m_pRightInputBuffer[C_FACTOR4_IR_LENGTH - m_nWriteIndex - 1] = xnR;
	m_pRightInputBuffer[2 * C_FACTOR4_IR_LENGTH - m_nWriteIndex - 1] = xnR; //make a vecorizable structure - store it right from the beginning like this

	// reset: read index for Delay Line -> write index
	m_nReadIndexDL = m_nWriteIndex;

	// accumulator
	float yn_accumL = 0;
	float yn_accumR = 0;

	// Don't have to convolve on the first L-1, only need one convolution at the end
	if (i == 0) {
#if defined _WIN64 || defined JUCE_LINUX
		//Auto vectorizable version Windows AVX/SSE
		int offset = C_FACTOR4_IR_LENGTH - m_nReadIndexDL;
		for (int j = 0; j < C_FACTOR4_IR_LENGTH; j++)
		{
		// do the sum of products
            yn_accumL += m_pLeftInputBuffer[offset + j] * m_pIRBuffer[j];
            yn_accumR += m_pRightInputBuffer[offset + j] * m_pIRBuffer[j];
		}
#elif _WIN32
		//ASM version with SSE
		//see http://www.drdobbs.com/optimizing-cc-with-inline-assembly-progr/184401967?pgno=3
		__declspec(align(16)) float yn_accL[4];
		__declspec(align(16)) float yn_accR[4];
		/*register int n = C_FACTOR4_IR_LENGTH;
		register int j;
		register int m_pRead = m_nReadIndexDL + C_FACTOR4_IR_LENGTH;
		register int m_bRead = 0;		*/
		//register not supported anymore!
		int n = C_FACTOR4_IR_LENGTH;
		int j;
		int m_pRead = m_nReadIndexDL + C_FACTOR4_IR_LENGTH;
		int m_bRead = 0;		
		float *lpIBufL = m_pLeftInputBuffer;
		float *lpIBufR = m_pRightInputBuffer;
		float *lpFBuf = m_pIRBuffer;

		__asm {
			xorps xmm0, xmm0  // make 0
			xorps xmm5, xmm5  // make 0
		}
		for (j = 0; j < C_FACTOR4_IR_LENGTH; j += 8) {
			//register int m_iOffset = (C_FACTOR4_IR_LENGTH - m_nReadIndexDL + j) * 4;
			//register int m_jOffset = j * 4;
			int m_iOffset = (C_FACTOR4_IR_LENGTH - m_nReadIndexDL + j) * 4;
			int m_jOffset = j * 4;
			__asm {
					//filter buffer	
					mov esi, lpFBuf
					mov eax, m_jOffset
					movaps xmm3, dword ptr[esi + eax]
					add eax, 10h
					movaps xmm4, dword ptr[esi + eax]

					//left buffer
					mov esi, lpIBufL
					mov eax, [m_iOffset]
					movaps xmm1, dword ptr[esi + eax]
					add eax, 10h
					movaps xmm2, dword ptr[esi + eax]

					mulps xmm1, xmm3
					mulps xmm2, xmm4
					addps xmm1, xmm2
					addps xmm0, xmm1

					//right buffer
					mov esi, lpIBufR
					mov eax, [m_iOffset]
					movaps xmm1, dword ptr[esi + eax]
					add eax, 10h
					movaps xmm2, dword ptr[esi + eax]

					mulps xmm1, xmm3
					mulps xmm2, xmm4
					addps xmm1, xmm2
					addps xmm5, xmm1					
			}
		}
		__asm {
			movaps yn_accL, xmm0
			movaps yn_accR, xmm5
		}
		yn_accL[0] += yn_accL[1] + yn_accL[2] + yn_accL[3];
		yn_accumL = yn_accL[0];
		yn_accR[0] += yn_accR[1] + yn_accR[2] + yn_accR[3];
		yn_accumR = yn_accR[0];

#elif _MACOSX
    //MacOSX intrinsic version with SSE, works with WIN32/64 as well
        if (i == 0) {
        //see http://www.drdobbs.com/optimizing-cc-with-inline-assembly-progr/184401967?pgno=3
#if defined _MACOSX
            typedef float Sse[4] __attribute__ ((aligned(16)));
            Sse sse4 __attribute__ ((aligned(16))) = { 0.0, 0.0, 0.0, 0.0 };
#elif _WIN32 || _WIN64
            __declspec(align(16)) float sse4[4] = { 0.0, 0.0, 0.0, 0.0 };
#endif
            __unused __m128 xmm0_4 = _mm_load_ps(sse4); //initialize
            __unused __m128 xmm5_4 = _mm_load_ps(sse4);
            __unused __m128 xmm1_4 = _mm_load_ps(sse4);
            __unused __m128 xmm2_4 = _mm_load_ps(sse4);
            __unused __m128 xmm3_4 = _mm_load_ps(sse4);
            __unused __m128 xmm4_4 = _mm_load_ps(sse4);
            for (int j = 0; j < C_FACTOR4_IR_LENGTH; j += 8) {
                int m_iOffset = (C_FACTOR4_IR_LENGTH - m_nReadIndexDL + j);
                //filter
                xmm3_4 = _mm_load_ps(&m_pIRBuffer[j]);
                xmm4_4 = _mm_load_ps(&m_pIRBuffer[j + 4]);
                
                //left
                xmm1_4 = _mm_load_ps(&m_pLeftInputBuffer[m_iOffset]);
                xmm2_4 = _mm_load_ps(&m_pLeftInputBuffer[m_iOffset + 4]);
                
                xmm1_4 = _mm_mul_ps(xmm1_4 , xmm3_4);
                xmm2_4 = _mm_mul_ps(xmm2_4 , xmm4_4);
                xmm1_4 = _mm_add_ps(xmm1_4 , xmm2_4);
                xmm0_4 = _mm_add_ps(xmm0_4 , xmm1_4);
                
                //right
                xmm1_4 = _mm_load_ps(&m_pRightInputBuffer[m_iOffset]);
                xmm2_4 = _mm_load_ps(&m_pRightInputBuffer[m_iOffset+4]);
                
                xmm1_4 = _mm_mul_ps(xmm1_4 , xmm3_4);
                xmm2_4 = _mm_mul_ps(xmm2_4 , xmm4_4);
                xmm1_4 = _mm_add_ps(xmm1_4 , xmm2_4);
                xmm5_4 = _mm_add_ps(xmm5_4 , xmm1_4);
            }
            
            xmm0_4 =_mm_hadd_ps(xmm0_4,xmm0_4);
            xmm0_4 =_mm_hadd_ps(xmm0_4,xmm0_4);
            xmm5_4 =_mm_hadd_ps(xmm5_4,xmm5_4);
            xmm5_4 =_mm_hadd_ps(xmm5_4,xmm5_4);
            _mm_store_ss(&yn_accumL, xmm0_4);
            _mm_store_ss(&yn_accumR, xmm5_4);
        }
#endif
	}

	// check for wrap of delay line (no need to check IR buffer)
	if (m_nReadIndexDL < 0)
		m_nReadIndexDL = (m_nReadIndexDL + C_FACTOR4_IR_LENGTH ) % C_FACTOR4_IR_LENGTH;

	// write out
	fLeftOutput = yn_accumL;

	// write out
	fRightOutput = yn_accumR;

	// incremnent the pointers and wrap if necessaryx
	m_nWriteIndex++;
	if (m_nWriteIndex >= C_FACTOR4_IR_LENGTH)
		m_nWriteIndex = 0;

	return true;
}

/*
// generated asm by VS 2013 AVX vectorized in debug mode

// This can be optimized!! Don't have to convolve on the first L-1, only need one convolution at the end
if (i == 0) {
0F94D601  cmp         dword ptr [i],0
0F94D605  mov         eax,dword ptr [esi+0Ch]
0F94D608  vmovss      dword ptr [eax+ecx*4],xmm0
0F94D60D  mov         edi,dword ptr [esi+18h]
0F94D610  vxorps      xmm0,xmm0,xmm0
0F94D614  mov         dword ptr [esi+10h],edi
0F94D617  vmovaps     xmm2,xmm0
0F94D61B  jne         CDecimator::decimateNextOutputSample+272h (0F94D812h)
{
// do the sum of products
yn_accumL += m_pLeftInputBuffer[m_nReadIndexDL + C_FACTOR4_IR_LENGTH - j] * m_pIRBuffer[j];
0F94D621  mov         ecx,dword ptr [esi+8]
yn_accumR += m_pRightInputBuffer[m_nReadIndexDL + C_FACTOR4_IR_LENGTH - j] * m_pIRBuffer[j];
0F94D624  mov         edx,dword ptr [esi+0Ch]
0F94D627  mov         edi,dword ptr [esi+4]
0F94D62A  vxorps      ymm3,ymm3,ymm3
0F94D62E  vmovups     ymm4,ymm3
0F94D632  push        ebx
0F94D633  xor         ebx,ebx
0F94D635  vmovups     ymm5,ymm3
0F94D639  vmovups     ymm6,ymm4
0F94D63D  lea         ecx,[ecx]

for (int j = 0; j < C_FACTOR4_IR_LENGTH; j++)
0F94D640  mov         eax,dword ptr [esi+10h]
0F94D643  sub         eax,ebx
{
// do the sum of products
yn_accumL += m_pLeftInputBuffer[m_nReadIndexDL + C_FACTOR4_IR_LENGTH - j] * m_pIRBuffer[j];
0F94D645  vmovss      xmm0,dword ptr [ecx+eax*4+800h]
0F94D64E  vinsertps   xmm0,xmm0,dword ptr [ecx+eax*4+7FCh],10h
0F94D659  vinsertps   xmm0,xmm0,dword ptr [ecx+eax*4+7F8h],20h
0F94D664  vinsertps   xmm1,xmm0,dword ptr [ecx+eax*4+7F4h],30h
0F94D66F  vmovss      xmm0,dword ptr [ecx+eax*4+7F0h]
0F94D678  vinsertps   xmm0,xmm0,dword ptr [ecx+eax*4+7ECh],10h
0F94D683  vinsertps   xmm0,xmm0,dword ptr [ecx+eax*4+7E8h],20h
0F94D68E  vinsertps   xmm0,xmm0,dword ptr [ecx+eax*4+7E4h],30h
0F94D699  vinsertf128 ymm0,ymm1,xmm0,1
0F94D69F  vmulps      ymm0,ymm0,ymmword ptr [edi+ebx*4]
0F94D6A4  vaddps      ymm3,ymm0,ymm3
yn_accumR += m_pRightInputBuffer[m_nReadIndexDL + C_FACTOR4_IR_LENGTH - j] * m_pIRBuffer[j];
0F94D6A8  vmovss      xmm0,dword ptr [edx+eax*4+800h]
yn_accumR += m_pRightInputBuffer[m_nReadIndexDL + C_FACTOR4_IR_LENGTH - j] * m_pIRBuffer[j];
0F94D6B1  vinsertps   xmm0,xmm0,dword ptr [edx+eax*4+7FCh],10h
0F94D6BC  vinsertps   xmm0,xmm0,dword ptr [edx+eax*4+7F8h],20h
0F94D6C7  vinsertps   xmm1,xmm0,dword ptr [edx+eax*4+7F4h],30h
0F94D6D2  vmovss      xmm0,dword ptr [edx+eax*4+7F0h]
0F94D6DB  vinsertps   xmm0,xmm0,dword ptr [edx+eax*4+7ECh],10h
0F94D6E6  vinsertps   xmm0,xmm0,dword ptr [edx+eax*4+7E8h],20h
0F94D6F1  vinsertps   xmm0,xmm0,dword ptr [edx+eax*4+7E4h],30h
0F94D6FC  vinsertf128 ymm0,ymm1,xmm0,1
0F94D702  vmulps      ymm0,ymm0,ymmword ptr [edi+ebx*4]
0F94D707  vaddps      ymm4,ymm0,ymm4
0F94D70B  vmovss      xmm0,dword ptr [ecx+eax*4+7E0h]
0F94D714  vinsertps   xmm0,xmm0,dword ptr [ecx+eax*4+7DCh],10h
0F94D71F  vinsertps   xmm0,xmm0,dword ptr [ecx+eax*4+7D8h],20h
0F94D72A  vinsertps   xmm1,xmm0,dword ptr [ecx+eax*4+7D4h],30h
0F94D735  vmovss      xmm0,dword ptr [ecx+eax*4+7D0h]
0F94D73E  vinsertps   xmm0,xmm0,dword ptr [ecx+eax*4+7CCh],10h
0F94D749  vinsertps   xmm0,xmm0,dword ptr [ecx+eax*4+7C8h],20h
0F94D754  vinsertps   xmm0,xmm0,dword ptr [ecx+eax*4+7C4h],30h
0F94D75F  vinsertf128 ymm0,ymm1,xmm0,1
0F94D765  vmulps      ymm0,ymm0,ymmword ptr [edi+ebx*4+20h]
0F94D76B  vaddps      ymm5,ymm0,ymm5
0F94D76F  vmovss      xmm0,dword ptr [edx+eax*4+7E0h]
0F94D778  vinsertps   xmm0,xmm0,dword ptr [edx+eax*4+7DCh],10h
0F94D783  vinsertps   xmm0,xmm0,dword ptr [edx+eax*4+7D8h],20h
0F94D78E  vinsertps   xmm1,xmm0,dword ptr [edx+eax*4+7D4h],30h
0F94D799  vmovss      xmm0,dword ptr [edx+eax*4+7D0h]
0F94D7A2  vinsertps   xmm0,xmm0,dword ptr [edx+eax*4+7CCh],10h
0F94D7AD  vinsertps   xmm0,xmm0,dword ptr [edx+eax*4+7C8h],20h
0F94D7B8  vinsertps   xmm0,xmm0,dword ptr [edx+eax*4+7C4h],30h
0F94D7C3  vinsertf128 ymm0,ymm1,xmm0,1
0F94D7C9  vmulps      ymm0,ymm0,ymmword ptr [edi+ebx*4+20h]
0F94D7CF  add         ebx,10h
0F94D7D2  vaddps      ymm6,ymm0,ymm6
0F94D7D6  cmp         ebx,200h
0F94D7DC  jl          CDecimator::decimateNextOutputSample+0A0h (0F94D640h)

for (int j = 0; j < C_FACTOR4_IR_LENGTH; j++)
0F94D7E2  mov         edi,dword ptr [esi+18h]
0F94D7E5  vaddps      ymm0,ymm6,ymm4
0F94D7E9  vhaddps     ymm0,ymm0,ymm0
0F94D7ED  vhaddps     ymm1,ymm0,ymm0
0F94D7F1  vextractf128 xmm0,ymm1,1
0F94D7F7  vaddps      xmm2,xmm1,xmm0
0F94D7FB  vaddps      ymm0,ymm5,ymm3
0F94D7FF  vhaddps     ymm0,ymm0,ymm0
0F94D803  vhaddps     ymm1,ymm0,ymm0
0F94D807  vextractf128 xmm0,ymm1,1
0F94D80D  vaddps      xmm0,xmm1,xmm0
0F94D811  pop         ebx
}

*/
// generated asm by VS 2013 SS2 vectorized in debug mode
/*
{
// do the sum of products
yn_accumL += m_pLeftInputBuffer[m_nReadIndexDL + C_FACTOR4_IR_LENGTH - j] * m_pIRBuffer[j];
0F3FE5A8  mov         eax,dword ptr [esi+10h]
0F3FE5AB  xorps       xmm7,xmm7
0F3FE5AE  mov         ecx,dword ptr [esi+8]
0F3FE5B1  movaps      xmm0,xmm7
yn_accumR += m_pRightInputBuffer[m_nReadIndexDL + C_FACTOR4_IR_LENGTH - j] * m_pIRBuffer[j];
0F3FE5B4  mov         edx,dword ptr [esi+0Ch]
0F3FE5B7  xor         edi,edi
0F3FE5B9  mov         esi,dword ptr [esi+4]
0F3FE5BC  movaps      xmm6,xmm7
0F3FE5BF  mov         dword ptr [ebp-8],eax
0F3FE5C2  movaps      xmm5,xmm0
0F3FE5C5  movaps      xmmword ptr [ebp-20h],xmm0
0F3FE5C9  lea         esp,[esp]
{
// do the sum of products
yn_accumL += m_pLeftInputBuffer[m_nReadIndexDL + C_FACTOR4_IR_LENGTH - j] * m_pIRBuffer[j];
0F3FE5D0  mov         eax,dword ptr [ebp-8]
0F3FE5D3  movups      xmm4,xmmword ptr [esi+edi*4]
{
// do the sum of products
yn_accumL += m_pLeftInputBuffer[m_nReadIndexDL + C_FACTOR4_IR_LENGTH - j] * m_pIRBuffer[j];
0F3FE5D7  sub         eax,edi
0F3FE5D9  movss       xmm2,dword ptr [ecx+eax*4+7F4h]
0F3FE5E2  movss       xmm0,dword ptr [ecx+eax*4+7F8h]
0F3FE5EB  movss       xmm1,dword ptr [ecx+eax*4+7FCh]
0F3FE5F4  movss       xmm3,dword ptr [ecx+eax*4+800h]
0F3FE5FD  unpcklps    xmm3,xmm0
yn_accumR += m_pRightInputBuffer[m_nReadIndexDL + C_FACTOR4_IR_LENGTH - j] * m_pIRBuffer[j];
0F3FE600  movss       xmm0,dword ptr [edx+eax*4+7F8h]
0F3FE609  unpcklps    xmm1,xmm2
0F3FE60C  movss       xmm2,dword ptr [edx+eax*4+7F4h]
0F3FE615  unpcklps    xmm3,xmm1
0F3FE618  movss       xmm1,dword ptr [edx+eax*4+7FCh]
0F3FE621  mulps       xmm3,xmm4
0F3FE624  unpcklps    xmm1,xmm2
0F3FE627  movss       xmm2,dword ptr [ecx+eax*4+7E4h]
0F3FE630  addps       xmm7,xmm3
0F3FE633  movss       xmm3,dword ptr [edx+eax*4+800h]
0F3FE63C  unpcklps    xmm3,xmm0
0F3FE63F  movss       xmm0,dword ptr [ecx+eax*4+7E8h]
0F3FE648  unpcklps    xmm3,xmm1
0F3FE64B  movss       xmm1,dword ptr [ecx+eax*4+7ECh]
0F3FE654  mulps       xmm3,xmm4
0F3FE657  movups      xmm4,xmmword ptr [esi+edi*4+10h]
0F3FE65C  add         edi,8
0F3FE65F  unpcklps    xmm1,xmm2
0F3FE662  movss       xmm2,dword ptr [edx+eax*4+7E4h]
0F3FE66B  addps       xmm3,xmmword ptr [ebp-20h]
0F3FE66F  movaps      xmmword ptr [ebp-20h],xmm3
0F3FE673  movss       xmm3,dword ptr [ecx+eax*4+7F0h]
0F3FE67C  unpcklps    xmm3,xmm0
0F3FE67F  movss       xmm0,dword ptr [edx+eax*4+7E8h]
0F3FE688  unpcklps    xmm3,xmm1
0F3FE68B  movss       xmm1,dword ptr [edx+eax*4+7ECh]
0F3FE694  mulps       xmm3,xmm4
0F3FE697  unpcklps    xmm1,xmm2
0F3FE69A  addps       xmm6,xmm3
0F3FE69D  movss       xmm3,dword ptr [edx+eax*4+7F0h]
0F3FE6A6  unpcklps    xmm3,xmm0
0F3FE6A9  unpcklps    xmm3,xmm1
0F3FE6AC  mulps       xmm3,xmm4
0F3FE6AF  addps       xmm5,xmm3
0F3FE6B2  cmp         edi,200h
0F3FE6B8  jl          CDecimator::decimateNextOutputSample+0C0h (0F3FE5D0h)

for (int j = 0; j < C_FACTOR4_IR_LENGTH; j++)
0F3FE6BE  addps       xmm5,xmmword ptr [ebp-20h]
0F3FE6C2  mov         esi,dword ptr [ebp-0Ch]
0F3FE6C5  addps       xmm6,xmm7
0F3FE6C8  mov         edi,dword ptr [esi+18h]
0F3FE6CB  movaps      xmm2,xmm5
0F3FE6CE  movhlps     xmm2,xmm5
0F3FE6D1  movaps      xmm1,xmm6
0F3FE6D4  addps       xmm2,xmm5
0F3FE6D7  movhlps     xmm1,xmm6
0F3FE6DA  addps       xmm1,xmm6
0F3FE6DD  movaps      xmm0,xmm2
0F3FE6E0  shufps      xmm0,xmm2,0F5h
0F3FE6E4  addss       xmm2,xmm0
0F3FE6E8  movaps      xmm0,xmm1
0F3FE6EB  shufps      xmm0,xmm1,0F5h
0F3FE6EF  addss       xmm1,xmm0
}
*/

/*
		//ASM version with SSE (working)
		//see http://www.drdobbs.com/optimizing-cc-with-inline-assembly-progr/184401967?pgno=3
		__declspec(align(16)) float yn_accL[4];
		__declspec(align(16)) float yn_accR[4];
		register int n = C_FACTOR4_IR_LENGTH;
		register int j;
		register int m_pRead = m_nReadIndexDL + C_FACTOR4_IR_LENGTH;
		register int m_bRead = 0;		
		float *lpIBufL = m_pLeftInputBuffer;
		float *lpIBufR = m_pRightInputBuffer;
		float *lpFBuf = m_pIRBuffer;

		__asm {
			xorps xmm0, xmm0  // make 0
			xorps xmm5, xmm5  // make 0
		}
		for (j = 0; j < C_FACTOR4_IR_LENGTH; j += 8) {
			register int m_iOffset = (C_FACTOR4_IR_LENGTH - m_nReadIndexDL + j) * 4;
			register int m_jOffset = j * 4;
			__asm {
					//filter buffer	
					mov esi, lpFBuf
					mov eax, m_jOffset
					movaps xmm3, dword ptr[esi + eax]
					add eax, 10h
					movaps xmm4, dword ptr[esi + eax]

					//left buffer
					mov esi, lpIBufL
					mov eax, [m_iOffset]
					movaps xmm1, dword ptr[esi + eax]
					add eax, 10h
					movaps xmm2, dword ptr[esi + eax]

					mulps xmm1, xmm3
					mulps xmm2, xmm4
					addps xmm1, xmm2
					addps xmm0, xmm1

					//right buffer
					mov esi, lpIBufR
					mov eax, [m_iOffset]
					movaps xmm1, dword ptr[esi + eax]
					add eax, 10h
					movaps xmm2, dword ptr[esi + eax]

					mulps xmm1, xmm3
					mulps xmm2, xmm4
					addps xmm1, xmm2
					addps xmm5, xmm1					
			}
		}
		__asm {
			movaps yn_accL, xmm0
			movaps yn_accR, xmm5
		}
		yn_accL[0] += yn_accL[1] + yn_accL[2] + yn_accL[3];
		yn_accumL = yn_accL[0];
		yn_accR[0] += yn_accR[1] + yn_accR[2] + yn_accR[3];
		yn_accumR = yn_accR[0];
*/

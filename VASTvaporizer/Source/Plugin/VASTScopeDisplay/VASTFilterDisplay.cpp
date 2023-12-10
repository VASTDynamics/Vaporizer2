/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTFilterDisplay.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTAudioProcessor.h"
#include "../../Engine/VASTVcf.h"
#include "../../Engine/VASTSingleNote.h"

#define c_multiplier 8 // 8 is still grainy, 16 is good enough
//#define c_fftOrder 15 //multiplier 4
#define c_fftOrder 14 //multiplier 8
						//int order = 11; //multiplier 1
						//int order = 16; //multiplier 32
#define c_fftSize C_WAVE_TABLE_SIZE * c_multiplier // c_fftSize 1 << c_fftOrder // C_WAVE_TABLE_SIZE * c_multiplier 
#define c_maxDisp float(c_fftSize / 2.f)
#define c_minDisp float(c_fftSize / 512.f) //lower are too coarse
#define c_maxminDisp float(c_maxDisp) / float(c_minDisp)

VASTFilterDisplay::VASTFilterDisplay(VASTAudioProcessor* processor) :
	m_VCF{ {processor->m_pVASTXperience.m_Set, 0, 0, true}, {processor->m_pVASTXperience.m_Set, 0, 1, true}, {processor->m_pVASTXperience.m_Set, 0, 2, true} } //filter 1-3
{
	myProcessor = processor;
	myEditor = nullptr;
	
	//myFont.setTypefaceName("Open Sans"); //bold 1-2, regular 2
	//myFont.setDefaultMinimumHorizontalScaleFactor(1.0);
	//myFont.setSizeAndStyle(14.f, Font::bold, 1.0f, 0.0f); // no squashing, no kerning

	setOpaque(true); //avoid repaints of parents
	setRepaintsOnMouseActivity(false); //performance

	setPaintingIsUnclipped(true);

	resized();                  // initialise image
	
	setComponentID("filterDisplay");
	
	mb_init = false;
	startTimer(50);   
}

VASTFilterDisplay::~VASTFilterDisplay() {	
	deleted_ = true;
	while (m_iThreadsRunning.load() > 0)
		Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 10);
	stopTimer();
	this->setLookAndFeel(nullptr);
	fft = nullptr;
}

void VASTFilterDisplay::lookAndFeelChanged() {
	if (!myProcessor->isCurrentEditorInitialized())
		return;

	if (myEditor == nullptr)
		return;
	if (myEditor->vaporizerComponent == nullptr)
		return;
	b_alreadyPainted = false;
    resized();
}

//==============================================================================
void VASTFilterDisplay::resized()
{
	if (!myProcessor->isCurrentEditorInitialized())
		return;

	waveformImageBuffer.reset(new Image(Image::RGB,
		jmax(1, getScreenBounds().getWidth()), jmax(1, getScreenBounds().getHeight()),
		true));
    if (myEditor == nullptr) return;
    Colour cBack =  myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground);
    Colour cGridlines =  myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayGrid);
    
	waveformImageBuffer->clear(waveformImageBuffer->getBounds(), cBack);
	Graphics g(*waveformImageBuffer.get());
	g.setGradientFill(ColourGradient(cBack.brighter(0.06f), 0.0f, 0.0f,
		cBack.darker(1.00f), waveformImageBuffer->getWidth(), (float)waveformImageBuffer->getHeight(), false));
	g.fillRect(0, 0, waveformImageBuffer->getWidth(), waveformImageBuffer->getHeight());
	//scales
	float fGridHeight = waveformImageBuffer->getHeight() / 8.f;

	for (int horiz = 0; horiz < 8; horiz++) {
		g.setColour(cGridlines);
		if (horiz % 2 == 0)
			g.setColour(cGridlines.brighter(0.25f));
		g.drawLine(0.f, horiz * fGridHeight, waveformImageBuffer->getWidth(), horiz * fGridHeight, 0.5f);
	}
	
	/*
	for (int verti = 0; verti < l_numVertical; verti++) {
		g.setColour(cGridlines);
		if (verti % 3 == 0)
			g.setColour(cGridlines.brighter(0.25f));
		g.drawLine(verti * fGridWidth, 0.f, verti * fGridWidth, waveformImageBuffer->getHeight(), 0.5f);
	}
	*/

	g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeSelection));
	g.drawRect(0.f, 0.f, float(waveformImageBuffer->getWidth()), float(waveformImageBuffer->getHeight()), 0.5f);
	g.drawLine(1.f, waveformImageBuffer->getHeight() * 0.5f, float(waveformImageBuffer->getWidth() -1.f), waveformImageBuffer->getHeight() * 0.5f, 0.5f);
	
	float fontSize = (waveformImageBuffer->getWidth() / 1000.f) * 16.f;
	Font myFont = myProcessor->getCurrentVASTLookAndFeel()->getDefaultFont();
	myFont.setSizeAndStyle(fontSize, Font::plain, 1.0f, 0.0f); // no squashing, no kerning
	g.setFont(myFont);
    
	struct strfreq
	{
		String display;
		double freq;
	};
	strfreq sf[29] = {
		   {"100", 100.0},
		   {"200", 200.0},
		   {"", 300.0},
		   {"", 400.0},
		   {"500", 500.0},
		   {"", 600.0},
		   {"", 700.0},
		   {"", 800.0},
		   {"", 900.0},
		   {"1k", 1000.0},
		   {"2k", 2000.0},
		   {"", 3000.0},
		   {"", 4000.0},
		   {"5k", 5000.0},
		   {"", 6000.0},
		   {"", 7000.0},
		   {"", 8000.0},
		   {"", 9000.0},
		   {"10k", 10000.0},
		   {"", 11000.0},
		   {"", 12000.0},
		   {"", 13000.0},
		   {"", 14000.0},
		   {"", 15000.0},
		   {"", 16000.0},
		   {"", 17000.0},
		   {"", 18000.0},
		   {"", 19000.0},
		   {"20k", 20000.0}
	};

	int fontHeight = myFont.getHeightInPoints();
	for (int i = 0; i < 29; i++) {
		float pos = getSkewForTargetFrequency(sf[i].freq) * waveformImageBuffer->getWidth(); //CHECK xoffsets		
		if (sf[i].display != "") {
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayText));
			g.drawText(sf[i].display, pos - 15.f, waveformImageBuffer->getHeight() - fontHeight - 4.f, 30.f, fontHeight + 2.f, Justification::centred, false);
		}
		g.setColour(cGridlines.brighter(0.25f));
		g.drawLine(pos, 0.f, pos, waveformImageBuffer->getHeight(), 0.5f);
	}
	
	g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayText));
	const String dbText = "+48dB";
	float stringWidth = myFont.getStringWidthFloat(dbText);
	g.drawText(dbText, waveformImageBuffer->getWidth() - stringWidth - 4.f, 2.f, stringWidth, fontHeight + 2.f, Justification::centredRight, false);
	//g.drawText("-32dB", waveformImageBuffer->getWidth() - 40.f, waveformImageBuffer->getHeight() - 21.f, 35.f, 20.f, Justification::centredRight, false);

	//*waveformImage = waveformImageBuffer->createCopy();
	//waveformImage->duplicateIfShared();

	if (myProcessor != nullptr)
		doUpdates(true);
}

void VASTFilterDisplay::paint(Graphics& g)
{	
	if (b_alreadyPainted == false) {
		doUpdates(true);
		b_alreadyPainted = true;
	}

	if (b_newImageReady) {
		
		ScopedLock llock(m_criticalSection);

		if (waveformImageNext.get() != nullptr) {
			waveformImageNext->duplicateIfShared();
			waveformImage.reset(new Image(waveformImageNext->createCopy()));
			b_newImageReady = false;
		}
	}

	if (waveformImage.get() != nullptr) {
		auto* img = dynamic_cast<Image*> (waveformImage.get());
		if (img != nullptr) {
			img->duplicateIfShared();
			g.drawImageWithin(*img, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
		}
	}
}

void VASTFilterDisplay::doUpdates(bool force) {
	if (this->isShowing()) {
		if (m_iThreadsRunning.load() <= 0) {
			m_iThreadsRunning.store(m_iThreadsRunning.load() + 1);
			std::thread update_thread(&VASTFilterDisplay::updateThread, this, force);
			update_thread.detach();
		}
		else
			m_bRestart = true;
	}
}

void VASTFilterDisplay::setEditor(VASTAudioProcessorEditor* editor) { myEditor = editor; }

void VASTFilterDisplay::setProcessor(VASTAudioProcessor* processor) { myProcessor = processor; }

void VASTFilterDisplay::timerCallback() {
	if (b_newImageReady) {

		ScopedLock llock(m_criticalSection);

		if (waveformImageNext.get() != nullptr) {
			waveformImageNext->duplicateIfShared();
			waveformImage.reset(new Image(waveformImageNext->createCopy()));
			if (b_newImageReady) {
				b_newImageReady = false;
				repaint();
			}
		}
	}

	if (m_bRestart) {
		VDBG("VASTFilterDisplay Display Restart!");
		m_bRestart = false;
		doUpdates(false);
	}
}

void VASTFilterDisplay::updateThread(VASTFilterDisplay* display, bool force) {
	if (display == nullptr) {
		return;
	}
	if (!force)
		if ((display->isShowing() == false) || (display->getWidth() <= 1) || (display->getHeight() <= 1)) {
			display->m_iThreadsRunning.store(display->m_iThreadsRunning.load() - 1);
			return;
		}
	if ((display->myProcessor == nullptr) || (display->myProcessor->initNotCompleted())) {
		display->m_iThreadsRunning.store(display->m_iThreadsRunning.load() - 1);
		return;
	}
	if (display->m_iThreadsRunning.load() > 1) {
		display->m_iThreadsRunning.store(display->m_iThreadsRunning.load() - 1);
		return;
	}

	//std::unique_ptr<dsp::FFT> fft;
	//AudioSampleBuffer fftResults;
	//std::unique_ptr<CVASTVcf> m_VCF[3];
	//VASTQFilter m_QFilter;

	bool l_display = true;
	int width = display->waveformImageBuffer->getWidth() - 6.f;
	float height = display->waveformImageBuffer->getHeight() - 6.f;

	if ((width <= 0) || (height <= 0)) {
		display->m_iThreadsRunning.store(display->m_iThreadsRunning.load() - 1);
		return;
	}

	display->m_screenWidthScale = float(display->getScreenBounds().getWidth()) / float(display->getWidth());
	display->m_screenHeightScale = float(display->getScreenBounds().getHeight()) / float(display->getHeight());

	//float halfHeight = height * 0.5f;
	float xOffset = 3.f;
	float yOffset = 3.f;

	CVASTSettings *l_Set = &display->myProcessor->m_pVASTXperience.m_Set;
	if (display->m_lastSampleRate != l_Set->m_nSampleRate) //check for changed sample rate (e.g. standalone)
		display->mb_init = false;
	display->m_lastSampleRate = l_Set->m_nSampleRate;

	if (display->mb_init == false) {
		display->m_QFilter.initQuadFilter(&display->myProcessor->m_pVASTXperience.m_Set);
		display->fft = std::make_unique<dsp::FFT>(c_fftOrder); //scoped pointer
		/*
		for (int filter = 0; filter < 3; filter++) {
			display->m_VCF[filter] = std::make_unique<CVASTVcf>(*l_Set, 0, filter, true);
		}
		*/
		display->mb_init = true;
	}
	for (int filter = 0; filter < 3; filter++) {
		display->m_VCF[filter].init(); //isUI
		display->m_VCF[filter].prepareForPlay();
	}

	Image waveformImage; 
	waveformImage = display->waveformImageBuffer->createCopy();
	waveformImage.duplicateIfShared(); //needed?
	Graphics g(waveformImage);

	std::vector<dsp::Complex<float>> l_filterBuffer_in = std::vector<dsp::Complex<float>>(c_fftSize);
	std::vector<dsp::Complex<float>> l_filterBuffer_out = std::vector<dsp::Complex<float>>(c_fftSize);
	for (int i = 0; i < c_fftSize; i++) {
		l_filterBuffer_in[i].imag(0.f);
		l_filterBuffer_out[i].real(0.f);
	}

	modMatrixInputState inputStateCopy;
	inputStateCopy.voiceNo = 0;
	inputStateCopy.currentFrame = 0;

	AudioSampleBuffer buffer = AudioSampleBuffer(2, c_fftSize);
	buffer.clear();
	buffer.setSample(0, 0, 1.f); //impulse
	buffer.setSample(1, 0, 1.f); //impulse
	dsp::AudioBlock<float> filterBlock(buffer);

	modMatrixInputState l_inputState;
	l_inputState.currentFrame = 0;

	for (int filter = 0; filter < 3; filter++) {
		int ftype = 0.f;
		int onoff = 0.f;
		switch (filter)
		{
		case 0:
			ftype = *l_Set->m_State->m_uFilterType_Filter1;
			onoff = *l_Set->m_State->m_bOnOff_Filter1;
			break;
		case 1:
			ftype = *l_Set->m_State->m_uFilterType_Filter2;
			onoff = *l_Set->m_State->m_bOnOff_Filter2;
			break;
		case 2:
			ftype = *l_Set->m_State->m_uFilterType_Filter3;
			onoff = *l_Set->m_State->m_bOnOff_Filter3;
			break;
		}
		if (onoff == static_cast<int>(SWITCH::SWITCH_ON)) {
			//process filter					
			if (((ftype >= FILTERTYPE::DQFLPF12) && (ftype <= FILTERTYPE::DQFCOMBWSSINE)) || 
				(ftype == FILTERTYPE::COMBFILTER) || (ftype == FILTERTYPE::SCREAMFILTER))
				display->m_QFilter.processBlock(nullptr, &l_inputState, nullptr, l_Set, filter, filterBlock, 0, c_fftSize, true, false, display->m_VCF, true); //warmup
			display->m_QFilter.processBlock(nullptr, &l_inputState, nullptr, l_Set, filter, filterBlock, 0, c_fftSize, true, false, display->m_VCF, false); //doit
		}
	}

	//if (b_processed) {
	float fftData[c_fftSize * 2];
	for (int idx = 0; idx < c_fftSize; idx++) {

		fftData[idx] = filterBlock.getSample(0, idx);
	}

	display->fft->performFrequencyOnlyForwardTransform(fftData);
	
	//Sinc interpolation here?
	//https://stackoverflow.com/questions/31836598/subsampling-an-array-of-numbers

	//if (l_filterBuffer_out.empty()) l_display = false;
	if (l_display == true) {

		//https://dsp.stackexchange.com/questions/26927/what-is-a-frequency-bin

		auto mindB = -48.0f;
		auto maxdB = 48.0f;

		AudioSampleBuffer fftResults;
		fftResults.setSize(1, int(width), false, false, true);
				for (int x = 0; x < width; x++) {

			auto skewedProportionX = c_minDisp * pow(c_maxminDisp, float(x) / float(width));
			auto fftDataIndex = jlimit(0, c_fftSize / 2, int(skewedProportionX));


			auto level = jmap(jlimit(mindB, maxdB, Decibels::gainToDecibels(fftData[fftDataIndex])
			),
				//- Decibels::gainToDecibels((float)fftSize)),				
				mindB, maxdB, 0.0f, 1.0f);
					   		
			fftResults.setSample(0, x, level);
		}

		//draw
		Path myWave;
		Colour g1 = display->myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayArea);
		Colour g2 = g1.withAlpha(0.f);
		for (int x = 0; x < width; x++) {
			double yVal = fftResults.getSample(0, x);
			if (x == 0) {
				myWave.startNewSubPath(xOffset + 0, yOffset + height - yVal * height);
			}
			else {
				myWave.lineTo(xOffset + x, yOffset + height - yVal * height);
			}

			g.setGradientFill(ColourGradient(g2, juce::Point<float>(xOffset + x, yOffset + height), g1, juce::Point<float>(xOffset + x, yOffset + height - yVal * height), true));
			g.drawLine(xOffset + x, yOffset + height - yVal * height, xOffset + x, yOffset + height, 1.f /* has to be 1*/);
		}
		g.setColour(display->myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayLine));
		g.strokePath(myWave, PathStrokeType(1.0f * display->myEditor->getProcessor()->getPluginScaleWidthFactor() * display->m_screenWidthScale));
	}
	
	//MessageManagerLock mml(Thread::getCurrentThread()); //https://forum.juce.com/t/drawing-graphics-using-background-threads/6584	
	ScopedLock llock(display->m_criticalSection);

	waveformImage.duplicateIfShared();
	if (display->waveformImageNext.get() != nullptr)
		display->waveformImageNext->duplicateIfShared();
	display->waveformImageNext.reset(new Image(waveformImage));
	display->b_newImageReady = true;
	display->m_iThreadsRunning.store(display->m_iThreadsRunning.load() - 1);
}

float VASTFilterDisplay::getSkewForTargetFrequency(float targetFreq)
{
	//https://dsp.stackexchange.com/questions/24458/fftw-log-log-plot

	CVASTSettings *l_Set = &myProcessor->m_pVASTXperience.m_Set;
	float skewedProportionX = (targetFreq  / l_Set->m_nSampleRate) * c_fftSize;
	float ret = log(skewedProportionX / c_minDisp) / log(c_maxminDisp);
	return ret;
	

	/*
	CVASTSettings *l_Set = &myProcessor->m_pVASTXperience.m_Set;
	float skewedProportionX = (targetFreq * 2.f) / l_Set->m_nSampleRate;
	return 1.0f - std::pow(1.0 - skewedProportionX, 5.0);	
	*/
}

/*
void VASTFilterDisplay::updateContent(bool force) {
	if (!force)
		if ((this->isShowing() == false) || (getWidth() <= 1) || (getHeight() <= 1)) {
			return;
		}
	if ((myProcessor == nullptr) || (myProcessor->initNotCompleted())) return;
	m_bThreadRunning = true;

	bool l_display = true;
	int width = getWidth() - 6.f;
	float height = getHeight() - 6.f;
	//float halfHeight = height * 0.5f;
	float xOffset = 3.f;
	float yOffset = 3.f;

	CVASTSettings *l_Set = &myProcessor->m_pVASTXperience.m_Set;
	if (mb_init == false) {
		m_QFilter.initQuadFilter(&myProcessor->m_pVASTXperience.m_Set);
		fft = new dsp::FFT(c_fftOrder); //scoped pointer
		for (int filter = 0; filter < 3; filter++) {
			m_VCF[filter] = new CVASTVcf();
		}
		mb_init = true;
	}
	for (int filter = 0; filter < 3; filter++) {
		m_VCF[filter]->init(*l_Set, 0, filter, true); //isUI
		m_VCF[filter]->prepareForPlay();
	}

	waveformImage = waveformImageBuffer;
	waveformImage.duplicateIfShared();
	Graphics g(waveformImage);

	std::vector<dsp::Complex<float>> l_filterBuffer_in = std::vector<dsp::Complex<float>>(c_fftSize);
	std::vector<dsp::Complex<float>> l_filterBuffer_out = std::vector<dsp::Complex<float>>(c_fftSize);
	for (int i = 0; i < c_fftSize; i++) {
		l_filterBuffer_in[i].imag(0.f);
		l_filterBuffer_out[i].real(0.f);
	}

	//modMatrixInputState inputStateBuffer = l_Set->bufferInputState.load();
	modMatrixInputState inputStateCopy;
	inputStateCopy.voiceNo = 0;
	inputStateCopy.currentFrame = 0;
	//inputStateCopy = inputStateBuffer;
	//inputStateCopy.voiceNo = myProcessor->m_pVASTXperience.m_Poly.getLastNotePlayed();

	int numSamples = c_fftSize;
	AudioSampleBuffer buffer = AudioSampleBuffer(2, c_fftSize);
	buffer.clear();
	buffer.setSample(0, 0, 1.f); //impulse
	buffer.setSample(1, 0, 1.f); //impulse
	dsp::AudioBlock<float> filterBlock(buffer);

	modMatrixInputState l_inputState;
	l_inputState.currentFrame = 0;
	//float l_maxDispAdjustFactor = 1.f;
	//float l_scaleAdjustFactor = 1.f;
	//bool b_processed = false;

	for (int filter = 0; filter < 3; filter++) {
		int ftype = 0.f;
		int onoff = 0.f;
		switch (filter)
		{
		case 0:
			ftype = *l_Set->m_State->m_uFilterType_Filter1;
			onoff = *l_Set->m_State->m_bOnOff_Filter1;
			break;
		case 1:
			ftype = *l_Set->m_State->m_uFilterType_Filter2;
			onoff = *l_Set->m_State->m_bOnOff_Filter2;
			break;
		case 2:
			ftype = *l_Set->m_State->m_uFilterType_Filter3;
			onoff = *l_Set->m_State->m_bOnOff_Filter3;
			break;
		}
		if (onoff == static_cast<int>(SWITCH::SWITCH_ON)) {
			//process filter					
			if ((ftype >= FILTERTYPE::DQFLPF12) && (ftype <= FILTERTYPE::DQFCOMBWSSINE))
				m_QFilter.processBlock(nullptr, &l_inputState, nullptr, l_Set, filter, filterBlock, 0, c_fftSize, true, false, m_VCF, true); //warmup
			m_QFilter.processBlock(nullptr, &l_inputState, nullptr, l_Set, filter, filterBlock, 0, c_fftSize, true, false, m_VCF, false); //doit
		}
	}

	//HACK
	//b_processed = true;




	//if (b_processed) {
	float fftData[c_fftSize * 2];
	for (int idx = 0; idx < c_fftSize; idx++) {
		
		//int pos = (l_Set->m_RoutingBuffers.m_circularFilterOutputPos + idx) % l_Set->m_RoutingBuffers.m_circularFilterOutputLen;
		//fftData[idx] = l_Set->m_RoutingBuffers.CircularFilterOutput->getSample(0, pos);
		fftData[idx] = filterBlock.getSample(0, idx);
	}
	
	//dsp::WindowingFunction<float> window(C_WAVE_TABLE_SIZE * multiplier, dsp::WindowingFunction<float>::hann);
	//window.multiplyWithWindowingTable(fftData, C_WAVE_TABLE_SIZE * multiplier);


	//for (int idx = 0; idx < C_WAVE_TABLE_SIZE * multiplier; idx++) {
		//l_filterBuffer_in[idx].imag(filterBlock.getChannelPointer(0)[idx]); //zero pad				
	//}

	//fft->perform(&*l_filterBuffer_in.begin(), &*l_filterBuffer_out.begin(), false);
	fft->performFrequencyOnlyForwardTransform(fftData);

	//}

	//Sinc interpolation here?
	//https://stackoverflow.com/questions/31836598/subsampling-an-array-of-numbers

	//if (l_filterBuffer_out.empty()) l_display = false;
	if (l_display == true) {		

		//https://dsp.stackexchange.com/questions/26927/what-is-a-frequency-bin

		//l_filterBuffer_out is upsampled by factor multiplier
		//Fs is l_Set->m_nSampleRate * multiplier
		//BinFreq is (l_Set->m_nSampleRate * multiplier) / (l_filterBuffer_out.size() / 2);

		//double binFreq = (l_Set->m_nSampleRate * multiplier) / (l_filterBuffer_out.size()) * l_scaleAdjustFactor; //is definetly dependent on sampleRate!

		//double minDisp = 78.f;
		//double maxDisp = 20000.f;

		auto mindB = -48.0f; 
		auto maxdB = 48.0f; 

		fftResults.setSize(1, int(width), false, false, true);
		//fftResultsCopy.setSize(1, int(width), false, false, true);
		for (int x = 0; x < width; x++) {

			auto skewedProportionX = c_minDisp * pow(c_maxminDisp, float(x)/float(width));
			auto fftDataIndex = jlimit(0, c_fftSize / 2, int(skewedProportionX));


			auto level = jmap(jlimit(mindB, maxdB, Decibels::gainToDecibels(fftData[fftDataIndex])
				),
				//- Decibels::gainToDecibels((float)fftSize)),				
				mindB, maxdB, 0.0f, 1.0f);

			fftResults.setSample(0, x, level);
		}		

		//draw
		Path myWave;
		for (int x = 0; x < width; x++) {
			double yVal = fftResults.getSample(0, x);
			if (x == 0) {
				myWave.startNewSubPath(xOffset + 0, yOffset + height - yVal * height);
			}
			else {
				myWave.lineTo(xOffset + x, yOffset + height - yVal * height);
			}

			//if (isRed) { //error
				//g.setColour(Colours::red);
			//}
			//else
				g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayArea));
			g.drawLine(xOffset + x, yOffset + height - yVal * height, xOffset + x, yOffset + height);
		}
		//if (isRed)
			//vassertfalse;
		g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayLine));
		g.strokePath(myWave, PathStrokeType(2.0f));
		//DropShadow(Colour(0.5f, 0.5f, 0.5f, 0.3f), 1.0, { 3, 1 }).drawForPath(g, myWave);
	}

	//repaint();
	b_newImageReady = true;
	m_bThreadRunning = false;
}
*/

/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTFreqDomainViewport.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTAudioProcessor.h"
#include "../VASTWaveTableEditorComponent.h"
#include "../../Engine/VASTSingleNote.h"
#include "../VASTControls/VASTManualFreqdomainValueEditor.h"

const float reToImSliderRatio = 0.64f;

VASTFreqDomainViewport::VASTFreqDomainViewport()
{
	myProcessor = nullptr;
	myEditor = nullptr;
	myWtEditor = nullptr;

	setOpaque(true);
    
	//Image oldImage(waveformImage);
	waveformImage = Image(Image::RGB, 400, 300, true);
	waveformImage.clear(waveformImage.getBounds(), juce::Colour::fromFloatRGBA(0.1f, 0.1f, 0.12f, 1.0f));

	setComponentID("freqDomainViewport");

	setPaintingIsUnclipped(true);

	m_scaling = 8;
	m_sliderThumbWidth = 3;
	
	setSize(1500, 100);
	
	m_reSliderBottomY = getHeight() * reToImSliderRatio;
	m_reSliderTopY = 0.0f;
	m_imSliderBottomY = getHeight();
	m_imSliderTopY = getHeight() * reToImSliderRatio;
	m_sliderWidth = m_sliderThumbWidth + m_Offset * 2;

	//maxRe = sqrtf(C_WAVE_TABLE_SIZE / 2.0f); //normalize with n/2
	maxRe = 0.707f;
	maxReDisp = 0.6f;
	maxIm = float(M_PI); 
}

VASTFreqDomainViewport::~VASTFreqDomainViewport() {
	this->setLookAndFeel(nullptr);
	stopTimer();
}

void VASTFreqDomainViewport::setZoomFactor(int zoom) {
	if (myWtEditor == nullptr) return;
	if (!myWtEditor->c_waveTableEditorView) return;
	if (!myWtEditor->c_waveTableEditorView->c_viewportFreqDomain) return;
	if (myWtEditor->getEditorView()->c_viewportFreqDomain->getWidth() == 0)
		return;
	m_scaling = zoom;	
	m_screenWidthScale = float(myWtEditor->getEditorView()->c_viewportFreqDomain->getScreenBounds().getWidth()) / float(myWtEditor->getEditorView()->c_viewportFreqDomain->getWidth());
	m_screenHeightScale = float(myWtEditor->getEditorView()->c_viewportFreqDomain->getScreenBounds().getHeight()) / float(myWtEditor->getEditorView()->c_viewportFreqDomain->getHeight());
	m_sliderThumbWidth = m_scaling * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale;
	m_Offset = 1.f * myProcessor->getPluginScaleWidthFactor();
	m_sliderWidth = (m_sliderThumbWidth + m_Offset * 2 * m_screenWidthScale);

	int newWidth = m_sliderWidth * m_numSliders + 50.f;
	juce::Rectangle<int> lVisibleArea = myWtEditor->getFreqDomainViewport()->getViewArea();
	setSize(newWidth / m_screenWidthScale, lVisibleArea.getHeight());
}

void VASTFreqDomainViewport::resized()
{
	if (myWtEditor == nullptr) return;
	if (!myWtEditor->c_waveTableEditorView) return;
	if (!myWtEditor->c_waveTableEditorView->c_viewportFreqDomain) return;

	m_screenWidthScale = float(myWtEditor->getEditorView()->c_viewportFreqDomain->getScreenBounds().getWidth()) / float(myWtEditor->getEditorView()->c_viewportFreqDomain->getWidth());
	m_screenHeightScale = float(myWtEditor->getEditorView()->c_viewportFreqDomain->getScreenBounds().getHeight()) / float(myWtEditor->getEditorView()->c_viewportFreqDomain->getHeight());
	m_sliderThumbWidth = m_scaling * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale;
	m_Offset = 1.f * myProcessor->getPluginScaleWidthFactor();
	m_sliderWidth = (m_sliderThumbWidth + m_Offset * 2 * m_screenWidthScale);

	m_dirty = true;
}

void VASTFreqDomainViewport::paint(Graphics& g)
{
	//g.drawImageAt(waveformImage, 0, 0);	
	juce::Rectangle<int> lVisibleArea = myWtEditor->getFreqDomainViewport()->getViewArea();
	g.drawImageWithin(waveformImage, lVisibleArea.getX(), 0, lVisibleArea.getWidth(), lVisibleArea.getHeight(), juce::RectanglePlacement::stretchToFit);

}

void VASTFreqDomainViewport::updateContent(bool force) {
	if (!force) {
		if (this->isShowing() == false)
			return;
	}
	if ((getWidth() <= 1) || (getHeight() <= 1))
		return;

	if (myProcessor == nullptr) return;
	if (myProcessor == nullptr) return;
	if (myWtEditor == nullptr) return;
	if (!myWtEditor->c_waveTableEditorView) return;
	if (!myWtEditor->getEditorView()->c_viewportFreqDomain) return;

	if (!(force || m_dirty))
		if (!(myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->getOscBank()].getAndClearSoftChangedFlagFdv())) 
		//		if (!(myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->getOscBank()].getAndClearSoftChangedFlagFdv() || myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->getOscBank()].isChanged())) 
			return;
	m_dirty = false;

	if ((m_needsInit) || (!approximatelyEqual(m_screenWidthScale, float(myWtEditor->getEditorView()->c_viewportFreqDomain->getScreenBounds().getWidth()) / float(myWtEditor->getEditorView()->c_viewportFreqDomain->getWidth())))) {
		setZoomFactor(m_scaling);
		m_needsInit = false;
	}

	std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
    if (wavetable->m_isBeingUpdated.load() == true) //safety
        return;
    if (wavetable->getNumPositions() == 0)
		return;

	//VDBG("VASTFreqDomainViewport::updateContent");

	juce::Rectangle<int> lVisibleArea = myWtEditor->getFreqDomainViewport()->getViewArea();
	AffineTransform af;
	lVisibleArea = lVisibleArea.transformedBy(af.scaled(m_screenWidthScale, m_screenHeightScale));

	waveformImage = Image(Image::RGB, jmax(1, lVisibleArea.getWidth()), jmax(1, lVisibleArea.getHeight()), false);
	waveformImage.clear(waveformImage.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFreqDomainBinBackground));

	m_sliderThumbWidth = m_scaling * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale;
	m_Offset = 1.f * myProcessor->getPluginScaleWidthFactor();
	m_sliderWidth = (m_sliderThumbWidth + m_Offset * 2 * m_screenWidthScale);		
	m_reSliderBottomY = waveformImage.getHeight() * reToImSliderRatio;
	m_reSliderTopY = 0.0f;
	m_imSliderBottomY = waveformImage.getHeight();
	m_imSliderTopY = waveformImage.getHeight()  * reToImSliderRatio;
	
	Graphics g(waveformImage);

	int visiStart = lVisibleArea.getX();
	int visiEnd = lVisibleArea.getTopRight().getX();
	int sliderStart = visiStart / m_sliderWidth;
	int sliderEnd = visiEnd / m_sliderWidth;
	if (sliderEnd >= m_numSliders) sliderEnd = m_numSliders;

	for (int i = sliderStart; i < sliderEnd; i++) { //skip 1st!!, only display left mirror part
													//for (int i = 0; i < m_numSliders; i++) { //skip 1st!!, only display left mirror part
		juce::Colour backgroundCol = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFreqDomainBinBorder); //grey		
		if (m_mouseover_arrayidx == (i + 1))
			backgroundCol = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFreqDomainBinBorderMouseover);
		g.setColour(backgroundCol); //grey		

		g.drawRect(m_sliderWidth * (i - sliderStart), m_reSliderTopY, m_sliderWidth, m_reSliderBottomY - m_reSliderTopY, 1.0f * myProcessor->getPluginScaleWidthFactor());
		g.drawRect(m_sliderWidth * (i - sliderStart), m_imSliderTopY, m_sliderWidth, m_imSliderBottomY - m_imSliderTopY, 1.0f * myProcessor->getPluginScaleWidthFactor());

		if ((i + 1) && !((i + 1) & (i))) { //zweierpotenz i+1
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFreqDomainBinBackgroundOctaves)); //greenish			
			g.fillRect(m_sliderWidth * (i - sliderStart) + m_Offset, m_reSliderTopY, m_sliderThumbWidth, m_reSliderBottomY - m_reSliderTopY);
			g.fillRect(m_sliderWidth * (i - sliderStart) + m_Offset, m_imSliderTopY, m_sliderThumbWidth, m_imSliderBottomY - m_imSliderTopY);
		}
		g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFreqDomainBinMagnitude));				
		/*
		float real = wavetable->getFreqDomainBuffer(myWtEditor->getWtPos())[i + 1].real();
		float imag = wavetable->getFreqDomainBuffer(myWtEditor->getWtPos())[i + 1].imag();
		float magnitudeDB = 20.f * log(sqrt(real * real + imag * imag));  //https://www.rohde-schwarz.com/us/faq/converting-the-real-and-imaginary-numbers-to-magnitude-in-db-and-phase-in-degrees.-faq_78704-30465.html
		//float gainMultiplier = powf(2.0f, magnitudeDB / 6);
		float percentageRe = (1.f + magnitudeDB / 100.f);
		if ((percentageRe < 0.f) || (isinf(percentageRe))) percentageRe = 0.f;
		*/
		
		int wtPos = myWtEditor->getWtPos();
		if (myWtEditor->getWtPos() >= wavetable->getNumPositions())
			wtPos--;
		std::vector<dsp::Complex<float>>* vec = wavetable->getFreqDomainBuffer(wtPos);
		float percentageRe = getDomainBufferSlotRealPercentage((*vec)[i + 1]);

		//float barHeight = m_reSliderBottomY * abs(wavetable->getFreqDomainBuffer(myWtEditor->getWtPos())[i+1]) / maxReDisp;
		float barHeight = m_reSliderBottomY * percentageRe;
		
		if (!isnan(barHeight)) {
			g.fillRect(m_sliderWidth * (i - sliderStart) + m_Offset, m_reSliderBottomY - barHeight, m_sliderThumbWidth, barHeight);

			if (barHeight > 0.01f) { //skip imag noise??? CHECK
				g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFreqDomainBinPhases));
				
				/*
				float phase = atan2(imag, real);
				float percentageIm = ((phase + M_PI) * 0.5f) / M_PI; // phase is -pi .. pi
				
				if (percentageIm < 0.00001f) percentageIm = 1.f; //CHECK HACK
				*/
				float percentageIm = getDomainBufferSlotImagPercentage((*vec)[i + 1]);

				//barHeight = m_imSliderBottomY * (1 - reToImSliderRatio) * abs(arg(wavetable->getFreqDomainBuffer(myWtEditor->getWtPos())[i + 1])) / maxIm;
				barHeight = m_imSliderBottomY * (1 - reToImSliderRatio) * percentageIm;

				if (!isnan(barHeight)) {
					g.fillRect(m_sliderWidth * (i - sliderStart) + m_Offset, m_imSliderBottomY - barHeight, m_sliderThumbWidth, barHeight);
				}
			}
		}
	}
	repaint();
}

void VASTFreqDomainViewport::visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea) {
	if (this->isVisible()) { //skip on close
		updateContentAsync();
		//updateContent(false); //CHECK was true
	}
}

void VASTFreqDomainViewport::startAutoUpdate() {
	startTimer(50);     // orig value: repaint every 50 millisecons
						//startTimer(400);     // repaint every 0.4 seconds
}

void VASTFreqDomainViewport::stopAutoUpdate() {
	stopTimer();
}

void VASTFreqDomainViewport::timerCallback() {
	if (!myProcessor->m_initCompleted)
		return;
	updateContent(false);
}

void VASTFreqDomainViewport::updateContentAsync() {
	m_dirty = true;
}

int VASTFreqDomainViewport::mouseXGetBinArrayIdx(int posX) {
	juce::Rectangle<int> lVisibleArea = myWtEditor->getFreqDomainViewport()->getViewArea();
	int pos = posX - (lVisibleArea.getX() - int(lVisibleArea.getX() / m_sliderWidth) * m_sliderWidth);
	int arrayidx = jlimit<int>(1, 1023, 1 + int(pos / (m_sliderWidth / m_screenWidthScale) /*+ 0.5f*/)); //skip first
	return arrayidx;
}

void VASTFreqDomainViewport::mouseMove(const MouseEvent& event) {
	int posX = event.getPosition().getX();
	int posY = event.getPosition().getY();
	if (posY < m_reSliderBottomY) { //real slider
		m_mouseover_arrayidx = mouseXGetBinArrayIdx(posX);
		m_dirty = true;
	}
	else if (posY > m_imSliderTopY / m_screenHeightScale) { //imag slider
		m_mouseover_arrayidx = mouseXGetBinArrayIdx(posX);
		m_dirty = true;
	}
	else
		m_mouseover_arrayidx = -1;
}

void VASTFreqDomainViewport::mouseExit(const MouseEvent& event) {
	m_mouseover_arrayidx = -1;
}

void VASTFreqDomainViewport::adjustFreqDomainInternalThreaded(std::vector<sFreqDomainBuffer> domainBuffer, bool clipBins, VASTWaveTableEditorComponent* myWtEditor, VASTAudioProcessor* myProcessor) {
    
    if (myWtEditor->numFreqThreads.load() > 0) return; //just ignore if other thread is running
    
    myWtEditor->numFreqThreads+=1;
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
	std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(); //check!

	//std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
	//myWtEditor->setCurWavetable(wavetable); //make a copy
	//wavetable = myWtEditor->getCurWavetable(); //read the copy and work on the copy

	for (int i = 0; i < domainBuffer.size(); i++) {
		sFreqDomainBuffer domainBufferPos = domainBuffer[i];
		wavetable->setFreqDomainTables(domainBufferPos.wtPos, &domainBufferPos.domainBuffer, true, clipBins, myProcessor->getWTmode());
	}

	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->getOscBank()].setWavetableSoftFade(wavetable);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
    myWtEditor->numFreqThreads-=1;
}

static const double c_freqbin_maxdb = -16.f;
static const double c_freqbin_mindb = -192.f;
double VASTFreqDomainViewport::getDomainBufferSlotRealPercentage(dsp::Complex<float> currentSlotVal) {
	double real = currentSlotVal.real();
	double imag = currentSlotVal.imag();
	double magnitudeDB = 20.0 * log(sqrt(real * real + imag * imag));  //https://www.rohde-schwarz.com/us/faq/converting-the-real-and-imaginary-numbers-to-magnitude-in-db-and-phase-in-degrees.-faq_78704-30465.html
																	  //float gainMultiplier = powf(2.0f, magnitudeDB / 6);
	if (isinf(magnitudeDB) || (magnitudeDB < c_freqbin_mindb)) magnitudeDB = c_freqbin_mindb;
	double percentageRe = 0.f;
	if (magnitudeDB < c_freqbin_mindb) magnitudeDB = c_freqbin_mindb; //clip
	if (magnitudeDB > c_freqbin_maxdb) magnitudeDB = c_freqbin_maxdb; //clip
	if (!isinf(magnitudeDB)) {
		double logarithmicRe = jmap(magnitudeDB, c_freqbin_mindb, c_freqbin_maxdb, 0.0, 1.0); //goes down to min -192 db max 0.0db
		percentageRe = powf(logarithmicRe, 3); 
	}
	jassert((percentageRe >= 0.f) && (percentageRe <= 1.f));
	return percentageRe;
}

double VASTFreqDomainViewport::getDomainBufferSlotImagPercentage(dsp::Complex<float> currentSlotVal) {
	
	double phase = atan2(currentSlotVal.imag(), currentSlotVal.real());
	double percentageIm = ((phase + 3.1415926535897932384626433832795) * 0.5) / 3.1415926535897932384626433832795; // phase is -pi .. pi

	if (percentageIm < 0.001) percentageIm = 0.0; //to avoid that 100 and 0% 
	if (percentageIm > 0.95) percentageIm = 0.0; //to avoid that 100 and 0% 
	return percentageIm;
}

void VASTFreqDomainViewport::setEditor(VASTAudioProcessorEditor* editor) { myEditor = editor; }

void VASTFreqDomainViewport::setProcessor(VASTAudioProcessor* processor) { myProcessor = processor; }

void VASTFreqDomainViewport::setWTEditor(VASTWaveTableEditorComponent* wtEditor) { myWtEditor = wtEditor; }

dsp::Complex<float> VASTFreqDomainViewport::setDomainBufferSlotRealPercentage(double percentage, dsp::Complex<float> currentSlotVal) {
	//https://www.rohde-schwarz.com/us/faq/converting-the-real-and-imaginary-numbers-to-magnitude-in-db-and-phase-in-degrees.-faq_78704-30465.html
	//The formulas for calculating the magnitude and the phase from the real(Re) and imaginary(Im) numbers are :
	//magnitude[dB] = 20 * Log(sqr(Re ^ 2 + Im ^ 2))
	//phase = arctan(Im / Re)
	//float magnitudeDB = 20.f * log(sqrt(domainBuffer[slot].real() * domainBuffer[slot].real() + domainBuffer[slot].imag() * domainBuffer[slot].imag()));		

	double oldPerc = getDomainBufferSlotRealPercentage(currentSlotVal);
	if (oldPerc == percentage)
		return currentSlotVal; // do nothing

	if (percentage > 1.0) percentage = 1.0;
	if (percentage < 0.0) percentage = 0.0;

	double logarithmicRe = double(powf(percentage, 1.0f / 3.0f));
	double magnitudeDB = jmap(logarithmicRe, 0.0, 1.0, c_freqbin_mindb, c_freqbin_maxdb); //check 0 to 1?

	double magnitude = abs(exp(magnitudeDB / 20.0f)); //clean
	if (magnitudeDB == c_freqbin_mindb)
		magnitude = 0.f;
	
	//https://www.researchgate.net/post/How_do_I_convert_a_magnitude_and_phase_into_a_complex_number

	float phase = arg(currentSlotVal);
	if (oldPerc < 0.001f) phase = float(M_PI);
	return std::polar(float(magnitude), phase);
}

dsp::Complex<float> VASTFreqDomainViewport::setDomainBufferSlotImagPercentage(double percentage, dsp::Complex<float> currentSlotVal) {
	//float magnitude = sqrt(domainBuffer[slot].real() * domainBuffer[slot].real() + domainBuffer[slot].imag() * domainBuffer[slot].imag());
	//https://www.researchgate.net/post/How_do_I_convert_a_magnitude_and_phase_into_a_complex_number

	if (percentage > 1.0) percentage = 1.0;
	if (percentage < 0.0) percentage = 0.0;

	//float phase = percentage * M_PI / 0.5f - M_PI; // phase is -pi .. pi
	float phase = percentage * 2.0 * 3.1415926535897932384626433832795 - 3.1415926535897932384626433832795; // phase is -pi .. pi
	float magnitude = abs(currentSlotVal);

	return std::polar(magnitude, phase);
}

void VASTFreqDomainViewport::adjustFreqDomainReal(int slot, double percentage) {
	if (myWtEditor == nullptr) return;
	std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();	
	std::vector<sFreqDomainBuffer> domainBuffer;
	
	int msbeg = wavetable->getSelectedWtPos();
	int msend = wavetable->getSelectedWtPos();
	if (wavetable->isMultiSelected()) {
		msbeg = wavetable->getMultiSelectBegin();
		msend = wavetable->getMultiSelectEnd();
	}

	for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
		sFreqDomainBuffer domainBufferPos;
		domainBufferPos.wtPos = wtPos;
		domainBufferPos.domainBuffer = *wavetable->getFreqDomainBuffer(wtPos);

		if (myProcessor->getBinEditMode() == FreqEditMode::SingleBin) {
			domainBufferPos.domainBuffer[slot] = setDomainBufferSlotRealPercentage(percentage, domainBufferPos.domainBuffer[slot]);
		}
		else if (myProcessor->getBinEditMode() == FreqEditMode::EverySecond) {
			for (int i = 0; i < C_WAVE_TABLE_SIZE / 2 - slot; i++) {
				float perc = percentage * 2.f / (i / 2.f + 2.f);
				if (perc > 0.001f)
					if (i % 2 == 0) domainBufferPos.domainBuffer[i + slot] = setDomainBufferSlotRealPercentage(perc, domainBufferPos.domainBuffer[i + slot]);
			}
		}
		else if (myProcessor->getBinEditMode() == FreqEditMode::EveryThird) {
			for (int i = 0; i < C_WAVE_TABLE_SIZE / 2 - slot; i++) {
				float perc = percentage * 2.f / (i / 3.f + 2.f);
				if (perc > 0.001f)
					if (i % 3 == 0) domainBufferPos.domainBuffer[i + slot] = setDomainBufferSlotRealPercentage(perc, domainBufferPos.domainBuffer[i + slot]);
			}
		}
		else if (myProcessor->getBinEditMode() == FreqEditMode::Pile) {
			for (int i = 0; i < 10; i++) {
				if (i + slot < C_WAVE_TABLE_SIZE / 2) {
					float perc = percentage * 2.f / (i + 2.f);
					if (perc > 0.05f)
						domainBufferPos.domainBuffer[i + slot] = setDomainBufferSlotRealPercentage(perc, domainBufferPos.domainBuffer[i + slot]);
				}
				if (slot - i > 0) {
					float perc = percentage * 2.f / (i + 2.f);
					if (perc > 0.05f)
						domainBufferPos.domainBuffer[slot - i] = setDomainBufferSlotRealPercentage(perc, domainBufferPos.domainBuffer[slot - i]);
				}
			}
		}
		else if (myProcessor->getBinEditMode() == FreqEditMode::DragShift) {
			for (int i = 1; i < C_WAVE_TABLE_SIZE / 2; i++) {
				int newPos = i + m_arrayidxWhenDragStart - slot;
				if ((newPos > 0) && (newPos < C_WAVE_TABLE_SIZE / 2))
					domainBufferPos.domainBuffer[i] = m_domainBufferWhenDragStart[wtPos - msbeg].domainBuffer[newPos];
				else 
					domainBufferPos.domainBuffer[i] = 0;
			}			
		}
		else if (myProcessor->getBinEditMode() == FreqEditMode::DragStretch) {
			if (m_arrayidxWhenDragStart - slot != 0) {
				for (int i = 1; i < C_WAVE_TABLE_SIZE / 2; i++)
					domainBufferPos.domainBuffer[i] = 0;
				for (int i = 1; i < C_WAVE_TABLE_SIZE / 2; i++) {
					float skip = (slot - m_arrayidxWhenDragStart);
					if (skip < 0.f)
						skip = abs(1 / float(skip / 8.f - 1));
					else
						skip = (skip / 8.f) + 1.f;
					int newPos = i * skip;
					if ((newPos >= 1) && (newPos < C_WAVE_TABLE_SIZE / 2))
						domainBufferPos.domainBuffer[newPos] = m_domainBufferWhenDragStart[wtPos - msbeg].domainBuffer[i];
				}
			}
			else {
				for (int i = 1; i < C_WAVE_TABLE_SIZE / 2; i++) {
					domainBufferPos.domainBuffer[i] = m_domainBufferWhenDragStart[wtPos - msbeg].domainBuffer[i];
				}
			}
		}
		domainBuffer.push_back(domainBufferPos);
	}
	bool bClip = (myProcessor->getBinMode() == BinMode::ClipBin);
	std::thread edit_thread(&VASTFreqDomainViewport::adjustFreqDomainInternalThreaded, domainBuffer, bClip, myWtEditor, myProcessor);
	edit_thread.detach();
}

void VASTFreqDomainViewport::adjustFreqDomainImag(int slot, double percentage) {
	if (myWtEditor == nullptr) return;
	std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
	std::vector<sFreqDomainBuffer> domainBuffer;

	int msbeg = wavetable->getSelectedWtPos();
	int msend = wavetable->getSelectedWtPos();
	if (wavetable->isMultiSelected()) {
		msbeg = wavetable->getMultiSelectBegin();
		msend = wavetable->getMultiSelectEnd();
	}

	for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
		sFreqDomainBuffer domainBufferPos;
		domainBufferPos.wtPos = wtPos;
		domainBufferPos.domainBuffer = *wavetable->getFreqDomainBuffer(wtPos);		
		if (myProcessor->getBinEditMode() == FreqEditMode::SingleBin) {
			domainBufferPos.domainBuffer[slot] = setDomainBufferSlotImagPercentage(percentage, domainBufferPos.domainBuffer[slot]);
			VDBG("Percentage: " << percentage);
		}
		else if (myProcessor->getBinEditMode() == FreqEditMode::EverySecond) {
			for (int i = 0; i < C_WAVE_TABLE_SIZE / 2 - slot; i++) {
				float perc = percentage * 2.f / (i / 2.f + 2.f);
				if (perc > 0.001f)
					if (i % 2 == 0) domainBufferPos.domainBuffer[i + slot] = setDomainBufferSlotImagPercentage(perc, domainBufferPos.domainBuffer[i + slot]);
			}
		}
		else if (myProcessor->getBinEditMode() == FreqEditMode::EveryThird) {
			for (int i = 0; i < C_WAVE_TABLE_SIZE / 2 - slot; i++) {
				float perc = percentage * 2.f / (i / 3.f + 21.f);
				if (perc > 0.001f)
					if (i % 3 == 0) domainBufferPos.domainBuffer[i + slot] = setDomainBufferSlotImagPercentage(perc, domainBufferPos.domainBuffer[i + slot]);
			}
		}
		else if (myProcessor->getBinEditMode() == FreqEditMode::Pile) {
			for (int i = 0; i < 10; i++) {
				if (i + slot < C_WAVE_TABLE_SIZE / 2) {
					float perc = percentage * 2.f / (i + 2.f);
					if (perc > 0.05f)
						domainBufferPos.domainBuffer[i + slot] = setDomainBufferSlotImagPercentage(perc, domainBufferPos.domainBuffer[i + slot]);
				}
				if (slot - i > 0) {
					float perc = percentage * 2.f / (i + 2.f);
					if (perc > 0.05f)
						domainBufferPos.domainBuffer[slot - i] = setDomainBufferSlotImagPercentage(perc, domainBufferPos.domainBuffer[slot - i]);
				}
			}
		}
		else if (myProcessor->getBinEditMode() == FreqEditMode::DragShift) {
			for (int i = 1; i < C_WAVE_TABLE_SIZE / 2; i++) {
				int newPos = i + m_arrayidxWhenDragStart - slot;
				if ((newPos > 0) && (newPos < C_WAVE_TABLE_SIZE / 2))
					domainBufferPos.domainBuffer[i] = m_domainBufferWhenDragStart[wtPos - msbeg].domainBuffer[newPos];
				else
					domainBufferPos.domainBuffer[i] = 0;
			}
		}
		else if (myProcessor->getBinEditMode() == FreqEditMode::DragStretch) {
			if (m_arrayidxWhenDragStart - slot != 0) {
				for (int i = 1; i < C_WAVE_TABLE_SIZE / 2; i++)
					domainBufferPos.domainBuffer[i] = 0;
				for (int i = 1; i < C_WAVE_TABLE_SIZE / 2; i++) {
					float skip = (slot - m_arrayidxWhenDragStart);
					if (skip < 0.f)
						skip = abs(1 / float(skip / 8.f - 1));
					else
						skip = (skip / 8.f) + 1.f;
					int newPos = i * skip;
					if ((newPos >= 1) && (newPos < C_WAVE_TABLE_SIZE / 2))
						domainBufferPos.domainBuffer[newPos] = m_domainBufferWhenDragStart[wtPos - msbeg].domainBuffer[i];
				}
			}
			else {
				for (int i = 1; i < C_WAVE_TABLE_SIZE / 2; i++) {
					domainBufferPos.domainBuffer[i] = m_domainBufferWhenDragStart[wtPos - msbeg].domainBuffer[i];
				}
			}
		}
		domainBuffer.push_back(domainBufferPos);
	}
	bool bClip = (myProcessor->getBinMode() == BinMode::ClipBin);
	std::thread edit_thread(&VASTFreqDomainViewport::adjustFreqDomainInternalThreaded, domainBuffer, bClip, myWtEditor, myProcessor);
	edit_thread.detach();
}

void VASTFreqDomainViewport::mouseDown(const MouseEvent &e) {
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	int x = e.getMouseDownX();
	int y = e.getMouseDownY();

	//int arrayidx = jlimit<int>(1, 1227, int(x / (m_sliderWidth / m_screenWidthScale) + 0.5f)); //skip first
	int arrayidx = mouseXGetBinArrayIdx(x);
	if (myWtEditor == nullptr) return;
	if (arrayidx >= C_WAVE_TABLE_SIZE / 2) return;

	m_arrayidxWhenDragStart = arrayidx;
	if ((myProcessor->getBinEditMode() == FreqEditMode::DragShift) || (myProcessor->getBinEditMode() == FreqEditMode::DragStretch)) {
		std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();		
		int msbeg = wavetable->getSelectedWtPos();
		int msend = wavetable->getSelectedWtPos();
		if (wavetable->isMultiSelected()) {
			msbeg = wavetable->getMultiSelectBegin();
			msend = wavetable->getMultiSelectEnd();
		}
		m_domainBufferWhenDragStart.clear();
		for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
			sFreqDomainBuffer domainBufferPos;
			domainBufferPos.wtPos = wtPos;
			domainBufferPos.domainBuffer = *wavetable->getFreqDomainBuffer(wtPos);
			m_domainBufferWhenDragStart.push_back(domainBufferPos);
		}
	}

	if (modifiers.isPopupMenu()) { //context menu
		PopupMenu mainMenu;
		mainMenu.setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());

		std::shared_ptr<CVASTWaveTable> lwavetable = myWtEditor->getBankWavetable();
		std::vector<dsp::Complex<float>>* lvec = lwavetable->getFreqDomainBuffer(myWtEditor->getWtPos());
		int lPartialNo = arrayidx;
		float lCurMagnitudeValue = getDomainBufferSlotRealPercentage((*lvec)[lPartialNo]);;
		float lCurPhaseValue = getDomainBufferSlotImagPercentage((*lvec)[lPartialNo]);

		mainMenu.addSectionHeader(TRANS("Bin (harmonics) data"));
		mainMenu.addItem(1, "Bin number: " + String(lPartialNo), false);
		mainMenu.addItem(2, "Cur. magnitude value: " + String(lCurMagnitudeValue), false);
		mainMenu.addItem(3, "Cur. phase value: " + String(lCurPhaseValue), false);
		mainMenu.addItem(4, "Type in values");
		mainMenu.addSeparator();
		PopupMenu subMenuClipMode;
		mainMenu.addSectionHeader(TRANS("Clip mode"));
		subMenuClipMode.addItem(11, TRANS("Clip"), true, (myProcessor->getBinMode() == BinMode::ClipBin));
		subMenuClipMode.addItem(12, TRANS("Normalize"), true, (myProcessor->getBinMode() == BinMode::NormalizeAll));
		mainMenu.addSubMenu(TRANS("Clip mode types"), subMenuClipMode, true);
		mainMenu.addSeparator();
		PopupMenu subMenuEditMode;
		mainMenu.addSectionHeader(TRANS("Edit mode"));
		subMenuEditMode.addItem(14, TRANS("Single bin"), true, (myProcessor->getBinEditMode() == FreqEditMode::SingleBin));
		subMenuEditMode.addItem(15, TRANS("Every 2nd"), true, (myProcessor->getBinEditMode() == FreqEditMode::EverySecond));
		subMenuEditMode.addItem(16, TRANS("Every third"), true, (myProcessor->getBinEditMode() == FreqEditMode::EveryThird));
		subMenuEditMode.addItem(17, TRANS("Pile"), true, (myProcessor->getBinEditMode() == FreqEditMode::Pile));
		subMenuEditMode.addItem(18, TRANS("Drag & Shift"), true, (myProcessor->getBinEditMode() == FreqEditMode::DragShift));
		subMenuEditMode.addItem(19, TRANS("Drag & Stretch"), true, (myProcessor->getBinEditMode() == FreqEditMode::DragStretch));
		mainMenu.addSubMenu(TRANS("Edit mode types"), subMenuEditMode, true);
		mainMenu.addSeparator();
		PopupMenu subMenuFilter;
		mainMenu.addSectionHeader(TRANS("Filter"));
		subMenuFilter.addItem(20, TRANS("Clear on left (high pass filter)"), true, false);
		subMenuFilter.addItem(21, TRANS("Clear on right (low pass filter)"), true, false);		
		mainMenu.addSubMenu(TRANS("Filter types"), subMenuFilter, true);
		
		mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
			juce::ModalCallbackFunction::create([this, arrayidx, lPartialNo, lCurMagnitudeValue, lCurPhaseValue, y](int result) {
			if (result == 0) {
				// user dismissed the menu without picking anything
			}
			else if (result == 4) { //type in
				std::unique_ptr<VASTManualFreqdomainValueEditor> l_veditor = std::make_unique<VASTManualFreqdomainValueEditor>(myProcessor, myWtEditor, lPartialNo); //OK will be owned by the cb
				l_veditor->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
				l_veditor->setSize(300.f * myProcessor->getPluginScaleWidthFactor(), 60.f * myProcessor->getPluginScaleHeightFactor());
				l_veditor->setOpaque(true);

				Point<int> cPoint = myEditor->vaporizerComponent->getLocalPoint(this, Point<int>((arrayidx - 1) * m_sliderWidth / m_screenWidthScale, 0));
				juce::Rectangle<int> newBounds = juce::Rectangle<int>(cPoint.getX(), cPoint.getY() - getHeight() * 0.5f, m_sliderWidth / m_screenWidthScale, getHeight());

				l_veditor->setTextMagnitude(String(lCurMagnitudeValue));
				l_veditor->setTextPhase(String(lCurPhaseValue));

				//CallOutBox &cb =
				juce::CallOutBox::launchAsynchronously(std::move(l_veditor), newBounds, myEditor->vaporizerComponent.get());
			}
			else if (result == 11) {
				myWtEditor->setBinMode(BinMode::ClipBin);
				myWtEditor->updateAll(true);
			}
			else if (result == 12) {
				myWtEditor->setBinMode(BinMode::NormalizeAll);
				myWtEditor->updateAll(true);
			}
			else if (result == 14) {
				myWtEditor->setBinEditMode(FreqEditMode::SingleBin);
				myWtEditor->updateAll(true);
			}
			else if (result == 15) {
				myWtEditor->setBinEditMode(FreqEditMode::EverySecond);
				myWtEditor->updateAll(true);
			}
			else if (result == 16) {
				myWtEditor->setBinEditMode(FreqEditMode::EveryThird);
				myWtEditor->updateAll(true);
			}
			else if (result == 17) {
				myWtEditor->setBinEditMode(FreqEditMode::Pile);
				myWtEditor->updateAll(true);
			}
			else if (result == 18) {
				myWtEditor->setBinEditMode(FreqEditMode::DragShift);
				myWtEditor->updateAll(true);
			}
			else if (result == 19) {
				myWtEditor->setBinEditMode(FreqEditMode::DragStretch);
				myWtEditor->updateAll(true);
			}
			else if (result == 20) {
				//clear on left
				std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
				std::vector<sFreqDomainBuffer> domainBuffer;

				int msbeg = wavetable->getSelectedWtPos();
				int msend = wavetable->getSelectedWtPos();
				if (wavetable->isMultiSelected()) {
					msbeg = wavetable->getMultiSelectBegin();
					msend = wavetable->getMultiSelectEnd();
				}

				for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
					sFreqDomainBuffer domainBufferPos;
					domainBufferPos.wtPos = wtPos;
					domainBufferPos.domainBuffer = *wavetable->getFreqDomainBuffer(wtPos);

					if (y < m_reSliderBottomY / m_screenHeightScale) { //real slider
						for (int i = 0; i < arrayidx; i++) {
							domainBufferPos.domainBuffer[i] = setDomainBufferSlotImagPercentage(0.f, domainBufferPos.domainBuffer[i]);
							domainBufferPos.domainBuffer[i] = setDomainBufferSlotRealPercentage(0.f, domainBufferPos.domainBuffer[i]);
						}
					}
					else if (y > m_imSliderTopY / m_screenHeightScale) { //imag slider
						for (int i = 0; i < arrayidx; i++)
							domainBufferPos.domainBuffer[i] = setDomainBufferSlotImagPercentage(0.f, domainBufferPos.domainBuffer[i]);
					}

					domainBuffer.push_back(domainBufferPos);
				}
				bool bClip = (myProcessor->getBinMode() == BinMode::ClipBin);
				std::thread edit_thread(&VASTFreqDomainViewport::adjustFreqDomainInternalThreaded, domainBuffer, bClip, myWtEditor, myProcessor);
				edit_thread.detach();
			}
			else if (result == 21) {
				//clear on right
				std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
				std::vector<sFreqDomainBuffer> domainBuffer;

				int msbeg = wavetable->getSelectedWtPos();
				int msend = wavetable->getSelectedWtPos();
				if (wavetable->isMultiSelected()) {
					msbeg = wavetable->getMultiSelectBegin();
					msend = wavetable->getMultiSelectEnd();
				}

				for (int wtPos = msbeg; wtPos <= msend; wtPos++) {
					sFreqDomainBuffer domainBufferPos;
					domainBufferPos.wtPos = wtPos;
					domainBufferPos.domainBuffer = *wavetable->getFreqDomainBuffer(wtPos);
					if (y < m_reSliderBottomY / m_screenHeightScale) { //real slider
						for (int i = arrayidx + 1; i < C_WAVE_TABLE_SIZE / 2; i++) {
							domainBufferPos.domainBuffer[i] = setDomainBufferSlotImagPercentage(0.f, domainBufferPos.domainBuffer[i]);
							domainBufferPos.domainBuffer[i] = setDomainBufferSlotRealPercentage(0.f, domainBufferPos.domainBuffer[i]);
						}
					}
					else if (y > m_imSliderTopY / m_screenHeightScale) { //imag slider
						for (int i = arrayidx + 1; i < C_WAVE_TABLE_SIZE / 2; i++)
							domainBufferPos.domainBuffer[i] = setDomainBufferSlotImagPercentage(0.f, domainBufferPos.domainBuffer[i]);
					}

					domainBuffer.push_back(domainBufferPos);
				}
				bool bClip = (myProcessor->getBinMode() == BinMode::ClipBin);
				std::thread edit_thread(&VASTFreqDomainViewport::adjustFreqDomainInternalThreaded, domainBuffer, bClip, myWtEditor, myProcessor);
				edit_thread.detach();
			}
		}));
	}
	else if (modifiers.isLeftButtonDown()) { //select it

		if (y < m_reSliderBottomY / m_screenHeightScale) { //real slider
			float percentage = (m_reSliderBottomY / m_screenHeightScale - y) / (m_reSliderBottomY / m_screenHeightScale);
			if (percentage < 0.08) percentage = 0.f;
			jassert((percentage >= 0.f) && (percentage <= 1.f));
			adjustFreqDomainReal(arrayidx, percentage);
		}
		else if (y > m_imSliderTopY / m_screenHeightScale) { //imag slider
			float percentage = (m_imSliderBottomY / m_screenHeightScale - y) / (m_imSliderBottomY / m_screenHeightScale * (1 - reToImSliderRatio));
			if (percentage > 0.94) percentage = 0.94f;
			if (percentage < 0.06) percentage = 0.f;
			jassert((percentage >= 0.f) && (percentage <= 1.f));
			adjustFreqDomainImag(arrayidx, percentage);
		}
	}
}

void VASTFreqDomainViewport::mouseDrag(const MouseEvent& e) { // show value
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	if (modifiers.isLeftButtonDown()) { //draw bins
		int starty = e.getMouseDownY();
		int start = e.getMouseDownX();
		int end = start + e.getDistanceFromDragStartX();
		int endy = starty + e.getDistanceFromDragStartY();
		if (end < 0) end = 0;
		if (end > getWidth() - 1)
			end = getWidth() - 1;
		if (endy < 0) endy = 0;
		if (endy > getHeight() - 1)
			endy = getHeight() - 1;

		int arrayidx = mouseXGetBinArrayIdx(end);
		if (starty < m_reSliderBottomY / m_screenHeightScale) { //real slider
			float percentage = (m_reSliderBottomY / m_screenHeightScale - endy) / (m_reSliderBottomY / m_screenHeightScale);
			if (percentage < 0.08) percentage = 0.f;
			jassert((percentage >= 0.f) && (percentage <= 1.f));
			adjustFreqDomainReal(arrayidx, percentage);
		}
		else if (starty > m_imSliderTopY / m_screenHeightScale) { //imag slider
			float percentage = (m_imSliderBottomY / m_screenHeightScale - endy) / (m_imSliderBottomY / m_screenHeightScale * (1 - reToImSliderRatio));
			if (percentage > 0.94) percentage = 0.94f;
			if (percentage < 0.06) percentage = 0.f;
			jassert((percentage >= 0.f) && (percentage <= 1.f));
			adjustFreqDomainImag(arrayidx, percentage);
		}
	}
}

void VASTFreqDomainViewport::mouseDoubleClick(const MouseEvent& e) {
	int x = e.getMouseDownX();
	int y = e.getMouseDownY();
	int arrayidx = mouseXGetBinArrayIdx(x);
	if (y < m_reSliderBottomY / m_screenHeightScale) { //real slider
		float percentage = 0.f;
		adjustFreqDomainReal(arrayidx, percentage);
	}
	else if (y > m_imSliderTopY / m_screenHeightScale) { //imag slider
		float percentage = 0.f;
		adjustFreqDomainImag(arrayidx, percentage);
	}
}

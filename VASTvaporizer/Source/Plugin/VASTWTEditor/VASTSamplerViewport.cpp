/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTSamplerViewport.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTAudioProcessor.h"
#include "../VASTWaveTableEditorComponent.h"
#include "VASTSamplerViewport.h"
#include "../../Engine/VASTSingleNote.h"
#include "../VASTControls/VASTManualScaleEditor.h"
#include <thread>

VASTSamplerViewport::VASTSamplerViewport()
{
	myProcessor = nullptr;
	myEditor = nullptr;
	myWtEditor = nullptr;

	setOpaque(true);
	resized();                  // initialise image
	setComponentID("samplerViewport");
	
	setPaintingIsUnclipped(true);

	//setSize(1500, 38);

	m_scaling = 1;	
	m_Offset = 1;
	
	waveformImage = Image(Image::RGB, 400, 500, true);
	waveformImageBufferOff = Image(Image::RGB, 400, 500, true);

	//myFont.setTypefaceName("Open Sans"); //bold 1-2, regular 2
	//myFont.setDefaultMinimumHorizontalScaleFactor(1.0);
	//myFont.setSizeAndStyle(14.f, Font::plain, 1.0f, 0.0f); // no squashing, no kerning
}

VASTSamplerViewport::~VASTSamplerViewport() {
	this->setLookAndFeel(nullptr);
}

void VASTSamplerViewport::lookAndFeelChanged() {
	waveformImageBufferOff = Image(Image::RGB, 400, 500, true);
	waveformImageBufferOff.clear(waveformImageBufferOff.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeOff));
	updateContentAsync();
}

void VASTSamplerViewport::setZoomFactor(int zoom) {
	m_scaling = zoom;

	int width = myWtEditor->getSamplerViewport()->getWidth() * m_scaling;
	setSize(width, myWtEditor->getSamplerViewport()->getHeight());
}

void VASTSamplerViewport::setEditor(VASTAudioProcessorEditor* editor) { myEditor = editor; }

void VASTSamplerViewport::setProcessor(VASTAudioProcessor* processor) { myProcessor = processor; }

void VASTSamplerViewport::setWTEditor(VASTWaveTableEditorComponent* wtEditor) { myWtEditor = wtEditor; }

void VASTSamplerViewport::resized()
{
	//waveformImage = waveformImage.rescaled(jmax(1, getWidth()), jmax(1, getHeight()), juce::Graphics::ResamplingQuality::highResamplingQuality);
	
	m_needsRescale = true;
	updateContentAsync();
	/*
	//async update
	std::thread update_thread(&VASTSamplerViewport::threadedUpdate, this);
	update_thread.detach();
	*/
}

void VASTSamplerViewport::threadedUpdate(VASTSamplerViewport* viewport) {
	MessageManagerLock mmlock;
	if (viewport != nullptr)
		viewport->updateContent(false);
}

void VASTSamplerViewport::startAutoUpdate() {
	startTimer(40);     // orig value: repaint every 150 microseconds
}

void VASTSamplerViewport::stopAutoUpdate() {
	stopTimer();
}

void VASTSamplerViewport::updateContentAsync() {
	m_needsUpdate = true;
}

void VASTSamplerViewport::timerCallback() {
	if (!myProcessor->m_initCompleted)
		return;
	if (m_needsUpdate)
		updateContent(false);
	else {
		VASTSamplerSound* samplerSound = myProcessor->m_pVASTXperience.m_Poly.getSamplerSound(); //not changed
		if (samplerSound != nullptr)
			if (samplerSound->getPositionChanged()) {
				m_needsPositionUpdate = true;
				repaint();
			}
	}
}

void VASTSamplerViewport::paint(Graphics& g)
{
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();

	juce::Rectangle<int> lVisibleArea = myWtEditor->getSamplerViewport()->getViewArea();
	if (m_needsRescale) {
		float scale = Desktop::getInstance().getGlobalScaleFactor();
		waveformImage = waveformImage.rescaled(jmax(1, int(lVisibleArea.getWidth() * scale)), jmax(1, int(lVisibleArea.getHeight() * scale)), juce::Graphics::ResamplingQuality::highResamplingQuality);
		waveformImageWithBorder = waveformImage.createCopy();
		handleBorderDisplay();
	}
	else 
	if (m_needsPositionUpdate) {
		m_needsPositionUpdate = false;
		handleBorderDisplay();
		//g.drawImageAt(waveformImageWithBorder.getClippedImage(juce::Rectangle<int>(m_lastPostMarker, 0, m_lastPostMarker, lVisibleArea.getHeight())), lVisibleArea.getBottomLeft().getX() + m_lastPostMarker, 0);
		//return;
	}

	g.drawImageWithin(waveformImageWithBorder, lVisibleArea.getBottomLeft().getX(), 0, lVisibleArea.getWidth(), lVisibleArea.getHeight(), juce::RectanglePlacement::stretchToFit);	
	//g.drawImageAt(waveformImageWithBorder, lVisibleArea.getBottomLeft().getX(), 0);
}

void VASTSamplerViewport::visibleAreaChanged(const juce::Rectangle<int>& ) {
	if (this->isVisible()) { //skip on close
		updateContentAsync();
	}
}

void VASTSamplerViewport::updateContent(bool force) {
	if (!force)
		if (this->isVisible() == false) //was isShowing()!
			return;
	if ((getWidth() <= 1) || (getHeight() <= 1))
		return;
	if (myProcessor == nullptr) {
		return;
	}
	
	if (myWtEditor->getSamplerViewport()->getViewArea().getWidth() == 0)
		return;

	float scale = Desktop::getInstance().getGlobalScaleFactor();
	m_screenWidthScale = scale * float(myWtEditor->getSamplerViewport()->getScreenBounds().getWidth()) / float(myWtEditor->getSamplerViewport()->getWidth());
	m_screenHeightScale = scale * float(myWtEditor->getSamplerViewport()->getScreenBounds().getHeight()) / float(myWtEditor->getSamplerViewport()->getHeight());

	juce::Rectangle<int> lVisibleArea = myWtEditor->getSamplerViewport()->getViewArea();
	if (myWtEditor == nullptr) {
		waveformImage = waveformImageBufferOff.rescaled(jmax(1, int(lVisibleArea.getWidth() * scale)), jmax(1, int(lVisibleArea.getHeight() * scale)));;
		waveformImage.duplicateIfShared();
		waveformImageWithBorder = waveformImageBufferOff.rescaled(jmax(1, int(lVisibleArea.getWidth() * scale)), jmax(1, int(lVisibleArea.getHeight() * scale)));
		return;
	}

	if (waveformImageBufferOff.isNull()) {
		waveformImageBufferOff = Image(Image::RGB, 400, 500, true);
		waveformImageBufferOff.clear(waveformImageBufferOff.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeOff));
	}

	//VDBG("VASTSamplerViewport::updateContent");

	m_needsUpdate = false;

	VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
	if (samplerSound == nullptr) {
		waveformImage = waveformImageBufferOff.rescaled(jmax(1, int(lVisibleArea.getWidth() * scale)), jmax(1, int(lVisibleArea.getHeight() * scale)));
		waveformImage.duplicateIfShared();
		waveformImageWithBorder = waveformImageBufferOff.rescaled(jmax(1, int(lVisibleArea.getWidth() * scale)), jmax(1, int(lVisibleArea.getHeight() * scale)));
		handleBorderDisplay();
		repaint();
		return;
	}
	if (samplerSound->getAudioDataChanged() == nullptr) {
		waveformImage = waveformImageBufferOff.rescaled(jmax(1, int(lVisibleArea.getWidth() * scale)), jmax(1, int(lVisibleArea.getHeight() * scale)));
		waveformImage.duplicateIfShared();
		waveformImageWithBorder = waveformImageBufferOff.rescaled(jmax(1, int(lVisibleArea.getWidth() * scale)), jmax(1, int(lVisibleArea.getHeight() * scale)));
		handleBorderDisplay();
		repaint();
		return;
	}
	setSelectionFromSound();

	AffineTransform af;
	lVisibleArea = lVisibleArea.transformedBy(af.scaled(m_screenWidthScale, m_screenHeightScale));

	double iViewPortwidth = getScreenBounds().getWidth() * scale;
	double iImageBottom = lVisibleArea.getHeight();
	double iImageHalf = lVisibleArea.getHeight() / 2.0f;

	waveformImage = Image(Image::RGB, jmax(1, int(lVisibleArea.getWidth() )) , jmax(1, int(lVisibleArea.getHeight() )), false);
	waveformImage.clear(waveformImage.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeOff));

	waveformImage = Image(Image::RGB, lVisibleArea.getWidth() , lVisibleArea.getHeight() , true);
	waveformImage.clear(waveformImage.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));
	waveformImageWithBorder = Image(Image::RGB, lVisibleArea.getWidth() , lVisibleArea.getHeight() , true);
	waveformImageWithBorder.clear(waveformImage.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));

	Graphics g(waveformImage);

	AudioSampleBuffer sampleBuffer = *samplerSound->getAudioDataChanged(); //copy it to avoid thread issues
	int numSamples = samplerSound->getLengthChanged();
	double step = double(numSamples) / double(iViewPortwidth);
	Path myWave;
	Path myArea;
	Path myArea2;
	myArea.startNewSubPath(0, iImageHalf);
	myArea2.startNewSubPath(0, iImageHalf);

	int visiStart = lVisibleArea.getBottomLeft().getX();
	int visiEnd = (iViewPortwidth < lVisibleArea.getTopRight().getX()) ? iViewPortwidth : lVisibleArea.getTopRight().getX();

	bool lSnap = myWtEditor->getSamplerEditor()->getHeader()->getSnapBox()->getToggleState();
	for (int i = visiStart; i < visiEnd; i++) {		
		double sample = sampleBuffer.getSample(0, int(i * step)); //left channel only?
		if (lSnap) {
			if (i < visiEnd - 1) {
				if (1 + int((i + 1) * step) < numSamples) {
					double sample1 = sampleBuffer.getSample(0, 1 + int(i * step)); //left channel only?
					double sample2 = sampleBuffer.getSample(0, 1 + int((i + 1) * step)); //left channel only?
					if (((sample1 >= 0) && (sample2 < 0)) ||
						((sample1 <= 0) && (sample2 > 0)) ||
						((sample1 == 0) && (sample2 == 0))) {
						g.setFillType(FillType().colour);
						g.setColour(Colours::darkgrey.withAlpha(0.5f));
						g.drawLine(i - visiStart, 0, i - visiStart, iImageBottom, 1.0 * myProcessor->getPluginScaleHeightFactor() * m_screenHeightScale);
					}
				}
			}
		}
		
		float area2Sample = ((sample * 0.5f) + 1.0f) / 2.f;
		sample = (sample + 1.0) / 2.0; //normalize to 0..1

		//g.setGradientFill(ColourGradient(g2, juce::Point<float>(i, iImageHalf), g1, juce::Point<float>(i, iImageBottom - (sample * iImageBottom)), true));
		//g.drawLine(i - visiStart, iImageHalf, i - visiStart, iImageBottom - (sample * iImageBottom), 1.0 * myProcessor->getPluginScaleHeightFactor());
		//if (i == visiStart) myWave.startNewSubPath(i - visiStart, iImageBottom - (sample * iImageBottom));
		myWave.lineTo(i - visiStart, iImageBottom - (sample * iImageBottom));
		myArea.lineTo(i - visiStart, iImageBottom - (sample * iImageBottom));
		myArea2.lineTo(i - visiStart, iImageBottom - (area2Sample * iImageBottom));
	}
	
	Colour c2 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colSamplerArea).withAlpha(1.0f); // blue 
	Colour c1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colSamplerArea).withAlpha(0.4f); // blue 
	Colour c0 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colSamplerLine).darker(0.1f); // blue 

	g.setColour(c0);
	g.strokePath(myWave, PathStrokeType(1.9f * myProcessor->getPluginScaleHeightFactor() * m_screenHeightScale));

	myArea.lineTo(visiEnd - visiStart, iImageHalf);
	myArea.closeSubPath(); 
	myArea2.lineTo(visiEnd - visiStart, iImageHalf);
	myArea2.closeSubPath();

	/*ColourGradient cg = ColourGradient(c2, juce::Point<float>(0, lVisibleArea.getHeight()), c1, juce::Point<float>(0, 0), false);
	cg.clearColours();
	cg.addColour(0.0, c0);
	cg.addColour(0.3, c1);
	cg.addColour(0.5, c2);
	cg.addColour(0.8, c1);
	cg.addColour(1.0, c0);
	g.setGradientFill(cg);
	*/
	g.setColour(c1);
	g.fillPath(myArea);
	g.setColour(c2);
	g.fillPath(myArea2);

	handleBorderDisplay();
	m_needsRescale = false;

	repaint();	
}

void VASTSamplerViewport::selectNothing() {
	VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
	if (samplerSound == nullptr) return;
	m_selection.iWavSelectionStartSample = -1;
	m_selection.iWavSelectionEndSample = -1;
	samplerSound->clearLoopChanged();
	updateContent(false);
	notifySelectionChanged();
}

void VASTSamplerViewport::selectAll() {
	VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
	if (samplerSound == nullptr) return;
	double  length = samplerSound->getLengthChanged();
	//double  step = length / getWidth();
	m_selection.iWavSelectionStartSample = 0;
	m_selection.iWavSelectionEndSample = length - 1;
	samplerSound->setLoopChanged(m_selection.iWavSelectionStartSample, m_selection.iWavSelectionEndSample);
	updateContent(false);
	notifySelectionChanged();
}

void VASTSamplerViewport::setSelectionFromSound() {
	VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
	if (samplerSound == nullptr) return;

	if (!samplerSound->hasLoopChanged()) {
		m_selection.iWavSelectionStartSample = -1;
		m_selection.iWavSelectionEndSample = -1;
	}
	else {
		m_selection.iWavSelectionStartSample = samplerSound->getLoopStartChanged();
		m_selection.iWavSelectionEndSample = samplerSound->getLoopEndChanged();
	}
}
sSelectionWav* VASTSamplerViewport::getSelection() {
	return &m_selection;
}

bool VASTSamplerViewport::isInterestedInFileDrag(const StringArray& files) {
	for (int i = 0; i < files.size(); i++) {
		if (files[i].endsWithIgnoreCase(".wav")) return true;
		if (files[i].endsWithIgnoreCase(".aif")) return true;
		if (files[i].endsWithIgnoreCase(".flac")) return true;
		if (files[i].endsWithIgnoreCase(".mp3")) return true;
	}
	return false;
}

void VASTSamplerViewport::setSelectionFromWavSamples(int startSample, int endSample) {
	VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
	if (samplerSound == nullptr) return;

	m_selection.iWavSelectionStartSample = startSample;
	m_selection.iWavSelectionEndSample = endSample; //can be the same as startSample
	notifySelectionChanged();
}

void VASTSamplerViewport::mouseDrag(const MouseEvent &e) { // show value
	if (myWtEditor == nullptr) return;
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
	if (samplerSound == nullptr) return;
	double length = samplerSound->getLengthChanged();
	double step = length / getWidth();
	bool lSnap = myWtEditor->getSamplerEditor()->getHeader()->getSnapBox()->getToggleState();

	if (modifiers.isShiftDown()) { //adjust selection
		double pos = e.getMouseDownX() + e.getDistanceFromDragStartX();
		double start = m_selection.iWavSelectionStartSample / step;
		double end = (m_selection.iWavSelectionEndSample + 1) / step;

		if (lSnap) {
			juce::Array<int> lZeroCrossings = *samplerSound->getZeroCrossings();
			int i = 0;
			for (i = 0; i < lZeroCrossings.size(); i++)
				if (lZeroCrossings[i] > pos * step)
					break;
			if (i < (lZeroCrossings.size() - 1)) { //else not found
				if (i == 0)
					pos = lZeroCrossings[0] / step;
				else {
					if ((abs(pos - lZeroCrossings[i] / step)) >
						abs(pos - lZeroCrossings[i - 1] / step))
						pos = lZeroCrossings[i - 1] / step;
					else
						pos = lZeroCrossings[i] / step;
				}
			}
			else
				pos = lZeroCrossings[lZeroCrossings.size() - 1] / step;
		}

		double midpoint = (start + end) * 0.5;
		if (pos > midpoint)
			end = pos;
		else
			start = pos;
		if (start< 0) start = 0;
		if (end > getScreenBounds().getWidth() - 1.0) end = getScreenBounds().getWidth() - 1.0;
		m_selection.iWavSelectionStartSample = round(start * step);
		m_selection.iWavSelectionEndSample = round(end * step) - 1;
	}
	else { //normal selection
		double start = e.getMouseDownX();
		double end = start + e.getDistanceFromDragStartX();

		if (lSnap) {
			juce::Array<int> lZeroCrossings = *samplerSound->getZeroCrossings();
			int i = 0;
			for (i = 0; i < lZeroCrossings.size(); i++)
				if (lZeroCrossings[i] > start * step)
					break;
			if (i < (lZeroCrossings.size() - 1)) { //else not found
				if (i==0) 
					start = lZeroCrossings[0] / step;
				else {
					if ((abs(start - lZeroCrossings[i] / step)) >
						abs(start - lZeroCrossings[i-1] / step))
						start = lZeroCrossings[i - 1] / step;
					else 
						start = lZeroCrossings[i] / step;
				}
			}
			else
				start = lZeroCrossings[lZeroCrossings.size() - 1] / step;
			
			for (i = 0; i < lZeroCrossings.size(); i++)
				if (lZeroCrossings[i] > end * step)
					break;
			if (i < (lZeroCrossings.size() - 1)) { //else not found
				if (i == 0)
					end = lZeroCrossings[0] / step;
				else {
					if ((abs(end - lZeroCrossings[i] / step)) >
						abs(end - lZeroCrossings[i - 1] / step))
						end = lZeroCrossings[i - 1] / step;
					else
						end = lZeroCrossings[i] / step;
				}
			}
			else
				end = lZeroCrossings[lZeroCrossings.size() - 1] / step;
		}

		if (end < 0) end = 0;
		if (end > getScreenBounds().getWidth() - 1)
			end = getScreenBounds().getWidth() - 1;
		if (start < end) {
			m_selection.iWavSelectionStartSample = round(start * step);
			m_selection.iWavSelectionEndSample = round(end * step) - 1;			
		}
		else {
			m_selection.iWavSelectionStartSample = round(end * step);
			m_selection.iWavSelectionEndSample = round(start * step) - 1;
		}
	}
	if (m_selection.iWavSelectionStartSample > m_selection.iWavSelectionEndSample)
		m_selection.iWavSelectionEndSample = m_selection.iWavSelectionStartSample;

	myProcessor->m_pVASTXperience.m_Poly.getSamplerSoundChanged()->setLoopChanged(m_selection.iWavSelectionStartSample, m_selection.iWavSelectionEndSample);

	notifySelectionChanged();
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();
	handleBorderDisplay();
	repaint(); //paints waveformimage
}

void VASTSamplerViewport::notifySelectionChanged() {
	VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
	if (samplerSound != nullptr) {
		m_selection.iZeroCrossings = 0;

		int numSamples = m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample;
		if (samplerSound->getAudioDataChanged()->getNumSamples() < m_selection.iWavSelectionEndSample) //safety check
			return;

		juce::Array<int> lZeroCrossings = *samplerSound->getZeroCrossings();
		int counter = -1;
		int zerocrossings = 0;
		int lz = 0;
		int numlz = lZeroCrossings.size();
		while ((counter < numSamples - 1) && (lz < numlz)) {
			counter++;
			int sPos = m_selection.iWavSelectionStartSample + counter;
			while ((lz < numlz) && (lZeroCrossings[lz] < sPos)) {
				lz++;
			}
			if (lZeroCrossings[lz] == sPos) {
				zerocrossings++;
			}
			if (lz == numlz - 1)
				break;
		}
			   		 
		m_selection.iZeroCrossings = zerocrossings;

		sampleDetermineFreq();

		if (myWtEditor != NULL) {
			int numSelectionSamples = m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample;
			int proposedWTposCountForSample = 1.f / (samplerSound->getSourceSampleRate() / m_selection.dDeterminedFreq / numSelectionSamples);
			if (proposedWTposCountForSample < 1) proposedWTposCountForSample = 1;
			if (proposedWTposCountForSample > 256) proposedWTposCountForSample = 256;
			myProcessor->setParameterText("m_iWavWTPos", String(proposedWTposCountForSample), false);
			myWtEditor->notifySelectionChanged();
		}
	}
}

void VASTSamplerViewport::mouseDoubleClick(const MouseEvent &) {
	VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
	if (samplerSound == nullptr) return;

	if (samplerSound->hasLoopChanged())
		selectNothing();
	else 
		selectAll();
	notifySelectionChanged();
	updateContent(false);
}

void VASTSamplerViewport::mouseMove(const MouseEvent& ) {
	m_bMouseover = true;
	handleBorderDisplay();
	repaint();
}

void VASTSamplerViewport::handleBorderDisplay() {
	//selection
	if (waveformImage.isNull())
		return;
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();
	Graphics g(waveformImageWithBorder);
	g.drawImageAt(waveformImage, 0, 0);

	juce::Rectangle<int> lVisibleArea = myWtEditor->getSamplerViewport()->getViewArea();
	AffineTransform af;
	lVisibleArea = lVisibleArea.transformedBy(af.scaled(m_screenWidthScale, m_screenHeightScale));

	float lDrawheight = lVisibleArea.getHeight();
	if (m_bMouseover) {
		g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeSelection).darker(0.6f));
	}
	else {
		g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeSelection).darker(1.0f));
	}	
	float scale = Desktop::getInstance().getGlobalScaleFactor();
	g.drawRect(0.f, 0.f, float(waveformImage.getWidth() - 1), float(waveformImage.getHeight() - 1), 1.f * scale);

	int visiStart = lVisibleArea.getBottomLeft().getX();
	double iViewPortwidth = getScreenBounds().getWidth() * scale;
	int visiEnd = (iViewPortwidth < lVisibleArea.getTopRight().getX()) ? iViewPortwidth : lVisibleArea.getTopRight().getX();

	VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
	if (samplerSound != nullptr) {
		int numSamples = samplerSound->getLengthChanged();
		double step = double(numSamples) / double(iViewPortwidth);

		//current pos marker
		g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorPosMarker));

		for (int i = 0; i < sizeof(myProcessor->m_pVASTXperience.m_Poly.m_samplerViewportPosMarker) / sizeof(double); i++) {
			double posx = myProcessor->m_pVASTXperience.m_Poly.m_samplerViewportPosMarker [i] / step;
			if ((posx - visiStart > 0) && (posx - visiStart < lVisibleArea.getWidth()))
				g.drawLine(posx - visiStart, 0.f, posx - visiStart, float(lDrawheight - 1.0 * scale), 1.f * scale);
			m_lastPostMarker = posx - visiStart;
		}
			
		//selection
		if (m_selection.iWavSelectionStartSample >= 0) {
			g.setColour(juce::Colour::fromFloatRGBA(1.f, 1.0f, 1.0f, 1.0f));
			double start = m_selection.iWavSelectionStartSample / step;
			double end = (m_selection.iWavSelectionEndSample + 1) / step;

			float startPoint = (start - visiStart > 0) ? start - visiStart : 0.f;
			float endPoint = (end - visiStart < lVisibleArea.getWidth()) ? end - visiStart : lVisibleArea.getWidth();
			g.drawLine(startPoint, 0.f, endPoint, 0.f, 1.f * scale);
			g.drawLine(startPoint, float(lDrawheight - 1.0 * scale), endPoint, float(lDrawheight - 1.0 * scale), 1.f  * scale);
			g.drawLine(startPoint, 0.f, startPoint, float(lDrawheight - 1.0 * scale), 1.f * scale);
			g.drawLine(endPoint, 0.f, endPoint, float(lDrawheight - 1.0  * scale), 1.f * scale);
		}
	}

	float fontSize = (lVisibleArea.getWidth() / 1000.f) * 16.f;
	Font myFont = myProcessor->getCurrentVASTLookAndFeel()->getDefaultFont();
	myFont.setSizeAndStyle(fontSize, Font::plain, 1.0f, 0.0f); // no squashing, no kerning
	g.setFont(myFont);
	g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colSamplerText));
	int visiwidth = visiEnd - visiStart;
	int num = 0;
	if (m_selection.iWavSelectionEndSample >= 0)
		num = m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample + 1;
	if (samplerSound != nullptr) {
		String sSamples;
		if ((m_selection.iWavSelectionStartSample < 0) || (m_selection.iWavSelectionEndSample < 0))
			sSamples = String(num);
		else 
			sSamples = String(num) + " (" + String(m_selection.iWavSelectionStartSample) + "-" + String(m_selection.iWavSelectionEndSample) + ")";
		g.drawText(TRANS("Samples ") + sSamples, juce::Rectangle<int>(0.f, lDrawheight - fontSize - 2.f, visiwidth * 0.33f, fontSize + 2.f), Justification::centred, false);
		g.drawText(TRANS("Zero crossings ") + String(m_selection.iZeroCrossings), juce::Rectangle<int>(visiwidth * 0.33f, lDrawheight - fontSize - 2.f, visiwidth * 0.33f, fontSize + 2.f), Justification::centred, false);
		g.drawText(TRANS("Estimated freq ") + String(m_selection.dDeterminedFreq) + " Hz", juce::Rectangle<int>(visiwidth * 0.66f, lDrawheight - fontSize - 2.f, visiwidth * 0.33f, fontSize + 2.f), Justification::centred, false);
	}
}

void VASTSamplerViewport::mouseExit(const MouseEvent& event) {
	m_bMouseover = false;
	handleBorderDisplay();
	repaint();
}

void VASTSamplerViewport::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) {
	if (wheel.deltaY != 0.0f) {
		int newPoint = event.getPosition().getX();
		juce::Rectangle<int> lVisibleArea1 = myWtEditor->getSamplerViewport()->getViewArea();
		float perc = float(newPoint - lVisibleArea1.getX()) / float(lVisibleArea1.getWidth());
		float width1 = getWidth();
		myWtEditor->getSamplerEditor()->getHeader()->getWAVScale()->setValue(myWtEditor->getSamplerEditor()->getHeader()->getWAVScale()->getValue() + (wheel.deltaY * 5), NotificationType::sendNotificationSync);
		if (wheel.deltaY > 0.f) { //zoom in
			juce::Rectangle<int> lVisibleArea2 = myWtEditor->getSamplerViewport()->getViewArea();
			float width2 = getWidth();			
			float middle = newPoint * (width2 / width1) - lVisibleArea2.getWidth() * perc;
			myWtEditor->getSamplerViewport()->setViewPosition(Point<int>(middle, 0));
		}
		else {
			juce::Rectangle<int> lVisibleArea2 = myWtEditor->getSamplerViewport()->getViewArea();
			float width2 = getWidth();
			float middle = newPoint * (width2 / width1) - lVisibleArea2.getWidth() * perc;
			myWtEditor->getSamplerViewport()->setViewPosition(Point<int>(middle, 0));
		}
		//updateContent(false); //not async
		updateContentAsync(); 
	}	
}

void VASTSamplerViewport::mouseDown(const MouseEvent &) {
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	//int x = e.getMouseDownX();
	//int y = e.getMouseDownY();

	//int arrayidx = int(x / (m_ImageTotalWidth + m_Offset));

	if (myWtEditor == nullptr) return;
	std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
	
	if (modifiers.isPopupMenu()) { //context menu
		PopupMenu mainMenu;
		mainMenu.setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());

		mainMenu.addSectionHeader(TRANS("Select"));
		mainMenu.addItem(1, TRANS("Select all (Ctrl + a)"), true);
		mainMenu.addItem(19, TRANS("Find zero loop"), true);
		mainMenu.addSeparator();
		PopupMenu subMenuEdit;
		mainMenu.addSectionHeader(TRANS("Edit"));
		subMenuEdit.addItem(8, TRANS("Clear all"), true);
		subMenuEdit.addItem(9, TRANS("Cut out selection"), true);
		subMenuEdit.addItem(10, TRANS("Trim to selection"), true);
		subMenuEdit.addItem(11, TRANS("Reverse selection"), true);
		mainMenu.addSubMenu(TRANS("Edit types"), subMenuEdit, true);

		mainMenu.addSeparator();
		PopupMenu subMenuResample;
		mainMenu.addSectionHeader(TRANS("Resample"));
		subMenuResample.addItem(15, TRANS("Resample selection to wavetable position(s)"), true);
		subMenuResample.addItem(16, TRANS("Resample selection to selected wavetable position(s)"), true);
		subMenuResample.addItem(17, TRANS("Resample selection to exact number of wavetable position"), true);
		subMenuResample.addItem(18, TRANS("Resample selection with fixed number of samples"), true);
		mainMenu.addSubMenu(TRANS("Resample types"), subMenuResample, true);

		mainMenu.addSeparator();
		PopupMenu subMenuResamplePatch;
		mainMenu.addSectionHeader(TRANS("Create patch"));
		subMenuResamplePatch.addItem(20, TRANS("Create mono (left channel) wavetable patch from sample selection"), true);
		subMenuResamplePatch.addItem(21, TRANS("Create stereo wavetable patch from sample selection"), true);
		subMenuResamplePatch.addItem(22, TRANS("Create mono (left channel) wavetable patch from sample selection (with freerunning LFO)"), true);
		subMenuResamplePatch.addItem(23, TRANS("Create stereo wavetable patch from sample selection (with freerunning LFO)"), true);
		mainMenu.addSubMenu(TRANS("Create patch types"), subMenuResamplePatch, true);

		mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
			juce::ModalCallbackFunction::create([this](int result) {

			if (result == 0) {
				// user dismissed the menu without picking anything
			}
			if (result == 1) {
				//select all
				selectAll();
				updateContent(false);
				notifySelectionChanged();
			}
			else if (result == 8) {
				//clear all
				myProcessor->m_pVASTXperience.m_Poly.clearSamplerSoundChanged();
				myWtEditor->updateAll(false);
			}
			else if (result == 9) {
				//cut out selection
				VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
				if (samplerSound == nullptr) return;
				if ((m_selection.iWavSelectionStartSample < 0) || (m_selection.iWavSelectionEndSample < 0)) selectAll();
				AudioSampleBuffer newbuffer;
				int numchannels = samplerSound->getAudioDataChanged()->getNumChannels();
				int numsamples = samplerSound->getAudioDataChanged()->getNumSamples();
				newbuffer.setSize(numchannels, numsamples - (m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample + 1), false, true, false);
				newbuffer.copyFrom(0, 0, samplerSound->getAudioDataChanged()->getReadPointer(0, 0), m_selection.iWavSelectionStartSample);
				if (numchannels > 1)
					newbuffer.copyFrom(1, 0, samplerSound->getAudioDataChanged()->getReadPointer(1, 0), m_selection.iWavSelectionStartSample);
				newbuffer.copyFrom(0, m_selection.iWavSelectionStartSample, samplerSound->getAudioDataChanged()->getReadPointer(0, m_selection.iWavSelectionEndSample + 1), numsamples - (m_selection.iWavSelectionEndSample + 1));
				if (numchannels > 1)
					newbuffer.copyFrom(1, m_selection.iWavSelectionStartSample, samplerSound->getAudioDataChanged()->getReadPointer(1, m_selection.iWavSelectionEndSample + 1), numsamples - (m_selection.iWavSelectionEndSample + 1));
				samplerSound->getAudioDataChanged()->setSize(numchannels, newbuffer.getNumSamples(), true, true, false);
				samplerSound->getAudioDataChanged()->copyFrom(0, 0, newbuffer.getReadPointer(0, 0), newbuffer.getNumSamples());
				if (numchannels > 1)
					samplerSound->getAudioDataChanged()->copyFrom(1, 0, newbuffer.getReadPointer(1, 0), newbuffer.getNumSamples());
				samplerSound->setAudioDataChangedFlag();
				selectAll();
				updateContent(false);
				notifySelectionChanged();
				myWtEditor->updateAll(false);
			}
			else if (result == 10) {
				//trim to selection
				VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
				if (samplerSound == nullptr) return;
				if ((m_selection.iWavSelectionStartSample < 0) || (m_selection.iWavSelectionEndSample < 0)) selectAll();
				//float length = samplerSound->getLength();
				AudioSampleBuffer newbuffer;
				int numchannels = samplerSound->getAudioDataChanged()->getNumChannels();
				newbuffer.setSize(numchannels, m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample + 1, false, true, false);
				newbuffer.copyFrom(0, 0, samplerSound->getAudioDataChanged()->getReadPointer(0, m_selection.iWavSelectionStartSample), m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample + 1);
				if (numchannels > 1)
					newbuffer.copyFrom(1, 0, samplerSound->getAudioDataChanged()->getReadPointer(1, m_selection.iWavSelectionStartSample), m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample + 1);
				samplerSound->getAudioDataChanged()->setSize(numchannels, newbuffer.getNumSamples(), true, true, false);
				samplerSound->getAudioDataChanged()->copyFrom(0, 0, newbuffer.getReadPointer(0, 0), newbuffer.getNumSamples());
				if (numchannels > 1)
					samplerSound->getAudioDataChanged()->copyFrom(1, 0, newbuffer.getReadPointer(1, 0), newbuffer.getNumSamples());
				samplerSound->setAudioDataChangedFlag();
				selectAll();
				updateContent(false);
				notifySelectionChanged();
				myWtEditor->updateAll(false);
			}
			else if (result == 11) {
				//reverse selection
				VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
				if (samplerSound == nullptr) return;
				if ((m_selection.iWavSelectionStartSample < 0) || (m_selection.iWavSelectionEndSample < 0)) selectAll();
				//float length = samplerSound->getLength();
				AudioSampleBuffer newbuffer;
				int numchannels = samplerSound->getAudioDataChanged()->getNumChannels();
				newbuffer.setSize(numchannels, samplerSound->getAudioDataChanged()->getNumSamples(), false, true, false);
				newbuffer.copyFrom(0, 0, samplerSound->getAudioDataChanged()->getReadPointer(0, 0), samplerSound->getAudioDataChanged()->getNumSamples());
				if (numchannels > 1)
					newbuffer.copyFrom(1, 0, samplerSound->getAudioDataChanged()->getReadPointer(1, 0), samplerSound->getAudioDataChanged()->getNumSamples());
				for (int i = m_selection.iWavSelectionStartSample; i <= m_selection.iWavSelectionEndSample; i++) {
					newbuffer.setSample(0, i, samplerSound->getAudioDataChanged()->getReadPointer(0, m_selection.iWavSelectionStartSample)[m_selection.iWavSelectionEndSample - i]);
					if (numchannels > 1)
						newbuffer.setSample(1, i, samplerSound->getAudioDataChanged()->getReadPointer(1, m_selection.iWavSelectionStartSample)[m_selection.iWavSelectionEndSample - i]);
				}
				samplerSound->getAudioDataChanged()->setSize(numchannels, newbuffer.getNumSamples(), true, true, false);
				samplerSound->getAudioDataChanged()->copyFrom(0, 0, newbuffer.getReadPointer(0, 0), newbuffer.getNumSamples());
				if (numchannels > 1)
					samplerSound->getAudioDataChanged()->copyFrom(1, 0, newbuffer.getReadPointer(1, 0), newbuffer.getNumSamples());
				samplerSound->setAudioDataChangedFlag();
				updateContent(false);
				myWtEditor->updateAll(false);
			}
			else if (result == 15) {
				//scale selection to wt
				myWtEditor->wavSelectionToWt(0, nullptr);
				myWtEditor->updateAll(false);
			}
			else if (result == 16) {
				//scale selection to selected position 
				VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
				if (samplerSound == nullptr) return;
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetableupdate = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable();
				if ((m_selection.iWavSelectionStartSample < 0) || (m_selection.iWavSelectionEndSample < 0)) selectAll();
				int lChannel = 0; //left only?
				auto* subBufferTarget = samplerSound->getAudioDataChanged()->getReadPointer(lChannel, m_selection.iWavSelectionStartSample);
				int selectionLength = m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample + 1;

				int wtPosStart = myWtEditor->getWtPos();
				int wtPosEnd = myWtEditor->getWtPos();
				if (wavetableupdate->isMultiSelected()) {
					wtPosStart = wavetableupdate->getMultiSelectBegin();
					wtPosEnd = wavetableupdate->getMultiSelectEnd();
				}
				myWtEditor->scaleAudioBufferToWTPos(wavetableupdate.get(), wtPosStart, wtPosEnd - wtPosStart + 1, subBufferTarget, selectionLength);
				myWtEditor->updateAll(false);

				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetableupdate);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
			}
			else if (result == 17) {
				//scale selection to exact number of selected position 
				int targetPosNo = myWtEditor->getSamplerEditor()->getHeader()->getWAVWTPosSlider()->getValue();
				VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
				if (samplerSound == nullptr) return;
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetableupdate = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable();
				wavetableupdate->clear(); //delet all positions
				if ((m_selection.iWavSelectionStartSample < 0) || (m_selection.iWavSelectionEndSample < 0)) selectAll();
				int lChannel = 0; //left only?
				auto* subBufferTarget = samplerSound->getAudioDataChanged()->getReadPointer(lChannel, m_selection.iWavSelectionStartSample);
				int selectionLength = m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample + 1;
				myWtEditor->scaleAudioBufferToWTPos(wavetableupdate.get(), 0, targetPosNo, subBufferTarget, selectionLength);
				wavetableupdate->setSelectedWtPos(0);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetableupdate);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				myWtEditor->updateAll(false);
			}
			else if (result == 18) {
				//scale selection with fixed number of samples to exact number of selected position
				int targetPosNo = myWtEditor->getSamplerEditor()->getHeader()->getWAVWTPosSlider()->getValue();
				int selectionLength = m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample + 1;
				std::unique_ptr<VASTManualScaleEditor> l_veditor = std::make_unique<VASTManualScaleEditor>(myProcessor, myWtEditor, this);
				l_veditor->setLookAndFeel(myProcessor->getCurrentVASTLookAndFeel());
				l_veditor->setSize(400.f * myProcessor->getPluginScaleWidthFactor(), 60.f * myProcessor->getPluginScaleHeightFactor());
				l_veditor->setOpaque(true);
				//Point<float> cPoint = myEditor->vaporizerComponent->getLocalPoint(_comp, Point<float>(0.f, 0.f));
				Point<float> cPoint = myWtEditor->getSamplerEditor()->getLocalPoint(myWtEditor->getSamplerEditor(), Point<float>(0.f, 0.f));
				juce::Rectangle<int> newBounds = juce::Rectangle<int>(cPoint.getX(), cPoint.getY(), waveformImage.getWidth(), waveformImage.getHeight());

				l_veditor->setSamples(String(int(selectionLength / targetPosNo)));
				l_veditor->setCycles(String(targetPosNo));
				juce::CallOutBox::launchAsynchronously(std::move(l_veditor), newBounds, myWtEditor->getSamplerEditor());
			}
			else if (result == 19) {
				//find zero loop
				myWtEditor->findZeroLoop();
				myWtEditor->updateAll(false);
			}
			else if (result == 20) {
				//Create mono(left channel) wavetable patch from sample selection			
				myWtEditor->wavSelectionToWavetablePatch(false, false);
				myWtEditor->updateAll(false);
			}
			else if (result == 21) {
				//Create stereo wavetable patch from sample selection
				myWtEditor->wavSelectionToWavetablePatch(true, false);
				myWtEditor->updateAll(false);
			}
			else if (result == 22) {
				//Create mono (left channel) wavetable patch from sample selection (with freerunning LFO)
				myWtEditor->wavSelectionToWavetablePatch(false, true);
				myWtEditor->updateAll(false);
			}
			else if (result == 23) {
				//Create stereo wavetable patch from sample selection (with freerunning LFO)
				myWtEditor->wavSelectionToWavetablePatch(true, true);
				myWtEditor->updateAll(false);
			}
		}));
	}
}

void VASTSamplerViewport::sampleDetermineFreq() {
	//based on FFT
	VASTSamplerSound* samplerSound = myWtEditor->getCurSamplerSound();
	if (samplerSound == nullptr) return;
	int copyStart = samplerSound->getLoopStartChanged();
	int copyEnd = samplerSound->getLoopEndChanged();

	m_selection.dDeterminedFreq = myWtEditor->sampleDetermineFreq(copyStart, copyEnd, 0); //only left channel here
	
	//estimatedZeroCrossingsPerCycle 
	if (m_selection.dDeterminedFreq > 0.f) {
        m_selection.estimatedZeroCrossingsPerCycle = 0;
        if (((m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample) > 0) && (m_selection.iZeroCrossings > 0)) {
                double zeroCycleFreq = 1.f / ((double(m_selection.iWavSelectionEndSample - m_selection.iWavSelectionStartSample) / double((m_selection.iZeroCrossings - 1.0))) / samplerSound->getSourceSampleRate());
                m_selection.estimatedZeroCrossingsPerCycle = round(zeroCycleFreq / m_selection.dDeterminedFreq);
        }
        if (m_selection.estimatedZeroCrossingsPerCycle == 0)
			m_selection.dDeterminedFreq = 0.f;
	}
}

void VASTSamplerViewport::filesDropped(const StringArray& files, int x, int ) {
	VASTSamplerSound* lVASTSamplerSound = myWtEditor->loadWavFile(files[0]); //only one
	if (lVASTSamplerSound != nullptr) {
		VASTSynthesiserSound* lSynthSound = (VASTSynthesiserSound*)(myProcessor->m_pVASTXperience.m_Poly.getSynthesizer()->getSound(0));
		lSynthSound->addSamplerSoundChanged(lVASTSamplerSound);
	}
	selectAll();
	myWtEditor->updateAll(false);
}

String VASTSamplerViewport::getTooltip()
{
	return TRANS("Click and drag to change selected area. Rightclick for menu.");
}
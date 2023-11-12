/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTModMatCurveDisplay.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTAudioProcessor.h"
#include "../VASTWaveTableEditorComponent.h"

VASTModMatCurveDisplay::VASTModMatCurveDisplay(int slot, VASTAudioProcessorEditor* editor, VASTAudioProcessor* processor) : myProcessor(processor), myEditor(editor)
{
	m_slot = slot;
	setComponentID("curveDisplay");

	setOpaque(true); //avoid repaints of parents
	setRepaintsOnMouseActivity(false); //performance

	startAutoUpdate();
	setPaintingIsUnclipped(true);
	resized(); // initialise image
}

VASTModMatCurveDisplay::~VASTModMatCurveDisplay() {
	this->setLookAndFeel(nullptr);
	//stopTimer();
}

void VASTModMatCurveDisplay::lookAndFeelChanged() {
	resized();
}

void VASTModMatCurveDisplay::resized()
{
	if (myEditor == nullptr) return;
	Colour cBack = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground);

	waveformImageBuffer = Image(Image::RGB,
		jmax(1, getScreenBounds().getWidth()), jmax(1, getScreenBounds().getHeight()),
		true);
	waveformImageBuffer.clear(waveformImage.getBounds(), cBack);
	Graphics g(waveformImageBuffer);

	g.setGradientFill(ColourGradient(cBack.brighter(0.06f), 0.0f, 0.0f,
		cBack.darker(1.00f), waveformImageBuffer.getWidth(), (float)waveformImageBuffer.getHeight(), false));
	g.fillRect(0, 0, waveformImageBuffer.getWidth(), waveformImageBuffer.getHeight());
	//g.setColour(juce::Colours::lightgrey.darker(0.3f));
	g.setColour(juce::Colours::black);
	g.drawRect(0.f, 0.f, float(waveformImageBuffer.getWidth()), float(waveformImageBuffer.getHeight()), 1.f);

	waveformImageBufferOff = waveformImageBuffer;
	waveformImageBufferOff.duplicateIfShared();
	Graphics g1(waveformImageBufferOff);
	g1.setGradientFill(ColourGradient(cBack.darker(0.6f), 0.0f, 0.0f,
		cBack.darker(1.00f), waveformImageBuffer.getWidth(), (float)waveformImageBuffer.getHeight(), false));
	g1.fillRect(0, 0, waveformImageBuffer.getWidth(), waveformImageBuffer.getHeight());

	waveformImage = waveformImageBuffer;
	waveformImage.duplicateIfShared();

	if (myProcessor != nullptr)
		updateContent(true);
}

void VASTModMatCurveDisplay::paint(Graphics& g)
{		
	g.drawImageWithin(waveformImage, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
}

void VASTModMatCurveDisplay::updateContent(bool force) {
	if (!force) {
		if (this->isShowing() == false) 
			return;		
	}
	if ((getWidth() <= 1) || (getHeight() <= 1)) 
		return;

	if (myProcessor == nullptr) {
		return;
	}
	if (myProcessor->initNotCompleted()) {
		return;
	}

	bool l_display = true;
	
	if (l_display == false) {
		//repaint();
		return;
	}
	
	m_screenWidthScale = float(getScreenBounds().getWidth()) / float(getWidth());
	m_screenHeightScale = float(getScreenBounds().getHeight()) / float(getHeight());

	if (myProcessor->m_pVASTXperience.m_Set.modMatrixSlotUsed[m_slot] == false) {
		if (!m_offWasPainted) {
			waveformImage = waveformImageBufferOff;
			waveformImage.duplicateIfShared();
			m_offWasPainted = true;
			repaint();
		}
		return;
	}
	m_offWasPainted = false;

	double curvy = 0.5;
	float modval = 0.f;
	float lastSrceVals[C_MAX_POLY] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
	int polarity = 0;
	myProcessor->m_pVASTXperience.m_Set.modMatrixSlotGetValues(m_slot, modval, curvy, polarity, lastSrceVals);
	curvy = (1.f - curvy * 0.01f) * 0.5f;
	modval *= 0.01f;
	
	bool bchanged = false;
	for (int voice = 0; voice < C_MAX_POLY; voice++) if (lastSrceVals[voice] != m_lastSrceVals[voice]) { bchanged = true; break; }
	if ((!force) && ((lastCurvy == curvy) && (lastModVal == modval) && (!bchanged))) //no change //TODOO!!!!!!
		return;
	std::copy(std::begin(lastSrceVals), std::end(lastSrceVals), std::begin(m_lastSrceVals));

	waveformImage = waveformImageBuffer;
	waveformImage.duplicateIfShared();
	Graphics g(waveformImage);

	AffineTransform t = AffineTransform::scale(m_screenWidthScale, m_screenHeightScale);
	g.addTransform(t);

	if (l_display) {
		//g.setColour(juce::Colour::fromFloatRGBA(0.13f, 0.13f, 0.15f, 1.0f));
		//g.fillRect(0, 0, waveformImage.getWidth(), waveformImage.getHeight());
	
		float startYPos = (0.5f + (0.5f * modval)) * (getHeight() - 4.f * myProcessor->getPluginScaleHeightFactor()) + 2.f * myProcessor->getPluginScaleHeightFactor(); //bottom left
		float endYPos = (0.5f - (0.5f * modval)) * (getHeight() - 4.f * myProcessor->getPluginScaleHeightFactor()) + 2.f * myProcessor->getPluginScaleHeightFactor();
		float startXPos = 0.f * (getWidth() - 4.f * myProcessor->getPluginScaleWidthFactor()) + 2.f * myProcessor->getPluginScaleWidthFactor();
		float endXPos = 1.f * (getWidth() - 4.f * myProcessor->getPluginScaleWidthFactor()) + 2.f * myProcessor->getPluginScaleWidthFactor();
		Path pline;
		Path pline2;

		//check
		if (modval >= 0)
			curvy = 1.f - curvy; //flip y axis

		if (curvy <= 0.000001) {
			if (modval >= 0.f) {
				pline.startNewSubPath(startXPos, startYPos);
				pline.lineTo(endXPos, startYPos);
				pline2.startNewSubPath(endXPos, startYPos);
				pline2.lineTo(endXPos, endYPos);
			}
			else {
				pline2.startNewSubPath(startXPos, startYPos);
				pline2.lineTo(startXPos, endYPos);
				pline.startNewSubPath(startXPos, endYPos);
				pline.lineTo(endXPos, endYPos);
			}
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeArea));
			g.strokePath(pline2, PathStrokeType(2.0f * myProcessor->getPluginScaleWidthFactor()));
		}
		else if (curvy >= 0.999999) {
			if (modval >= 0.f) {
				pline2.startNewSubPath(startXPos, startYPos);
				pline2.lineTo(startXPos, endYPos);
				pline.startNewSubPath(startXPos, endYPos);
				pline.lineTo(endXPos, endYPos);
			}
			else {
				pline.startNewSubPath(startXPos, startYPos);
				pline.lineTo(endXPos, startYPos);
				pline2.startNewSubPath(endXPos, startYPos);
				pline2.lineTo(endXPos, endYPos);
			}
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeArea));
			g.strokePath(pline2, PathStrokeType(1.5f * myProcessor->getPluginScaleWidthFactor() ));
		}
		else {
			pline.startNewSubPath(startXPos, startYPos);
			float xDist = endXPos - startXPos; //sharp line			
			float weightedX = 0.0f;
			float middleY = 0.0f;
			if (startYPos > endYPos) {
				weightedX = startXPos + xDist * (1.0f - curvy);
				middleY = (startYPos - endYPos) * (1.0f - curvy) + jmin(startYPos, endYPos);
			}
			else {
				weightedX = endXPos - xDist * (1.0f - curvy);
				middleY = (endYPos - startYPos) * (1.0f - curvy) + jmin(startYPos, endYPos);
			}
			pline.cubicTo(weightedX, middleY, weightedX, middleY, endXPos, endYPos);
		}

		//draw solid curve line		
		g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeArea));
		g.strokePath(pline, PathStrokeType(1.5f * myProcessor->getPluginScaleWidthFactor()));

		for (int voice = 0; voice < C_MAX_POLY; voice++) {
			if (myProcessor->m_pVASTXperience.m_Poly.isVoicePlaying(voice)) {
				float distance = pline.getLength() * (lastSrceVals[voice] + 1.f) * 0.5f;
				juce::Point pH = pline.getPointAlongPath(distance);
				float xP = pH.getX() - 2.f * myProcessor->getPluginScaleWidthFactor();
				float yP = pH.getY() - 2.f * myProcessor->getPluginScaleHeightFactor();

				g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeLine).contrasting(voice / float(C_MAX_POLY)));
				g.fillRect(xP, yP, 3.5f * myProcessor->getPluginScaleWidthFactor(), 3.5f * myProcessor->getPluginScaleHeightFactor());
			}
		}
	}
	lastCurvy = curvy;
	lastModVal = modval;

	repaint();
}

void VASTModMatCurveDisplay::startAutoUpdate() {
	startTimer(50);
}

void VASTModMatCurveDisplay::stopAutoUpdate() {
	stopTimer();
}

void VASTModMatCurveDisplay::timerCallback() {
	updateContent(false);
}

void VASTModMatCurveDisplay::mouseDrag(const MouseEvent &e) { // show value
	//ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
}

void VASTModMatCurveDisplay::mouseDown(const MouseEvent &e) {
}

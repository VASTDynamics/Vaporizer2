/*
VAST Dynamics

How to calculate quadratic beziers
x from 0 to 1: 
y = (1-x)^2  * startval  + 2x(1-x) * intermediateval + x^2 * endval

http://slideplayer.com/slide/6554987/

*/

#include "VASTLFOEditor.h"
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

VASTLFOEditor::VASTLFOEditor(AudioProcessor* processor, String suffix)
	: myProcessor((VASTAudioProcessor*)processor), mySuffix(suffix)
{
	//myFont.setTypefaceName("Open Sans"); //bold 1-2, regular 2
	//myFont.setDefaultMinimumHorizontalScaleFactor(1.0);
	//myFont.setSizeAndStyle(14.f, Font::bold, 1.0f, 0.0f); // no squashing, no kerning

	std::fill(m_dispReset, m_dispReset + C_MAX_POLY, true);
	fillBuffers();

	setOpaque(true); //avoid repaints of parents

	resized();
	//updateContent(true);
}

VASTLFOEditor::~VASTLFOEditor() {
	this->setLookAndFeel(nullptr);
	stopTimer();
}

void VASTLFOEditor::resized() {
	m_needsRescale = false;
	m_dirty = true;
}

void VASTLFOEditor::fillBuffers() { //for UI only
	struct timeval tp;
	myProcessor->m_pVASTXperience.m_Set._gettimeofday(&tp);
	juce::Random rand(tp.tv_sec); //seed
	m_noise_buffer = std::vector<float>(C_WAVE_TABLE_SIZE);
	//float shheight = 0.f;
	//int shlength = 0;
	for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
		m_noise_buffer[i] = rand.nextFloat() * 2.f - 1.f;
				
		sSampleAndHold sandh; 
		sandh.height = rand.nextFloat() * 2.f - 1.f;
		sandh.length = rand.nextFloat() * C_WAVE_TABLE_SIZE * 0.5f; //arbitrary value
		m_sh_buffer.push_back(sandh);
	}
}

void VASTLFOEditor::lookAndFeelChanged() {
	if (myProcessor->isCurrentEditorInitialized())
		updateContent(true);
}

void VASTLFOEditor::paint(Graphics& g)
{
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();

	if (m_needsRescale) {
		waveformImage = waveformImage.rescaled(jmax(1, getWidth()), jmax(1, getHeight()), juce::Graphics::ResamplingQuality::highResamplingQuality);
		waveformImageWithBorder = waveformImage.createCopy();
		handleBorderDisplay();
	}
	else
		if (m_needsPositionUpdate) {
			m_needsPositionUpdate = false;
			handleBorderDisplay();
		}

	g.drawImageWithin(waveformImageWithBorder, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
}


VASTLFOEditor::drawState VASTLFOEditor::getActualDrawState() {
	VASTLFOEditor::drawState lDrawState;
	lDrawState.wave = 0;
	lDrawState.ramp = 0;
	lDrawState.freq = 0;
	lDrawState.polarity = 0;
	lDrawState.sync = false;
	lDrawState.beats = 0;
	lDrawState.phase = 0.f;
	if (mySuffix == "LFO1") {
		lDrawState.lfonr = 0;
		lDrawState.wave = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOWave_LFO1;
		lDrawState.pervoice = (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bLFOPerVoice_LFO1 == 1);
		lDrawState.ramp = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFORamp_LFO1;
		lDrawState.polarity = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOPolarity_LFO1;
		lDrawState.freq = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFOFreq_LFO1;
		lDrawState.sync = (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bLFOSynch_LFO1 == 1);
		lDrawState.beats = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOTimeBeats_LFO1;
		lDrawState.phase = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFOPhase_LFO1;
	}
	else if (mySuffix == "LFO2") {
		lDrawState.lfonr = 1;
		lDrawState.wave = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOWave_LFO2;
		lDrawState.pervoice = (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bLFOPerVoice_LFO2 == 1);
		lDrawState.ramp = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFORamp_LFO2;
		lDrawState.polarity = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOPolarity_LFO2;
		lDrawState.freq = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFOFreq_LFO2;
		lDrawState.sync = (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bLFOSynch_LFO2 == 1);
		lDrawState.beats = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOTimeBeats_LFO2;
		lDrawState.phase = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFOPhase_LFO2;
	}
	else if (mySuffix == "LFO3") {
		lDrawState.lfonr = 2;
		lDrawState.wave = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOWave_LFO3;
		lDrawState.pervoice = (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bLFOPerVoice_LFO3 == 1);
		lDrawState.ramp = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFORamp_LFO3;
		lDrawState.polarity = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOPolarity_LFO3;
		lDrawState.freq = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFOFreq_LFO3;
		lDrawState.sync = (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bLFOSynch_LFO3 == 1);
		lDrawState.beats = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOTimeBeats_LFO3;
		lDrawState.phase = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFOPhase_LFO3;
	}
	else if (mySuffix == "LFO4") {
		lDrawState.lfonr = 3;
		lDrawState.wave = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOWave_LFO4;
		lDrawState.pervoice = (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bLFOPerVoice_LFO4 == 1);
		lDrawState.ramp = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFORamp_LFO4;
		lDrawState.polarity = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOPolarity_LFO4;
		lDrawState.freq = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFOFreq_LFO4;
		lDrawState.sync = (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bLFOSynch_LFO4 == 1);
		lDrawState.beats = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOTimeBeats_LFO4;
		lDrawState.phase = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFOPhase_LFO4;
	}
	else if (mySuffix == "LFO5") {
		lDrawState.lfonr = 4;
		lDrawState.wave = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOWave_LFO5;
		lDrawState.pervoice = (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bLFOPerVoice_LFO5 == 1);
		lDrawState.ramp = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFORamp_LFO5;
		lDrawState.polarity = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOPolarity_LFO5;
		lDrawState.freq = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFOFreq_LFO5;
		lDrawState.sync = (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bLFOSynch_LFO5 == 1);
		lDrawState.beats = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uLFOTimeBeats_LFO5;
		lDrawState.phase = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fLFOPhase_LFO5;
	}

	lDrawState.lIntervalTime = myProcessor->m_pVASTXperience.m_Set.getIntervalTimeFromDAWBeats(myDrawState.beats) / 1000.f;
	return lDrawState;
}

void VASTLFOEditor::handleBorderDisplay() {
	if (waveformImage.isNull())
		return;
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();
	Graphics g(waveformImageWithBorder);
	g.drawImageAt(waveformImage, 0, 0);

	for (int voiceNo = 0; voiceNo < myProcessor->m_pVASTXperience.m_Set.m_uMaxPoly; voiceNo++) {
		if (!m_dispReset[voiceNo]) {
			float markerPos = 1.f - (lastLFOVal[voiceNo] + 1.f) * 0.5f; //0..1

			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorPosMarker).darker(0.5f).withAlpha(0.4f));
			g.drawLine(m_xbounds, m_ybounds + m_drawheight * markerPos, waveformImage.getWidth() - m_xbounds, m_ybounds + m_drawheight * markerPos, 1.0f * m_screenWidthScale * myProcessor->getPluginScaleWidthFactor());

			float pointSize = 8.f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale;
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorPosMarker).withAlpha(0.7f));
			g.fillEllipse(m_xbounds - pointSize * 0.5f, m_ybounds + m_drawheight * markerPos - pointSize * 0.5f, pointSize, pointSize);
		}
		if (!myDrawState.pervoice)
			break; //do only 0
	}
}

void VASTLFOEditor::updateContent(bool force) {
	if (!force) {
		if ((this->isShowing() == false)) {
			return;
		}
		if (!m_dirty) {
			return;
		}
	}
	if ((getWidth() <= 1) || (getHeight() <= 1))
		return;

	m_screenWidthScale = float(getScreenBounds().getWidth()) / float(getWidth());
	m_screenHeightScale = float(getScreenBounds().getHeight()) / float(getHeight());

	m_xbounds = (float)getScreenBounds().getWidth() * 0.02f;
	m_ybounds = (float)getScreenBounds().getHeight() * 0.05f;
	m_drawheight = (float)getScreenBounds().getHeight() - 2 * m_ybounds;
	m_drawwidth = (float)getScreenBounds().getWidth() - 2 * m_xbounds; //check was ybounds here

	waveformImage = Image(Image::RGB, jmax(1, getScreenBounds().getWidth()), jmax(1, getScreenBounds().getHeight()), false);
	waveformImage.clear(waveformImage.getBounds(), myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));
	waveformImageWithBorder = Image(Image::RGB, jmax(1, waveformImage.getWidth()), jmax(1, waveformImage.getHeight()), false);
	waveformImageWithBorder.clear(waveformImage.getBounds(), myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));
	Graphics g(waveformImage);

	float fontSize = (m_drawwidth / 1000.f) * 16.f;
	Font myFont = myProcessor->getCurrentVASTLookAndFeel()->getDefaultFont();
	myFont.setSizeAndStyle(fontSize, Font::plain, 1.0f, 0.0f); // no squashing, no kerning
	g.setFont(myFont);

	float halfHeight = m_drawheight * 0.5f;

	myDrawState = getActualDrawState();

	std::vector<float> l_waveBuffer = std::vector<float>(C_WAVE_TABLE_SIZE);
	
	CVASTWaveTable::getNaiveSamplesFromWave(l_waveBuffer, myDrawState.wave);

	if (myDrawState.wave == WAVE::noise) {
		l_waveBuffer = m_noise_buffer;
	}
	
	float lFreq = myDrawState.freq;
	float displayPeriod = 10.f; //10 seconds display
	if (myDrawState.sync) {
		lFreq = 1.f / myDrawState.lIntervalTime;
		displayPeriod = 16.f * myDrawState.lIntervalTime / myProcessor->m_pVASTXperience.m_Set.getIntervalRatio(myDrawState.beats); //show 16 beats
	}  
	float cyclesToDisplay = displayPeriod * lFreq;
	if (cyclesToDisplay < 1.f) {
		displayPeriod = displayPeriod * 4.f;
		cyclesToDisplay = displayPeriod * lFreq;
	}
	else if (cyclesToDisplay > 64.f) {
		displayPeriod = displayPeriod / 4.f;
		cyclesToDisplay = displayPeriod * lFreq;
	}
	
	float beatsPerDisplay = (displayPeriod / myDrawState.lIntervalTime) * myProcessor->m_pVASTXperience.m_Set.getIntervalRatio(myDrawState.beats); //show ramp
	if (beatsPerDisplay < (myDrawState.ramp * 2.f)) {
		beatsPerDisplay = myDrawState.ramp * 2.f;
		displayPeriod = myDrawState.lIntervalTime * (beatsPerDisplay / myProcessor->m_pVASTXperience.m_Set.getIntervalRatio(myDrawState.beats));
		cyclesToDisplay = displayPeriod * lFreq;
	}

	//gridlines
	if (myDrawState.sync) {
		Colour cGridlines = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayGrid).withAlpha(0.6f);
		g.setColour(cGridlines);
		float fGridWidth = m_drawwidth / beatsPerDisplay;
		for (int verti = 0; verti <= int(beatsPerDisplay); verti++) {
			float xpoint = verti;
			(verti % 4 == 0.f) ? g.setColour(cGridlines.withAlpha(1.0f)) : g.setColour(cGridlines.withAlpha(0.4f));
			g.drawLine(m_xbounds + xpoint * fGridWidth, m_ybounds, m_xbounds + xpoint * fGridWidth, m_ybounds + m_drawheight, 1.f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale);
		}
	}
	else {
		Colour cGridlines = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayGrid).withAlpha(0.6f);
		g.setColour(cGridlines);
		float fGridWidth = m_drawwidth / displayPeriod;
		for (int verti = 0; verti <= int(displayPeriod); verti++) {
			float xpoint = verti;
			g.drawLine(m_xbounds + xpoint * fGridWidth, m_ybounds, m_xbounds + xpoint * fGridWidth, m_ybounds + m_drawheight, 1.f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale);
		}
	}

	Path myWave;
	Colour c1 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOEditorArea);
	Colour c2 = c1.withAlpha(0.f);
	int sanhcounter = 0;
	sSampleAndHold sandh = m_sh_buffer[sanhcounter];
	int sandhlength = 0;
	float step = float((l_waveBuffer.size()) / float(m_drawwidth)) * cyclesToDisplay;
	for (int x = 0; x < m_drawwidth; x++) {
		float yVal = 0.f;
		if (myDrawState.wave == WAVE::samplehold) {
				if (sandhlength <= 0) {
					sandh = m_sh_buffer[sanhcounter++];
					sandhlength = sandh.length / cyclesToDisplay;
				}
				yVal = sandh.height;
				sandhlength--;
		}
		else {
			yVal = l_waveBuffer[int(l_waveBuffer.size() * myDrawState.phase / 100.f + x * step) % l_waveBuffer.size()];
			yVal *= 0.98f; //make a bit smaller
		}

		//Ramp
		float factor = 1.f;
		if (myDrawState.ramp > 0)
			factor = ((x) / (m_drawwidth * (myDrawState.ramp / beatsPerDisplay)));
		if (factor < 1.f) {
			yVal *= factor;
		}

		float mpoint = m_ybounds + halfHeight - (halfHeight * yVal);
		if (x == 0) myWave.startNewSubPath(m_xbounds, mpoint);
		myWave.lineTo(m_xbounds+ x, mpoint);

		if (myDrawState.polarity == 0) {//unipolar
			g.setGradientFill(ColourGradient(c2, juce::Point<float>(m_xbounds + x, m_ybounds + m_drawheight), c1, juce::Point<float>(m_xbounds + x, mpoint), true));
			g.drawLine(m_xbounds + x, mpoint, m_xbounds + x, m_ybounds + m_drawheight, 1.f /** myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale */);
		}
		else { //bipolar
			g.setGradientFill(ColourGradient(c2, juce::Point<float>(m_xbounds + x, m_ybounds + halfHeight), c1, juce::Point<float>(m_xbounds + x, mpoint), true));
			g.drawLine(m_xbounds + x, mpoint, m_xbounds + x, m_ybounds + halfHeight, 1.f /** myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale */);
		}
	}
	g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLFOEditorLine)); 
	g.strokePath(myWave, PathStrokeType(1.0f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale));

	// Text
	g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLabelText));
	if (myDrawState.sync)
		g.drawText(String(int(beatsPerDisplay + 0.5f)) + " beats", juce::Rectangle<int>(waveformImage.getWidth() * 0.8f, waveformImage.getHeight() - myFont.getHeight(), waveformImage.getWidth() * 0.2f - m_xbounds, myFont.getHeight()), Justification::centredRight, false);
	else
		g.drawText(String(int(displayPeriod)) + " s", juce::Rectangle<int>(waveformImage.getWidth() * 0.8f, waveformImage.getHeight() - myFont.getHeight(), waveformImage.getWidth() * 0.2f - m_xbounds, myFont.getHeight()), Justification::centredRight, false);

	handleBorderDisplay();
	m_needsRescale = false;
	m_dirty = false;
	repaint();
}

/*
void VASTLFOEditor::mouseDown(const MouseEvent & e)
{
}

void VASTLFOEditor::mouseUp(const MouseEvent & e) {
}

void VASTLFOEditor::mouseDrag(const MouseEvent & e)
{
}
*/

bool VASTLFOEditor::checkForPositionChanges() {
	bool valChanged = false;
	if (myDrawState.pervoice)
		for (int i = 0; i < myProcessor->m_pVASTXperience.m_Set.m_uMaxPoly; i++) {
			if (myProcessor->m_pVASTXperience.m_Poly.isVoicePlaying(i)) {

				if (lastLFOVal[i] != myProcessor->m_pVASTXperience.m_Poly.m_fLastLFOOscValue[myDrawState.lfonr][i].load()) {
					lastLFOVal[i] = myProcessor->m_pVASTXperience.m_Poly.m_fLastLFOOscValue[myDrawState.lfonr][i].load();
					m_dispReset[i] = false;
					valChanged = true;
				}
			}
			else {
				if (m_dispReset[i] == false)
					valChanged = true;
				m_dispReset[i] = true;		
			}
		}
	else {
		bool lastdispReset0 = m_dispReset[0];
		if (lastLFOVal[0] != myProcessor->m_pVASTXperience.m_Poly.m_fLastGlobalLFOOscValue[myDrawState.lfonr].load()) {
			for (int i = 0; i < myProcessor->m_pVASTXperience.m_Set.m_uMaxPoly; i++) {
				m_dispReset[i] = true;
				lastLFOVal[i] = myProcessor->m_pVASTXperience.m_Poly.m_fLastGlobalLFOOscValue[myDrawState.lfonr].load();
				if (myProcessor->m_pVASTXperience.m_Poly.isVoicePlaying(i)) {
					lastLFOVal[0] = lastLFOVal[i];
					m_dispReset[0] = false;
					m_dispReset[i] = false;
					valChanged = true;
                    VDBG("i: " << i << " lastLFOVal[0] " << lastLFOVal[0]);
                    jassert(lastLFOVal[0] != 0.0f);
				}
			}
		}			
		if (lastdispReset0 != m_dispReset[0])
			valChanged = true;
	}

	return valChanged;
}

bool VASTLFOEditor::checkForChanges() {
	VASTLFOEditor::drawState lDrawState = getActualDrawState();
	if ((myDrawState.beats != lDrawState.beats) ||
		(myDrawState.pervoice != lDrawState.pervoice) ||
		(myDrawState.freq != lDrawState.freq) ||
		(myDrawState.lIntervalTime != lDrawState.lIntervalTime) ||
		(myDrawState.phase != lDrawState.phase) ||
		(myDrawState.polarity != lDrawState.polarity) ||
		(myDrawState.ramp != lDrawState.ramp) ||
		(myDrawState.sync != lDrawState.sync) ||
		(myDrawState.wave != lDrawState.wave))
		return true;
	return false;
}

void VASTLFOEditor::startAutoUpdate() {
	startTimer(35);
}

void VASTLFOEditor::stopAutoUpdate() {
	stopTimer();
}

void VASTLFOEditor::timerCallback() {
	m_dirty = m_dirty || checkForChanges();

	if (checkForPositionChanges()) {
		m_needsPositionUpdate = true;
		repaint();
	}

	if (m_dirty)
		updateContent(false);
}


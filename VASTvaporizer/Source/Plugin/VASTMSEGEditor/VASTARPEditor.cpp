/*
VAST Dynamics

*/

#include "VASTARPEditor.h"
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessor.h"
#include "../../Engine/VASTARPData.h"

VASTARPEditor::VASTARPEditor(AudioProcessor* processor, VASTARPData* data, VASTARPData* datalive)
	: myProcessor((VASTAudioProcessor*)processor), myData(data), myDataLive(datalive)
{
	//myFont.setTypefaceName("Open Sans"); //bold 1-2, regular 2
	//myFont.setDefaultMinimumHorizontalScaleFactor(1.0);
	//myFont.setSizeAndStyle(14.f, Font::bold, 1.0f, 0.0f); // no squashing, no kerning
	m_isDragged = -1;
	m_isDraggedCurve = false;
	
	setOpaque(true); //avoid repaints of parents

	resized();
	//updateContent(true);
}

VASTARPEditor::~VASTARPEditor() {
	this->setLookAndFeel(nullptr);
	stopTimer();
}

void VASTARPEditor::lookAndFeelChanged() {
	if (myProcessor->isCurrentEditorInitialized())
		updateContent(true);
}

void VASTARPEditor::resized()
{
	m_dirty = true;
}

void VASTARPEditor::paint(Graphics& g)
{
	if (!m_dirty) //dont paint if it has to be updated
		//g.drawImageAt(waveformImage, 0, 0);
		g.drawImageWithin(waveformImage, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
}

void VASTARPEditor::updateContent(bool force)
{
	if (myData == nullptr) return;
	if (myDataLive == nullptr) return;
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

	float fontSize = (m_drawwidth / 1000.f) * 16.f;
	Font myFont = myProcessor->getCurrentVASTLookAndFeel()->getDefaultFont();
	myFont.setSizeAndStyle(fontSize, myFont.getStyleFlags(), 1.0f, 0.0f); // no squashing, no kerning
	
	m_screenWidthScale = float(getScreenBounds().getWidth()) / float(getWidth());
	m_screenHeightScale = float(getScreenBounds().getHeight()) / float(getHeight());

	m_xbounds = (float)getScreenBounds().getWidth() * 0.02f;
	m_ybounds = (float)getScreenBounds().getHeight() * 0.05f;
	m_drawheight = (float)getScreenBounds().getHeight() - 2 * m_ybounds;
	m_drawwidth = (float)getScreenBounds().getWidth() - 2 * m_xbounds;

	waveformImage = Image(Image::RGB, jmax(1, getScreenBounds().getWidth()), jmax(1, getScreenBounds().getHeight()), false);
	waveformImage.clear(waveformImage.getBounds(), myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));
	Graphics g(waveformImage);

	g.setFont(myFont);

	int numSteps = myData->getNumSteps();	
	float stepWidth = m_drawwidth / float(numSteps);
	float octaveHeight = m_drawheight / 7.0f; //seven octaves (including semitones up and down)

	float insetPixels = 2.0f  * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale;

	//current pos marker
	if ((myDataLive->m_dispActiveStep.load() >= 0) && (numSteps >= myDataLive->m_dispActiveStep.load())) {
		g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorPosMarker));

		//float markerPos = m_xbounds + myData->m_dispActiveStep.load() * (m_drawwidth / float(numSteps));
		g.fillRect(juce::Rectangle<float>(m_xbounds + myDataLive->m_dispActiveStep.load()  * stepWidth + insetPixels, m_ybounds + insetPixels, stepWidth - 2.f* insetPixels, m_drawheight -2.f * insetPixels));
		//g.drawLine(markerPos, 0.0f + m_ybounds, markerPos, m_drawheight + m_ybounds, 6.0f);
	}

	for (int i = 0; i < numSteps; i++) {
		//step velocity
		VASTARPData::ArpStep curStep = myData->getStep(i);
		if (curStep.gate > 0) {
			const Colour c1 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorVelocityFrom); 
			const Colour c2 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorVelocityTo); 

			g.setGradientFill(ColourGradient(c2, m_xbounds, m_ybounds + m_drawheight, c1, m_xbounds + m_drawwidth, m_ybounds, false));

			float velocityHeight = (curStep.velocity / 127.f) * (m_drawheight - 2.f * insetPixels);
			g.fillRect(juce::Rectangle<float>(m_xbounds + i * stepWidth + insetPixels, m_ybounds + (m_drawheight - velocityHeight), stepWidth - 2.f* insetPixels, (m_drawheight - 2.f * insetPixels) - (m_drawheight - velocityHeight)));
		}

		//step octave
		if (curStep.gate > 0) {
			int oct = 5 - (curStep.octave + 2);
			float tonewidth = (curStep.gate / 4.f) * stepWidth;
			float semitonesadd = 0.f;
			if (curStep.semitones != 0) {
				semitonesadd = -float(curStep.semitones) * (octaveHeight / 12.f);
				g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorStep));
				g.fillRoundedRectangle(juce::Rectangle<float>(m_xbounds + i * stepWidth + insetPixels, m_ybounds + oct * octaveHeight + semitonesadd + insetPixels, tonewidth - 2.f* insetPixels, octaveHeight - 2.f * insetPixels), 8.f  * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale);
				g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorStepText)); 
				g.drawText(String(curStep.semitones), juce::Rectangle<float>(m_xbounds + i * stepWidth + insetPixels, m_ybounds + oct * octaveHeight + semitonesadd + insetPixels, tonewidth - 2.f* insetPixels, octaveHeight - 2.f * insetPixels), juce::Justification::centred, false);
			}
			else {
				g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorStepOctave)); 
				g.fillRoundedRectangle(juce::Rectangle<float>(m_xbounds + i * stepWidth + insetPixels, m_ybounds + oct * octaveHeight + semitonesadd + insetPixels, tonewidth - 2.f* insetPixels, octaveHeight - 2.f * insetPixels), 8.f  * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale);
			}
		}
	}

	//gridlines
	g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colARPEditorGrid));
	g.drawRect(juce::Rectangle<float>(m_xbounds, m_ybounds, m_drawwidth, m_drawheight), 1.f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale);
	for (int i = 0; i < numSteps; i++) {
		g.drawVerticalLine(m_xbounds + i * stepWidth, m_ybounds, m_ybounds + m_drawheight);
	}
	for (int i = 0; i < 7; i++) {
		g.drawHorizontalLine(m_ybounds + i * octaveHeight, m_xbounds, m_xbounds + m_drawwidth);
	}

	// Text
	bool sync = static_cast<bool>(*myProcessor->m_pVASTXperience.m_Set.m_State->m_bARPSynch);
	int beats = static_cast<int>(*myProcessor->m_pVASTXperience.m_Set.m_State->m_uARPTimeBeats);
	float speed = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fARPSpeed;
	float lIntervalTime = speed / 1000.f;
	float displayPeriod = myData->getNumSteps() * lIntervalTime;
	if (sync) {
		lIntervalTime = float(myProcessor->m_pVASTXperience.m_Set.getIntervalTimeFromDAWBeats(beats)) / 1000.f;
		displayPeriod = myData->getNumSteps() * float(myProcessor->m_pVASTXperience.m_Set.getIntervalRatio(beats)) * lIntervalTime;
	}
	float beatsPerDisplay = displayPeriod / lIntervalTime;

	int fontHeight = static_cast<int>(myFont.getHeightInPoints());
	g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLabelText));
	if (sync)
		g.drawText(String(int(beatsPerDisplay + 0.5f)) + " beats", juce::Rectangle<int>(waveformImage.getWidth() * 0.8f, waveformImage.getHeight() - fontHeight - 2.f, waveformImage.getWidth() * 0.2f - m_xbounds, fontHeight + 2.f), Justification::centredRight, false);
	else
		g.drawText(String(int(displayPeriod * 1000.f)) + " ms", juce::Rectangle<int>(waveformImage.getWidth() * 0.8f, waveformImage.getHeight() - fontHeight - 2.f, waveformImage.getWidth() * 0.2f - m_xbounds, fontHeight + 2.f), Justification::centredRight, false);

	m_dirty = false;
	repaint();
}

void VASTARPEditor::mouseDown(const MouseEvent & e)
{
	if (myData == nullptr) return;

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	// check the mod keys ..
	int numSteps = myData->getNumSteps();
	//float stepWidth = m_drawwidth / float(numSteps);
	float octaveHeight = m_drawheight / m_screenHeightScale / 7.0; //seven octaves (including semitones up and down)

	float mouseY = e.getMouseDownY();
	float mouseX = e.getMouseDownX();

	int clickedStep = ((mouseX - m_xbounds / m_screenWidthScale) / (m_drawwidth / m_screenWidthScale)) * float(numSteps);
	if ((clickedStep < 0) || (clickedStep > numSteps)) return;

	if (e.getNumberOfClicks() > 1) { //double click
		if (modifiers.isLeftButtonDown()) {
			myData->incGate(clickedStep);
		}
	}
	else {
		if (modifiers.isPopupMenu()) {
			int clickedVelocity = 127.f - ((mouseY - m_ybounds / m_screenHeightScale) / (m_drawheight / m_screenHeightScale)) * 127.f;
			if ((clickedVelocity >= 0) && (clickedVelocity <= 127))
				myData->setVelocity(clickedStep, clickedVelocity);
		}
		else if (modifiers.isLeftButtonDown()) {
			int clickedOctave = round(3.f - ((mouseY - m_ybounds / m_screenHeightScale) / octaveHeight) + .5f);
			if ((clickedOctave <= 2) && (clickedOctave >= -2)) {
				myData->setOctave(clickedStep, clickedOctave);
				myData->setSemitone(clickedStep, 0);
			}
		}
	}
	m_isDragged = clickedStep;
	
	repaint();
}

void VASTARPEditor::mouseUp(const MouseEvent &) {
	m_isDragged = -1;
}

void VASTARPEditor::mouseDrag(const MouseEvent & e)
{
	Component::mouseDrag(e);

	if (myData == nullptr) return;
	float mouseY = e.getMouseDownY() + e.getDistanceFromDragStartY();
	//float mouseX = e.getMouseDownX();
	//float octaveHeight = m_drawheight / 7.0; //seven octaves (including semitones up and down)

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	// check the mod keys ..
	if (m_isDragged>=0)	{
		if (modifiers.isPopupMenu()) {
			int clickedVelocity = 127.f - ((mouseY - m_ybounds / m_screenHeightScale) / (m_drawheight / m_screenHeightScale)) * 127.f;
			if ((clickedVelocity >= 0) && (clickedVelocity <= 127))
				myData->setVelocity(m_isDragged, clickedVelocity);
		}
		else if (modifiers.isLeftButtonDown()) {
			int clickedSemitone = -round(e.getDistanceFromDragStartY() / ((m_drawheight / m_screenHeightScale) / 24.f) + .5f);
			if ((clickedSemitone <= 11) && (clickedSemitone >= -11))
				myData->setSemitone(m_isDragged, clickedSemitone);
		}
	}
	repaint();
}

void VASTARPEditor::mouseMove(const MouseEvent& e) {
	Component::mouseMove(e);

	updateContent(true);

	int numSteps = myData->getNumSteps();
	float mouseX = e.getMouseDownX();
	m_mouseOverStep = ((mouseX - m_xbounds / m_screenWidthScale) / (m_drawwidth / m_screenWidthScale)) * float(numSteps);
	for (int step = 0; step < numSteps; step++) {
		Graphics g(waveformImage);
		float stepWidth = (m_drawwidth / m_screenWidthScale) / float(numSteps);
		float insetPixels = 0.0f;
		if (step == m_mouseOverStep) {
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeSelection).darker(0.7f));
			g.drawRect(juce::Rectangle<float>(m_xbounds + step * stepWidth * m_screenWidthScale + insetPixels, m_ybounds + insetPixels, stepWidth * m_screenWidthScale - 2.f* insetPixels, m_drawheight - 2.f * insetPixels));
		}
	}
	repaint();
}

void VASTARPEditor::mouseExit(const MouseEvent& ) {
	updateContent(true);
}

void VASTARPEditor::startAutoUpdate() {
	startTimer(50); //50 milliseconds
}
void VASTARPEditor::stopAutoUpdate() {
	stopTimer();
}

void VASTARPEditor::timerCallback() {
	if (myDataLive == nullptr) return;
	if (myDataLive->getAndClearDirtyFlag()) {
		m_dirty = true;
	}
	if (m_dirty)
		updateContent(false);
}

inline bool VASTARPEditor::isInterestedInFileDrag(const StringArray& files) {
	for (int i = 0; i < files.size(); i++) {
		if (files[i].endsWithIgnoreCase(".mid")) return true;
	}
	return false;
}

void VASTARPEditor::filesDropped(const StringArray& files, int, int) {
	String file = files[0];
	File midiFile = File(file);
	std::unique_ptr<FileInputStream> midiFileInputStream;
	midiFileInputStream = midiFile.createInputStream();

	MidiFile M;
	M.readFrom(*midiFileInputStream);

	//M.convertTimestampTicksToSeconds();

	//for (int t = 0; t < M.getNumTracks(); t++) {

	//int l_lastNoteOn = 0;
	int numTracks = M.getNumTracks();
	for (int track = 0; track < numTracks; track++) {
		const MidiMessageSequence* curTrack = M.getTrack(track);
		int numEvents = curTrack->getNumEvents();
		bool bHasNotes = false;
		for (int i = 0; i < numEvents; i++) {
			MidiMessage& m = curTrack->getEventPointer(i)->message;
			if (m.isNoteOn()) {
				bHasNotes = true;
				break;
			}
		}
		
		if (bHasNotes) {
			int arpNoteReal = -1;
			
			//clear??
			
			int firstNoteNumber = -1;
			for (int i = 0; i < numEvents; i++) {
				MidiMessage& m = curTrack->getEventPointer(i)->message;
				
				if (!m.isSysEx()) {
					if (m.isNoteOn()) {
						arpNoteReal++;
						if (firstNoteNumber == -1) firstNoteNumber = m.getNoteNumber(); //first note is root
						int duration = m.getTempoSecondsPerQuarterNote();
						int semitones = m.getNoteNumber() - firstNoteNumber;
						int octaves = semitones / 12;
						semitones = semitones % 12;
						int velocity = m.getVelocity();
						VDBG("Track " << track << " Midinote: " << m.getNoteNumber() << " Duration: " << duration << " Velocity: " << m.getVelocity());
						//} else if (m.isNoteOn()) {
		//					//l_lastNoteOn = m.getTempoSecondsPerQuarterNote();

						if ((octaves <= 2) && (octaves >= -2) && (semitones <= 11) && (semitones >= -11) && (velocity <= 127) && (velocity >= 0)) {
							if (arpNoteReal > 63) break;
							myData->arpChangeSteps(arpNoteReal + 1);
							myData->setSemitone(arpNoteReal, semitones);
							myData->setOctave(arpNoteReal, octaves);
							myData->setVelocity(arpNoteReal, velocity);
						}
						//else
							//vassertfalse;


					}
				}
			}


			break;
		}
	}
	updateContent(true);
}

String VASTARPEditor::getTooltip()
{
    return TRANS("Left mouse click: choose octave (-2..2). Left drag up/down: choose semitone (-11..11). Left doubleclick: toggle note gate length (off, 1/4, 1/2, 3/4, legato). Right drag up/down: choose note velocity.");
}

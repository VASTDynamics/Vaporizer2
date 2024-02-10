/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTOscilloscope.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTAudioProcessor.h"
#include "../VASTWaveTableEditorComponent.h"
#include "../../Engine/VASTSingleNote.h"
#include "../../Engine/Oscillator/VASTWaveTable.h"

#define C_MAX_UI_STEPS_AFTER_NOTEOFF 30

VASTOscilloscope::VASTOscilloscope()
{
	myProcessor = nullptr;
	myEditor = nullptr;
	myWtEditor = nullptr;

	setComponentID("audioOscilloscope");
	
	setToOpenGLRender();
	
	setOpaque(true); //avoid repaints of parents
	setRepaintsOnMouseActivity(false); //performance
	
	setPaintingIsUnclipped(true);

	resized(); // initialise image
	
	m_selection.iScreenSelectionStart = 0;
	m_selection.iScreenSelectionEnd = getWidth();
	m_selection.iWTSelectionStart = 0;
	m_selection.iWTSelectionEnd = C_WAVE_TABLE_SIZE - 1;

	m_safeWtPosFloat = 0.0f;
	m_safePhaseFloat = 0.0f;
	m_safeWTFXType = 0;
	m_safeWTFXVal = 0.f;

	m_last_active_voice = -1;
	m_last_active_counter = C_MAX_UI_STEPS_AFTER_NOTEOFF; 

	m_freeHandBuffer.clear();

	/*
	mOpenGLContext.setComponentPaintingEnabled(false); //true
	mOpenGLContext.setRenderer(this);
	mOpenGLContext.setContinuousRepainting(true); //true
	mOpenGLContext.attachTo(*this);
	*/
}

VASTOscilloscope::~VASTOscilloscope() {
	
	setToStandardRender(); //detach openGLContext

	this->setLookAndFeel(nullptr);
	stopTimer();
}

void VASTOscilloscope::setToStandardRender() {
	//mOpenGLContext.detach();
}

void VASTOscilloscope::setEditor(VASTAudioProcessorEditor* editor) { 
	myEditor = editor; 
}

void VASTOscilloscope::setProcessor(VASTAudioProcessor* processor) {
	myProcessor = processor;
}

void VASTOscilloscope::setWTEditor(VASTWaveTableEditorComponent* wtEditor) { 
	myWtEditor = wtEditor;
}

String VASTOscilloscope::getTooltip()
{
	if (myWtEditor == nullptr)
		return TRANS("Use right mouse click to toggle single cycle versus perspective wavetable display.");
	else
		return TRANS("Click and drag to change selected area / draw. Doubleclick toggles selection. Rightclick for menu.");
}

void VASTOscilloscope::setToOpenGLRender() {
	/*
	if (((myProcessor != nullptr) && !myProcessor->m_disableOpenGLGFX) || (myProcessor == nullptr)) {
		if (!mOpenGLContext.isAttached()) {
			mOpenGLContext.setComponentPaintingEnabled(true);
			mOpenGLContext.attachTo(*this);
		}
	}
	*/
	
}

//void VASTOscilloscope::newOpenGLContextCreated() {
//	mOpenGLContext.setSwapInterval(1);
//	mBackgroundTexture.bind();
//}
//void VASTOscilloscope::openGLContextClosing() {
//
//}
/*
void VASTOscilloscope::renderOpenGL() {

	if ((m_count++ % 100) != 0) { //fps 

		if (m_newImageForRepaint) {
			m_newImageForRepaint = false;

			const juce::MessageManagerLock mmLock(juce::ThreadPoolJob::getCurrentThreadPoolJob());

			if (!mmLock.lockWasGained()) {
				//return;
			}

			if (waveformImageWithBorder.isNull())
				waveformImageWithBorder = waveformImage.createCopy();

			if (!m_dirty.load()) {
				//g.drawImageWithin(waveformImageWithBorder, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
				mBackgroundTexture.loadImage(waveformImageWithBorder);
			}
			else {
				if (myProcessor->nonThreadsafeIsBlockedProcessingInfo())
					//g.drawImageWithin(waveformImageWithBorder, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
					mBackgroundTexture.loadImage(waveformImageWithBorder);
				else {
					if (waveformImageLastSafe.isNull())
						//g.drawImageWithin(waveformImageBufferOff, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
						mBackgroundTexture.loadImage(waveformImageBufferOff);
					else
						//g.drawImageWithin(waveformImageLastSafe, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
						mBackgroundTexture.loadImage(waveformImageLastSafe);
				}
			}
			//mBackgroundTexture.bind();
		}

		//OpenGLHelpers::clear(Colours::black);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR?

		// Draw a textured quad
		glBegin(GL_QUADS);
		float w = getWidth();
		float h = getHeight();
		glTexCoord2f(1, 1); glVertex3f(1, 1, 0);
		glTexCoord2f(1, 0); glVertex3f(1, -1, 0);
		glTexCoord2f(0, 0); glVertex3f(-1, -1, 0);
		glTexCoord2f(0, 1); glVertex3f(-1, 1, 0);
		glEnd();
		
		//juce::Rectangle<int> imageSize(0, 0, waveformImageWithBorder.getWidth(), waveformImageWithBorder.getHeight());
		//juce::Rectangle<int> textureSize(0, 0, mBackgroundTexture.getWidth(), mBackgroundTexture.getHeight());
		//mOpenGLContext.copyTexture(textureSize, imageSize, getWidth(), getHeight(), false);
		//mBackgroundTexture.release();
	}
}
*/

void VASTOscilloscope::lookAndFeelChanged() {
	waveformImageBufferOff = Image(Image::RGB, 400, 500, true);
	waveformImageBufferOff.clear(waveformImageBufferOff.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeOff));
    m_dirty.store(true);
	m_hasWaveformImagePerspectiveBuffer = false;
}

void VASTOscilloscope::resized()
{
	float scale = 1.f;
	if (myProcessor != nullptr) {
		m_screenWidthScale = float(getScreenBounds().getWidth()) / float(getWidth());
	}
	waveformImageBufferOff = waveformImageBufferOff.rescaled(jmax(1, int(getScreenBounds().getWidth() * scale)), jmax(1, int(getScreenBounds().getHeight() * scale)), juce::Graphics::ResamplingQuality::lowResamplingQuality);
	waveformImageWithBorder = waveformImageWithBorder.rescaled(jmax(1, int(getScreenBounds().getWidth() * scale)), jmax(1, int(getScreenBounds().getHeight() * scale)), juce::Graphics::ResamplingQuality::highResamplingQuality);

	m_hasWaveformImagePerspectiveBuffer = false;

    Component::SafePointer<VASTOscilloscope> osci_(this);
    Timer::callAfterDelay(50, [osci_, this] {
        if (osci_ != nullptr)
            updateContentAsync(); });
}

void VASTOscilloscope::paint(Graphics& g)
{	
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();

	if (!m_dirty.load()) {
		g.drawImageWithin(waveformImageWithBorder, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
	}
	else {
		if (myProcessor->nonThreadsafeIsBlockedProcessingInfo()) 
			g.drawImageWithin(waveformImageWithBorder, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
		else {
			if (waveformImageLastSafe.isNull())
				g.drawImageWithin(waveformImageBufferOff, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
			else 
				g.drawImageWithin(waveformImageLastSafe, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
		}
	}	
}

void VASTOscilloscope::updateContentAsync() {
    m_dirty.store(true);
}

void VASTOscilloscope::updateContent(bool force) {
	if (!force) {
		if (this->isShowing() == false)
			return;
	}
	if ((getWidth() <= 1) || (getHeight() <= 1))
		return;
	if (myProcessor == nullptr)
		return;
	if (myProcessor->initNotCompleted())
		return;
	if (myProcessor->nonThreadsafeIsBlockedProcessingInfo())
		return;

	if (force) {
		m_hasWaveformImagePerspectiveBuffer = false;
	}

	if (waveformImageBufferOff.isNull()) {
		waveformImageBufferOff = Image(Image::RGB, 400, 500, true);
		waveformImageBufferOff.clear(waveformImageBufferOff.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeOff));
		m_screenWidthScale = float(getScreenBounds().getWidth()) / float(getWidth());
		float scale = Desktop::getInstance().getGlobalScaleFactor() * m_screenWidthScale;
		waveformImageBufferOff = waveformImageBufferOff.rescaled(jmax(1, int(getScreenBounds().getWidth() * scale)), jmax(1, int(getScreenBounds().getHeight() * scale)));
	}

	bool l_display = false;
	int l_oscillatorBank = 0;

	String l_oscbank = getParentComponent()->getComponentID();
	//int bank = 0;
	if (l_oscbank == "OscA") {
		l_oscillatorBank = 0;
	}
	else if (l_oscbank == "OscB") {
		l_oscillatorBank = 1;
	}
	else if (l_oscbank == "OscC") {
		l_oscillatorBank = 2;
	}
	else if (l_oscbank == "OscD") {
		l_oscillatorBank = 3;
	}
	else if (l_oscbank == "WTEditorView") {
		if (myWtEditor != NULL) {
			l_oscillatorBank = myWtEditor->getOscBank();
		}
	}
	else {
		repaint();
		return;
	}

	/*
    bool editorSelectedOscBank = false;
	if ((myWtEditor == NULL) && (myEditor != nullptr) && (myEditor->vaporizerComponent != nullptr) && (myEditor->vaporizerComponent->getWaveTableEditorComponent() != nullptr) ) {
		if (l_oscillatorBank == myEditor->vaporizerComponent->getWaveTableEditorComponent()->getOscBank())
			editorSelectedOscBank = true;
	}
     */

	int note = myProcessor->m_pVASTXperience.m_Poly.getLastNotePlayed();
	if (note < 0) {
		note = 0;
	}

	/*
	CVASTSingleNote *m_myNote = myProcessor->m_pVASTXperience.m_Poly.m_singleNote[note];	
	if (m_myNote == nullptr) {
		jassertfalse; //should never be nullptr
		return;  //CHECK
	}
	vassert((m_myNote->m_safePhaseFloat[l_oscillatorBank] >= 0.f) && (m_myNote->m_safePhaseFloat[l_oscillatorBank] <= 1.f)); //safetycheck
	*/

	l_display = false;

	bool l_soloMode = false;
	if (l_oscbank != "WTEditorView") {
		if (m_soloMode != myProcessor->m_pVASTXperience.m_Poly.m_OscBank[l_oscillatorBank].getSoloMode())
			m_hasWaveformImagePerspectiveBuffer = false;
		l_soloMode = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[l_oscillatorBank].getSoloMode();
		m_soloMode = l_soloMode;
	}

	std::vector<myFloat>* l_waveBuffer = nullptr;
	std::vector<myFloat>* l_waveBufferNext = nullptr;

	//CVASTWaveTableGenerator &generator = CVASTWaveTableGenerator::getInstance();
	std::shared_ptr<CVASTWaveTable> wavetable;
	int safeWtPosInt = 0;
	//float morph = 0.f;
	float phase = 0.f;
	float wtPerc = 0.f;
	int wtfxType = 0;
	float wtfxVal = 0.f;
	bool exitWithRepaint = false;
	bool bPerspectiveDisplay = myProcessor->m_bTogglePerspectiveDisplay[l_oscillatorBank];
	if (myWtEditor != nullptr) bPerspectiveDisplay = false;

	if (myWtEditor != nullptr) {
		//wavetable = myWtEditor->getCurWavetable();
		wavetable = myWtEditor->getBankWavetable();

		//CHECK SAFETY
		if (wavetable->getNumPositions() == 0) {
			return;
		}

		safeWtPosInt = myWtEditor->getWtPos();
		m_safeWtPosFloat = safeWtPosInt;
		
		if (wavetable->m_isBeingUpdated.load() == true) //safety
			return;

		l_waveBuffer = wavetable->getNaiveTable(safeWtPosInt); //editor without FX
	
		l_display = true;

		if (myProcessor->m_pVASTXperience.m_Poly.m_OscBank[l_oscillatorBank].getAndClearSoftChangedFlagOscEditor() == false) {
			if (!(force || m_dirty.load()))
				return;
		}
	}
	else {
		wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[l_oscillatorBank].getWavetablePointer();

		if (wavetable->m_isBeingUpdated.load() == true) //safety
			return;

		if (wavetable->getNumPositions() > 0) {
			if (l_oscillatorBank == 0) {
				phase = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fOscPhase_OscA * 0.01f;
				wtPerc = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fOscWTPos_OscA * 0.01f;
				wtfxType = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_uWTFX_OscA;
				wtfxVal = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fWTFXVal_OscA;
			}
			else {
				if (l_oscillatorBank == 1) {
					phase = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fOscPhase_OscB * 0.01f;
					wtPerc = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fOscWTPos_OscB * 0.01f;
					wtfxType = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_uWTFX_OscB;
					wtfxVal = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fWTFXVal_OscB;
				}
				else {
					if (l_oscillatorBank == 2) {
						phase = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fOscPhase_OscC * 0.01f;
						wtPerc = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fOscWTPos_OscC * 0.01f;
						wtfxType = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_uWTFX_OscC;
						wtfxVal = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fWTFXVal_OscC;
					}
					else {
						if (l_oscillatorBank == 3) {
							phase = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fOscPhase_OscD * 0.01f;
							wtPerc = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fOscWTPos_OscD * 0.01f;
							wtfxType = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_uWTFX_OscD;
							wtfxVal = *myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_fWTFXVal_OscD;
						}
					}
				}
			}
		}

		//knobs tweaked but not playing?
		if (myProcessor->m_pVASTXperience.m_Poly.isVoicePlaying(note)) {
			//played again after reset
			if (!m_bLast_update_was_with_voice_playing)
				m_dirty.store(true);

			if (m_safeWtPosFloat.load() != myProcessor->m_pVASTXperience.m_Poly.m_currentWTPosFloatPercentage[l_oscillatorBank][note].load() * (wavetable->getNumPositions() - 1))
                m_dirty.store(true);
		}
		else {
			if (!l_soloMode) {
				if (m_safeWtPosFloat != wtPerc * (wavetable->getNumPositions() - 1))
                    m_dirty.store(true);
			}
			else {
				if (m_safeWtPosFloat != wavetable->getMultiSelectBegin() + wtPerc * (wavetable->getMultiSelectEnd() - wavetable->getMultiSelectBegin()))
                    m_dirty.store(true);
			}
		}

		if (phase != m_safePhaseFloat)
			force = true;
		if (wtfxType != m_safeWTFXType)
			force = true;
		if (wtfxVal != m_safeWTFXVal)
			force = true;
		m_safeWTFXType = wtfxType;
		m_safeWTFXVal = wtfxVal;

		//check for buffer refresh
		if (myProcessor->m_pVASTXperience.m_Poly.m_OscBank[l_oscillatorBank].getAndClearSoftChangedFlagStructure())
			m_hasWaveformImagePerspectiveBuffer = false;

		//check if dirty
		if ((myProcessor->m_pVASTXperience.m_Poly.m_OscBank[l_oscillatorBank].getAndClearSoftChangedFlagOsc() == false)) {
			if (!(force || m_dirty.load())) {
				return;
			}
		}

		if (wavetable->getNumPositions() > 0) {
			m_screenWidthScale = float(getScreenBounds().getWidth()) / float(getWidth());
			float scale = Desktop::getInstance().getGlobalScaleFactor() * m_screenWidthScale;
			if (l_oscillatorBank == 0) {
				if ((*myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_iNumOscs_OscA > 0) && (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bOscOnOff_OscA == static_cast<int>(SWITCH::SWITCH_ON))) {
					l_display = true;
				}
				else {
					waveformImage = waveformImageBufferOff.rescaled(jmax(1, int(getScreenBounds().getWidth() * scale)), jmax(1, int(getScreenBounds().getHeight() * scale)));
					waveformImage.duplicateIfShared();
					exitWithRepaint = true;
				}
			}
			else {
				if (l_oscillatorBank == 1) {
					if ((*myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_iNumOscs_OscB > 0) && (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bOscOnOff_OscB == static_cast<int>(SWITCH::SWITCH_ON))) {
						l_display = true;
					}
					else {
						waveformImage = waveformImageBufferOff.rescaled(jmax(1, int(getScreenBounds().getWidth() * scale)), jmax(1, int(getScreenBounds().getHeight() * scale)));
						waveformImage.duplicateIfShared();
						exitWithRepaint = true;
					}
				}
				else {
					if (l_oscillatorBank == 2) {
						if ((*myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_iNumOscs_OscC > 0) && (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bOscOnOff_OscC == static_cast<int>(SWITCH::SWITCH_ON))) {
							l_display = true;
						}
						else {
							waveformImage = waveformImageBufferOff.rescaled(jmax(1, int(getScreenBounds().getWidth() * scale)), jmax(1, int(getScreenBounds().getHeight() * scale)));
							waveformImage.duplicateIfShared();
							exitWithRepaint = true;
						}
					}
					else {
						if (l_oscillatorBank == 3) {
							if ((*myProcessor->m_pVASTXperience.m_Poly.m_Set->m_State->m_iNumOscs_OscD > 0) && (*myProcessor->m_pVASTXperience.m_Set.m_State->m_bOscOnOff_OscD == static_cast<int>(SWITCH::SWITCH_ON))) {
								l_display = true;
							}
							else {
								waveformImage = waveformImageBufferOff.rescaled(jmax(1, int(getScreenBounds().getWidth() * scale)), jmax(1, int(getScreenBounds().getHeight() * scale)));
								waveformImage.duplicateIfShared();
								exitWithRepaint = true;
							}
						}
					}
				}
			}
			if (myProcessor->m_pVASTXperience.m_Poly.isVoicePlaying(note)) {
				m_last_active_voice = note;
				m_last_active_counter = C_MAX_UI_STEPS_AFTER_NOTEOFF;
			}
			else {
				m_last_active_counter--;
				if (m_last_active_counter < 0) {
					m_last_active_voice = -1;
					m_last_active_counter = C_MAX_UI_STEPS_AFTER_NOTEOFF - 1; 
				}
			}

			//if (m_last_active_voice != -1) {
			if (m_last_active_counter == C_MAX_UI_STEPS_AFTER_NOTEOFF) {
				if (!l_soloMode)
					m_safeWtPosFloat = myProcessor->m_pVASTXperience.m_Poly.m_currentWTPosFloatPercentage[l_oscillatorBank][note].load() * (wavetable->getNumPositions() - 1);
				else
					m_safeWtPosFloat = wavetable->getMultiSelectBegin() + myProcessor->m_pVASTXperience.m_Poly.m_currentWTPosFloatPercentage[l_oscillatorBank][note].load() * (wavetable->getMultiSelectEnd() - wavetable->getMultiSelectBegin());
				m_last_active_voice = note;
				m_bLast_update_was_with_voice_playing = true;
				m_safePhaseFloat = myProcessor->m_pVASTXperience.m_Poly.m_safePhaseFloat[l_oscillatorBank][note].load();
				vassert((m_safePhaseFloat >= 0.f) && (m_safePhaseFloat <= 1.f));
			}
			else {				
				if (!l_soloMode)
					m_safeWtPosFloat = wtPerc * (wavetable->getNumPositions() - 1);
				else
					m_safeWtPosFloat = wavetable->getMultiSelectBegin() + wtPerc * (wavetable->getMultiSelectEnd() - wavetable->getMultiSelectBegin());
				m_safePhaseFloat = phase;
				vassert((m_safePhaseFloat >= 0.f) && (m_safePhaseFloat <= 1.f));
				m_bLast_update_was_with_voice_playing = false;
			}

			safeWtPosInt = int(m_safeWtPosFloat);
			//l_waveBuffer = wavetable->getNaiveTable(int(m_safeWtPosFloat));
			
			if (wavetable->getNumPositions() == 1) bPerspectiveDisplay = false;
			if (!bPerspectiveDisplay) //not used in perspective
				l_waveBuffer = wavetable->getNaiveTableWithFXForDisplay(int(m_safeWtPosFloat), wtfxType, wtfxVal, (!m_bLast_update_was_with_voice_playing || (m_last_active_counter != C_MAX_UI_STEPS_AFTER_NOTEOFF)));
		}
	}
    m_dirty.store(true); //prevent image painting
	if (exitWithRepaint) {
		handleBorderDisplay();
		repaint();
		waveformImageLastSafe = waveformImageWithBorder.createCopy();
        m_dirty.store(false);
		return;
	}

    if (wavetable->m_isBeingUpdated.load() == true) //safety
        return;
    
    if (!bPerspectiveDisplay) {
		if (l_waveBuffer != nullptr) {
			if (l_waveBuffer->empty()) {
				l_display = false;
			}
		} else 
			l_display = false;
    } 
	
	if (l_display == false) {
		//repaint();
        m_dirty.store(false);
		return;
	}

	//VDBG("VASTOscilloscope::updateContent - " + l_oscbank);

	m_screenWidthScale = float(getScreenBounds().getWidth()) / float(getWidth());
	m_screenHeightScale = float(getScreenBounds().getHeight()) / float(getHeight());

	//float scale = Desktop::getInstance().getGlobalScaleFactor() * myProcessor->getPluginScaleWidthFactor();
	float scale = Desktop::getInstance().getGlobalScaleFactor() * m_screenWidthScale;
	
	waveformImage = Image(Image::RGB, jmax(1, getScreenBounds().getWidth()) * scale, jmax(1, getScreenBounds().getHeight()) * scale, false);
	waveformImage.clear(waveformImage.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));			

	waveformImageWithBorder = Image(Image::RGB, jmax(1, getScreenBounds().getWidth()) * scale, jmax(1, getScreenBounds().getHeight()) * scale, false);
	waveformImageWithBorder.clear(waveformImage.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));
	
	Graphics g(waveformImage);
	float fontSize = (waveformImage.getWidth() / 300.f) * 16.f;
	Font myFont = myProcessor->getCurrentVASTLookAndFeel()->getDefaultFont();
	myFont.setSizeAndStyle(fontSize, Font::plain, 1.0f, 0.0f); // no squashing, no kerning
	g.setFont(myFont);

	const float halfHeight = waveformImage.getHeight() * 0.5f;
	
	if (!bPerspectiveDisplay) {
		//grid
		if (myWtEditor != nullptr) {
			if (myProcessor->m_iWTEditorGridMode != OscillatorGridMode::NoGrid) {
				int numGridlines = pow(2, myProcessor->m_iWTEditorGridMode + 1);
				g.setColour(juce::Colour::fromFloatRGBA(0.5f, 0.7f, 0.5f, 0.25f));
				Point<float> p1, p2;
				for (int i = 0; i < numGridlines + 1; i++) {
					if ((i > 0) && (i < numGridlines)) {
						p1 = toScreenCoordinate(Point<float>(i * (waveformImage.getWidth() / float(numGridlines)), 0.f));
						p2 = toScreenCoordinate(Point<float>(i  * (waveformImage.getWidth() / float(numGridlines)), waveformImage.getHeight()));
						g.drawLine(p1.x, p1.y, p2.x, p2.y, 1.f * scale);
					}
					p1 = toScreenCoordinate(Point<float>(0.f, i * (waveformImage.getHeight() / float(numGridlines))));
					p2 = toScreenCoordinate(Point<float>(waveformImage.getWidth(), i * (waveformImage.getHeight() / float(numGridlines))));
					g.drawLine(p1.x, p1.y, p2.x, p2.y, 1.f * scale);
				}
			}
		}

		std::vector<float> l_waveBufferNextCopy;
		bool b_hasNext = false;
		if (safeWtPosInt < (wavetable->getNumPositions() - 1)) { //has next?
			b_hasNext = true;
			l_waveBufferNext = wavetable->getNaiveTableWithFXForDisplay(int(m_safeWtPosFloat) + 1, wtfxType, wtfxVal, (!m_bLast_update_was_with_voice_playing || (m_last_active_counter != C_MAX_UI_STEPS_AFTER_NOTEOFF)));
			l_waveBufferNextCopy = *l_waveBufferNext; //copy it for safety
			if (l_waveBufferNext == nullptr) {
                m_dirty.store(false);
				return;
			}
		}
		float interpol = m_safeWtPosFloat - safeWtPosInt;
		Path myWave;
		Path myArea;
		myArea.startNewSubPath(0, halfHeight);
		Colour c1, c2;
		if (!l_soloMode)
			c1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeArea);
		else
			c1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeAreaSolo);
		//g2 = g1.withAlpha(0.15f);
		c2 = c1.withAlpha(0.1f);
		float halfHeightY = toScreenCoordinate(Point<float>(0, halfHeight)).y;

		float step = float(l_waveBuffer->size()) / float(waveformImage.getWidth());
		if (b_hasNext) {
			if (l_waveBufferNextCopy.size() != C_WAVE_TABLE_SIZE) {
                m_dirty.store(false);
				return;
			}
		}

		for (int x = 0; x < waveformImage.getWidth(); x++) {
			int xWT = int((x * step) + (m_safePhaseFloat * C_WAVE_TABLE_SIZE)) % C_WAVE_TABLE_SIZE;
			float yVal = (*l_waveBuffer)[xWT];
			if (isnan(yVal))
				return;
			if (b_hasNext) { //interpolate
				float samp1 = l_waveBufferNextCopy[xWT];
				//yVal = yVal * (1.0f - interpolquad) + samp1 * interpolquad;
				yVal = yVal * (1.0f - interpol) + samp1 * interpol;
			}

			float mpoint = halfHeight - (halfHeight  * yVal);
			mpoint = toScreenCoordinate(Point<float>(x, mpoint)).y;
			if (x == 0) myWave.startNewSubPath(0, mpoint);
			myWave.lineTo(x, mpoint);		
			myArea.lineTo(x, mpoint);
		}

		// Shade under line
		myArea.lineTo(waveformImage.getWidth(), halfHeightY);
		myArea.closeSubPath();
		ColourGradient cg = ColourGradient(c2, juce::Point<float>(0, waveformImage.getHeight()), c1, juce::Point<float>(0, 0), false);
		cg.clearColours();
		cg.addColour(0.0, c1);
		cg.addColour(0.5, c2);
		cg.addColour(1.0, c1);
		g.setGradientFill(cg);
		g.fillPath(myArea);

		if (!l_soloMode)
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeLine));
		else
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeLineSolo));
		g.strokePath(myWave, PathStrokeType(1.8f * scale)); //was 1.3f
		
		if (myWtEditor == nullptr) { //text not for editor
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPositionViewportText).brighter(0.4f));
			g.drawText(juce::String(safeWtPosInt + 1.f) + "/" + juce::String(wavetable->getNumPositions()), juce::Rectangle<float>(0.f, 0.f, waveformImage.getWidth() - 6.f, waveformImage.getHeight() - 3.f), juce::Justification::bottomRight, false);
		}

		handleBorderDisplay();
	}
	else { //perspective display		
		//**lock
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[l_oscillatorBank].mSharedPtrSoftFadeLock.enter();
		int numWTPos = wavetable->getNumPositions();
		std::vector<float> wtcopy[C_MAX_NUM_POSITIONS];
		for (int pos = 0; pos < numWTPos; pos++)
			wtcopy[pos] = *wavetable->getNaiveTable(pos); //copy

		int wtposfrom = numWTPos - 1;
		int wtposto = 0;
		int numPos = numWTPos;
		if (l_soloMode) {
			wtposfrom = wavetable->getMultiSelectEnd();
			wtposto = wavetable->getMultiSelectBegin();
			numPos = wtposfrom - wtposto + 1;
		}
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[l_oscillatorBank].mSharedPtrSoftFadeLock.exit();
		//**lock

		float xoffset = 0.f;
		float yoffset = 0.f;
		float xOffsetCur = 0.f;
		float yOffsetCur = 0.f;
		float yscale = 0.2f;
		float widthreduction = waveformImage.getWidth() / 5.f;
		float reducedwidth = waveformImage.getWidth() - widthreduction;

		if (numPos <= 1)
			reducedwidth = waveformImage.getWidth();

		float step = float(C_WAVE_TABLE_SIZE) / float(reducedwidth);

		Path myCurrentWtposWave;
		float lastyoffset = -99999999.f;
		int lastwtposint = C_MAX_NUM_POSITIONS;
		float interpol = m_safeWtPosFloat - safeWtPosInt;

		if (waveformImagePerspectiveBuffer.isNull() || !m_hasWaveformImagePerspectiveBuffer) {

			for (int wtpos = wtposfrom; wtpos >= wtposto; wtpos--) {
				Path myWave;
				Colour g1, g2;
				g1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeArea);
				g2 = g1.withAlpha(0.15f);

				if (numPos > 1) {
					xoffset = widthreduction / (float(numPos) - 1.f) * float(wtpos - wtposto); //moving to right for higher wtpos
					yoffset = -(halfHeight * 1.2f) / (float(numPos) - 1.f) * float(wtpos - wtposto) + (halfHeight - halfHeight * yscale * 2.f); //moving up for higher wtpos
				}

				if ((abs(yoffset - lastyoffset) > 4.f) || ((wtpos - wtposto) == 0)) //only draw if at least 2 pixel away // was 2.f
				{
					for (int x = 0; x < reducedwidth; x++) {
						int xWT = int((x * step) + (m_safePhaseFloat * C_WAVE_TABLE_SIZE)) % C_WAVE_TABLE_SIZE;
						float yVal = wtcopy[wtpos][xWT];
						///*TESTING*/ if (!(approximatelyEqual(yVal, 0.f)))
							///*TESTING*/		lAllWTPosZero = false;

						float halfHeightY = toScreenCoordinate(Point<float>(0 + xoffset, halfHeight + yoffset)).y;
						float mpoint = halfHeightY - (halfHeight  * yVal * yscale);
						mpoint = toScreenCoordinate(Point<float>(x + xoffset, mpoint)).y;
						if (x == 0)
							myWave.startNewSubPath(xoffset, mpoint);
						myWave.lineTo((x + xoffset), mpoint);
					}

					if ((wtpos <= safeWtPosInt) && (lastwtposint > safeWtPosInt)) {
						xOffsetCur = xoffset;
						yOffsetCur = yoffset;
					}
					lastyoffset = yoffset;
					lastwtposint = wtpos;

					if (!l_soloMode)
						g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeArea).withMultipliedAlpha(0.3f));
					else
						g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeAreaSolo).withMultipliedAlpha(0.3f));

					g.strokePath(myWave, PathStrokeType(1.8f * scale)); //was 1.3f
				}
			}
			m_hasWaveformImagePerspectiveBuffer = true;
			waveformImagePerspectiveBuffer = waveformImage.createCopy();
		}
		else {
			//float w = waveformImagePerspectiveBuffer.getWidth();
			//float h = waveformImagePerspectiveBuffer.getHeight();
			//int l_phaseoffset = (1.f - m_safePhaseFloat) * reducedwidth;
			g.drawImage(waveformImagePerspectiveBuffer, 0, 0, waveformImagePerspectiveBuffer.getWidth(), waveformImagePerspectiveBuffer.getHeight(), 0, 0, waveformImagePerspectiveBuffer.getWidth(), waveformImagePerspectiveBuffer.getHeight());
			//if (l_phaseoffset != 0) {
				//g.drawImage(waveformImagePerspectiveBuffer, l_phaseoffset, 0, reducedwidth - l_phaseoffset, h, 0, 0, reducedwidth - l_phaseoffset, h);
			//}
		}
		
		int voice = myProcessor->m_pVASTXperience.m_Set.m_uMaxPoly - 1;
		bool drawn = false;
		while (voice >= -1) { //voice >= -1??
			float lwtPos = m_safeWtPosFloat - wtposto; //wtposto for solomode
            if (voice >= 0) {
                lwtPos = myProcessor->m_pVASTXperience.m_Poly.m_currentWTPosFloatPercentage[l_oscillatorBank][voice].load() * (numPos - 1);
            }
			if ((voice == -1) || (myProcessor->m_pVASTXperience.m_Poly.isVoicePlaying(voice))) { //-1 is always drawn
				if (voice > -1)
					drawn = true;
				else
					if (drawn)
						break; //dont draw in that case
				myCurrentWtposWave.clear();
				if (numPos > 1) {
					xoffset = widthreduction / (float(numPos) - 1.f) * lwtPos; //moving to right for higher wtpos
					yoffset = -(halfHeight * 1.2f) / (float(numPos) - 1.f) * lwtPos + (halfHeight - halfHeight * yscale * 2.f); //moving up for higher wtpos
					//xoffset = widthreduction / (float(numPos) - 1.f) * m_safeWtPosFloat; //moving to right for higher wtpos
					//yoffset = -(halfHeight * 1.2f) / (float(numPos) - 1.f) * m_safeWtPosFloat + (halfHeight - halfHeight * yscale * 2.f); //moving up for higher wtpos
				}
				else {
					xoffset = 0.f;
					yoffset = 0.f;
				}
				for (int x = 0; x < reducedwidth; x++) {
					int xWT = int((x * step) + (m_safePhaseFloat * C_WAVE_TABLE_SIZE)) % C_WAVE_TABLE_SIZE;
					//float yVal = wtcopy[safeWtPosInt][xWT];
					float yVal = wtcopy[int(lwtPos) + wtposto][xWT]; //wtposto for solomode
					//if (lwtPos < numWTPos - 1) { //interpolate
					if (lwtPos < numPos - 1) { //interpolate
						//float samp1 = wtcopy[safeWtPosInt + 1][xWT];
						float samp1 = wtcopy[int(lwtPos) + wtposto + 1][xWT]; //wtposto for solomode
						yVal = yVal * (1.0f - interpol) + samp1 * interpol;
					}

					float halfHeightY = toScreenCoordinate(Point<float>(0 + xoffset, halfHeight + yoffset)).y;
					float mpoint = halfHeightY - (halfHeight  * yVal * yscale);
					mpoint = toScreenCoordinate(Point<float>(x + xoffset, mpoint)).y;
					if (x == 0) myCurrentWtposWave.startNewSubPath(xoffset, mpoint);
					myCurrentWtposWave.lineTo((x + xoffset), mpoint);
				}

				if (!l_soloMode)
					if (voice == -1)
						g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeLine));
					else 
						g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeLine).contrasting(voice / float(myProcessor->m_pVASTXperience.m_Set.m_uMaxPoly)).darker(voice / float(myProcessor->m_pVASTXperience.m_Set.m_uMaxPoly)));
				else {
					g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeLineSolo));
				}
				g.strokePath(myCurrentWtposWave, PathStrokeType(1.8f * m_screenWidthScale * myProcessor->getPluginScaleWidthFactor()));
			}
			voice--;
		}

		g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPositionViewportText).brighter(0.4f));
		g.drawText(juce::String(safeWtPosInt + 1.f) + "/" + juce::String(numWTPos), juce::Rectangle<float>(0.f, 0.f, waveformImage.getWidth() - 6.f, waveformImage.getHeight() - 3.f), juce::Justification::bottomRight, false);

		handleBorderDisplay();
	}

	waveformImageLastSafe = waveformImageWithBorder.createCopy();

    m_dirty.store(false);
	m_newImageForRepaint = true;
	repaint();
}

juce::Point<float> VASTOscilloscope::toScreenCoordinate(Point<float> inCoord) {
	return juce::Point<float>(inCoord.getX(), 0.02f * getHeight() + inCoord.getY() * 0.96f);
}


juce::Point<float> VASTOscilloscope::toSnapToGridCoordinate(Point<float> inCoord) {
	return juce::Point<float>(inCoord.getX(), 0.02f * getHeight() + inCoord.getY() * 0.96f);
}

juce::Point<float>VASTOscilloscope::fromScreenCoordinate(Point<float> inCoord) {
	Point<float> p = juce::Point<float>(inCoord.getX(), (inCoord.getY() - 0.02f * getHeight()) / 0.96f);
	return p;
}

void VASTOscilloscope::startAutoUpdate(bool isEditor) {
	if (isEditor)
		startTimer(120);
	else 
		startTimer(80);     // orig value: repaint every 50 milliseconds
							//startTimer(400);     // repaint every 0.4 seconds
}

void VASTOscilloscope::stopAutoUpdate() {
	stopTimer();
}

void VASTOscilloscope::timerCallback() {
	updateContent(false);
}

void VASTOscilloscope::mouseDrag(const MouseEvent &e) { // show value
	if (myWtEditor == nullptr) return;
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	int numGridlines = pow(2, myProcessor->m_iWTEditorGridMode + 1);
	float stepx = (static_cast<float>(C_WAVE_TABLE_SIZE) / numGridlines);
	float stepy = (1.0f / numGridlines);
	if (myProcessor->m_iWTEditorDrawMode == OscillatorEditMode::SelectMode) {
		if (modifiers.isShiftDown()) { //adjust selection
			int pos = e.getMouseDownX() + e.getDistanceFromDragStartX();
			int midpoint = (m_selection.iScreenSelectionEnd + m_selection.iScreenSelectionStart) * 0.5f;
			if (pos > midpoint)
				m_selection.iScreenSelectionEnd = e.getMouseDownX() + e.getDistanceFromDragStartX();
			else
				m_selection.iScreenSelectionStart = e.getMouseDownX() + e.getDistanceFromDragStartX();
			if (m_selection.iScreenSelectionStart < 0) m_selection.iScreenSelectionStart = 0;
			if (m_selection.iScreenSelectionEnd > getWidth() - 1) m_selection.iScreenSelectionEnd = getWidth() - 1;
		}
		else { //normal selection
			int start = e.getMouseDownX();
			int end = start + e.getDistanceFromDragStartX();
			if (end < 0) end = 0;
			if (end > getWidth()) end = getWidth();
			if (start < end) {
				m_selection.iScreenSelectionStart = start;
				m_selection.iScreenSelectionEnd = end;
			}
			else {
				m_selection.iScreenSelectionStart = end;
				m_selection.iScreenSelectionEnd = start;
			}
		}

		m_selection.iWTSelectionStart = (float(m_selection.iScreenSelectionStart) / float(getWidth())) * (C_WAVE_TABLE_SIZE - 1);
		m_selection.iWTSelectionEnd = (float(m_selection.iScreenSelectionEnd) / float(getWidth())) * (C_WAVE_TABLE_SIZE - 1);
		if (m_selection.iWTSelectionStart < 0) m_selection.iWTSelectionEnd = 0;
		if (m_selection.iWTSelectionEnd >= C_WAVE_TABLE_SIZE) m_selection.iWTSelectionEnd = C_WAVE_TABLE_SIZE - 1;
		
		if (myProcessor->m_iWTEditorGridMode != OscillatorGridMode::NoGrid) {
			//snap
			m_selection.iWTSelectionStart = int(float(m_selection.iWTSelectionStart) / stepx + 0.5f)  * stepx;
			if (m_selection.iWTSelectionStart <= 0) m_selection.iWTSelectionStart = 0;
			m_selection.iWTSelectionEnd = int(float(m_selection.iWTSelectionEnd) / stepx + 0.5f)  * stepx;
			if (m_selection.iWTSelectionEnd > C_WAVE_TABLE_SIZE - 1) m_selection.iWTSelectionEnd = C_WAVE_TABLE_SIZE - 1;
		}

		//updateContent(true);
		notifySelectionChanged();
		if (waveformImageWithBorder.isNull())
			waveformImageWithBorder = waveformImage.createCopy();
		Graphics g(waveformImageWithBorder);
		g.drawImageAt(waveformImage, 0, 0);
		handleBorderDisplay();		
		repaint(); //paints waveformimage		
	}
	else if (myProcessor->m_iWTEditorDrawMode == OscillatorEditMode::DrawMode) {
		bool right = false;
		if (modifiers.isPopupMenu()) {
			right = true;
		}
		int start = e.getMouseDownX();
		int starty = e.getMouseDownY();
		int endy = starty + e.getDistanceFromDragStartY();
		if (endy > getHeight()) endy = getHeight();
		if (endy < fromScreenCoordinate(Point<float>(0.0f, 0.0f)).getY())
			endy = toScreenCoordinate(Point<float>(0.0f, 0.0f)).getY();
		int end = start + e.getDistanceFromDragStartX();
		if (end < fromScreenCoordinate(Point<float>(0.0f, 0.0f)).getX())
			end = toScreenCoordinate(Point<float>(0.0f, 0.0f)).getX();
		if (end > getWidth()) end = getWidth();
		int ScreenSelectionStart = 0;
		int ScreenSelectionEnd = 0;
		float curvy = 0.f;

		if (!right) {
			if (start < end) {
				ScreenSelectionStart = start;
				ScreenSelectionEnd = end;
				m_val1 = ((1 - (fromScreenCoordinate(Point<float>(start, starty)).getY() / float(getHeight()))) * 2.f) - 1;
				m_val2 = ((1 - (fromScreenCoordinate(Point<float>(start, endy)).getY() / float(getHeight()))) * 2.f) - 1;
			}
			else {
				ScreenSelectionStart = end;
				ScreenSelectionEnd = start;
				m_val1 = ((1 - (fromScreenCoordinate(Point<float>(start, endy)).getY() / float(getHeight()))) * 2.f) - 1;
				m_val2 = ((1 - (fromScreenCoordinate(Point<float>(start, starty)).getY() / float(getHeight()))) * 2.f) - 1;
			}

			m_wtselstart = (float(ScreenSelectionStart) / float(getWidth())) * (C_WAVE_TABLE_SIZE - 1);
			m_wtselend = (float(ScreenSelectionEnd) / float(getWidth())) * (C_WAVE_TABLE_SIZE - 1);
			if (myProcessor->m_iWTEditorGridMode != OscillatorGridMode::NoGrid) {
				m_wtselstart = int(float(m_wtselstart) / stepx + 0.5f)  * stepx;
				m_wtselend = int(float(m_wtselend) / stepx + 0.5f)  * stepx;
				//VDBG("start: " + String(m_wtselstart) + " end: " + String(m_wtselend) + " " + String(ScreenSelectionStart) + " " + String(ScreenSelectionEnd));
				if (m_wtselend == m_wtselstart) {
					std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getCurWavetable();
					if (start < end) {
						if (m_wtselend < C_WAVE_TABLE_SIZE - stepx) {
							m_wtselend += stepx;
							m_val1 = m_val2;
							m_val2 = (*wavetable->getNaiveTable(myWtEditor->getWtPos()))[m_wtselend];
						}
					}
					else {
						if (m_wtselstart > stepx) {
							m_wtselstart -= stepx;
							m_val2 = m_val1;
							m_val1 = (*wavetable->getNaiveTable(myWtEditor->getWtPos()))[m_wtselstart];
						}
					}
				}
				m_val1 = (int(((m_val1 + 1.f) * 0.5f) / stepy + 0.5f) * stepy) * 2.f - 1.f;
				m_val2 = (int(((m_val2 + 1.f) * 0.5f) / stepy + 0.5f) * stepy) * 2.f - 1.f;
			}
		}
		else {
			if (m_rightDownY < 0) 
				m_rightDownY = e.getPosition().y;
			if (m_val2 > m_val1) { //rising edge
				curvy = -float((m_rightDownY - e.getPosition().y) * 4.f) / float(getHeight()) * 100.f; //4 is arbitrary accelerator
			}
			else { //falling edge
				curvy = +float((m_rightDownY - e.getPosition().y) * 4.f) / float(getHeight()) * 100.f; //4 is arbitrary accelerator
			}
			curvy = (curvy > 100.f) ? 100.f : curvy;
			curvy = (curvy < -100.f) ? -100.f : curvy;
		}
		m_wtselstart = (m_wtselstart  >(C_WAVE_TABLE_SIZE - 1)) ? C_WAVE_TABLE_SIZE - 1 : m_wtselstart;
		m_wtselstart = m_wtselstart  < 0 ? 0 : m_wtselstart;
		m_wtselend = (m_wtselend >(C_WAVE_TABLE_SIZE - 1)) ? C_WAVE_TABLE_SIZE - 1 : m_wtselend;
		m_wtselend = m_wtselend < 0 ? 0 : m_wtselend;

		std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::DrawConnect, curvy, /*myWtEditor->getWtPos(), myWtEditor->getWtPos(),*/ m_wtselstart, m_wtselend, myWtEditor, m_val1, m_val2, nullptr, true); //connect value 0?
		edit_thread.detach();
	}
	else { //draw mode free
		int start = e.getMouseDownX();
		int starty = e.getMouseDownY();
		int endy = starty + e.getDistanceFromDragStartY();
		int end = start + e.getDistanceFromDragStartX();
		if (end < 0) end = 0;
		if (end > getWidth()) end = getWidth();
	
		Point<float> lMouseDown((float(start) / float(getWidth())) * (C_WAVE_TABLE_SIZE - 1), ((1 - (fromScreenCoordinate(Point<float>(start, starty)).getY() / float(getHeight()))) * 2.f) - 1); //mouseDownPoint to wavetable
		m_freeHandBuffer.addIfNotAlreadyThere(lMouseDown);

		Point<float> lDragTo((float(end) / float(getWidth())) * (C_WAVE_TABLE_SIZE - 1) , ((1 - (fromScreenCoordinate(Point<float>(end, endy)).getY() / float(getHeight()))) * 2.f) - 1); //dragtopoint to wavetable
		m_freeHandBuffer.add(lDragTo);

		std::thread edit_thread(&VASTWaveTableEditorComponent::threadedFreehandDraw, /*myWtEditor->getWtPos(), myWtEditor->getWtPos(),*/ myWtEditor, m_freeHandBuffer);
		edit_thread.detach();		
	}
}


void VASTOscilloscope::mouseDoubleClick(const MouseEvent&) {
	if (myWtEditor == nullptr) {
		if (myEditor != nullptr) {			
			auto* cTabbedComponent = myEditor->vaporizerComponent->getTabbedComponent();
			if (cTabbedComponent != nullptr) {
				cTabbedComponent->setCurrentTabIndex(0, true); //editor tab on double click
			}
		}
	}
	else {
		selectAll(false);
		notifySelectionChanged();
	}
}

void VASTOscilloscope::selectAll(bool noUIUpdate) {
	if (myWtEditor == nullptr) return;
	m_selection.iScreenSelectionStart = 0;
	m_selection.iScreenSelectionEnd = getWidth();
	m_selection.iWTSelectionStart = (float(m_selection.iScreenSelectionStart) / float(getWidth())) * (C_WAVE_TABLE_SIZE - 1);
	m_selection.iWTSelectionEnd = (float(m_selection.iScreenSelectionEnd) / float(getWidth())) * (C_WAVE_TABLE_SIZE - 1);
	if (!noUIUpdate) {
		updateContent(true);
		notifySelectionChanged();
	}
}

void VASTOscilloscope::notifySelectionChanged() {
	jassert(m_selection.iWTSelectionStart >= 0);
	jassert(m_selection.iWTSelectionEnd <= C_WAVE_TABLE_SIZE - 1);
	if (myWtEditor != nullptr) {
		myWtEditor->notifySelectionChanged();
	}
}

bool VASTOscilloscope::isInterestedInFileDrag(const StringArray& files) {
	if (myWtEditor == nullptr) 
		return false;
	bool l_interested = false;
	for (int i = 0; i < files.size(); i++) {
		if (files[i].endsWithIgnoreCase(".wav")) 
			l_interested = true;
		if (files[i].endsWithIgnoreCase(".aif")) 
			l_interested = true;
		if (files[i].endsWithIgnoreCase(".flac")) 
			l_interested = true;
		if (files[i].endsWithIgnoreCase(".mp3")) 
			l_interested = true;
	}
	//if (l_interested)
		//VDBG("Interested in drag and drop");
	return l_interested;
};

void VASTOscilloscope::filesDropped(const StringArray& files, int , int ) {
	if (myWtEditor == nullptr) return;
	myWtEditor->loadWTFile(files[0]); //only one
}

/*
void VASTOscilloscope::fileDragExit(const StringArray& files) {
	//filesDropped(files, 0, 0);
}
*/

void VASTOscilloscope::mouseUp(const MouseEvent &) {
	m_rightDownY = -1;
	if (myWtEditor == nullptr) return;
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	if ((modifiers.isLeftButtonDown() == false) && (modifiers.isPopupMenu() == false))
		m_isBeingDragged = false;
}

void VASTOscilloscope::mouseWheelMove(const MouseEvent &, const MouseWheelDetails &wheel) {
	if (myWtEditor == nullptr) return;
	if (myProcessor->m_iWTEditorDrawMode != OscillatorEditMode::SelectMode) return;
	bool lExtend = (wheel.deltaY > 0.f);

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	int numGridlines = pow(2, myProcessor->m_iWTEditorGridMode + 1);
	float stepx = (static_cast<float>(C_WAVE_TABLE_SIZE) / numGridlines);

	myWtEditor->copySelectionToLocalBuffer();

	if (modifiers.isShiftDown()) { //adjust selection
		if (myProcessor->m_iWTEditorGridMode != OscillatorGridMode::NoGrid) {
			//snap
			m_selection.iWTSelectionStart = int(float(m_selection.iWTSelectionStart) / stepx + 0.5f)  * stepx;
			m_selection.iWTSelectionEnd = int(float(m_selection.iWTSelectionEnd) / stepx + 0.5f)  * stepx;
			if (m_selection.iWTSelectionEnd >= m_selection.iWTSelectionStart) {
				if (lExtend) {
					m_selection.iWTSelectionStart += stepx;
					m_selection.iWTSelectionEnd += stepx;
				}
				else {
					m_selection.iWTSelectionStart -= stepx;
					m_selection.iWTSelectionEnd -= stepx;
				}
			}
		}
		else {
			if (m_selection.iWTSelectionEnd >= m_selection.iWTSelectionStart) {
				if (lExtend) {
					m_selection.iWTSelectionStart += 1;
					m_selection.iWTSelectionEnd += 1;
				}
				else {
					m_selection.iWTSelectionStart -= 1;
					m_selection.iWTSelectionEnd -= 1;
				}
			}
		}
	}
	else { //normal selection
		if (myProcessor->m_iWTEditorGridMode != OscillatorGridMode::NoGrid) {
			//snap
			m_selection.iWTSelectionStart = int(float(m_selection.iWTSelectionStart) / stepx + 0.5f)  * stepx;
			m_selection.iWTSelectionEnd = int(float(m_selection.iWTSelectionEnd) / stepx + 0.5f)  * stepx;
			if (m_selection.iWTSelectionEnd >= m_selection.iWTSelectionStart) {
				if (lExtend) {
					m_selection.iWTSelectionStart -= stepx;
					m_selection.iWTSelectionEnd += stepx;
				}
				else {
					m_selection.iWTSelectionStart += stepx;
					m_selection.iWTSelectionEnd -= stepx;
				}
			}	
		}
		else {
			std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getCurWavetable();
			if (lExtend) {
				while (m_selection.iWTSelectionStart > 0) {					
					if ((*wavetable->getNaiveTable(myWtEditor->getWtPos()))[m_selection.iWTSelectionStart] * (*wavetable->getNaiveTable(myWtEditor->getWtPos()))[static_cast<std::vector<float, std::allocator<float>>::size_type>(m_selection.iWTSelectionStart) - 1] > 0.f)
						m_selection.iWTSelectionStart--;
					else {
						m_selection.iWTSelectionStart--;
						break;
					}
				}
				while (m_selection.iWTSelectionEnd < C_WAVE_TABLE_SIZE - 1) {
					if ((*wavetable->getNaiveTable(myWtEditor->getWtPos()))[m_selection.iWTSelectionEnd] * (*wavetable->getNaiveTable(myWtEditor->getWtPos()))[static_cast<std::vector<float, std::allocator<float>>::size_type>(m_selection.iWTSelectionEnd) + 1] > 0.f)
						m_selection.iWTSelectionEnd++;
					else {
						m_selection.iWTSelectionEnd++;
						break;
					}
				}
			}
			else {
				while (m_selection.iWTSelectionStart < m_selection.iWTSelectionEnd) {
					if ((*wavetable->getNaiveTable(myWtEditor->getWtPos()))[m_selection.iWTSelectionStart] * (*wavetable->getNaiveTable(myWtEditor->getWtPos()))[static_cast<std::vector<float, std::allocator<float>>::size_type>(m_selection.iWTSelectionStart) + 1] > 0.f)
						m_selection.iWTSelectionStart++;
					else {
						m_selection.iWTSelectionStart++;
						break;
					}
				}
				while (m_selection.iWTSelectionEnd > m_selection.iWTSelectionStart) {
					if ((*wavetable->getNaiveTable(myWtEditor->getWtPos()))[m_selection.iWTSelectionEnd] * (*wavetable->getNaiveTable(myWtEditor->getWtPos()))[static_cast<std::vector<float, std::allocator<float>>::size_type>(m_selection.iWTSelectionEnd) - 1] > 0.f)
						m_selection.iWTSelectionEnd--;
					else {
						m_selection.iWTSelectionEnd--;
						break;
					}
				}
			}
		}
	}

	if (m_selection.iWTSelectionStart > m_selection.iWTSelectionEnd)
		m_selection.iWTSelectionEnd = m_selection.iWTSelectionStart;
	if (m_selection.iWTSelectionStart <= 0) m_selection.iWTSelectionStart = 0;
	if (m_selection.iWTSelectionEnd <= 0) m_selection.iWTSelectionEnd = 0;
	if (m_selection.iWTSelectionEnd > C_WAVE_TABLE_SIZE - 1) m_selection.iWTSelectionEnd = C_WAVE_TABLE_SIZE - 1;
	if (m_selection.iWTSelectionStart > C_WAVE_TABLE_SIZE - 1) m_selection.iWTSelectionStart = C_WAVE_TABLE_SIZE - 1;

	//updateContent(true);
	notifySelectionChanged();
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();
	//Graphics g(waveformImageWithBorder);
	//g.drawImageAt(waveformImage, 0, 0);
	handleBorderDisplay();
	repaint(); //paints waveformimage		
}

void VASTOscilloscope::handleBorderDisplay() {
	if ((getWidth() <= 1) || (getHeight() <= 1)) return;

	if (myProcessor == nullptr)
		return;
	if (myProcessor->initNotCompleted())
		return;
	if (myProcessor->nonThreadsafeIsBlockedProcessingInfo())
		return;

	int l_oscillatorBank = 0;

	String l_oscbank = getParentComponent()->getComponentID();
	//int bank = 0;
	if (l_oscbank == "OscA") {
		l_oscillatorBank = 0;
	}
	else if (l_oscbank == "OscB") {
		l_oscillatorBank = 1;
	}
	else if (l_oscbank == "OscC") {
		l_oscillatorBank = 2;
	}
	else if (l_oscbank == "OscD") {
		l_oscillatorBank = 3;
	}
	else if (l_oscbank == "WTEditorView") {
		if (myWtEditor != NULL) {
			l_oscillatorBank = myWtEditor->getOscBank();
		}
	}
	if (waveformImage.isNull())
		return;
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();
	Graphics g(waveformImageWithBorder);	
	g.drawImageAt(waveformImage, 0, 0);

	bool editorSelectedOscBank = false;
	float scale = Desktop::getInstance().getGlobalScaleFactor() * m_screenWidthScale;
	if ((myWtEditor == NULL) && (myEditor != nullptr) && (myEditor->vaporizerComponent != nullptr) && (myEditor->vaporizerComponent->getWaveTableEditorComponent() != nullptr)) {
		if (l_oscillatorBank == myEditor->vaporizerComponent->getWaveTableEditorComponent()->getOscBank())
			editorSelectedOscBank = true;
	}
	if (editorSelectedOscBank) {
        if (myEditor!=nullptr)
            g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeSelection));
	}
	else if (m_bMouseover) {
        if (myEditor!=nullptr)
            g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeSelection).darker(0.6f));
	}
	else {
        if (myEditor!=nullptr)
            g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeSelection).darker(1.0f));
	}
	g.drawRect(0.f, 0.f, float(waveformImage.getWidth() - 1), float(waveformImage.getHeight() - 1), 1.f * scale);

	if (l_oscbank == "WTEditorView") {
		if ((getHeight() > 0.f) && (getWidth() > 0.f)) {
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeSelection));
			float start = float(m_selection.iWTSelectionStart) / float(C_WAVE_TABLE_SIZE - 1) * float(waveformImage.getWidth() - 1);
			float end = float(m_selection.iWTSelectionEnd) / float(C_WAVE_TABLE_SIZE - 1) * float(waveformImage.getWidth() - 1);
			g.drawRect(start, 0.f, end - start, float(waveformImage.getHeight() - 1), 1.f * scale);
		}
	}

	m_newImageForRepaint = true;
	//VDBG("handleBorderDisplay " + l_oscbank + " " + String(editorSelectedOscBank == true ? "true" : "false") + " " + String(m_bMouseover == true ? "true" : "false"));
}

void VASTOscilloscope::mouseDown(const MouseEvent &) {
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	if (myWtEditor == nullptr) {
		//click on bank osci
		int l_oscillatorBank = 0;
		VASTWaveTableEditorComponent* _wteditor = myEditor->vaporizerComponent->getWaveTableEditorComponent();
		if (_wteditor != nullptr) {
			String l_oscbank = getParentComponent()->getComponentID();
			//int bank = 0;
			if (l_oscbank == "OscA") {
				_wteditor->getEditorView()->getHeader()->getOscAButton()->setToggleState(true, juce::NotificationType::sendNotificationSync);
				l_oscillatorBank = 0;
			}
			else if (l_oscbank == "OscB") {
				_wteditor->getEditorView()->getHeader()->getOscBButton()->setToggleState(true, juce::NotificationType::sendNotificationSync);
				l_oscillatorBank = 1;
			}
			else if (l_oscbank == "OscC") {
				_wteditor->getEditorView()->getHeader()->getOscCButton()->setToggleState(true, juce::NotificationType::sendNotificationSync);
				l_oscillatorBank = 2;
			}
			else if (l_oscbank == "OscD") {
				_wteditor->getEditorView()->getHeader()->getOscDButton()->setToggleState(true, juce::NotificationType::sendNotificationSync);
				l_oscillatorBank = 3;
			}
		}

		if (modifiers.isPopupMenu())
			myProcessor->togglePerspectiveDisplay(l_oscillatorBank);
		updateContent(true);
		handleBorderDisplay();
		repaint();
		return;
	}

	if ((!m_isBeingDragged) && (myWtEditor != nullptr)) {
		myWtEditor->copySelectionToLocalBuffer();
		m_freeHandBuffer.clear();
	}

	if (modifiers.isPopupMenu()) { //context menu
		std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();

		PopupMenu mainMenu;
		mainMenu.setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
		PopupMenu subMenuEdit;
		String sedit = TRANS("Edit mode ");
		if (myProcessor->m_iWTEditorDrawMode == OscillatorEditMode::SelectMode)
			sedit = sedit + "(Select mode)";
		else if (myProcessor->m_iWTEditorDrawMode == OscillatorEditMode::DrawMode)
			sedit = sedit + "(Line / arc draw mode)";
		else if (myProcessor->m_iWTEditorDrawMode == OscillatorEditMode::DrawModeFree)
			sedit = sedit + "(Freehand draw mode)";
		mainMenu.addSectionHeader(TRANS("Edit mode"));
		subMenuEdit.addItem(1, TRANS("Select mode"), true, (myProcessor->m_iWTEditorDrawMode == OscillatorEditMode::SelectMode));
		subMenuEdit.addItem(2, TRANS("Line / arc draw mode"), true, (myProcessor->m_iWTEditorDrawMode == OscillatorEditMode::DrawMode));
		subMenuEdit.addItem(3, TRANS("Freehand draw mode"), true, (myProcessor->m_iWTEditorDrawMode == OscillatorEditMode::DrawModeFree));
		mainMenu.addSubMenu(sedit, subMenuEdit, true);

		PopupMenu subMenuGrid;
		String sgrid = TRANS("Grid ");
		mainMenu.addSectionHeader("Grid");
		if (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::NoGrid)
			sgrid = sgrid + "(none)";
		else if (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid4x4)
			sgrid = sgrid + "(4 x 4)";
		else if (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid8x8)
			sgrid = sgrid + "(8 x 8)";
		else if (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid16x16)
			sgrid = sgrid + "(16 x 16)";
		else if (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid32x32)
			sgrid = sgrid + "(32 x 32)";
		else if (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid64x64)
			sgrid = sgrid + "(64 x 64)";
		else if (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid128x128)
			sgrid = sgrid + "(128 x 128)";
		subMenuGrid.addItem(7, TRANS("None"), true, (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::NoGrid));
		subMenuGrid.addItem(8, TRANS("4 x 4"), true, (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid4x4));
		subMenuGrid.addItem(9, TRANS("8 x 8"), true, (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid8x8));
		subMenuGrid.addItem(10, TRANS("16 x 16"), true, (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid16x16));
		subMenuGrid.addItem(11, TRANS("32 x 32"), true, (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid32x32));
		subMenuGrid.addItem(12, TRANS("64 x 64"), true, (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid64x64));
		subMenuGrid.addItem(13, TRANS("128 x 128"), true, (myProcessor->m_iWTEditorGridMode == OscillatorGridMode::Grid128x128));
		mainMenu.addSubMenu(sgrid, subMenuGrid, true);
		
		mainMenu.addSeparator();
		mainMenu.addSectionHeader(TRANS("Copy & Paste"));
		PopupMenu subMenuCopyWavetable;
		subMenuCopyWavetable.addItem(14, TRANS("Copy wavetable to OSC A"), true);
		subMenuCopyWavetable.addItem(15, TRANS("Copy wavetable to OSC B"), true);
		subMenuCopyWavetable.addItem(16, TRANS("Copy wavetable to OSC C"), true);
		subMenuCopyWavetable.addItem(17, TRANS("Copy wavetable to OSC D"), true);
		mainMenu.addSubMenu(TRANS("Copy wavetable"), subMenuCopyWavetable, true);
		PopupMenu subMenuCopyOSC;
		subMenuCopyOSC.addItem(51, TRANS("Copy all OSC settings to OSC A"), true);
		subMenuCopyOSC.addItem(52, TRANS("Copy all OSC settings to OSC B"), true);
		subMenuCopyOSC.addItem(53, TRANS("Copy all OSC settings to OSC C"), true);
		subMenuCopyOSC.addItem(54, TRANS("Copy all OSC settings to OSC D"), true);
		mainMenu.addSubMenu(TRANS("Copy all OSC settings"), subMenuCopyOSC, true);
		PopupMenu subMenuAppendCycle;
		subMenuAppendCycle.addItem(20, TRANS("Append current single cycle position to OSC A"), true);
		subMenuAppendCycle.addItem(21, TRANS("Append current single cycle position to OSC B"), true);
		subMenuAppendCycle.addItem(22, TRANS("Append current single cycle position to OSC C"), true);
		subMenuAppendCycle.addItem(23, TRANS("Append current single cycle position to OSC D"), true);
		mainMenu.addSubMenu(TRANS("Append current single cycle position to OSC A"), subMenuAppendCycle, true);
		mainMenu.addSeparator();
		mainMenu.addSectionHeader(TRANS("Trim & Cut"));
		mainMenu.addItem(35, TRANS("Cut out selection"), true);
		mainMenu.addItem(36, TRANS("Trim to Selection"), true);
		mainMenu.addSeparator();
		mainMenu.addSectionHeader(TRANS("Use MSEG"));
		PopupMenu subMenuUseMSEG;
		subMenuUseMSEG.addItem(40, TRANS("Use MSEG1 as single cycle waveform"), true);
		subMenuUseMSEG.addItem(41, TRANS("Use MSEG2 as single cycle waveform"), true);
		subMenuUseMSEG.addItem(42, TRANS("Use MSEG3 as single cycle waveform"), true);
		subMenuUseMSEG.addItem(43, TRANS("Use MSEG4 as single cycle waveform"), true);
		subMenuUseMSEG.addItem(44, TRANS("Use MSEG5 as single cycle waveform"), true);
		mainMenu.addSubMenu(TRANS("Use MSEG as single cycle waveform"), subMenuUseMSEG, true);

		mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
			juce::ModalCallbackFunction::create([this, wavetable](int result) {

			if (result == 0) {
				// user dismissed the menu without picking anything
			}
			else if (result == 1) {
				myProcessor->m_iWTEditorDrawMode = OscillatorEditMode::SelectMode;
				myWtEditor->updateAll(true);
			}
			else if (result == 2) {
				myProcessor->m_iWTEditorDrawMode = OscillatorEditMode::DrawMode;
				myWtEditor->updateAll(true);
			}
			else if (result == 3) {
				myProcessor->m_iWTEditorDrawMode = OscillatorEditMode::DrawModeFree;
				myWtEditor->updateAll(true);
			}
			else if (result == 7) {
				myProcessor->m_iWTEditorGridMode = OscillatorGridMode::NoGrid;
				myWtEditor->updateAll(true);
			}
			else if (result == 8) {
				myProcessor->m_iWTEditorGridMode = OscillatorGridMode::Grid4x4;
				myWtEditor->updateAll(true);
			}
			else if (result == 9) {
				myProcessor->m_iWTEditorGridMode = OscillatorGridMode::Grid8x8;
				myWtEditor->updateAll(true);
			}
			else if (result == 10) {
				myProcessor->m_iWTEditorGridMode = OscillatorGridMode::Grid16x16;
				myWtEditor->updateAll(true);
			}
			else if (result == 11) {
				myProcessor->m_iWTEditorGridMode = OscillatorGridMode::Grid32x32;
				myWtEditor->updateAll(true);
			}
			else if (result == 12) {
				myProcessor->m_iWTEditorGridMode = OscillatorGridMode::Grid64x64;
				myWtEditor->updateAll(true);
			}
			else if (result == 13) {
				myProcessor->m_iWTEditorGridMode = OscillatorGridMode::Grid128x128;
				myWtEditor->updateAll(true);
			}
			else if (result == 14) {
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[0].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetablecopy = wavetable->getClonedInstance(true, false); //copy it, delete generated stuff
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[0].setWavetableSoftFade(wavetablecopy);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[0].removeSoftFadeEditor();
				myWtEditor->updateAll(false);
			}
			else if (result == 15) {
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[1].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetablecopy = wavetable->getClonedInstance(true, false); //copy it, delete generated stuff
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[1].setWavetableSoftFade(wavetablecopy);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[1].removeSoftFadeEditor();
				myWtEditor->updateAll(false);
			}
			else if (result == 16) {
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[2].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetablecopy = wavetable->getClonedInstance(true, false); //copy it, delete generated stuff
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[2].setWavetableSoftFade(wavetablecopy);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[2].removeSoftFadeEditor();
				myWtEditor->updateAll(false);
			}
			else if (result == 17) {
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[3].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wavetablecopy = wavetable->getClonedInstance(true, false); //copy it, delete generated stuff
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[3].setWavetableSoftFade(wavetablecopy);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[3].removeSoftFadeEditor();
				myWtEditor->updateAll(false);
			}
			else if (result == 20) {
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[0].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wt = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[0].getWavetablePointer();
				wt->addPosition();
				wt->setNaiveTable((wt->getNumPositions() - 1), *wavetable->getNaiveTable(myWtEditor->getWtPos()), true, myProcessor->getWTmode());
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[0].setWavetableSoftFade(wt);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[0].removeSoftFadeEditor();
				myWtEditor->updateAll(false);
			}
			else if (result == 21) {
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[1].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wt = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[1].getWavetablePointer();
				wt->addPosition();
				wt->setNaiveTable((wt->getNumPositions() - 1), *wavetable->getNaiveTable(myWtEditor->getWtPos()), true, myProcessor->getWTmode());
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[1].setWavetableSoftFade(wt);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[1].removeSoftFadeEditor();
				myWtEditor->updateAll(false);
			}
			else if (result == 22) {
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[2].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wt = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[2].getWavetablePointer();
				wt->addPosition();
				wt->setNaiveTable((wt->getNumPositions() - 1), *wavetable->getNaiveTable(myWtEditor->getWtPos()), true, myProcessor->getWTmode());
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[2].setWavetableSoftFade(wt);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[2].removeSoftFadeEditor();
				myWtEditor->updateAll(false);
			}
			else if (result == 23) {
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[3].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wt = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[3].getWavetablePointer();
				wt->addPosition();
				wt->setNaiveTable((wt->getNumPositions() - 1), *wavetable->getNaiveTable(myWtEditor->getWtPos()), true, myProcessor->getWTmode());
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[3].setWavetableSoftFade(wt);
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[3].removeSoftFadeEditor();
				myWtEditor->updateAll(false);
			}
			else if (result == 35) {
				//cut out
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wt = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable();
				std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);

				float newLength = C_WAVE_TABLE_SIZE - (m_selection.iWTSelectionEnd - m_selection.iWTSelectionStart);
				if (newLength > 0) {
					float step = newLength / C_WAVE_TABLE_SIZE;
					if (newLength == 1.f)
						for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) samples[i] = 0.f;
					else {
						for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
							float newPos = 0.f;
							if (i < (m_selection.iWTSelectionStart / step))
								newPos = i * step;
							else
								newPos = (m_selection.iWTSelectionEnd / step - m_selection.iWTSelectionStart / step + i) * step;

							samples[i] = (*myWtEditor->getBankWavetable()->getNaiveTable(myWtEditor->getWtPos()))[newPos];
							int intpart = newPos;
							float fracpart = newPos - intpart;
							samples[i] = (1 - fracpart) * (*myWtEditor->getBankWavetable()->getNaiveTable(myWtEditor->getWtPos()))[intpart] + fracpart * ((*myWtEditor->getBankWavetable()->getNaiveTable(myWtEditor->getWtPos()))[((intpart + 1) < C_WAVE_TABLE_SIZE - 1) ? static_cast<std::vector<float, std::allocator<float>>::size_type>(intpart) + 1 : intpart]);
						}
					}
					wt->setNaiveTable(myWtEditor->getWtPos(), samples, false, myProcessor->getWTmode());
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wt);
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
					selectAll(false); //UI update later
					notifySelectionChanged();
					myWtEditor->updateAll(false);
				}
			}
			else if (result == 36) {
				//trim to
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
				std::shared_ptr<CVASTWaveTable> wt = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable();
				std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);

				float newLength = m_selection.iWTSelectionEnd - m_selection.iWTSelectionStart;
				if (newLength > 0) {
					float step = newLength / C_WAVE_TABLE_SIZE;
					for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
						float newPos = 0.f;
						newPos = (m_selection.iWTSelectionStart / step + i) * step;
						int intpart = newPos;
						float fracpart = newPos - intpart;
						samples[i] = (1 - fracpart) * (*myWtEditor->getBankWavetable()->getNaiveTable(myWtEditor->getWtPos()))[intpart] + fracpart * ((*myWtEditor->getBankWavetable()->getNaiveTable(myWtEditor->getWtPos()))[((intpart + 1) < C_WAVE_TABLE_SIZE - 1) ? static_cast<std::vector<float, std::allocator<float>>::size_type>(intpart) + 1 : intpart]);
					}
					wt->setNaiveTable(myWtEditor->getWtPos(), samples, false, myProcessor->getWTmode());
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wt);
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
					selectAll(false); //UI Update later
					notifySelectionChanged();
					myWtEditor->updateAll(false);
				}
			}
			else if (result == 40) {
				singleCycleFromMSEG(0);
			}
			else if (result == 41) {
				singleCycleFromMSEG(1);
			}
			else if (result == 42) {
				singleCycleFromMSEG(2);
			}
			else if (result == 43) {
				singleCycleFromMSEG(3);
			}
			else if (result == 44) {
				singleCycleFromMSEG(4);
			}
			else if (result == 51) { //copy osc
				String oscFrom;
				if (myWtEditor->m_bank == 0)
					oscFrom = "_OscA";
				else if (myWtEditor->m_bank == 1)
					oscFrom = "_OscB";
				else if (myWtEditor->m_bank == 2)
					oscFrom = "_OscC";
				else if (myWtEditor->m_bank == 3)
					oscFrom = "_OscD";
				String oscTo = "_OscA";
				if (oscFrom != oscTo) {
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[0].addSoftFadeEditor();
					std::shared_ptr<CVASTWaveTable> wavetablecopy = wavetable->getClonedInstance(true, false); //copy it, delete generated stuff
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[0].setWavetableSoftFade(wavetablecopy);
					StringArray oscparams = { "m_fOscDetune" , "m_fOscGain" , "m_fOscCents", "m_fOscWTPos" , "m_fOscMorph" , "m_fSpread" , "m_iNumOscs" , "m_fOscPan",
						"m_iOscOct", "m_fOscPhase" , "m_bExciterOnOff" , "m_bOscInverterOnOff" , "m_bOscRetrigOnOff" , "m_bOscOnOff", "m_uVCAEnv", "m_uWTFX", "m_fWTFXVal",
						"m_uOscRouting1", "m_uOscRouting2" };
					for (int i = 0; i < oscparams.size(); i++) {
						AudioProcessorParameterWithID* paramFrom = myProcessor->m_parameterState.getParameter(oscparams[i] + oscFrom);
						AudioProcessorParameterWithID* paramTo = myProcessor->m_parameterState.getParameter(oscparams[i] + oscTo);
						paramTo->setValueNotifyingHost(paramFrom->getValue());
					}
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[0].removeSoftFadeEditor();
					myWtEditor->updateAll(false);
				}
			}
			else if (result == 52) { //copy osc
				String oscFrom;
				if (myWtEditor->m_bank == 0)
					oscFrom = "_OscA";
				else if (myWtEditor->m_bank == 1)
					oscFrom = "_OscB";
				else if (myWtEditor->m_bank == 2)
					oscFrom = "_OscC";
				else if (myWtEditor->m_bank == 3)
					oscFrom = "_OscD";
				String oscTo = "_OscB";
				if (oscFrom != oscTo) {
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[1].addSoftFadeEditor();
					std::shared_ptr<CVASTWaveTable> wavetablecopy = wavetable->getClonedInstance(true, false); //copy it, delete generated stuff
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[1].setWavetableSoftFade(wavetablecopy);
					StringArray oscparams = { "m_fOscDetune" , "m_fOscGain" , "m_fOscCents", "m_fOscWTPos" , "m_fOscMorph" , "m_fSpread" , "m_iNumOscs" , "m_fOscPan",
						"m_iOscOct", "m_fOscPhase" , "m_bExciterOnOff" , "m_bOscInverterOnOff" , "m_bOscRetrigOnOff" , "m_bOscOnOff", "m_uVCAEnv", "m_uWTFX", "m_fWTFXVal",
						"m_uOscRouting1", "m_uOscRouting2" };
					for (int i = 0; i < oscparams.size(); i++) {
						AudioProcessorParameterWithID* paramFrom = myProcessor->m_parameterState.getParameter(oscparams[i] + oscFrom);
						AudioProcessorParameterWithID* paramTo = myProcessor->m_parameterState.getParameter(oscparams[i] + oscTo);
						paramTo->setValueNotifyingHost(paramFrom->getValue());
					}
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[1].removeSoftFadeEditor();
					myWtEditor->updateAll(false);
				}
			}
			else if (result == 53) { //copy osc
				String oscFrom;
				if (myWtEditor->m_bank == 0)
					oscFrom = "_OscA";
				else if (myWtEditor->m_bank == 1)
					oscFrom = "_OscB";
				else if (myWtEditor->m_bank == 2)
					oscFrom = "_OscC";
				else if (myWtEditor->m_bank == 3)
					oscFrom = "_OscD";
				String oscTo = "_OscC";
				if (oscFrom != oscTo) {
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[2].addSoftFadeEditor();
					std::shared_ptr<CVASTWaveTable> wavetablecopy = wavetable->getClonedInstance(true, false); //copy it, delete generated stuff
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[2].setWavetableSoftFade(wavetablecopy);
					StringArray oscparams = { "m_fOscDetune" , "m_fOscGain" , "m_fOscCents", "m_fOscWTPos" , "m_fOscMorph" , "m_fSpread" , "m_iNumOscs" , "m_fOscPan",
						"m_iOscOct", "m_fOscPhase" , "m_bExciterOnOff" , "m_bOscInverterOnOff" , "m_bOscRetrigOnOff" , "m_bOscOnOff", "m_uVCAEnv", "m_uWTFX", "m_fWTFXVal",
						"m_uOscRouting1", "m_uOscRouting2" };
					for (int i = 0; i < oscparams.size(); i++) {
						AudioProcessorParameterWithID* paramFrom = myProcessor->m_parameterState.getParameter(oscparams[i] + oscFrom);
						AudioProcessorParameterWithID* paramTo = myProcessor->m_parameterState.getParameter(oscparams[i] + oscTo);
						paramTo->setValueNotifyingHost(paramFrom->getValue());
					}
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[2].removeSoftFadeEditor();
					myWtEditor->updateAll(false);
				}
			}
			else if (result == 54) { //copy osc
				String oscFrom;
				if (myWtEditor->m_bank == 0)
					oscFrom = "_OscA";
				else if (myWtEditor->m_bank == 1)
					oscFrom = "_OscB";
				else if (myWtEditor->m_bank == 2)
					oscFrom = "_OscC";
				else if (myWtEditor->m_bank == 3)
					oscFrom = "_OscD";
				String oscTo = "_OscD";
				if (oscFrom != oscTo) {
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[3].addSoftFadeEditor();
					std::shared_ptr<CVASTWaveTable> wavetablecopy = wavetable->getClonedInstance(true, false); //copy it, delete generated stuff
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[3].setWavetableSoftFade(wavetablecopy);
					StringArray oscparams = { "m_fOscDetune" , "m_fOscGain" , "m_fOscCents", "m_fOscWTPos" , "m_fOscMorph" , "m_fSpread" , "m_iNumOscs" , "m_fOscPan",
						"m_iOscOct", "m_fOscPhase" , "m_bExciterOnOff" , "m_bOscInverterOnOff" , "m_bOscRetrigOnOff" , "m_bOscOnOff", "m_uVCAEnv", "m_uWTFX", "m_fWTFXVal",
						"m_uOscRouting1", "m_uOscRouting2" };
					for (int i = 0; i < oscparams.size(); i++) {
						AudioProcessorParameterWithID* paramFrom = myProcessor->m_parameterState.getParameter(oscparams[i] + oscFrom);
						AudioProcessorParameterWithID* paramTo = myProcessor->m_parameterState.getParameter(oscparams[i] + oscTo);
						paramTo->setValueNotifyingHost(paramFrom->getValue());
					}
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[3].removeSoftFadeEditor();
					myWtEditor->updateAll(false);
				}
			}
		}));
	}
}

void VASTOscilloscope::mouseMove(const MouseEvent& ) {
	m_bMouseover = true;	
	handleBorderDisplay();
	repaint();
}

void VASTOscilloscope::mouseExit(const MouseEvent& ) {
	m_bMouseover = false;
	handleBorderDisplay();
	repaint();
}

void VASTOscilloscope::singleCycleFromMSEG(int msegNo) {
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
	std::shared_ptr<CVASTWaveTable> wt = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable();
	std::vector<myFloat> samples = std::vector<myFloat>(C_WAVE_TABLE_SIZE);

	VASTMSEGData myData = VASTMSEGData(myProcessor->m_pVASTXperience.m_Set.m_MSEGData[msegNo]);
	VASTMSEGData myDataLive = VASTMSEGData(myProcessor->m_pVASTXperience.m_Set.m_MSEGData_changed[msegNo]);
	float totDur = myData.calcTotalDuration(); //ms
	for (int i = 0; i < myData.controlPoints.size(); i++) {
		//myData.controlPoints[i].isDecay = false;
		//myData.controlPoints[i].isSustain = false;
		myData.controlPoints[i].isLoopStart = false;
	}

	CVASTMSEGEnvelope envelope(myProcessor->m_pVASTXperience.m_Set, myData, myDataLive, -1, -1, -1);
	envelope.init();
	envelope.noteOn(0, false);
	
	float numSamples = (totDur / 1000) * myProcessor->m_pVASTXperience.m_Set.m_nSampleRate;
	float skips = numSamples / float(C_WAVE_TABLE_SIZE);
	float* envptr;
	int arraySize = static_cast<int>(ceil(skips) * C_WAVE_TABLE_SIZE);
	envptr = new float[arraySize];
	envelope.getEnvelopeRange(envptr, 0, ceil(skips) * C_WAVE_TABLE_SIZE, true); //no sustain
	for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
		float frac = skips * i;
		int envidx = frac;
		frac -= envidx;
		samples[i] = (1 - frac) * envptr[envidx] + frac * envptr[envidx + 1]; //linterp
		samples[i] = samples[i] * 2.f - 1.f;

		/*
		float val = 0.f;
		if (skips > 1) 
			for (int skip = 0; skip < skips; skip++) {
				val = envptr[envidx]; 
				envidx++;
			}
		else
			if (i % int(round(1.f / skips)) == 0) {
				val = envptr[envidx]; 
				envidx++;
			}
			else
				val = lastval;
		samples[i] = val * 2.f - 1.f;
		lastval = val;
		*/
	}
	delete[] envptr;
	envptr = NULL;

	wt->setNaiveTable(myWtEditor->getWtPos(), samples, false, myProcessor->getWTmode());
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wt);
	myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
	myWtEditor->updateAll(false);
    m_dirty.store(true);
}

/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTUndoableWavetableAction.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

VASTUndoableWavetableAction::VASTUndoableWavetableAction(CVASTOscillatorBank* bank) {
	m_bank = bank;
}

bool VASTUndoableWavetableAction::perform() {
	//VDBG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Perform action");
	return true;
};

bool VASTUndoableWavetableAction::undo() {

	if (m_bank->needsUndo()) {
		m_bank->undoLastWTChange();
		//VDBG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Undo action");
		return true;
	}
	return false;
}
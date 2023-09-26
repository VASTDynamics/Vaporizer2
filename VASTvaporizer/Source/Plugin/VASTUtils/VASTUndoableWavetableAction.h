/*
VAST Dynamics
*/
#ifndef VASTUNDOABLEWAVETABLEACTION_H_INCLUDED
#define VASTUNDOABLEWAVETABLEACTION_H_INCLUDED
#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTOscillatorBank.h"

class VASTUndoableWavetableAction : public UndoableAction
{
public:
	VASTUndoableWavetableAction(CVASTOscillatorBank* bank);

	bool perform();
	bool undo();

private:
	CVASTOscillatorBank* m_bank;
};

#endif  // VASTUNDOABLEWAVETABLEACTION_H_INCLUDED

/*
VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../../Engine/VASTEngineHeader.h"

//==============================================================================
class VASTGUINotifier
{
public:
    virtual void shallUpdateGUI(const String compID, bool toggleState) = 0;
    virtual ~VASTGUINotifier() = default;
};

// Copyright (c) 2020 National Instruments
// SPDX-License-Identifier: MIT

/*! \file
	\brief Tools to implement a debugging context
 */

#ifndef  _DEBUGGINGCONTEXT_H
#define _DEBUGGINGCONTEXT_H

#include "TypeAndDataManager.h"
#include "map"

namespace Vireo
{

#if kVireoOS_emscripten
#include <emscripten.h>
    extern "C" {
        extern void jsDebuggingContextDebugPointInterrupt(StringRef);
    }
    extern "C" {
        extern void jsDebuggingContextBreakPointInterrupt(StringRef);
    }
#endif

class DebuggingContext
{
 private:
    std::map<const char*, bool> _debugPointState;
    std::map<const char*, bool> _breakPointState;

 public:
    bool GetDebugPointState(const char* objectID)
    {
        for (auto itr : _debugPointState)
        {
            if (strcmp(itr.first, objectID) == 0)
            {
                return itr.second;
            }
        }
        return false;
    }
    void SetDebugPointState(const char* objectID, bool state)
    {
         _debugPointState[objectID] = state;
    }
    bool GetBreakPointState(const char* objectID)
    {
        for (auto itr : _breakPointState)
        {
            if (strcmp(itr.first, objectID) == 0)
            {
                return itr.second;
            }
        }
        return false;
    }
    void SetBreakPointState(const char* objectID, bool state)
    {
         _breakPointState[objectID] = state;
    }
};
}  // namespace Vireo

#endif  // ! _DEBUGGINGCONTEXT_H

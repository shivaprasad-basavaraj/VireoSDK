// Copyright (c) 2020 National Instruments
// SPDX-License-Identifier: MIT

/*! \file
	\brief Tools to implement a debugging context
 */

#ifndef  _DEBUGGINGCONTEXT_H
#define _DEBUGGINGCONTEXT_H

#include "TypeAndDataManager.h"
#include "map"
#include "DebugPoint.h"

namespace Vireo
{

#if kVireoOS_emscripten
#include <emscripten.h>
    extern "C" {
        extern void jsDebuggingContextDebugPointInterrupt(StringRef);
    }
#endif

class DebuggingContext
{
 private:
    std::map<SubString, DebugPointInstruction*, CompareSubString> _debugPointInstructionMap;
 public:
    bool GetDebugPointState(const char* objectID)
    {
        for (auto itr : _debugPointInstructionMap)
        {
            if (itr.first.CompareCStr(objectID))
            {
                return itr.second->_pDebugPointState != 0 ? true : false;
            }
        }
        return false;
    }
    void SetDebugPointState(const char* objectID, bool state)
    {
        for (auto itr : _debugPointInstructionMap)
        {
            if (itr.first.CompareCStr(objectID))
            {
                itr.second->SetDebugPointState(state ? 1 : 0);
            }
        }
    }
    void SetDebugPointInstruction(StringRef debugPointId, DebugPointInstruction* debugPointInstruction)
    {
        _debugPointInstructionMap[debugPointId->MakeSubStringAlias()] = debugPointInstruction;
    }
};
}  // namespace Vireo

#endif // ! _DEBUGGINGCONTEXT_H

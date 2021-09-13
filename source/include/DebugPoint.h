// Copyright (c) 2020 National Instruments
#ifndef  _DEBUGPOINT_H
#define _DEBUGPOINT_H

#include "TypeDefiner.h"

namespace Vireo
{

    struct DebugPointInstruction : public InstructionCore
    {
        _ParamDef(StringRef, DebugPointId);
        _ParamDef(Int8, DebugPointState);
        void SetDebugPointState(Int8 newState);
        NEXT_INSTRUCTION_METHOD()
    };

    //------------------------------------------------------------
    //! A wrapper that gives a raw block of elements a Begin(), End(), and Length() method. It does not own the data.
    class DebugPoint
    {
     public:
        DebugPoint();
    };
}

#endif // ! _DEBUGPOINT_H

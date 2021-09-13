// Copyright (c) 2020 National Instruments
#include "DebugPoint.h"
#include "TypeDefiner.h"
#include "DebuggingContext.h"
#include "ExecutionContext.h"

#if kVireoOS_emscripten
#include <emscripten.h>
#endif

namespace Vireo {
    //------------------------------------------------------------
    // Debug node which will check if breakpoint is set or not
    VIREO_FUNCTION_SIGNATURET(DebugPoint, DebugPointInstruction)
    {
        if (_Param(DebugPointState) != 0) {
#if kVireoOS_emscripten
            jsDebuggingContextDebugPointInterrupt(_Param(DebugPointId));
#endif
        }
        return _NextInstruction();
    }

    void DebugPointInstruction::SetDebugPointState(Int8 newState)
    {
        *(this->_pDebugPointState) = newState;
    }

    DEFINE_VIREO_BEGIN(Execution)
        DEFINE_VIREO_FUNCTION(DebugPoint, "p(i(String))")
        DEFINE_VIREO_END()
}  // namespace Vireo

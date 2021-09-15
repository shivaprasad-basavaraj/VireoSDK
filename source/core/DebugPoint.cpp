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
    // Debug node which will check if breakpoint is set or not and do return culdsac for VI pause state
    VIREO_FUNCTION_SIGNATURE1(DebugPoint, StringRef)
    {
#if kVireoOS_emscripten
      if (THREAD_EXEC()->debuggingContext->GetBreakPointState((const char*)_Param(0)->Begin())) {
            jsDebuggingContextBreakPointInterrupt(_Param(0));
            return THREAD_EXEC()->Pause(_NextInstruction());
        }
      if (THREAD_EXEC()->getVIPauseState() == 2){
          return THREAD_EXEC()->Pause(_NextInstruction());
      }
#endif
        return _NextInstruction();
    }

    DEFINE_VIREO_BEGIN(Execution)
        DEFINE_VIREO_FUNCTION(DebugPoint, "p(i(String))")
        DEFINE_VIREO_END()
}  // namespace Vireo

// Copyright (c) 2020 National Instruments
#include "DebugPoint.h"
#include "TypeDefiner.h"
#include "DebuggingContext.h"
#include "ExecutionContext.h"
#include "VirtualInstrument.h"

#if kVireoOS_emscripten
#include <emscripten.h>
#endif

namespace Vireo {
    struct SetValueHasUpdateParamBlock : InstructionCore
    {
        _ParamDef(StaticType, ValueType);
        _ParamDef(void, ValueData);
        NEXT_INSTRUCTION_METHOD()
    };

    VIREO_FUNCTION_SIGNATURET(SetValueHasUpdate, SetValueHasUpdateParamBlock)
    {
        VirtualInstrument* vi = THREAD_EXEC()->_runningQueueElt->OwningVI();
        if (vi->IsTopLevelVI())
            _ParamPointer(ValueType)->SetHasUpdate(true);
        return _NextInstruction();
    }

    void DebugPointInstruction::SetDebugPointState(Int8 newState)
    {
        *(this->_pDebugPointState) = newState;
    }

    //------------------------------------------------------------
    // Debug node which will check if breakpoint is set or not
    VIREO_FUNCTION_SIGNATURET(DebugPointInternal, DebugPointInstruction)
    {
        if (_Param(DebugPointState) != 0) {
#if kVireoOS_emscripten
            jsDebuggingContextDebugPointInterrupt(_Param(DebugPointId));
#endif
        }
        return _NextInstruction();
    }

    InstructionCore* EmitDebugPointInstruction(ClumpParseState* pInstructionBuilder)
    {
        InstructionCore* pInstruction = nullptr;
        TypeRef debugPointIdType = pInstructionBuilder->_argTypes[1];
        void* debugPointPointer = pInstructionBuilder->_argPointers[1];

        int argCount = pInstructionBuilder->_argCount;
        std::vector<TypeRef> localTypes;
        std::vector<void*> localPointers;

        for (int i = 2; i < argCount; i++)
        {
            localTypes.push_back(pInstructionBuilder->_argTypes[i]);
            localPointers.push_back(pInstructionBuilder->_argPointers[i]);
        }


        for (int i = 0; i < argCount - 2; i++)
        {
            TypeRef typeOfLocal = localTypes.at(i);
            void* localAddress = localPointers.at(i);
            SubString valueHasUpdateToken("SetValueHasUpdate");
            pInstructionBuilder->StartInstruction(&valueHasUpdateToken);
            pInstructionBuilder->InternalAddArgBack(nullptr, typeOfLocal);
            pInstructionBuilder->InternalAddArgBack(typeOfLocal, localAddress);
            pInstruction = pInstructionBuilder->EmitInstruction();
        }

        SubString debugOpToken("DebugPointInternal");
        pInstructionBuilder->StartInstruction(&debugOpToken);
        pInstructionBuilder->InternalAddArgBack(debugPointIdType, debugPointPointer);

        TypeRef debugPointStateType = pInstructionBuilder->_clump->TheTypeManager()->FindType(&TypeCommon::TypeUInt8);
        DefaultValueType *debugPointState = DefaultValueType::New(pInstructionBuilder->_clump->TheTypeManager(), debugPointStateType, true);
        void* data = THREAD_TADM()->Malloc(debugPointStateType->TopAQSize());
        debugPointState->InitData(data);
        debugPointState->FinalizeDVT();
        void* pData = static_cast<AQBlock1*>(debugPointState->Begin(kPAReadWrite));  // * passed as a param means nullptr
        pInstructionBuilder->InternalAddArgBack(debugPointStateType, pData);

        pInstruction = pInstructionBuilder->EmitInstruction();

        StringRef* debugPointId = (StringRef*)(debugPointPointer);
        THREAD_EXEC()->debuggingContext->SetDebugPointInstruction(*debugPointId, (DebugPointInstruction*)pInstruction);

        return pInstruction;
    }

    DEFINE_VIREO_BEGIN(Execution)
        DEFINE_VIREO_GENERIC(DebugPoint, "p(i(VarArgCount) i(String) i(*))", EmitDebugPointInstruction)
        DEFINE_VIREO_FUNCTION(SetValueHasUpdate, "p(i(StaticTypeAndData value))")
        DEFINE_VIREO_FUNCTION(DebugPointInternal, "p(i(String))")
        DEFINE_VIREO_END()
}  // namespace Vireo

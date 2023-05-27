#ifndef WREN_H
#define WREN_H

#ifndef WREN_SLOT_COUNT
#define WREN_SLOT_COUNT 4
#endif

#ifndef PATH_TO_WREN_CORE
#define PATH_TO_WREN_CORE "../scripts/core.wren"
#endif

#include "../../../lib/wren-cmake/src/include/wren.h"

#define CHECK_NULL(obj, error) \
    if (obj == NULL) { \
        wrenSetSlotHandle(vm, 0, (WrenHandle*)error); \
        wrenAbortFiber(vm, 0); \
        return; \
    }

typedef struct Wren
{
    WrenVM* vm;
    WrenConfiguration config;
    char* coreSrc;
} ScriptEngine;

ScriptEngine* newScriptEngine(char includeCore);
void destroyScriptEngine(ScriptEngine* engine);


#endif
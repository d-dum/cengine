#include "Wren.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../engine.h"

char* coreSrc = NULL;

ASSOCIATIVE_ARRAY(char*, st)
CREATE_NODE(char*, st)
FIND_NODE(char*, st)
NODE_APPEND(char*, st)

List_st* modules = NULL;

void writeFn(WrenVM* vm, const char* text) {
    printf("%s", text);
}

void dmAllocate(WrenVM* vm){
    DisplayManager** mng = (DisplayManager**) wrenSetSlotNewForeign(vm, 0, 0, sizeof(DisplayManager*));
    int width = (int) wrenGetSlotDouble(vm, 1);
    int height = (int) wrenGetSlotDouble(vm, 2);

    *mng = newDisplayManager(width, height);
}

void dmFinalize(void* data){
    DisplayManager** mng = (DisplayManager**) data;
    dmCleanup(*mng);
}

void meshAllocate(WrenVM* vm){
    Mesh** mesh = (Mesh**) wrenSetSlotNewForeign(vm, 0, 0, sizeof(DisplayManager*));
    char* path = (char*) wrenGetSlotString(vm, 1);
    char hasUvs = (char) wrenGetSlotBool(vm, 2);

    *mesh = loadFromOBJ(path, hasUvs);
}

void meshFinalize(void* data){
    Mesh** mesh = (Mesh**) data;
    deleteMesh(*mesh);
}

void entityAllocate(WrenVM* vm){
    //printf("Mesh is %s\n", wrenGetSlotType(vm, 1));
    Entity** en = (Entity**) wrenSetSlotNewForeign(vm, 0, 0, sizeof(Entity*));
    Mesh** mesh = (Mesh**) wrenGetSlotForeign(vm, 1);

    //printf("Mesh is %s\n", wrenGetSlotType(vm, 1));

    *en = newEntity(*mesh);
}

void entityFinalize(void* data){
    Entity** en = (Entity**) data;

    entityCleanup(*en);
}

void camAllocate(WrenVM* vm){
    Camera** cam = (Camera**) wrenSetSlotNewForeign(vm, 0, 0, sizeof(Camera*));

    float** position = (float**) wrenGetSlotForeign(vm, 1);
    float** lookAt = (float**) wrenGetSlotForeign(vm, 2);
    float** up = (float**) wrenGetSlotForeign(vm, 3);

    *cam = newCamera(*position, *lookAt, *up);
}

void camFinalize(void* data){
    Camera** cam = (Camera**) data;
    cameraCleanup(*cam);
}

void vec3Allocate(WrenVM* vm){
    float** vc = (float**) wrenSetSlotNewForeign(vm, 0, 0, sizeof(float*));

    float x = wrenGetSlotDouble(vm, 1);
    float y = wrenGetSlotDouble(vm, 2);
    float z = wrenGetSlotDouble(vm, 3);

    *vc = (float*) malloc(3 * sizeof(float));
    (*vc)[0] = (float)x;
    (*vc)[1] = (float)y;
    (*vc)[2] = (float)z;
}

void vec3Finalize(void* data){
    float** vc = (float**) data;
    free(*vc);
}

void vec3GetElement(WrenVM* vm){
    float** vc = (float**) wrenGetSlotForeign(vm, 0);
    int inx = (int) wrenGetSlotDouble(vm, 1);

    if(inx > 2 || inx < 0){
        wrenSetSlotHandle(vm, 0, (WrenHandle*) "Index out of bounds");
        wrenAbortFiber(vm, 0);
        return;
    }

    wrenSetSlotDouble(vm, 0, (*vc)[inx]);
}

void entityRotate(WrenVM* vm){
    Entity** en = (Entity**) wrenGetSlotForeign(vm, 0);
    float angle = wrenGetSlotDouble(vm, 1);
    float** vc = wrenGetSlotForeign(vm, 2);

    enRotate(*en, angle, *vc);
}

void entityTranslate(WrenVM* vm){
    Entity** en = (Entity**) wrenGetSlotForeign(vm, 0);
    float** vc = wrenGetSlotForeign(vm, 1);

    enTranslate(*en, *vc);
}

void entityScale(WrenVM* vm){
    Entity** en = (Entity**) wrenGetSlotForeign(vm, 0);
    float** vc = wrenGetSlotForeign(vm, 1);

    enScale(*en, *vc);
}

void meshAddTexture(WrenVM* vm){
    Mesh** mesh = (Mesh**) wrenGetSlotForeign(vm, 0);
    char* path = (char*) wrenGetSlotString(vm, 1);

    CHECK_NULL(mesh, "cannot use destroyed mesh");

    loadAnyTexture(*mesh, path);
}

void dmIsCloseRequested(WrenVM* vm){
    DisplayManager** mng = (DisplayManager**) wrenGetSlotForeign(vm, 0);

    CHECK_NULL(mng, "cannot use destgroyed dm");

    int res = isCloseRequested(*mng);
    wrenSetSlotBool(vm, 0, res);
}

void dmUpdate(WrenVM* vm){
    DisplayManager** mng = (DisplayManager**) wrenGetSlotForeign(vm, 0);

    CHECK_NULL(mng, "cannot use destroyed dm");

    update(*mng);
}

void shaderAllocate(WrenVM* vm){
    Shader** shader = (Shader**) wrenSetSlotNewForeign(vm, 0, 0, sizeof(Shader*));
    GLenum shaderType;

    char* path = (char*)wrenGetSlotString(vm, 1);
    int tp = (int) wrenGetSlotDouble(vm, 2);

    switch(tp){
        case 0:
            shaderType = VERTEX_SHADER;
            break;
        case 1:
            shaderType = FRAGMENT_SHADER;
            break;
    }

    
    *shader = newShader(path, shaderType);

    printf("Shader: %d\n", (*shader)->shaderId);
}

void shaderFinalize(void* data){
    Shader** shader = (Shader**) data;
    shaderCleanup(*shader);
}

void shaderProgramAllocate(WrenVM* vm){
    ShaderProgram** program = (ShaderProgram**) wrenSetSlotNewForeign(vm, 0, 0, sizeof(Shader*));
    Shader** vert = (Shader**) wrenGetSlotForeign(vm, 1);
    Shader** frag = (Shader**) wrenGetSlotForeign(vm, 2);

    Shader** shaders = malloc(sizeof(Shader*) * 2);
    
    shaders[0] = *vert;
    shaders[1] = *frag;

    *program = newShaderProgram(shaders, 2);
}

void shaderProgramFinalize(void* data){
    ShaderProgram** program = (ShaderProgram**) data;
    shaderProgramCleanupSimple(*program);
}

void shaderProgramStart(WrenVM* vm){
    ShaderProgram** prg = (ShaderProgram**) wrenGetSlotForeign(vm, 0);
    useProgram(*prg);
}

void shaderProgramStop(WrenVM* vm){
    ShaderProgram** prg = (ShaderProgram**) wrenGetSlotForeign(vm, 0);
    stopProgram(*prg);
}

void lighAllocate(WrenVM* vm){
    Light** light = (Light**) wrenSetSlotNewForeign(vm, 0, 0, sizeof(Light*));

    float** pos = wrenGetSlotForeign(vm, 1);
    float** color = wrenGetSlotForeign(vm, 2);

    *light = newLight(*pos, *color);
}

void lightFinalize(void* data){
    Light** light = (Light**) data;
    deleteLight(*light);
}

void rendererAllocate(WrenVM* vm){
    Renderer** rnd = (Renderer**) wrenSetSlotNewForeign(vm, 0, 0, sizeof(Renderer*));

    float fov = wrenGetSlotDouble(vm, 1);
    float height = wrenGetSlotDouble(vm, 2);
    float width = wrenGetSlotDouble(vm, 3);
    float near = wrenGetSlotDouble(vm, 4);
    float far = wrenGetSlotDouble(vm, 5);

    *rnd = newRenderer(fov, height, width, near, far);
}

void rendererFinalize(void* data){
    Renderer** rnd = (Renderer**) data;

    rendererCleanup(*rnd);
}

void rendererRenderEntity(WrenVM* vm){
    Renderer** rnd = (Renderer**) wrenGetSlotForeign(vm, 0);

    Entity** ent = (Entity**) wrenGetSlotForeign(vm, 1);
    ShaderProgram** prg = (ShaderProgram**) wrenGetSlotForeign(vm, 2);
    Light** lt = (Light**) wrenGetSlotForeign(vm, 3);

    renderEntity(*rnd, *ent, *prg, *lt);
}

void rndPrepare(WrenVM* vm){
    Renderer** rnd = (Renderer**) wrenGetSlotForeign(vm, 0);
    ShaderProgram** prg = (ShaderProgram**) wrenGetSlotForeign(vm, 1);
    Camera** cam = (Camera**) wrenGetSlotForeign(vm, 2);

    prepareRenderer(*rnd, *prg, *cam);
}

WrenForeignClassMethods bindForeignClass(WrenVM* vm, const char* module, const char* className){
    WrenForeignClassMethods methods;

    printf("Loading class: %s\n", className);

    if(strcmp(className, "DisplayManager") == 0){
        methods.allocate = &dmAllocate;
        methods.finalize = &dmFinalize;
    }else if(strcmp(className, "MeshLoader") == 0){
        methods.allocate = &meshAllocate;
        methods.finalize = &meshFinalize;
    }else if(strcmp(className, "Vec3") == 0){
        methods.allocate = &vec3Allocate;
        methods.finalize = &vec3Finalize;
    }else if(strcmp(className, "Entity") == 0){
        methods.allocate = &entityAllocate;
        methods.finalize = &entityFinalize;
    }else if(strcmp(className, "Camera") == 0){
        methods.allocate = &camAllocate;
        methods.finalize = &camFinalize;
    }else if(strcmp(className, "Shader") == 0){
        methods.allocate = &shaderAllocate;
        methods.finalize = &shaderFinalize;
    }else if(strcmp(className, "ShaderProgram") == 0){
        methods.allocate = &shaderProgramAllocate;
        methods.finalize = &shaderProgramFinalize;
    }else if(strcmp(className, "Light") == 0){
        methods.allocate = &lighAllocate;
        methods.finalize = &lightFinalize;
    }else if(strcmp(className, "Renderer") == 0){
        methods.allocate = &rendererAllocate;
        methods.finalize = &rendererFinalize;
    }else{
        methods.allocate = NULL;
        methods.finalize = NULL;
    }

    printf("Loaded class: %s\n", className);

    return methods;
}

void moduleComplete(WrenVM* vm, const char* name, WrenLoadModuleResult result){
    printf("LOAD FINISHED\n");
}

WrenLoadModuleResult wrenLoadModule(WrenVM* vm, const char* name){
    WrenLoadModuleResult result;
    result.onComplete = &moduleComplete;

    if(strcmp(name, "core") == 0){
        result.source = coreSrc;
    }else{
        if(modules == NULL){
            char path[50] = SCRIPT_ROOT;
            char* module = readFile(strcat(path, name));
            result.source = module;
            modules = createNode_st(name, module);
        }else{
            List_st* f;
            if((f = findNode_st(name, modules)) != NULL){
                result.source = f->value;
            }else{
                char path[50] = SCRIPT_ROOT;
                char* module = readFile(strcat(path, name));
                result.source = module;
                appendNode_st(modules, createNode_st(name, module));
            }
        }
    }

    return result;
}

void freeModule(){
    List_st* rt = modules;
    List_st* prev;

    while(1){
        prev = rt;
        rt = rt->next;
        free(prev->value);
        free(prev);

        if(rt == NULL)
            return;
    }
}

WrenForeignMethodFn bindForeignMethod(WrenVM* vm, const char* module,
    const char* className, bool isStatic, const char* signature){

    if (strcmp(className, "DisplayManager") == 0){
        if(!isStatic && strcmp(signature, "isCloseRequested()") == 0){
            return dmIsCloseRequested;
        }
        if(!isStatic && strcmp(signature, "update()") == 0){
            return dmUpdate;
        }
    }
    if(strcmp(className, "MeshLoader") == 0){
        if(!isStatic){
            if(strcmp(signature, "loadTexture(_)") == 0){
                return meshAddTexture;
            }
        }
    }
    if(strcmp(className, "Vec3") == 0){
        if(!isStatic){
            if(strcmp(signature, "getElement(_)") == 0){
                return vec3GetElement;
            }
        }
    }
    if(strcmp(className, "Entity") == 0){
        if(!isStatic){
            if(strcmp(signature, "translate(_)") == 0){
                return entityTranslate;
            }
            if(strcmp(signature, "rotate(_,_)") == 0){
                return entityRotate;
            }
            if(strcmp(signature, "scale(_)") == 0){
                return entityScale;
            }
        }
    }
    if(strcmp(className, "ShaderProgram") == 0){
        if(strcmp(signature, "start()") == 0){
            return shaderProgramStart;
        }
        if(strcmp(signature, "stop()") == 0){
            return shaderProgramStop;
        }
    }

    if(strcmp(className, "Renderer") == 0){
        if(strcmp(signature, "prepare(_,_)") == 0){
            return rndPrepare;
        }
        if(strcmp(signature, "render(_,_,_)") == 0){
            return rendererRenderEntity;
        }
    }

    return NULL;
}

void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
      printf("[%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      printf("[%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      printf("[Runtime Error] %s\n", msg);
    } break;
  }
}

void executeScript(ScriptEngine* engine, char* sourceOrPath, char isPath, char isEntry){
    char* src = isPath? 
        readFile(sourceOrPath) :
        sourceOrPath;
    
    WrenInterpretResult result = wrenInterpret(engine->vm, "main", src);

    switch (result) {
        case WREN_RESULT_COMPILE_ERROR:
        { printf("Compile Error!\n"); } break;
        case WREN_RESULT_RUNTIME_ERROR:
        { printf("Runtime Error!\n"); } break;
        case WREN_RESULT_SUCCESS:
        { printf("Success!\n"); } break;
    }

    if(isPath)
        free(src);
    
    if(result == WREN_RESULT_SUCCESS && isEntry){
        wrenEnsureSlots(engine->vm, 1);
        wrenGetVariable(engine->vm, "main", "GameEngine", 0);
        WrenHandle* gameEngineClass = wrenGetSlotHandle(engine->vm, 0);
        wrenSetSlotHandle(engine->vm, 0, gameEngineClass);
        WrenHandle* launchHandle = wrenMakeCallHandle(engine->vm, "launch()");
        printf("ENGINE LAUNCHED\n");
        result = wrenCall(engine->vm, launchHandle);
    }
}


ScriptEngine* newScriptEngine(char includeCore){
    if(includeCore)
        coreSrc = readFile(PATH_TO_WREN_CORE);
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.writeFn = &writeFn;
    config.errorFn = &errorFn;
    config.loadModuleFn = &wrenLoadModule;
    config.bindForeignClassFn = &bindForeignClass;
    config.bindForeignMethodFn = &bindForeignMethod;


    WrenVM* vm = wrenNewVM(&config);
    ScriptEngine* engine = malloc(sizeof(ScriptEngine));
    engine->config = config;
    engine->vm = vm;
    return engine;
}

void destroyScriptEngine(ScriptEngine* engine){
    free(coreSrc);
    freeModule();
    wrenFreeVM(engine->vm);
    engine->vm = NULL;
    free(engine);
}
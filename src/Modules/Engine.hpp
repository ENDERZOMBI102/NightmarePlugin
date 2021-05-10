#pragma once
#include "Module.hpp"
#include "Tier1.hpp"
#include "../Interface.hpp"

/**
 * Represents the engine DLL
 */
class Engine : public Module {
public:
    Interface* engineClient = nullptr;
    CHostState* hoststate = nullptr;
    Interface* engineTrace = nullptr;
    Interface* engineTool = nullptr;

    using _Cbuf_AddText = void(__cdecl*)(int slot, const char* pText, int nTickDelay);
#ifdef _WIN32
    using _GetScreenSize = int(__stdcall*)(int& width, int& height);
    using _PrecacheModel = void(__stdcall*)(const char* pName, bool bPreload);
    using _GetActiveSplitScreenPlayerSlot = int (*)();
#else
    using _GetScreenSize = int(__cdecl*)(void* thisptr, int& width, int& height);
    using _PrecacheModel = void(__cdecl*)(const char* pName, bool bPreload);
    using _GetActiveSplitScreenPlayerSlot = int (*)(void* thisptr);
#endif
    using _ClientCommand = int(*)(void* thisptr, void* pEdict, const char* szFmt, ...);
    using _TraceRay = void(__func*)(void* thisptr, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, CGameTrace* pTrace);

    _GetScreenSize GetScreenSize = nullptr;
    /**
     * Precache a model
     * @param pName name/path of the model
     * @param bPreload should this model be preloaded?
     */
    _PrecacheModel PrecacheModel = nullptr;
    _GetActiveSplitScreenPlayerSlot GetActiveSplitScreenPlayerSlot = nullptr;
    /**
     * @param slot
     * @param ptext
     * @param nTickDelay
     */
    _Cbuf_AddText Cbuf_AddText = nullptr;
    /**
     * Executes a command on the given client
     * @param pEdict client to execute the command on
     * @param szFmt command to execute
     */
    _ClientCommand ClientCommand = nullptr;
    /**
     * @param ray
     * @param fMask
     * @param pTraceFilter
     * @param pTrace
     */
    _TraceRay TraceRay = nullptr;

    void* s_CommandBuffer = nullptr;

    Engine();
    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE("engine"); }
};

extern Engine* engine;


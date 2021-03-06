#pragma once
#include "../Interface.hpp"
#include "Module.hpp"
#include "../Utils.hpp"

/**
 * Represents the server DLL
 */
class Server : public Module {
public:
    Interface* g_GameMovement = nullptr;
    Interface* g_ServerGameDLL = nullptr;
public:
    int tickBase;
public:
    Server();
    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE("server"); }

    //CGameMovement::ProcessMovement
    DECL_DETOUR(ProcessMovement, void* pPlayer, CMoveData* pMove);

    using _UTIL_PlayerByIndex = void* (__cdecl*)(int index);
    _UTIL_PlayerByIndex UTIL_PlayerByIndex = nullptr;

    /**
     * Gets the player denoted by this index
     * @param index index of the player
     */
    void* GetPlayer(int index);
    /**
     * Gets an handle for the entity denoted by this index
     * @param index index of the entity
     */
    void* GetEntityHandleByIndex(int index);

    CGlobalVars* gpGlobals = nullptr;
    CEntInfo* m_EntPtrArray = nullptr;
};

extern Server* server;

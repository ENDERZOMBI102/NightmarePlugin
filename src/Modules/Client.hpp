#pragma once
#include "../Interface.hpp"
#include "Module.hpp"
#include "../Utils.hpp"

/**
 * Represents the client DLL
 */
class Client : public Module {
public:
    Interface* g_HudChat;
    Interface* g_ClientTools;
    Interface* g_ClientDLL;

    using _ChatPrintf = void(*)(void* thisptr, int iPlayerIndex, int iFilter, const char* fmt, ...);
    /**
     * Print a message in the chat
     * @param iPlayerIndex ???
     * @param iFilter ???
     * @param fmt message to print
     */
    _ChatPrintf ChatPrintf = nullptr;

    using _NextParticleSystem = void*(__func*)(void* thisptr, void* searchResult);
    // TODO: i have no idea what this is
    _NextParticleSystem NextParticleSystem = nullptr;

    /**
     * TODO: i have no idea what this is
     * @param prev
     * @return
     */
    CNewParticleEffect* GetParticleSystem(CNewParticleEffect* prev);
    // TODO: does the "portalgun indicator" refer to the small light on the pgun model?
    /**
     *
     * @return current portalgun indicator color
     */
    inline Vector GetPortalGunIndicatorColor() const { return this->portalGunIndicatorColor; }
    /**
     * Sets the portalgun indicator color
     * @param v RGB vector
     */
    void SetPortalGunIndicatorColor(Vector v);
    /**
     * Updates the client's rendered portalgun indicator color
     */
    void UpdatePortalGunIndicatorColor();



    DECL_DETOUR(RenderView, const CViewSetup& view, int nClearFlags, int whatToDraw);
    //DECL_DETOUR(HudUpdate, unsigned int a2);
public:
    Client();
    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE("client"); }

private:
    Vector portalGunIndicatorColor;
};

extern Client* client;

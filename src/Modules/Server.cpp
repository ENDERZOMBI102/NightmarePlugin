#include "Server.hpp"

#include "../Command.hpp"
#include "../Interface.hpp"
#include "../Offsets.hpp"
#include "../Utils.hpp"

#include "Console.hpp"
#include "../NightmarePlugin.hpp"
#include "../CelesteMoveset.hpp"


// CGameMovement::ProcessMovement
REDECL(Server::ProcessMovement);
DETOUR(Server::ProcessMovement, void* pPlayer, CMoveData* pMove) {
    celesteMoveset.PreProcessMovement(pPlayer, pMove);

    auto result = Server::ProcessMovement(thisptr, pPlayer, pMove);
    server->tickBase = *reinterpret_cast<int*>((uintptr_t)pPlayer + Offsets::m_nTickBase);

    celesteMoveset.ProcessMovement(pPlayer, pMove);
    return result;
}

void* Server::GetPlayer(int index) {
    return this->UTIL_PlayerByIndex(index);
}

void* Server::GetEntityHandleByIndex(int index) {
    auto size = sizeof(CEntInfo2);
    CEntInfo* info = reinterpret_cast<CEntInfo*>((uintptr_t)server->m_EntPtrArray + size * index);
    return info->m_pEntity;
}

Server::Server()
    : Module() {
}
bool Server::Init() {

    if (auto g_ServerTools = Interface::Create(this->Name(), "VSERVERTOOLS0")) {
        auto GetIServerEntity = g_ServerTools->Original(Offsets::GetIServerEntity);
        Memory::Deref(GetIServerEntity + Offsets::m_EntPtrArray, &this->m_EntPtrArray);
        Interface::Delete(g_ServerTools);
    }

    this->g_GameMovement = Interface::Create(this->Name(), "GameMovement0");
    if (this->g_GameMovement) {
        this->g_GameMovement->Hook(Server::ProcessMovement_Hook, Server::ProcessMovement, Offsets::ProcessMovement);
    }

    this->g_ServerGameDLL = Interface::Create(this->Name(), "ServerGameDLL0");

    if (this->g_ServerGameDLL) {
        auto Think = this->g_ServerGameDLL->Original(Offsets::Think);
        Memory::Read<_UTIL_PlayerByIndex>(Think + Offsets::UTIL_PlayerByIndex, &this->UTIL_PlayerByIndex);
        Memory::DerefDeref<CGlobalVars*>((uintptr_t)this->UTIL_PlayerByIndex + Offsets::gpGlobals, &this->gpGlobals);
    }

    return this->hasLoaded = this->g_GameMovement && this->g_ServerGameDLL;
}
void Server::Shutdown() {
    if (this->g_GameMovement) {
        Interface::Delete(this->g_GameMovement);
    }

    if (this->g_ServerGameDLL) {
        Interface::Delete(this->g_ServerGameDLL);
    }
}

Server* server;

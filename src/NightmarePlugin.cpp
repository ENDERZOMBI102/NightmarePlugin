#include "NightmarePlugin.hpp"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <stdarg.h>

#include "Modules/Client.hpp"
#include "Modules/Server.hpp"
#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Module.hpp"
#include "Modules/Tier1.hpp"
#include "Modules/VScript.hpp"
#include "Modules/VGui.hpp"
#include "Modules/Surface.hpp"

#include "Offsets.hpp"
#include "Command.hpp"
#include "Game.hpp"
#include "Utils.hpp"
#include "Utils/Memory.hpp"
#include "Hud/Hud.hpp"

NightmarePlugin nightmarePlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(NightmarePlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, nightmarePlugin);

BEGIN_SCRIPTDESC_ROOT(NightmarePlugin, "The NightmarePlugin instance.")
DEFINE_SCRIPTFUNC(GetMode, "Returns current mode.")
DEFINE_SCRIPTFUNC(SetMode, "Sets the mod mode.")
DEFINE_SCRIPTFUNC(GetModeParam, "Returns mod-specific param with given ID.")
DEFINE_SCRIPTFUNC(SetModeParam, "Sets mod specific param if it's not read-only (i.e. not constantly updated).")
DEFINE_SCRIPTFUNC(IsDialogueEnabled, "Is dialogue enabled in audio settings?")
DEFINE_SCRIPTFUNC(SetPortalGunIndicatorColor, "Sets the color of portal gun indicator. Set to 0,0,0 to use default.")
DEFINE_SCRIPTFUNC(SetScreenCoverColor, "Sets color that covers the whole screen.")
DEFINE_SCRIPTFUNC(PrecacheModel, "Precaches model")
DEFINE_SCRIPTFUNC(GetBackupKey, "Gets currently set key used by script to recover parameters.")
DEFINE_SCRIPTFUNC(SetBackupKey, "Sets backup key used by script to recover parameters.")
DEFINE_SCRIPTFUNC(GetModeParamsNumber, "Maximum number of parameters you can assign.")
DEFINE_SCRIPTFUNC(AreModeParamsChanged, "Used by backup system. Returns true once if change to mode-specifc params was made.")
DEFINE_SCRIPTFUNC(RefreshEntity, "Executes 'Activate()' function for given entity.")
END_SCRIPTDESC()

NightmarePlugin::NightmarePlugin()
    : game(Game::CreateNew())
    , plugin(new Plugin())
    , modules(new Modules())
    , cheats(new Cheats())
    , clients()
    , mode(0)
{

}

// Used callbacks
bool NightmarePlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
    console = new Console();
    if (!console->Init())
        return false;

    if (this->game) {
        this->game->LoadOffsets();

        this->modules->AddModule<Tier1>(&tier1);
        this->modules->InitAll();

        if (tier1 && tier1->hasLoaded) {
            this->cheats->Init();

            this->modules->AddModule<Engine>(&engine);
            this->modules->AddModule<Client>(&client);
            this->modules->AddModule<Server>(&server);
            this->modules->AddModule<VScript>(&vscript);
            this->modules->AddModule<Surface>(&surface);
            this->modules->AddModule<VGui>(&vgui);
            this->modules->InitAll();

            if (engine && client && engine->hasLoaded && client->hasLoaded && server->hasLoaded) {

                this->StartMainThread();

                console->Print("Speedrun Mode Simple Modifier loaded (version %s)\n", this->Version());
                return true;
            } else {
                console->Warning("nightmarePlugin: Failed to load engine and client modules!\n");
            }
        } else {
            console->Warning("nightmarePlugin: Failed to load tier1 module!\n");
        }
    } else {
        console->Warning("nightmarePlugin: Game not supported!\n");
    }

    console->Warning("nightmarePlugin: Plugin failed to load!\n");
    return false;
}
void NightmarePlugin::Unload() {
    this->Cleanup();
}
const char* NightmarePlugin::GetPluginDescription() {
    return NightmarePlugin_SIGNATURE;
}
void NightmarePlugin::LevelShutdown() {
    console->DevMsg("NightmarePlugin::LevelShutdown\n");

    staminaHud->SetStaminaColor(Color(0, 0, 0, 0));
    this->SetBackupKey("0");

    // Make sure to clear the list after sending any client-side shutdown commands
    this->clients.clear();
}

void NightmarePlugin::ClientActive(void* pEntity) {
    console->DevMsg("NightmarePlugin::ClientActive -> pEntity: %p\n", pEntity);
    client->SetPortalGunIndicatorColor(Vector(0, 0, 0));

    if (!this->clients.empty() && this->clients.at(0) == pEntity) {

    }
}

void NightmarePlugin::ClientFullyConnect(void* pEdict) {
    this->clients.push_back(pEdict);
}

void NightmarePlugin::Cleanup() {
    if (console)
        console->Print("NightmarePlugin disabled.\n");

    if (this->cheats)
        this->cheats->Shutdown();

    if (this->modules)
        this->modules->ShutdownAll();

    SAFE_UNLOAD(this->cheats);
    SAFE_UNLOAD(this->game);
    SAFE_UNLOAD(this->plugin);
    SAFE_UNLOAD(this->modules);
    SAFE_UNLOAD(console);
}

//adds console line parameter in order to override background
void NightmarePlugin::ForceAct5MenuBackground() {
    console->CommandLine()->AppendParm("-background", "5");
}


void NightmarePlugin::SetMode(int mode) {
    this->mode = mode;
    //reset param table and other stuff when switching modes
    this->ResetModeVariables();
    if (this->mode != mode)this->paramsChanged = true;
}

void NightmarePlugin::ResetModeVariables() {
    for (int i = 0; i < MAX_MODE_PARAMETERS; i++) {
        this->modeParams[i] = 0.0f;
    }
}

void NightmarePlugin::PrecacheModel(const char* pName, bool bPreload) {
    engine->PrecacheModel(pName, bPreload);
}

void NightmarePlugin::SetBackupKey(const char* key) {
    backupKey = key;
    this->paramsChanged = true;
}

const char* NightmarePlugin::GetBackupKey() {
    return this->backupKey;
}

float NightmarePlugin::GetModeParam(int id) {
    if (id < 0 || id >= MAX_MODE_PARAMETERS)return -1.0f;
    return this->modeParams[id];
}

bool NightmarePlugin::SetModeParam(int id, float value) {
    if (id < 0 || id >= MAX_MODE_PARAMETERS)return false;
    if (this->modeParams[id] != value)this->paramsChanged = true;
    this->modeParams[id] = value;
    return true;
}

bool NightmarePlugin::AreModeParamsChanged() {
    bool changed = this->paramsChanged;
    if(changed)this->paramsChanged = false;
    return changed;
}

void NightmarePlugin::SetPortalGunIndicatorColor(Vector color) {
    client->SetPortalGunIndicatorColor(color);
}

void NightmarePlugin::SetScreenCoverColor(int r, int g, int b, int a) {
    staminaHud->SetStaminaColor(Color(r, g, b, a));
}

bool NightmarePlugin::IsDialogueEnabled() {
    return puzzlemaker_play_sounds.GetBool();
}

void NightmarePlugin::RefreshEntity(HSCRIPT hScript) {
    void* ent = (hScript) ? (void*)vscript->g_pScriptVM->GetInstanceValue(hScript) : NULL;
    if (ent) {
        using _Activate = void(__func*)(void* thisptr);
        using _Spawn = void(__func*)(void* thisptr);
        _Activate Activate = Memory::VMT<_Activate>(ent, Offsets::CBaseEntityActivate);
        _Spawn Spawn = Memory::VMT<_Spawn>(ent, Offsets::CBaseEntityActivate);
        Activate(ent);
        Spawn(ent);
    }
}



void NightmarePlugin::StartMainThread() {
    this->ForceAct5MenuBackground();
}

// Might fix potential deadlock
#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_DETACH) {
        nightmarePlugin.Cleanup();
    }
    return TRUE;
}
#endif

#pragma region Unused callbacks
void NightmarePlugin::LevelInit(char const* pMapName)
{
}
void NightmarePlugin::Pause() 
{
}
void NightmarePlugin::UnPause() 
{
}
void NightmarePlugin::GameFrame(bool simulating)
{
    isPaused = !simulating;
}
void NightmarePlugin::ClientDisconnect(void* pEntity)
{
}
void NightmarePlugin::ClientPutInServer(void* pEntity, char const* playername)
{
}
void NightmarePlugin::SetCommandClient(int index)
{
}
void NightmarePlugin::ClientSettingsChanged(void* pEdict)
{
}
int NightmarePlugin::ClientCommand(void* pEntity, const void*& args)
{
    return 0;
}
int NightmarePlugin::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID)
{
    return 0;
}
void NightmarePlugin::OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue)
{
}
void NightmarePlugin::OnEdictAllocated(void* edict)
{
}
void NightmarePlugin::OnEdictFreed(const void* edict)
{
}
int NightmarePlugin::ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) {
    return 0;
}
void NightmarePlugin::ServerActivate(void* pEdictList, int edictCount, int clientMax) {

}
#pragma endregion

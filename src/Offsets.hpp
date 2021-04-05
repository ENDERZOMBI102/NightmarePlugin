#pragma once

namespace Offsets {

// CCvar
extern int RegisterConCommand;
extern int UnregisterConCommand;
extern int FindCommandBase;
extern int m_pConCommandList;

// CEngineClient
extern int GetScreenSize;
extern int ClientCmd;
extern int GetActiveSplitScreenPlayerSlot;
extern int RenderView;
extern int HudUpdate;

// ConVar
extern int InternalSetValue;
extern int InternalSetFloatValue;
extern int InternalSetIntValue;

// CMatSystemSurface
extern int DrawSetColor;
extern int DrawFilledRect;
extern int DrawLine;
extern int DrawSetTextFont;
extern int DrawSetTextColor;
extern int GetFontTall;
extern int PaintTraverseEx;
extern int DrawColoredText;
extern int DrawTextLen;
extern int DisableClipping;
extern int StartDrawing;
extern int FinishDrawing;

extern int DrawGetTextureId;
extern int DrawGetTextureFile;
extern int DrawSetTextureFile;
extern int DrawSetTextureRGBA;
extern int DrawSetTexture;
extern int DrawGetTextureSize;
extern int DrawTexturedRect;
extern int IsTextureIDValid;
extern int CreateNewTextureID;

// CClientState
extern int SetSignonState;

// CVEngineServer
extern int ClientCommand;

// CBaseHudChat
extern int ChatPrintf;

// CCommandBuffer
extern int m_bWaitEnabled;

// IEngineTrace
extern int TraceRay;

// IEngineTool
extern int PrecacheModel;
extern int GetWorldToScreenMatrixForView;

// CSchemeManager
extern int GetIScheme;

// CScheme
extern int GetFont;

// CClientTools
extern int NextParticleSystem;

// CBaseEntity
extern int m_fFlags;
extern int m_nTickBase;
extern int m_bDucking;
extern int m_hActiveWeapon;
extern int m_bCanFirePortal1;
extern int m_hUseEntity;
extern int m_hGroundEntity;

// CServerGameDLL
extern int Think;

// CScriptManager
extern int CreateVM;

// IParticleEffect
extern int RenderParticles;

// IEngineVGuiInternal
extern int Paint;

// CServerTools
extern int GetIServerEntity;

// Others
extern int GetClientStateFunction;
extern int cl;
extern int AutoCompletionFunc;
extern int HostState_OnClientConnected;
extern int hoststate;
extern int Cbuf_AddText;
extern int s_CommandBuffer;
extern int CCommandBufferSize;
extern int GetHud;
extern int FindElement;
extern int ProcessMovement;
extern int UTIL_PlayerByIndex;
extern int gpGlobals;
extern int m_EntPtrArray;
extern int CBaseEntityActivate;
extern int CBaseEntitySpawn;
}

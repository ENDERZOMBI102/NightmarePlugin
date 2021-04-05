#pragma once
#include <stdarg.h>

#include "Module.hpp"

#include "../Interface.hpp"
#include "../Offsets.hpp"
#include "../Utils.hpp"

class Surface : public Module {
public:
    Interface* matsurface = nullptr;

    typedef unsigned long HFont;

    using _DrawSetColor = int(__func*)(void* thisptr, int r, int g, int b, int a);
    using _DrawFilledRect = int(__func*)(void* thisptr, int x0, int y0, int x1, int y1);
    using _DrawLine = int(__func*)(void* thisptr, int x0, int y0, int x1, int y1);
    using _DrawSetTextFont = int(__func*)(void* thisptr, HFont font);
    using _DrawSetTextColor = int(__func*)(void* thisptr, Color color);
#ifdef _WIN32
    using _GetFontTall = int(__stdcall*)(HFont font);
#else
    using _GetFontTall = int(__cdecl*)(void* thisptr, HFont font);
#endif
    using _DrawColoredText = int(__cdecl*)(void* thisptr, HFont font, int x, int y, int r, int g, int b, int a, char* fmt, ...);
    using _DrawTextLen = int(__cdecl*)(void* thisptr, HFont font, char* fmt, ...);
    using _DisableClipping = int(__cdecl*)(void* thisptr, bool disable);
    using _StartDrawing = int(__func*)(void* thisptr);
    using _FinishDrawing = int(__cdecl*)();

    using _DrawGetTextureId = int(__func*)(void* thisptr, char const* filename);
    using _DrawGetTextureFile = int(__func*)(void* thisptr, int id, char* filename, int maxlen);
    using _DrawSetTextureFile = int(__func*)(void* thisptr, int id, const char* filename, int hardwareFilter, bool forceReload);
    using _DrawSetTextureRGBA = int(__func*)(void* thisptr, int id, const unsigned char* rgba, int wide, int tall);
    using _DrawSetTexture = int(__func*)(void* thisptr, int id);
    using _DrawGetTextureSize = int(__func*)(void* thisptr, int id, int& wide, int& tall);
    using _DrawTexturedRect = int(__func*)(void* thisptr, int x0, int y0, int x1, int y1);
    using _IsTextureIDValid = int(__func*)(void* thisptr, int id);
    using _CreateNewTextureID = int(__func*)(void* thisptr, bool procedural);


    _GetFontTall GetFontTall = nullptr;
    _DrawSetColor DrawSetColor = nullptr;
    _DrawFilledRect DrawFilledRect = nullptr;
    _DrawLine DrawLine = nullptr;
    _DrawSetTextFont DrawSetTextFont = nullptr;
    _DrawSetTextColor DrawSetTextColor = nullptr;
    _DrawColoredText DrawColoredText = nullptr;
    _DrawTextLen DrawTextLen = nullptr;
    _DisableClipping DisableClipping = nullptr;
    _StartDrawing StartDrawing = nullptr;
    _FinishDrawing FinishDrawing = nullptr;

    _DrawGetTextureId DrawGetTextureId = nullptr;
    _DrawGetTextureFile DrawGetTextureFile = nullptr;
    _DrawSetTextureFile DrawSetTextureFile = nullptr;
    _DrawSetTextureRGBA DrawSetTextureRGBA = nullptr;
    _DrawSetTexture DrawSetTexture = nullptr;
    _DrawGetTextureSize DrawGetTextureSize = nullptr;
    _DrawTexturedRect DrawTexturedRect = nullptr;
    _IsTextureIDValid IsTextureIDValid = nullptr;
    _CreateNewTextureID CreateNewTextureID = nullptr;

public:
    int GetFontHeight(HFont font);
    int GetFontLength(HFont font, const char* fmt, ...);
    void DrawTxt(HFont font, int x, int y, Color clr, const char* fmt, ...);
    void DrawRect(Color clr, int x0, int y0, int x1, int y1);
    void DrawRectAndCenterTxt(Color clr, int x0, int y0, int x1, int y1, HFont font, Color fontClr, const char* fmt, ...);
    int GetWidth();
    int GetHeight();

    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE("vguimatsurface"); }
};

extern Surface* surface;

#include "Surface.hpp"

#include <stdarg.h>

#include "../Interface.hpp"
#include "Module.hpp"
#include "../Offsets.hpp"
#include "../Utils.hpp"
#include "Engine.hpp"

int Surface::GetFontHeight(HFont font)
{
#ifdef _WIN32
    return this->GetFontTall(font);
#else
    return this->GetFontTall(this->matsurface->ThisPtr(), font);
#endif
}
int Surface::GetFontLength(HFont font, const char* fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    char data[1024];
    vsnprintf(data, sizeof(data), fmt, argptr);
    va_end(argptr);
    return this->DrawTextLen(this->matsurface->ThisPtr(), font, data);
}
void Surface::DrawTxt(HFont font, int x, int y, Color clr, const char* fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    char data[1024];
    vsnprintf(data, sizeof(data), fmt, argptr);
    va_end(argptr);
    this->DrawColoredText(this->matsurface->ThisPtr(), font, x, y, clr.r(), clr.g(), clr.b(), clr.a(), data);
}
void Surface::DrawRect(Color clr, int x0, int y0, int x1, int y1)
{
    this->DrawSetColor(this->matsurface->ThisPtr(), clr.r(), clr.g(), clr.b(), clr.a());
    this->DrawFilledRect(this->matsurface->ThisPtr(), x0, y0, x1, y1);
    this->DrawSetColor(this->matsurface->ThisPtr(), 0,0,0,0);
}
void Surface::DrawRectAndCenterTxt(Color clr, int x0, int y0, int x1, int y1, HFont font, Color fontClr, const char* fmt, ...)
{
    this->DrawRect(clr, x0, y0, x1, y1);

    va_list argptr;
    va_start(argptr, fmt);
    char data[1024];
    vsnprintf(data, sizeof(data), fmt, argptr);
    va_end(argptr);

    auto tw = this->GetFontLength(font, data);
    auto th = this->GetFontHeight(font);

    // Center of rectangle
    auto xc = x0 + ((x1 - x0) / 2);
    auto yc = y0 + ((y1 - y0) / 2);

    this->DrawTxt(font, xc - (tw / 2), yc - (th / 2), fontClr, data);
}
int Surface::GetWidth() {
    int width, height;
    engine->GetScreenSize(width, height);
    return width;
}
int Surface::GetHeight() {
    int width, height;
    engine->GetScreenSize(width, height);
    return height;
}

bool Surface::Init()
{
    this->matsurface = Interface::Create(this->Name(), "VGUI_Surface0", false);
    if (this->matsurface) {
        this->DrawSetColor = matsurface->Original<_DrawSetColor>(Offsets::DrawSetColor);
        this->DrawFilledRect = matsurface->Original<_DrawFilledRect>(Offsets::DrawFilledRect);
        this->DrawLine = matsurface->Original<_DrawLine>(Offsets::DrawLine);
        this->DrawSetTextFont = matsurface->Original<_DrawSetTextFont>(Offsets::DrawSetTextFont);
        this->DrawSetTextColor = matsurface->Original<_DrawSetTextColor>(Offsets::DrawSetTextColor);
        this->GetFontTall = matsurface->Original<_GetFontTall>(Offsets::GetFontTall);
        this->DrawColoredText = matsurface->Original<_DrawColoredText>(Offsets::DrawColoredText);
        this->DisableClipping = matsurface->Original<_DisableClipping>(Offsets::DisableClipping);
        this->DrawTextLen = matsurface->Original<_DrawTextLen>(Offsets::DrawTextLen);

        this->DrawGetTextureId = matsurface->Original<_DrawGetTextureId>(Offsets::DrawGetTextureId);
        this->DrawGetTextureFile = matsurface->Original<_DrawGetTextureFile>(Offsets::DrawGetTextureFile);
        this->DrawSetTextureFile = matsurface->Original<_DrawSetTextureFile>(Offsets::DrawSetTextureFile);
        this->DrawSetTextureRGBA = matsurface->Original<_DrawSetTextureRGBA>(Offsets::DrawSetTextureRGBA);
        this->DrawSetTexture = matsurface->Original<_DrawSetTexture>(Offsets::DrawSetTexture);
        this->DrawGetTextureSize = matsurface->Original<_DrawGetTextureSize>(Offsets::DrawGetTextureSize);
        this->DrawTexturedRect = matsurface->Original<_DrawTexturedRect>(Offsets::DrawTexturedRect);
        this->IsTextureIDValid = matsurface->Original<_IsTextureIDValid>(Offsets::IsTextureIDValid);
        this->CreateNewTextureID = matsurface->Original<_CreateNewTextureID>(Offsets::CreateNewTextureID);

        auto PaintTraverseEx = matsurface->Original(Offsets::PaintTraverseEx);
        this->StartDrawing = Memory::Read<_StartDrawing>(PaintTraverseEx + Offsets::StartDrawing);
        this->FinishDrawing = Memory::Read<_FinishDrawing>(PaintTraverseEx + Offsets::FinishDrawing);
    }

    return this->hasLoaded = this->matsurface;
}
void Surface::Shutdown()
{
    Interface::Delete(this->matsurface);
}

Surface* surface;

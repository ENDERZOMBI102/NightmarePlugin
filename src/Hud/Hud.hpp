#pragma once

#include "../Utils/SDK.hpp"

/**
 * Abstract class representing a bare-bones HUD element
 *
 * An object of a subclass of this class is considered an in-game HUD.
 */
class Hud {
public:
    /**
     *
     * @return if this element should be drawn on top (of what?)
     */
	virtual bool DrawingOnTop() {return false;}
    /**
     * Draws this element
     */
	virtual void Draw();
};




class StaminaHud : public Hud {
private:
    int staminaTexture = 0;
    Color staminaColor;
public:
    void Draw();
    int GetStaminaTexture();
    void SetStaminaColor(Color c) { staminaColor = c; };
};

extern StaminaHud* staminaHud;



class CelesteBerryHud : public Hud {
private:
    int berryTexture[8];
    int indicatorTexture = 0;

    int displayBerryCount = 0;
    int oldBerryCount = 0;
    float posOffset = 0.0;
    float posOffset2 = 0.0; //ah, yes, good variable name // ENDER approves
    float collectAnimState = 0.0;
    float prevRealTime = 0;
public:
    void Draw();
    bool DrawingOnTop() { return true; }
    int GetBerryTexture(int type);
};

extern CelesteBerryHud* celesteBerryHud;
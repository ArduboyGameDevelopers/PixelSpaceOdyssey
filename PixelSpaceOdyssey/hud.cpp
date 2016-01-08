#include "hud.h"

#include <core.h>

#include "game.h"
#include "drawing.h"
#include "images.h"

static const int16_t kHudWidth = 128;
static const int16_t kHudHeight = 8;

static const int16_t kHudX = 0;
static const int16_t kHudY = 64 - kHudHeight;

static const int16_t kHudGunX = 1;
static const int16_t kHudGunY = 3;

static const int16_t kHudAmmoX = 9;
static const int16_t kHudAmmoY = 3;

static const int16_t kHudHeartX = 26;
static const int16_t kHudHeartY = 3;

static const int16_t kHudHealthX = 32;
static const int16_t kHudHealthY = 3;

static const int16_t kHudLogoX = 57;
static const int16_t kHudLogoY = 0;

static inline void drawAmmo(int remains)
{
    int x = kHudAmmoX;
    int y = kHudAmmoY;
    
    for (int i = 0; i < remains; ++i)
    {
        fillRect(x, y, 1, 4, WHITE);
        x += 2;
    }
}

static inline void drawHealth(int remains)
{
    int x = kHudHealthX;
    int y = kHudHealthY;
    
    if (remains > 0)
    {
        int w = remains * 2 + 1;
        fillRect(x, y, w, 4, WHITE);
        
        ++x;
        
        for (int i = 0; i < remains; ++i)
        {
            fillRect(x, y, 1, 2, BLACK);
            x += 2;
        }
    }
    
    int empty = kPlayerHealthMax - remains;
    ++x;
    
    for (int i = 0; i < empty; ++i)
    {
        setPixel(x, y, WHITE);
        x += 2;
    }
}

void drawHud()
{
    int16_t oldDrawTransX = drawTransX;
    int16_t oldDrawTransY = drawTransY;
    
    drawTransX = kHudX;
    drawTransY = kHudY;
    
    fillRect(0, 0, kHudWidth, kHudHeight, BLACK);
    
    // gun
    drawImagePtr(IMG_HUD_GUN, kHudGunX, kHudGunY, DM_LIT);
    
    // ammo
    drawAmmo(7);
    
    // heart
    drawImagePtr(IMG_HUD_HEART, kHudHeartX, kHudHeartY, DM_LIT);
    
    // health
    drawHealth(playerHealth);
    
    // logo
    drawImagePtr(IMG_HUD_LOGO, kHudLogoX, kHudLogoY, DM_LIT);
    
    drawTransX = oldDrawTransX;
    drawTransY = oldDrawTransY;
}

// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2024

#include <nds.h>

// This is autogenerated from city.png and city.grit
#include "city.h"

int main(int argc, char *argv[])
{
    videoSetMode(MODE_0_2D);

    vramSetPrimaryBanks(VRAM_A_MAIN_BG, VRAM_B_LCD, VRAM_C_LCD, VRAM_D_LCD);

    int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0,1);

    dmaCopy(cityTiles, bgGetGfxPtr(bg), cityTilesLen);
    dmaCopy(cityMap, bgGetMapPtr(bg), cityMapLen);
    dmaCopy(cityPal, BG_PALETTE, cityPalLen);

    // Setup two windows. Window 0 has priority over window 1. In this example,
    // windows 0 is a small rectangle in the center of the screen, and window 1
    // is a rectangle that covers most of the screen. First, the outside of the
    // big rectangle isn't inside window 1, so the background isn't visible. The
    // inside of window 1 has enabled the display of the background, so it is
    // visible. Last, windows 0 has disabled the background, and it takes
    // priority over window 1, so it isn't visible inside.
    windowSetBounds(WINDOW_0, 48, 48, SCREEN_WIDTH - 48, SCREEN_HEIGHT - 48);
    windowSetBounds(WINDOW_1, 16, 16, SCREEN_WIDTH - 16, SCREEN_HEIGHT - 16);
    bgWindowDisable(bg, WINDOW_0);
    bgWindowEnable(bg, WINDOW_1);
    bgWindowDisable(bg, WINDOW_OUT);
    windowEnable(WINDOW_0 | WINDOW_1);

    consoleDemoInit();

    printf("PAD:   Scroll background\n");
    printf("START: Exit to loader\n");

    int x = 150, y = 70;

    while (1)
    {
        swiWaitForVBlank();

        bgSetScroll(0, x, y);

        bgUpdate();

        scanKeys();

        u16 keys_held = keysHeld();

        if (keys_held & KEY_UP)
            y--;
        else if (keys_held & KEY_DOWN)
            y++;

        if (keys_held & KEY_LEFT)
            x--;
        else if (keys_held & KEY_RIGHT)
            x++;

        if (keys_held & KEY_START)
            break;
    }

    return 0;
}

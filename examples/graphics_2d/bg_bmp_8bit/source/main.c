// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2024

#include <nds.h>

// This is autogenerated from manga_bg.png and manga_bg.grit
#include "manga_bg.h"

int main(int argc, char *argv[])
{
    // Mode 5 lets you use layers 2 and 3 as a bitmap
    videoSetMode(MODE_5_2D);

    // The screen is 256x192 pixels. A 8-bit bitmap that size requires 48 KB
    // (256 x 192 / 1024). Each VRAM bank is 128 KB, so we need one bank to
    // have a double buffer setup.
    vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06000000, VRAM_B_LCD, VRAM_C_LCD,
                        VRAM_D_LCD);

    int bg = bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

    consoleDemoInit();

    printf("PAD:   Move square\n");
    printf("START: Exit to loader\n");

    int x = 20, y = 30;

    // Load the palette
    dmaCopy(manga_bgPal, BG_PALETTE, manga_bgPalLen);

    // Set the last color to magenta
    BG_PALETTE[255] = RGB5(31, 0, 31);

    while (1)
    {
        swiWaitForVBlank();

        uint16_t *backbuffer = bgGetGfxPtr(bg);

        // Bitmap base 0 is at offset 0 (start of VRAM A) and bitmap base 8 is
        // at offset 64 KB (4 * 128 * 128 B) (middle of VRAM A).
        if (bgGetMapBase(bg) == 4)
            bgSetMapBase(bg, 0);
        else
            bgSetMapBase(bg, 4);

        // Draw scene. Copy background image and draw a square on top of it.
        dmaCopy(manga_bgBitmap, backbuffer, manga_bgBitmapLen);

        for (int j = y; j < 50 + y; j++)
        {
            for (int i = x; i < 50 + x; i += 2)
            {
                // Set two pixels to the last color of the palette at once
                backbuffer[256 * j / 2 + i / 2] = 255 | (255 << 8);
            }
        }

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

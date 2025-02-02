/**
 * @file DemoSignature.c
 * @brief Signature demo
 *
 * @author Bridgetek
 *
 * @date 2019
 *
 * MIT License
 *
 * Copyright (c) [2019] [Bridgetek Pte Ltd (BRTChip)]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "Common.h"
#include "Platform.h"
#include "EVE_CoCmd.h"
#include "DemoSignature.h"

static EVE_HalContext s_halContext;
static EVE_HalContext* s_pHalContext;
void DemoSignature();

// ************************************ main loop ************************************
int main(int argc, char* argv[])
{
	s_pHalContext = &s_halContext;
	Gpu_Init(s_pHalContext);

	// read and store calibration setting
#if !defined(BT8XXEMU_PLATFORM) && GET_CALIBRATION == 1
	Esd_Calibrate(s_pHalContext);
	Calibration_Save(s_pHalContext);
#endif

	Flash_Init(s_pHalContext, TEST_DIR "/Flash/BT81X_Flash.bin", "BT81X_Flash.bin");
	EVE_Util_clearScreen(s_pHalContext);

	char* info[] =
	{ "Signature demo",
		"Support QVGA, WQVGA, WVGA",
		"EVE1/2/3/4",
		"WIN32, FT9XX, IDM2040"
	};

	while (TRUE) {
		WelcomeScreen(s_pHalContext, info);
		DemoSignature();
		EVE_Util_clearScreen(s_pHalContext);
		EVE_Hal_close(s_pHalContext);
		EVE_Hal_release();

		/* Init HW Hal for next loop*/
		Gpu_Init(s_pHalContext);
#if !defined(BT8XXEMU_PLATFORM) && GET_CALIBRATION == 1
		Calibration_Restore(s_pHalContext);
#endif
	}
	return 0;
}

// ************************************ application ************************************
#define BACKGROUND_STAR_HANDLE 13
#if defined(FT80X_ENABLE)
#define RAM_G_END_ADDR 0x40000 //General purpose graphics RAM 256 kB
#elif defined(FT81X_ENABLE)
#define RAM_G_END_ADDR (0x100000) //General purpose graphics RAM 1024 kB
#else
#warning "Should select a GPU chip in Platform.h"
#endif
#define SIZE_HOME_START_ICON (460) 
#define SIZE_LOGO (6703)

#define START_ICON_ADDR (RAM_G_END_ADDR  - SIZE_HOME_START_ICON*6) //*6 to Reserve space for inflate images.
#define LOGO_ADDR       (START_ICON_ADDR - SIZE_LOGO)

#define START_ICON_HANDLE 14
#define LOGO_ICON_HANDLE 15

static void rotate_around(int16_t x, int16_t y, int16_t a)
{
	EVE_CoCmd_loadIdentity(s_pHalContext);
	EVE_CoCmd_translate(s_pHalContext, F16(x), F16(y));
	EVE_CoCmd_rotate(s_pHalContext, a);
	EVE_CoCmd_translate(s_pHalContext, F16(-x), F16(-y));
	EVE_CoCmd_setMatrix(s_pHalContext);
}

void InitStar() {
	const char home_start_icon[SIZE_HOME_START_ICON] = { 0x78,0x9C,0xE5,0x94,0xBF,0x4E,0xC2,0x40,0x1C,0xC7,0x7F,0x2D,0x04,0x8B,0x20,0x45,0x76,0x14,0x67,0xA3,0xF1,0x0D,0x64,0x75,0xD2,0xD5,0x09,0x27,0x17,0x13,0xE1,0x0D,0xE4,0x0D,0x78,0x04,0x98,0x5D,0x30,0x26,0x0E,0x4A,0xA2,0x3E,0x82,0x0E,0x8E,0x82,0xC1,0x38,0x62,0x51,0x0C,0x0A,0x42,0x7F,0xDE,0xB5,0x77,0xB4,0x77,0x17,0x28,0x21,0x26,0x46,0xFD,0x26,0xCD,0xE5,0xD3,0x7C,0xFB,0xBB,0xFB,0xFD,0xB9,0x02,0xCC,0xA4,0xE8,0x99,0x80,0x61,0xC4,0x8A,0x9F,0xCB,0x6F,0x31,0x3B,0xE3,0x61,0x7A,0x98,0x84,0x7C,0x37,0xF6,0xFC,0xC8,0xDD,0x45,0x00,0xDD,0xBA,0xC4,0x77,0xE6,0xEE,0x40,0xEC,0x0E,0xE6,0x91,0xF1,0xD2,0x00,0x42,0x34,0x5E,0xCE,0xE5,0x08,0x16,0xA0,0x84,0x68,0x67,0xB4,0x86,0xC3,0xD5,0x26,0x2C,0x20,0x51,0x17,0xA2,0xB8,0x03,0xB0,0xFE,0x49,0xDD,0x54,0x15,0xD8,0xEE,0x73,0x37,0x95,0x9D,0xD4,0x1A,0xB7,0xA5,0x26,0xC4,0x91,0xA9,0x0B,0x06,0xEE,0x72,0xB7,0xFB,0xC5,0x16,0x80,0xE9,0xF1,0x07,0x8D,0x3F,0x15,0x5F,0x1C,0x0B,0xFC,0x0A,0x90,0xF0,0xF3,0x09,0xA9,0x90,0xC4,0xC6,0x37,0xB0,0x93,0xBF,0xE1,0x71,0xDB,0xA9,0xD7,0x41,0xAD,0x46,0xEA,0x19,0xA9,0xD5,0xCE,0x93,0xB3,0x35,0x73,0x0A,0x69,0x59,0x91,0xC3,0x0F,0x22,0x1B,0x1D,0x91,0x13,0x3D,0x91,0x73,0x43,0xF1,0x6C,0x55,0xDA,0x3A,0x4F,0xBA,0x25,0xCE,0x4F,0x04,0xF1,0xC5,0xCF,0x71,0xDA,0x3C,0xD7,0xB9,0xB2,0x48,0xB4,0x89,0x38,0x20,0x4B,0x2A,0x95,0x0C,0xD5,0xEF,0x5B,0xAD,0x96,0x45,0x8A,0x41,0x96,0x7A,0x1F,0x60,0x0D,0x7D,0x22,0x75,0x82,0x2B,0x0F,0xFB,0xCE,0x51,0x3D,0x2E,0x3A,0x21,0xF3,0x1C,0xD9,0x38,0x86,0x2C,0xC6,0x05,0xB6,0x7B,0x9A,0x8F,0x0F,0x97,0x1B,0x72,0x6F,0x1C,0xEB,0xAE,0xFF,0xDA,0x97,0x0D,0xBA,0x43,0x32,0xCA,0x66,0x34,0x3D,0x54,0xCB,0x24,0x9B,0x43,0xF2,0x70,0x3E,0x42,0xBB,0xA0,0x95,0x11,0x37,0x46,0xE1,0x4F,0x49,0xC5,0x1B,0xFC,0x3C,0x3A,0x3E,0xD1,0x65,0x0E,0x6F,0x58,0xF8,0x9E,0x5B,0xDB,0x55,0xB6,0x41,0x34,0xCB,0xBE,0xDB,0x87,0x5F,0xA9,0xD1,0x85,0x6B,0xB3,0x17,0x9C,0x61,0x0C,0x9B,0xA2,0x5D,0x61,0x10,0xED,0x2A,0x9B,0xA2,0x5D,0x61,0x10,0xED,0x2A,0x9B,0xA2,0x5D,0x61,0x10,0xED,0x2A,0x9B,0xED,0xC9,0xFC,0xDF,0x14,0x54,0x8F,0x80,0x7A,0x06,0xF5,0x23,0xA0,0x9F,0x41,0xF3,0x10,0x30,0x4F,0x41,0xF3,0x18,0x30,0xCF,0xCA,0xFC,0xFF,0x35,0xC9,0x79,0xC9,0x89,0xFA,0x33,0xD7,0x1D,0xF6,0x5E,0x84,0x5C,0x56,0x6E,0xA7,0xDA,0x1E,0xF9,0xFA,0xAB,0xF5,0x97,0xFF,0x2F,0xED,0x89,0x7E,0x29,0x9E,0xB4,0x9F,0x74,0x1E,0x69,0xDA,0xA4,0x9F,0x81,0x94,0xEF,0x4F,0xF6,0xF9,0x0B,0xF4,0x65,0x51,0x08 };
	Gpu_Hal_WrCmd32(s_pHalContext, CMD_INFLATE);
	Gpu_Hal_WrCmd32(s_pHalContext, START_ICON_ADDR);
	Gpu_Hal_WrCmdBuf(s_pHalContext, home_start_icon, sizeof(home_start_icon)); //Load from RAM

}

void DemoSignature() {
	uint16_t w, h, x, y, tag;

	int16_t sw = 2 * s_pHalContext->Width / 3;
	int16_t sh = sw / 3;
	int16_t ox = (s_pHalContext->Width - sw) / 2;
	int16_t oy = (2 * s_pHalContext->Height / 3) - sh;
	uint16_t a = 0;

	x = s_pHalContext->Width * 0.168;
	y = s_pHalContext->Height * 0.317;
	w = s_pHalContext->Width - (2 * x);
	h = s_pHalContext->Height - (2.5 * y);

	InitStar();

	EVE_CoCmd_dlStart(s_pHalContext);        // start

	/* Initialize the Star array */
	EVE_Cmd_wr32(s_pHalContext, BITMAP_HANDLE(BACKGROUND_STAR_HANDLE));    // handle for background stars
	EVE_Cmd_wr32(s_pHalContext, BITMAP_SOURCE(START_ICON_ADDR));      // Starting address in gram
	EVE_Cmd_wr32(s_pHalContext, BITMAP_LAYOUT(L4, 16, 32));  // format 
	EVE_Cmd_wr32(s_pHalContext, BITMAP_SIZE(NEAREST, REPEAT, REPEAT, 512, 512));

	EVE_Cmd_wr32(s_pHalContext, CLEAR(1, 1, 1));
	EVE_CoCmd_memZero(s_pHalContext, RAM_G, 480L * 272L); // Clear the gram frm 1024 
#if defined FT801_ENABLE
	EVE_CoCmd_cSketch(s_pHalContext, x, y, w, h, RAM_G, L8, 1500L);
#else
	EVE_CoCmd_sketch(s_pHalContext, x, y, w, h, RAM_G, L8);
#endif
	EVE_Cmd_wr32(s_pHalContext, BITMAP_HANDLE(1));    // handle for background stars
	EVE_Cmd_wr32(s_pHalContext, BITMAP_SOURCE(RAM_G));
	EVE_Cmd_wr32(s_pHalContext, BITMAP_LAYOUT(L8, w, h));
#if defined(FT81X_ENABLE)
	EVE_Cmd_wr32(s_pHalContext, BITMAP_LAYOUT_H(w >> 10, h >> 9));
#endif
	EVE_Cmd_wr32(s_pHalContext, BITMAP_SIZE(NEAREST, BORDER, BORDER, w, h));
#if defined(FT81X_ENABLE)
	EVE_Cmd_wr32(s_pHalContext, BITMAP_SIZE_H(w >> 9, h >> 9));
#endif

	EVE_CoCmd_swap(s_pHalContext);
	EVE_Cmd_waitFlush(s_pHalContext);

	while (1)
	{
		//  read the Tag 
		tag = Gesture_GetTag(s_pHalContext);
		// Clear the GRAM when the Clear button enter by user 
		if (tag == 'O')
		{
			Play_Sound(s_pHalContext, 0x50, 255, 0xc0);
			EVE_CoCmd_dlStart(s_pHalContext);
			EVE_CoCmd_memZero(s_pHalContext, RAM_G, 480L * 272L); // Clear the gram frm 1024 		
			EVE_Cmd_waitFlush(s_pHalContext);
		}
		EVE_CoCmd_dlStart(s_pHalContext);        // start
		EVE_Cmd_wr32(s_pHalContext, CLEAR(1, 1, 1));
		EVE_Cmd_wr32(s_pHalContext, BEGIN(BITMAPS));
		EVE_Cmd_wr32(s_pHalContext, SAVE_CONTEXT());
		EVE_Cmd_wr32(s_pHalContext, BLEND_FUNC(SRC_ALPHA, ONE));
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(78, 0, 0));
		// Background animation  
		EVE_Cmd_wr32(s_pHalContext, CMD_LOADIDENTITY);
		rotate_around(ox, oy, 47 * a);
		EVE_Cmd_wr32(s_pHalContext, VERTEX2II(0, 0, BACKGROUND_STAR_HANDLE, 1));

		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 40, 0));
		rotate_around(ox + sw, oy, 53 * a);
		EVE_Cmd_wr32(s_pHalContext, VERTEX2II(0, 0, BACKGROUND_STAR_HANDLE, 1));

		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 0, 78));
		rotate_around(ox, oy + sh, 57 * a);
		EVE_Cmd_wr32(s_pHalContext, VERTEX2II(0, 0, BACKGROUND_STAR_HANDLE, 1));
		EVE_Cmd_wr32(s_pHalContext, RESTORE_CONTEXT());
		// Signature area 
		EVE_CoCmd_fgColor(s_pHalContext, 0xffffff);        // Set the fg color
		EVE_Cmd_wr32(s_pHalContext, TAG_MASK(1));
		EVE_Cmd_wr32(s_pHalContext, TAG('S'));
		EVE_CoCmd_button(s_pHalContext, x, y, w, h, 31, OPT_FLAT, "");

		// Sketch on the screen

		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(0, 0, 0));
		EVE_Cmd_wr32(s_pHalContext, BEGIN(BITMAPS));
		EVE_Cmd_wr32(s_pHalContext, VERTEX2II(x, y, 1, 0));

		if (EVE_Hal_rd8(s_pHalContext, REG_TOUCH_TAG) == 'O')
			EVE_CoCmd_fgColor(s_pHalContext, 0x003300);
		else
			EVE_CoCmd_fgColor(s_pHalContext, 0x005500);
		EVE_Cmd_wr32(s_pHalContext, COLOR_RGB(255, 255, 255));
		EVE_Cmd_wr32(s_pHalContext, TAG('O'));
#if defined(DISPLAY_RESOLUTION_HVGA_PORTRAIT)
		EVE_CoCmd_button(s_pHalContext, s_pHalContext->Width / 2 - sw / 4, s_pHalContext->Height - sh / 2 - 3 - 30, sw / 2, sh / 2, 28, 0, "CLEAR");
#else
		EVE_CoCmd_button(s_pHalContext, s_pHalContext->Width / 2 - sw / 4, s_pHalContext->Height - sh / 2 - 3, sw / 2, sh / 2, 28, 0, "CLEAR");
#endif
		EVE_Cmd_wr32(s_pHalContext, TAG_MASK(0));
		EVE_Cmd_wr32(s_pHalContext, DISPLAY());
		EVE_CoCmd_swap(s_pHalContext);
		EVE_Cmd_waitFlush(s_pHalContext);
		a += 1;
	}
}

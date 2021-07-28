#include "gfx.hpp"
#include <pspge.h>
#include <pspdisplay.h>
#include <psputils.h>

namespace GFX {
	uint32_t* draw_buffer;
	uint32_t* disp_buffer;

	void init() {
		draw_buffer = static_cast<uint32_t*>(sceGeEdramGetAddr());
		disp_buffer = static_cast<uint32_t*>(sceGeEdramGetAddr()) + (272 * 512 * 4);

		sceDisplaySetMode(0, 480, 272);
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
	}

	void clear(uint32_t color) {
		for (int i = 0; i < 512 * 272; i++) {
			draw_buffer[i] = color;
		}
	}

	void swapBuffers() {
		uint32_t* temp = disp_buffer;
		disp_buffer = draw_buffer;
		draw_buffer = temp;

		sceKernelDcacheWritebackInvalidateAll();
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_NEXTFRAME);

	}

	void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t color) {
		if (x > 480) {
			x = 480;
		}
		if (y > 480) {
			y = 480;
		}
		if (x + w > 480) {
			w = 480 - x;
		}
		if (x + w > 272) {
			h = 272 - y;
		}

		int off = x + (y * 512);
		for (int y1 = 0; y1 < h; y1++) {
			for (int x1 = 0; x1 < w; x1++) {
				draw_buffer[x1 + off + y1 * 512] = color;
			}
		}
	}

}
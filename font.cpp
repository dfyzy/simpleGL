#include <windows.h>

#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "font.hpp"
#include "simpleUtil.hpp"

const unsigned TEX_GAP = 1;

namespace simpleUtil {
	FT_Library ftLib;
	int dpiX = 0, dpiY = 0;

	void initFonts() {
		if (FT_Init_FreeType(&ftLib)) {
			print("Failed to init FreeType Library");
		}

		HDC screen = GetDC(0);//win32

		dpiX = GetDeviceCaps(screen, LOGPIXELSX);
		dpiY = GetDeviceCaps(screen, LOGPIXELSY);

		ReleaseDC(0, screen);

		print("Fonts initialized");
	}

	void closeFonts() {
		FT_Done_FreeType(ftLib);
	}

}

using namespace simpleUtil;
using namespace simpleGL;

Font::Font(const char* path, int size) {
	print("Loading font");

	FT_Face face;

	if (FT_New_Face(ftLib, path, 0, &face)) {
		print("Failed to load the font");
		return;
	}

	if (FT_Set_Pixel_Sizes(face, 0, size)) {
		print("Failed to change size");
		return;
	}

	lineSpacing = 0.75f * face->height * size * dpiY / (face->units_per_EM * 72.0f);

	FT_Load_Char(face, 32, FT_LOAD_RENDER);//space
	spaceWidth = face->glyph->advance.x >> 6;

	width = 0;
	height = 0;
	for (int i = FIRST_CHAR; i <= LAST_CHAR; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			print("Failed loading char");
			return;
		}

		width += face->glyph->bitmap.width + TEX_GAP;
		height = std::max(height, face->glyph->bitmap.rows);
	}
	width -= TEX_GAP;

	genTexture(GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

	unsigned offset = 0;
	for(int i = FIRST_CHAR; i <= LAST_CHAR; i++) {
		FT_Load_Char(face, i, FT_LOAD_RENDER);

		FT_Bitmap& bitmap = face->glyph->bitmap;

		glyphData[i - FIRST_CHAR] = GlyphData {offset,
			bitmap.width, bitmap.rows,
			Vector(face->glyph->bitmap_left + bitmap.width * 0.5f, face->glyph->bitmap_top - bitmap.rows * 0.5f),
			face->glyph->advance.x >> 6};

		std::unique_ptr<unsigned char[]> data(new unsigned char[bitmap.width * bitmap.rows]);
		for (unsigned y = 0; y < bitmap.rows; y++)
			for (unsigned x = 0; x < bitmap.width; x++)
				data[y*bitmap.width + x] = bitmap.buffer[(bitmap.rows - 1 - y)*bitmap.width + x];

		glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, offset, 0, bitmap.width, bitmap.rows,
															GL_RED, GL_UNSIGNED_BYTE, data.get());

		offset += bitmap.width + TEX_GAP;
	}

	FT_Done_Face(face);
}

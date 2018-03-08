#include <windows.h>
#include <memory>

#include "font.h"
#include "log.h"

namespace {

constexpr unsigned TEX_GAP = 1;

}

namespace simpleGL {

FT_Library Font::ftLibrary = nullptr;
float Font::dpiX, Font::dpiY;

FT_Library Font::getFTLibrary() {
	if (ftLibrary == nullptr) {
		println("FreeType:load");

		if (FT_Init_FreeType(&ftLibrary)) {
			println("error:FreeType:failed to init");
			return nullptr;
		}

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		int widthMM, heightMM;
		glfwGetMonitorPhysicalSize(monitor, &widthMM, &heightMM);

		dpiX = mode->width / (widthMM / 25.4f);
		dpiY = mode->height / (heightMM / 25.4f);
	}

	return ftLibrary;
}

//TODO: ???
//void util::closeFonts() {
//	FT_Done_FreeType(ftLibrary);
//}

Font::Font(const std::string& path, int size) {
	println(std::string("Font:load:") + path);

	FT_Face face;

	if (FT_New_Face(getFTLibrary(), path.c_str(), 0, &face)) {
		println("error:Font:failed to create");
		return;
	}

	if (FT_Set_Pixel_Sizes(face, 0, size)) {
		println("error:Font:failed to resize");
		return;
	}

	lineSpacing = 0.75f * face->height * size * dpiY / (face->units_per_EM * 72.0f);

	FT_Load_Char(face, 32, FT_LOAD_RENDER);//space
	spaceWidth = face->glyph->advance.x >> 6;

	unsigned width = 0;
	unsigned height = 0;
	for (int i = FIRST_CHAR; i <= LAST_CHAR; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			println("Failed loading char");
			return;
		}

		width += face->glyph->bitmap.width + TEX_GAP;
		height = std::max(height, face->glyph->bitmap.rows);
	}
	width -= TEX_GAP;

	image.loadData(width, height, GL_RED, nullptr);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned offset = 0;
	for(int i = FIRST_CHAR; i <= LAST_CHAR; i++) {
		FT_Load_Char(face, i, FT_LOAD_RENDER);

		FT_Bitmap& bitmap = face->glyph->bitmap;

		glyphs[i - FIRST_CHAR] = Glyph {Texture(&image, Vector(offset, 0), Vector(bitmap.width, bitmap.rows)),
													Vector(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x >> 6};

		std::unique_ptr<unsigned char[]> data(new unsigned char[bitmap.width * bitmap.rows]);
		for (unsigned y = 0; y < bitmap.rows; y++)
			for (unsigned x = 0; x < bitmap.width; x++)
				data[y*bitmap.width + x] = bitmap.buffer[(bitmap.rows - 1 - y)*bitmap.width + x];

		glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, offset, 0, bitmap.width, bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, data.get());

		offset += bitmap.width + TEX_GAP;
	}

	FT_Done_Face(face);
}

}

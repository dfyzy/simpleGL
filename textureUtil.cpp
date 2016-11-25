#include <libpng/png.h>
#include <zlib.h>

#include "simpleUtil.hpp"
#include "simpleTexture.hpp"

namespace simpleUtil {

	SimpleTexture* emptyTexture = new SimpleTexture(100, 100, 0);//hmmm
	std::list<SimpleTexture*> textures;

	boost::mutex textureMutex;
	boost::condition_variable textureCondition;
	bool textureReady;

	std::string texturePath;
	SimpleTexture* returnTexture = nullptr;

	boost::mutex untextureMutex;
	SimpleTexture* unloadingTexture = nullptr;
	boost::condition_variable unloadCondition;
	bool unloaded;

	GLenum textureFilter = GL_NEAREST;
	bool needFiltering = false;

	inline void notifyTexture() {
		textureReady = true;
		textureCondition.notify_one();
	}

	void loadTexture() {
		print("Loading texture");

		FILE *file = fopen(texturePath.c_str(), "rb");
		if (!file) {
			print("Error opening texture");
			texturePath.clear();
			notifyTexture();
			return;
		}

		texturePath.clear();

		png_byte header[8];
		fread(header, 1, 8, file);
		if (png_sig_cmp(header, 0, 8)) {
			print("Not a png");
			fclose(file);
			notifyTexture();
			return;
		}

		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!png_ptr) {
			print("Failed to create read struct");
			fclose(file);
			notifyTexture();
			return;
		}

		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			print("Failed to create info struct");
			png_destroy_read_struct(&png_ptr, nullptr, nullptr);
			fclose(file);
			notifyTexture();
			return;
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			print("Libpng error");
			png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
			fclose(file);
			notifyTexture();
			return;
		}

		png_init_io(png_ptr, file);

		png_set_sig_bytes(png_ptr, 8);

		png_read_info(png_ptr, info_ptr);

		png_uint_32 width, height;
		png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, nullptr, nullptr, nullptr, nullptr);

		GLuint texture;
		glGenTextures(1, &texture);

		returnTexture = new SimpleTexture(width, height, texture);
		textures.push_back(returnTexture);
		notifyTexture();

		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		std::unique_ptr<png_byte> row(new png_byte[4*width]);

		for (png_uint_32 i = height; i > 0; i--) {
			png_read_row(png_ptr, row.get(), nullptr);
			glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, i - 1, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, row.get());
		}

		png_read_end(png_ptr, nullptr);
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		fclose(file);
	}

	void unloadTexture() {
		print("Unloading texture");

		GLuint texture = unloadingTexture->getTexture();
		glDeleteTextures(1, &texture);//not sure if this can change value of texture handle. no reason why it should.

		textures.remove(unloadingTexture);

		unloadingTexture = nullptr;

		unloaded = true;
		unloadCondition.notify_one();
	}

	void checkUnloading() {
		boost::lock_guard<boost::mutex> lock(untextureMutex);

		if (unloadingTexture)	unloadTexture();
	}

	void setFiltering(GLuint texture) {
		glBindTexture(GL_TEXTURE_RECTANGLE, texture);

		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, textureFilter);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, textureFilter);
	}

	void checkFiltering() {
		boost::lock_guard<boost::mutex> lock(textureMutex);

		if (needFiltering) {
			for (SimpleTexture* st : textures)
				setFiltering(st->getTexture());

			needFiltering = false;
		}
	}

	void checkTextures() {
		checkUnloading();

		textureMutex.lock();
		bool empty = texturePath.empty();
		textureMutex.unlock();

		//loadTexture happens only when main thread is waiting for notifyTexture so we don't need lock here
		if (!empty)	loadTexture();

		checkFiltering();
	}

}

using namespace simpleUtil;

namespace simpleGL {
	SimpleTexture* getEmptyTexture() {
		return emptyTexture;
	}

	SimpleTexture* loadTexture(std::string path) {
		boost::unique_lock<boost::mutex> lock(textureMutex);
		textureReady = false;

		texturePath = path;

		if (!isCurrentThread())
			do 	textureCondition.wait(lock);
			while	(!textureReady);
		else
			simpleUtil::loadTexture();

		return returnTexture;
	}

	void setTextureFiltering(GLenum tf) {
		if ((tf != GL_LINEAR) && (tf != GL_NEAREST)) {
			simpleUtil::print("Wrong filtering type");
			return;
		}

		boost::lock_guard<boost::mutex> lock(textureMutex);
		if (tf == textureFilter)	return;

		textureFilter = tf;
		needFiltering = true;
	}
}

SimpleTexture::SimpleTexture(unsigned width, unsigned height, GLuint id) : pixelWidth(width), pixelHeight(height), texture(id) {
	setFiltering(id);
}

SimpleTexture::~SimpleTexture() {
	print("Texture destructor");
	unload();
}

void SimpleTexture::unload() {
	boost::unique_lock<boost::mutex> lock(untextureMutex);

	unloadingTexture = this;
	unloaded = false;

	if (!isCurrentThread())
		do 	unloadCondition.wait(lock);
		while	(!unloaded);
	else
		simpleUtil::unloadTexture();
}

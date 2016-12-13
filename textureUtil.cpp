#include <libpng/png.h>
#include <zlib.h>

#include "simpleUtil.hpp"

namespace simpleUtil {

	SimpleTexture* emptyTexture = new SimpleTexture(100, 100, 0);//hmmm
	std::list<SimpleTexture*> textures;

	boost::mutex textureMutex;
	boost::condition_variable textureCondition;
	bool textureReady = true;

	std::string texturePath;
	SimpleTexture* returnTexture;

	boost::mutex untextureMutex;
	SimpleTexture* unloadingTexture;
	boost::condition_variable unloadCondition;
	bool unloaded = true;

	GLenum textureFilter = GL_NEAREST;
	bool needFiltering = false;

	inline void notifyTexture() {
		textureReady = true;
		textureCondition.notify_one();
	}

	SimpleTexture* loadTexture(bool toThread, std::string path) {
		print("Loading texture");

		FILE *file = fopen(path.c_str(), "rb");
		if (!file) {
			print("Error opening texture");
			if (toThread)	notifyTexture();
			return nullptr;
		}

		png_byte header[8];
		fread(header, 1, 8, file);
		if (png_sig_cmp(header, 0, 8)) {
			print("Not a png");
			fclose(file);
			if (toThread)	notifyTexture();
			return nullptr;
		}

		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!png_ptr) {
			print("Failed to create read struct");
			fclose(file);
			if (toThread)	notifyTexture();
			return nullptr;
		}

		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			print("Failed to create info struct");
			png_destroy_read_struct(&png_ptr, nullptr, nullptr);
			fclose(file);
			if (toThread)	notifyTexture();
			return nullptr;
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			print("Libpng error");
			png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
			fclose(file);
			if (toThread)	notifyTexture();
			return nullptr;
		}

		png_init_io(png_ptr, file);

		png_set_sig_bytes(png_ptr, 8);

		png_read_info(png_ptr, info_ptr);

		png_uint_32 width, height;
		png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, nullptr, nullptr, nullptr, nullptr);

		GLuint texture;
		glGenTextures(1, &texture);

		SimpleTexture* simpleTex = new SimpleTexture(width, height, texture);
		textures.push_back(simpleTex);

		if (toThread) {
			returnTexture = simpleTex;
			texturePath.clear();
			notifyTexture();
		}

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

		return simpleTex;
	}

	void unloadTexture(SimpleTexture* texture) {
		print("Unloading texture");

		textures.remove(texture);

		GLuint texId = texture->getTexture();
		glDeleteTextures(1, &texId);
	}

	void unloadTextureToThread() {
		unloadTexture(unloadingTexture);

		unloaded = true;
		unloadCondition.notify_one();
	}

	void checkUnloading() {
		boost::lock_guard<boost::mutex> lock(untextureMutex);

		if (!unloaded)	unloadTextureToThread();
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
		bool loadNeeded = !textureReady;
		textureMutex.unlock();

		//loadTexture happens only when main thread is waiting for notifyTexture so we don't need lock here
		if (loadNeeded)	loadTexture(true, texturePath);

		checkFiltering();
	}

}

using namespace simpleUtil;

namespace simpleGL {
	SimpleTexture* getEmptyTexture() {
		return emptyTexture;
	}

	SimpleTexture* loadTexture(std::string path) {
		if (isCurrentThread())	return simpleUtil::loadTexture(false, path);

		boost::unique_lock<boost::mutex> lock(textureMutex);

		texturePath = path;
		textureReady = false;

		do 	textureCondition.wait(lock);
		while	(!textureReady);

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
	if (isCurrentThread()) {
		simpleUtil::unloadTexture(this);
		return;
	}

	boost::unique_lock<boost::mutex> lock(untextureMutex);

	unloadingTexture = this;
	unloaded = false;

	do 	unloadCondition.wait(lock);
	while	(!unloaded);
}

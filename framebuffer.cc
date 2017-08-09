#include "simpleGL.h"
#include "util.h"

namespace {

struct dynUniform {
	enum E { CAMERA, RESOLUTION, COUNT };
	static int sizes[COUNT];
};
int dynUniform::sizes[COUNT] {12, 2};//if adding new - change 2 to 4.

constexpr int SAMPLES_NUM = 4;

GLenum stencilFormat {0};

GLuint dynamic {0};

GLuint currentFbo {0};
simpleGL::Matrix currentView;
unsigned currentWidth;
unsigned currentHeight;


void setUniform(float* data, dynUniform::E type) {
	int offset = 0;
	for (int i = 0; i < type; i++)
		offset += dynUniform::sizes[i];

	glBufferSubData(GL_UNIFORM_BUFFER, offset*sizeof(float), dynUniform::sizes[type]*sizeof(float), data);
}

void setResolution(unsigned width, unsigned height) {
	currentWidth = width;
	currentHeight = height;

	float data[] {(float)width, (float)height};

	setUniform(data, dynUniform::RESOLUTION);
	glViewport(0, 0, width, height);
}

void setViewMatrix(simpleGL::Matrix view) {
	currentView = view;

	simpleGL::Matrix m = view.inv();
	//transposing and padding
	float data[] {m.get(0, 0),		m.get(1, 0),	m.get(2, 0), 0,
						m.get(0, 1),	m.get(1, 1),	m.get(2, 1), 0,
						m.get(0, 2),	m.get(1, 2),	m.get(2, 2), 0};

	setUniform(data, dynUniform::CAMERA);
}

void setFbo(GLuint fbo) {
	currentFbo = fbo;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

GLenum getStencilFormat() {
	if (stencilFormat == 0) {
		simpleGL::util::print("simpleGL:stencil only support:");

		std::string word;

		if (GLEW_ARB_ES3_compatibility || GLEW_ARB_texture_stencil8) {
			word = "true";
			stencilFormat = GL_STENCIL_INDEX8;
		} else {
			word = "false";
			stencilFormat = GL_DEPTH24_STENCIL8;
		}

		simpleGL::util::println(word);
	}

	return stencilFormat;
}

}

namespace simpleGL {

Framebuffer::Framebuffer(unsigned width, unsigned height, GLint internalFormat, GLenum format, GLenum type,
									bool stencil, GLenum filtering, Color base) : base(base) {
	if (dynamic == 0) {
		util::println("Uniform buffer:load");

		glGenBuffers(1, &dynamic);
		glBindBuffer(GL_UNIFORM_BUFFER, dynamic);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, dynamic);

		int size = 0;
		for (int i = 0; i < dynUniform::COUNT; i++)	size += dynUniform::sizes[i];
		glBufferData(GL_UNIFORM_BUFFER, size*sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		setResolution(getWindowWidth(), getWindowHeight());
	}

	util::println("Framebuffer:load");

	glGenFramebuffers(1, &msaaFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo);

	glGenRenderbuffers(1, &colorRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, colorRbo);

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES_NUM, internalFormat, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRbo);

	if (stencil) {
		glGenRenderbuffers(1, &stencilRbo);
		glBindRenderbuffer(GL_RENDERBUFFER, stencilRbo);

		glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES_NUM, getStencilFormat(), width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilRbo);
	}

	glGenFramebuffers(1, &rectFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, rectFbo);

	image = (new Image(filtering))->loadData(width, height, internalFormat, format, type, nullptr);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, image->getId(), 0);
}

Framebuffer::~Framebuffer() {
	util::println(std::string("Framebuffer:unload"));

	glDeleteRenderbuffers(1, &colorRbo);
	glDeleteRenderbuffers(1, &stencilRbo);
	image->unload();
	glDeleteFramebuffers(1, &msaaFbo);
	glDeleteFramebuffers(1, &rectFbo);
}

void Framebuffer::bind(Matrix view) {
	lastWidth = currentWidth;
	lastHeight = currentHeight;
	lastFbo = currentFbo;
	lastView = currentView;

	setResolution(image->getWidth(), image->getHeight());
	setFbo(msaaFbo);
	setViewMatrix(view);

	glClearColor(base.r, base.g, base.b, base.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Framebuffer::unbind() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rectFbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFbo);

	glBlitFramebuffer(0, 0, image->getWidth(), image->getHeight(),
							0, 0, image->getWidth(), image->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

	setResolution(lastWidth, lastHeight);
	setFbo(lastFbo);
	setViewMatrix(lastView);
}


}

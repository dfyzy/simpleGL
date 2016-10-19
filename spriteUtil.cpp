#include <queue>

#include "simpleTexture.hpp"
#include "simpleUtil.hpp"
#include "threadUtil.hpp"

struct SpriteData {
	float position[3];
	float bounds[2];
	float color[4];
	float texData[4];
};

std::queue<SpriteData> spriteQueue;

bool notified = false;

unsigned spriteCount = 0;

void loadSprites() {



}

void checkSprite() {
	mutex.lock();
	bool empty = spriteQueue.size() == 0;
	mutex.unlock();

	if (!empty)	loadSprites();
}

unsigned* SimpleTexture::createSprite(float x, float y, float z, float w, float h, Color c, float texX, float texY, float texW, float texH) {
	SpriteData data;
	data.position[0] = x;	data.position[1] = y;	data.position[2] = z;
	data.bounds[0] = w;		data.bounds[1] = h;
	data.color[0] = c.r;		data.color[1] = c.g;		data.color[2] = c.b;		data.color[3] = c.a;
	data.texData[0] = texX;	data.texData[1] = texY;	data.texData[2] = texW;	data.texData[3] = texH;

	boost::unique_lock<boost::mutex> lock(mutex);
	spriteQueue.push(data);

	do		returnReady.wait(lock);
	while	(!notified);

	sprites.push_back(spriteCount++);

	return &(*--sprites.end());
}

#include "physics.h"
#include "physicalShape.h"
#include "simpleGL.h"
#include "log.h"

namespace simpleGL {

void Physics::update() {
	float deltaTime = getDeltaTime();

	for (PhysicalShape* shape : shapes)	shape->cacheNextTimeStep(deltaTime);

	//grid and stuff

	//float nextTime = currentTime + deltaTime;

	for (auto it = shapes.begin(); it != shapes.end(); it++) {
		auto otherit = it;

		while ((++otherit) != shapes.end()) {
			float lowestRoot = 2.0f;

			(*it)->tryCollision(*otherit, 0.0f, lowestRoot);
			(*otherit)->tryCollision(*it, 0.0f, lowestRoot);

			if (lowestRoot != 2.0f) {
				futureCollisions.push_back({lowestRoot, *it, *otherit});
				println("collision");
			}
		}
	}

	if (!futureCollisions.empty()) {
		float deltaLeft = futureCollisions[0].time*deltaTime;

		futureCollisions[0].firstShape->stop(futureCollisions[0].time);
		futureCollisions[0].secondShape->stop(futureCollisions[0].time);

		futureCollisions[0].firstShape->cacheNextTimeStep(deltaLeft);
		futureCollisions[0].secondShape->cacheNextTimeStep(deltaLeft);
	}

	futureCollisions.clear();

	for (PhysicalShape* shape : shapes)	shape->applyNextTimeStep();
}

}

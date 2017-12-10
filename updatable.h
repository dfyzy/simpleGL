#ifndef SIMPLE_UPDATABLE_H
#define SIMPLE_UPDATABLE_H

#include <list>

namespace simpleGL {

enum class EUpdateType {
	PreTick,
	Tick,
	PostTick
};

template<EUpdateType updateType>
class Updatable {
private:
	static std::list<Updatable<updateType>*> updatableList;

protected:
	virtual void update() =0;

public:
	static void updateAll() {
		for (Updatable<updateType>* updatable : updatableList)
			updatable->update();
	}

	Updatable() {
		updatableList.push_back(this);
	}
	virtual ~Updatable()
	{
		updatableList.remove(this);
	}

};

template<EUpdateType updateType>
std::list<Updatable<updateType>*> Updatable<updateType>::updatableList;

}

#endif

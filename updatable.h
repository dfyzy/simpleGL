#ifndef SIMPLE_UPDATABLE_H
#define SIMPLE_UPDATABLE_H

#include <list>

namespace simpleGL {

enum class EUpdateType {
	PreTick,
	Tick,
	PostTick,
	Physics,
	COUNT
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


//global update function
template<int i>
struct GlobalUpdateImpl : public GlobalUpdateImpl<i + 1> {
	~GlobalUpdateImpl() {
		Updatable<static_cast<EUpdateType>(i)>::updateAll();
	}
};

template<>
struct GlobalUpdateImpl<static_cast<int>(EUpdateType::COUNT)> {};

inline void globalUpdate() {
	GlobalUpdateImpl<0>();
}

}

#endif

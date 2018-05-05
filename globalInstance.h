#ifndef SIMPLE_GLOBAL_INSTANCE_H
#define SIMPLE_GLOBAL_INSTANCE_H

#include "log.h"

namespace simpleGL {

template<typename T>
class GlobalInstance {
private:
	static T* instance;

	bool createdInstance;

public:
	static T* get() {
		if (!instance) {
			println("warning:GlobalInstance:returning nullptr on get() call");
		}

		return instance;
	}

	template<typename ...Args>
	GlobalInstance(Args&&... args) {
		createdInstance = !instance;

		if (createdInstance)	instance = new T(args...);
		else						println("warning:GlobalInstance:Instance already exists");
	}

	~GlobalInstance() {
		if (createdInstance) {
			delete instance;
			instance = nullptr;
		}
	}

	T* operator->() {
		return instance;
	}

};

template<typename T>
T* GlobalInstance<T>::instance = nullptr;

template<typename T>
T* getInstance() { return GlobalInstance<T>::get(); }

}

#endif

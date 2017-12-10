#ifndef SIMPLE_PRESET_OBJECT_FACTORY_H
#define SIMPLE_PRESET_OBJECT_FACTORY_H

#include <list>
#include <string>

namespace simpleGL {

class PresetObject;

class PresetObjectFactory {
public:
	class Attribute {
	private:
		std::string name;

	public:
		Attribute(std::string name) : name(name) {}

		Attribute(const Attribute& other);

	};

	template<typename T>
	class ValueAttribute : public Attribute {
	protected:
		T value;

	public:
		ValueAttribute(std::string name, T defaultValue) : Attribute(name), value(defaultValue) {}

		T getValue() const { return value; }
		void setValue(T t) { value = t; }

	};

private:
	bool defaultObject = false;

	std::list<Attribute*> attributes;

protected:
	static PresetObjectFactory* constructingPresetObjectFactory;

public:
	//constructs a default object
	PresetObjectFactory() { defaultObject = true; }

	//constructs an instance object
	PresetObjectFactory(const PresetObjectFactory& other) {
		constructingPresetObjectFactory = this;

		onCreation();
	}

	~PresetObjectFactory(){
		if (!defaultObject)
		{
			onDestruction();
		}
	}

	virtual PresetObjectFactory* clone() =0;

	virtual void onCreation() {}
	virtual void onDestruction() {}

	virtual PresetObject* build() =0;
};

template<typename Parent>
class PresetObjectFactoryClonable : public Parent
{
public:
	PresetObjectFactoryClonable(const PresetObjectFactoryClonable<Parent>& other) : Parent(other) {
		Parent::constructingPresetObjectFactory = nullptr;
	}

	PresetObjectFactory* clone() override {
		PresetObjectFactoryClonable<Parent>* clone = new PresetObjectFactoryClonable<Parent>(*this);

		return clone;
	}

};

}

#endif

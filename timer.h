#ifndef SIMPLE_TIMER_H
#define SIMPLE_TIMER_H

#include <functional>

#include "simpleGL.h"
#include "math.h"

namespace simpleGL {

class AbstractTimer {
protected:
	static std::list<AbstractTimer*> timers;
	static bool firstConst;
	static void update();

	virtual bool step() =0;

	AbstractTimer();

public:
	~AbstractTimer() { timers.remove(this); }

};

template<typename Object, typename Value>
class SetterTimer : public AbstractTimer {
protected:
	struct Interval;
	struct Point {
		Value value;

		Interval* past {nullptr};
		Interval* future {nullptr};

		Point(Value value) : value(value) {}
		Point(Value value, Interval* past) : value(value), past(past) {}
	};

	struct Interval {
		double duration {0};

		Point* past {nullptr};
		Point* future {nullptr};

		Interval(Point* past) : past(past) {
			past->future = this;
		}
	};

	class Iterator {
	protected:
		Interval* interval;

	public:
		Iterator(Interval* interval) : interval(interval) {}

		virtual ~Iterator() {}

		Interval* get() const { return interval; }

		virtual Point* getPast() const { return interval->past; }
		virtual Point* getFuture() const { return interval->future; }
		virtual void next() { interval = interval->future->future; }
	};

	class InvertedIterator : public Iterator {
	public:
		InvertedIterator(Interval* interval) : Iterator(interval) {}

		Point* getPast() const { return Iterator::interval->future; }
		Point* getFuture() const { return Iterator::interval->past; }
		void next() { Iterator::interval = Iterator::interval->past->past; }
	};

//TOTHINK: private?
	Object* object;
	void (Object::*set)(Value);

	Interval* first;
	Interval* last {nullptr};
	Iterator* iterator {nullptr};

	double cursor {0};
	bool inverted {false};

	bool playing {false};

	bool step() {
		if (!playing)	return true;
		cursor += getDeltaTime();

		if (cursor > iterator->get()->duration) {
			cursor = 0;

			(object->*set)(iterator->getFuture()->value);

			iterator->next();
			if (iterator->get() == nullptr) {
				playing = false;
			}

			return true;
		}

		return false;
	}

public:
	SetterTimer(Object* object, void (Object::*set)(Value), Value firstV) : object(object), set(set) {
		first = new Interval(new Point(firstV));
	}

	void play(bool invert) {
		if (last == nullptr)	return;

		playing = true;
		inverted = invert;
		cursor = 0;

		if (iterator != nullptr)	delete iterator;//TOTHINK: delete on end.
		iterator = invert ? new InvertedIterator(last) : new Iterator(first);

		(object->*set)(iterator->getPast()->value);
	}

	void play() { play(false); }

	void add(double duration, Value value) {
		if (duration < 0)	return;

		if (last == nullptr) {
			last = first;
		} else {
			last = new Interval(last->future);
		}

		last->duration = duration;
		last->future = new Point(value, last);
	}

};

template<typename Object, typename Value>
class LerpTimer : public SetterTimer<Object, Value> {
protected:
	bool step() {
		if (SetterTimer<Object, Value>::step()) return true;

		(SetterTimer<Object, Value>::object->*SetterTimer<Object, Value>::set)(
			lerp(SetterTimer<Object, Value>::iterator->getPast()->value,
					SetterTimer<Object, Value>::iterator->getFuture()->value,
						SetterTimer<Object, Value>::cursor/SetterTimer<Object, Value>::iterator->get()->duration)
		);
	}

public:
	LerpTimer(Object* object, void (Object::*set)(Value), Value firstV) : SetterTimer<Object, Value>::SetterTimer(object, set, firstV) {}

};

class FuncTimer : public SetterTimer<FuncTimer, std::function<void()>> {
private:
	void wrapper(std::function<void()> func) { func(); }

public:
	FuncTimer() : SetterTimer(this, wrapper, []{}) {}

};

}

#endif

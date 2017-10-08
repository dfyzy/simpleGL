/* Allows to do stuff over time
 * Like animations and the likes
*/

#ifndef SIMPLE_TIMER_H
#define SIMPLE_TIMER_H

#include <functional>

#include "updatable.h"
#include "simpleGL.h"
#include "math.h"

namespace simpleGL {

class AbstractTimer : public Updatable<EUpdateType::PostTick> {
private:
	void update() override {
		step();
	}

protected:
	virtual bool step() =0;

public:
	virtual bool isPlaying() const =0;

	virtual void play(bool invert) =0;

};

template<typename Object, typename Value>
class SetterTimer : public AbstractTimer {
protected:
	typedef void (Object::*Func)(Value);

	struct Interval;
	struct TimePoint {
		Value value;

		Interval* past {nullptr};
		Interval* future {nullptr};

		TimePoint(Value value) : value(value) {}
		TimePoint(Value value, Interval* past) : value(value), past(past) {}
	};

	struct Interval {
		double duration {0};

		TimePoint* past {nullptr};
		TimePoint* future {nullptr};

		Interval(TimePoint* past) : past(past) {
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

		virtual TimePoint* getPast() const { return interval->past; }
		virtual TimePoint* getFuture() const { return interval->future; }
		virtual void next() { interval = interval->future->future; }
	};

	class InvertedIterator : public Iterator {
	public:
		InvertedIterator(Interval* interval) : Iterator(interval) {}

		TimePoint* getPast() const override { return Iterator::interval->future; }
		TimePoint* getFuture() const override { return Iterator::interval->past; }
		void next() override { Iterator::interval = Iterator::interval->past->past; }
	};

private:
	Object* object;
	Func set;

	Interval* first;
	Interval* last {nullptr};
	Iterator* iterator {nullptr};

	double cursor {0};
	bool inverted {false};

	bool playing {false};

protected:
	Iterator* getIterator() const { return iterator; }

	bool step() override {
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
	SetterTimer(Object* object, Func set, Value firstV) : object(object), set(set) {
		first = new Interval(new TimePoint(firstV));
	}

	Object* getObject() const { return object; }
	Func getSetter() const { return set; }

	double getCursor() const { return cursor; }

	bool isPlaying() const override { return playing; }

	void play(bool invert) override {
		if (last == nullptr)	return;

		playing = true;
		inverted = invert;
		cursor = 0;

		if (iterator != nullptr)	delete iterator;//TOTHINK: delete on end.
		iterator = invert ? new InvertedIterator(last) : new Iterator(first);

		(object->*set)(iterator->getPast()->value);
	}

	void add(double duration, Value value) {
		if (duration < 0)	return;

		if (last == nullptr) {
			last = first;
		} else {
			last = new Interval(last->future);
		}

		last->duration = duration;
		last->future = new TimePoint(value, last);
	}

};

template<typename Object, typename Value>
class LerpTimer : public SetterTimer<Object, Value> {
protected:
	bool step() override {
		if (SetterTimer<Object, Value>::step()) return true;

		(SetterTimer<Object, Value>::getObject()->*SetterTimer<Object, Value>::getSetter())(
			math::lerp(SetterTimer<Object, Value>::getIterator()->getPast()->value,
				SetterTimer<Object, Value>::getIterator()->getFuture()->value,
					SetterTimer<Object, Value>::getCursor()/SetterTimer<Object, Value>::getIterator()->get()->duration)
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

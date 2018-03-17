#ifndef SIMPLE_MODULES_H
#define SIMPLE_MODULES_H

#include "globalInstance.h"
#include "cursor.h"
#include "physics.h"

namespace simpleGL {

class Modules {
private:
	GlobalInstance<Cursor> cursor;
	GlobalInstance<Physics> physics;

	//todo lighting, fonts

public:
	Modules() {}

};

}

#endif

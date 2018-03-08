#include "matrix.h"

namespace simpleGL {

Matrix Matrix::translateRotateScale(Vector pos, Angle rot, Vector sc)
{
	return Matrix(sc.x*rot.cos(), -sc.y*rot.sin(), pos.x,
						sc.x*rot.sin(), sc.y*rot.cos(), pos.y);
}

Matrix Matrix::translate(Vector v) {

	return Matrix(1, 0, v.x,
						0, 1, v.y);
}

Matrix Matrix::scale(Vector v) {

	return Matrix(v.x, 0, 0,
						0, v.y, 0);
}

Matrix Matrix::rotate(Angle a) {

	return Matrix(a.cos(), -a.sin(), 0,
						a.sin(), a.cos(), 0);
}

}

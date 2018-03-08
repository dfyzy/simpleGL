/* 3x3 tranformation matrix
 *
 * Data is stored row-first in two rows
 * The last row is always assumed to be (0, 0, 1) so we don't store it
*/

#ifndef SIMPLE_MATRIX_H
#define SIMPLE_MATRIX_H

#include "vector.h"
#include "angle.h"

namespace simpleGL {

class Matrix {
private:
	float data[6];

	//don't use this function
	inline float cof(int i) const {
		return data[i]*data[5] - data[i + 3]*data[2];
	}

public:
	static Matrix translateRotateScale(Vector pos, Angle rot, Vector sc);
	static Matrix translate(Vector v);
	static Matrix scale(Vector v);
	static Matrix rotate(Angle a);

	Matrix(float* d) {
		for (int i = 0; i < 6; i++)	data[i] = d[i];
	}

	Matrix(float a0, float a1, float a2,
			float b0, float b1, float b2) {
		data[0] = a0;	data[1] = a1;	data[2] = a2;
		data[3] = b0;	data[4] = b1;	data[5] = b2;
	}

	Matrix() : Matrix(1, 0, 0,
							0, 1, 0) {}

	float get(int row, int coll) const {
		if (row < 0 || row > 1 ||
			coll < 0 || coll > 2) return std::nanf("");

		return data[row*3 + coll];
	}

	Vector getPosition() const {
		return Vector(data[2], data[5]);
	}

	float det() const {
		return data[0]*data[4] - data[1]*data[3];
	}

	Matrix inv() const {
		float dt = det();

		return Matrix (data[4]/dt, -data[1]/dt, cof(1)/dt,
							-data[3]/dt, data[0]/dt, -cof(0)/dt);
	}

	Matrix operator*(const Matrix& m) const {
		return Matrix(
			data[0]*m.data[0] + data[1]*m.data[3], data[0]*m.data[1] + data[1]*m.data[4], data[0]*m.data[2] + data[1]*m.data[5] + data[2],
			data[3]*m.data[0] + data[4]*m.data[3], data[3]*m.data[1] + data[4]*m.data[4], data[3]*m.data[2] + data[4]*m.data[5] + data[5]
		);
	}

	Vector operator*(const Vector& v) const {

		return Vector(data[0]*v.x + data[1]*v.y + data[2], data[3]*v.x + data[4]*v.y + data[5]);
	}

	bool operator==(const Matrix& m) const {
		for (int i = 0; i < 6; i++)
			if (data[i] != m.data[i]) return false;

		return true;
	}

	bool operator!=(const Matrix& m) const {
		return !operator==(m);
	}

};

}

#endif

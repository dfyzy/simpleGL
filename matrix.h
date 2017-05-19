#ifndef SIMPLE_MATRIX_H
#define SIMPLE_MATRIX_H

#include "math.h"

namespace simpleGL {

//3x3, row-first
class Matrix {
private:
	float data[9];

	//don't use this function
	inline float cof(int i) const {
		return data[i]*data[5] - data[i + 3]*data[2];
	}

public:
	static Matrix translate(Vector v);
	static Matrix scale(Vector v);
	static Matrix rotate(Angle a);

	Matrix(float* d) {
		for (int i = 0; i < 6; i++)	data[i] = d[i];
		data[6] = 0;	data[7] = 0;	data[8] = 1;
	}

	Matrix(float a0, float a1, float a2,
			float b0, float b1, float b2) {
		data[0] = a0;	data[1] = a1;	data[2] = a2;
		data[3] = b0;	data[4] = b1;	data[5] = b2;
		data[6] = 0;	data[7] = 0;	data[8] = 1;
	}

	Matrix() : Matrix(1, 0, 0,
							0, 1, 0) {}

	float get(int row, int coll) const {
		return data[row*3 + coll];
	}

	//this and inv func work only when last row of this matrix is (0, 0, 1); which is always;
	float det() const {

		return data[0]*data[4] - data[1]*data[3];
	}

	Matrix inv() const {
		float newData[6] {data[4], -data[1], cof(1),
								-data[3], data[0], -cof(0)};

		float dt = det();
		for (int i = 0; i < 6; i++)
			newData[i] /= dt;

		return Matrix(newData);
	}

	const Matrix operator*(const Matrix& m) const {
		Matrix result;

		for (int i = 0; i < 6; i++) {
			double d = 0;
			int row = 3*(i/3);
			for (int j = 0; j < 3; j++) {
				d += data[j + row]*m.data[3*j + i - row];
			}
			result.data[i] = d;
		}

		return result;
	}

	const Vector operator*(const Vector& v) const {

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

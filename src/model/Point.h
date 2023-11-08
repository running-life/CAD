#pragma once
#include <Eigen/Core>

class Point {
public:
	Point():position(0, 0, 0) {};
	Point(double x, double y, double z) : position(x, y, z) {};
	double x() {
		return position.x();
	}
	double y() {
		return position.y();
	}
	double z() {
		return position.z();
	}

	void setPosition(double x, double y, double z) {
		position.x() = x;
		position.y() = y;
		position.z() = z;
	}

private:
	Eigen::Vector3d position;
};
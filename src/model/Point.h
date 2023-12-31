#pragma once
#include <Eigen/Core>

class Point {
public:
	Point():position(0, 0, 0) {};
	Point(const Point& p): position(p.x(), p.y(), p.z()) { }
	Point(double x, double y, double z) : position(x, y, z) {};
	Point(const Eigen::Vector3f& p):position(p){}
	double x() const {
		return position.x();
	}
	double y() const {
		return position.y();
	}
	double z() const {
		return position.z();
	}

	void setPosition(double x, double y, double z) {
		position.x() = x;
		position.y() = y;
		position.z() = z;
	}

	Eigen::Vector3f position;
};
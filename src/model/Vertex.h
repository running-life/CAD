#pragma once
#include "Point.h"
#include <vector>
#include <iostream>


class Vertex {
public:
	Vertex() : point(nullptr) {}
	Vertex(const Point& p) {
		point = new Point(p);
	}
	Vertex(const Eigen::Vector3f& p) {
		point = new Point(p);
	}
	Vertex(const Vertex* v) {
		point = new Point(*(v->point));
	}


	friend std::ostream& operator << (std::ostream& os, const Vertex& v) {
		os << "(" << v.point->x() << ", " << v.point->y() << ", " << v.point->z() << ")";
		return os;
	}
	//static std::vector<Vertex*> list;
	Point* point;
};

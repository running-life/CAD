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
	~Vertex() {
		delete point;
	}

	//static std::vector<Vertex*> list;
private:
	Point* point;
};

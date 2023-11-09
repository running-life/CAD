#pragma once
#include "Solid.h"

class EulerOperation {
	static Solid* mvfs(Point p) {
		Solid* solid = new Solid;
		Face* face = new Face;
		Loop* loop = new Loop;
		Vertex* vertex = new Vertex(p);

	}
};
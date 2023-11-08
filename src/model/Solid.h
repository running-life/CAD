#pragma once

#include "Face.h"

class Solid {
public:
	Solid* preSolid;
	Solid* nextSolid;
	Face* sFaces;
	Edge* edges;
};
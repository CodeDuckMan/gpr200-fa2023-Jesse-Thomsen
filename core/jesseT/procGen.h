
#pragma once
#include "C:\Users\Jesse\Documents\GitHub\gpr200-fa2023-Jesse-Thomsen\core\ew\mesh.h"

namespace jesseT {
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float size, int subdivisions);
}

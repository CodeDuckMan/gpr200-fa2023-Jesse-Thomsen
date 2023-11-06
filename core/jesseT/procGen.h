
#pragma once
#include "..\ew\mesh.h"

namespace jesseT {
	static int createCylinderDisc(float height, float radius, int numSegments, ew::MeshData& mesh, bool isFacingout);
	static void createCylinderDiscIndices(int centerVertIndex, int startVertIndex, int numSegments, ew::MeshData& mesh, bool isTop);
	static void createCylinderSideIndices(int startVertIndex, int numColumns, ew::MeshData& mesh);
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float size, int subdivisions);
}

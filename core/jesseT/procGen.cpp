
#pragma once
#include "procGen.h"
#include <stdlib.h>

namespace jesseT {

	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData mesh;

		return mesh;
	}
	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;

		return mesh;
	}
	ew::MeshData createPlane(float size, int subdivisions) {
		ew::MeshData mesh;
		ew::Vec3 point = (0.0, 0.0, 0.0);
		ew::Vertex vertex;
		//Create Vetricies
		for (int row = 0; row <= subdivisions; row++)
		{
			for (int col = 0; col <= subdivisions; col++)
			{
				point.x = size * (col / subdivisions);
				point.z = -size * (row / subdivisions);
				vertex.pos = point;
				vertex.normal = ew::Vec3 (0.0, 1.0, 0.0);;
				vertex.uv = ew::Vec2(col, row);
				mesh.vertices.push_back(vertex);
			}
		}
		return mesh;
	}
}
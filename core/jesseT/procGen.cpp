
#pragma once
#include "procGen.h"
#include <stdlib.h>

namespace jesseT {
	int createCylinderDisc(float height, float radius, int numSegments, ew::MeshData &mesh, bool isFacingout)
	{
		int numVerticesAdded = 0;
		float thetaStep = (float)2 * ew::PI / numSegments;
		for (int i = 0; i <= numSegments; i++)
		{
			ew::Vertex vertex;
			float theta = i * (float)thetaStep;
			vertex.pos.x = cos(theta) * (float)radius;
			vertex.pos.z = sin(theta) * (float)radius;
			vertex.pos.y = height;
			if (isFacingout)
			{
				vertex.normal = ew::Vec3(cos(theta), 0.0, sin(theta));
				vertex.uv = ew::Vec2(cos(sin(theta)),height);
			}
			else
			{
				vertex.normal = ew::Vec3(0.0, 1.0, 0.0);;
				vertex.uv = ew::Vec2(vertex.pos.x, vertex.pos.z);
			}
			mesh.vertices.push_back(vertex);
			numVerticesAdded++;
		}
		return numVerticesAdded;
	}

	void createCylinderDiscIndices(int centerVertIndex, int startVertIndex, int numSegments, ew::MeshData& mesh, bool isTop)
	{
		if (isTop)
		{
			for (int i = 0; i < numSegments; i++)
			{
				mesh.indices.push_back(startVertIndex + i);
				mesh.indices.push_back(centerVertIndex);
				mesh.indices.push_back(startVertIndex + i + 1);
			}
		}
		else 
		{
			for (int i = 0; i < numSegments; i++)
			{
				mesh.indices.push_back(startVertIndex + i + 1);
				mesh.indices.push_back(centerVertIndex);
				mesh.indices.push_back(startVertIndex + i);
			}

		}

	}

	void createCylinderSideIndices(int startVertIndex, int numColumns, ew::MeshData& mesh)
	{
		int start;
		for (size_t i = 0; i < numColumns; i++)
		{
			start = startVertIndex + i;

			mesh.indices.push_back(start + numColumns);
			mesh.indices.push_back(start);
			mesh.indices.push_back(start + 1);

			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + numColumns + 1);
			mesh.indices.push_back(start + numColumns);
		}
	}

	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData mesh;

		return mesh;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData mesh;
		ew::Vertex vertex;
		float topY = (float)height / 2;
		float bottomY = -topY;
		int numVerticies = -1; // start at -1 because 0 will be held for the first vert

		vertex.pos.y = topY;
		mesh.vertices.push_back(vertex);
		numVerticies++;

		numVerticies += createCylinderDisc(topY, radius, numSegments, mesh, false);
		numVerticies += createCylinderDisc(topY, radius, numSegments, mesh, true);
		numVerticies += createCylinderDisc(bottomY, radius, numSegments, mesh, true);
		numVerticies += createCylinderDisc(bottomY, radius, numSegments, mesh, false);

		vertex.pos.y = bottomY;
		mesh.vertices.push_back(vertex);
		numVerticies++;

		int topStartVert = 1;
		int topCenterVert = 0;
		int sideStartVert = numSegments + 1;
		int numColumns = numSegments + 1;
		int bottomStartVert = numVerticies - 1 - numSegments;
		int bottomCenterVert = numVerticies;

		createCylinderDiscIndices(topCenterVert, topStartVert, numSegments, mesh, true);
		createCylinderSideIndices(sideStartVert, numColumns, mesh);
		createCylinderDiscIndices(bottomCenterVert, bottomStartVert, numSegments, mesh, false);

		return mesh;
	}

	ew::MeshData createPlane(float size, int subdivisions) {
		ew::MeshData mesh;

		//Create Vetricies
		for (int row = 0; row <= subdivisions; row++)
		{
			for (int col = 0; col <= subdivisions; col++)
			{
				ew::Vertex vertex;
				vertex.pos.x = size * ((float)col / subdivisions);
				vertex.pos.z = -size * ((float)row / subdivisions);
				vertex.normal = ew::Vec3 (0.0, 1.0, 0.0);;
				vertex.uv = ew::Vec2(col, row);
				mesh.vertices.push_back(vertex);
			}
		}
		int columns = subdivisions + 1;
		int start = 0;
		for (int row = 0; row < subdivisions; row++)
		{
			for (int col = 0; col < subdivisions; col++)
			{
				start = row * columns + col;
				mesh.indices.push_back(start);
				mesh.indices.push_back(start+1);
				mesh.indices.push_back(start+columns+1);

				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
				mesh.indices.push_back(start);
			}
		}
		return mesh;
	}
}
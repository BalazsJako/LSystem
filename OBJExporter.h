#pragma once

#include <string>
#include "LSystem.h"

namespace LSystem
{
	class OBJExporter
	{
	public:
		OBJExporter();
		~OBJExporter(void);

		void Export(const LSystem::State& aState, const Token aTerminal, const std::string& aFileName);

	private:
		typedef std::vector<glm::vec3> V3s;
		typedef std::vector<glm::vec2> V2s;

		struct Quad
		{
			int vi0, vi1, vi2, vi3;
			int ti0, ti1, ti2, ti3;
		};

		typedef std::vector<Quad> Quads;

		
		int StoreVertex(const glm::vec3& v);
		int StoreTexCoord(const glm::vec2& v);

		V3s mVertices;
		V2s mTexCoords;

		Quads mQuads;
	};
}
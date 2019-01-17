#include <fstream>
#include <iostream>
#include <algorithm>

#include "OBJExporter.h"

namespace LSystem
{
	OBJExporter::OBJExporter()
	{
	}

	OBJExporter::~OBJExporter()
	{
	}

	void OBJExporter::Export(const LSystem::State& aState, const Token aTerminal, const std::string& aFileName)
	{
		std::cerr << "Building mesh geometry..." << std::endl;

		for (int i = 0; i < (int) aState.size(); ++i)
		{
			const LSystem::StateElement& e = aState[i];
			const Token name = e.mModuleName;
			if (name == aTerminal)
			{
				const ArgumentValueList& args = e.mArgumentValues;

				if (args.ElementCount() != 8)
					throw std::exception("Terminal module has incorrect number of parameters. 8 expected.");

				glm::vec3 v0 = args.Get<glm::vec3>(0);
				glm::vec3 v1 = args.Get<glm::vec3>(2);
				glm::vec3 v2 = args.Get<glm::vec3>(4);
				glm::vec3 v3 = args.Get<glm::vec3>(6);

				glm::vec2 t0 = args.Get<glm::vec2>(1);
				glm::vec2 t1 = args.Get<glm::vec2>(3);
				glm::vec2 t2 = args.Get<glm::vec2>(5);
				glm::vec2 t3 = args.Get<glm::vec2>(7);

				int vi0 = StoreVertex(v0);
				int vi1 = StoreVertex(v1);
				int vi2 = StoreVertex(v2);
				int vi3 = StoreVertex(v3);

				int ti0 = StoreTexCoord(t0);
				int ti1 = StoreTexCoord(t1);
				int ti2 = StoreTexCoord(t2);
				int ti3 = StoreTexCoord(t3);

				Quad q = { vi0, vi1, vi2, vi3, ti0, ti1, ti2, ti3 };
				mQuads.push_back(q);
			}
		}

		std::ofstream myfile;
		myfile.open(aFileName.c_str());

		if (!myfile)
		{
			auto s = "Cannot wirte: '" + aFileName + "'";
			throw std::exception(s.c_str());
		}

		std::cerr << "Saving to '" << aFileName << "'...";

		myfile << "# Lsie generated file. " 
			<< mVertices.size() << " vertices, " 
			<< mTexCoords.size() << " texture coordinates, "
			<< mQuads.size() << " faces." << std::endl;

		myfile << "mtllib test.mtl" << std::endl;
		myfile << "usemtl test" << std::endl;

		for (const auto& v : mVertices)
			myfile << "v " << v[0] << " " << v[1] << " " << v[2] << std::endl;

		for (const auto& v : mTexCoords)
			myfile << "vt " << v[0] << " " << v[1] << std::endl;

		for (const auto& q : mQuads)
			myfile << "f " 
				<< q.vi0 << "/" << q.ti0 << " "
				<< q.vi1 << "/" << q.ti1 << " "
				<< q.vi2 << "/" << q.ti2 << " "
				<< q.vi3 << "/" << q.ti3 << " "
				<< std::endl;

		myfile.close();

		std::cerr << "Done." << std::endl;

		mVertices.clear();
		mTexCoords.clear();
		mQuads.clear();
	}

	int OBJExporter::StoreVertex(const glm::vec3& v)
	{
		auto it = std::find(mVertices.begin(), mVertices.end(), v);
		if (it == mVertices.end())
		{
			mVertices.push_back(v);
			return (int) mVertices.size();
		}
		return 1 + (int)(it - mVertices.begin());
	}

	int OBJExporter::StoreTexCoord(const glm::vec2& v)
	{
		auto it = std::find(mTexCoords.begin(), mTexCoords.end(), v);
		if (it == mTexCoords.end())
		{
			mTexCoords.push_back(v);
			return (int) mTexCoords.size();
		}
		return 1 + (int)(it - mTexCoords.begin());
	}
}
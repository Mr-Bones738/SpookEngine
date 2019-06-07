#include "Model.h"
#include "Engine.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
Model::Model(const std::string& name) {
	std::string fullPath = Engine::Instance().GetIO().GetContentPath(name);
	std::string ext = fullPath.substr(fullPath.find_last_of('.'));
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	rotateFactor = glm::vec3(0.0f, 0.0f, 0.0f);
	if (ext == ".obj") {
		LoadFromOBJ(fullPath);
	}
	else if (ext == ".md2") {
		LoadFromMD2(fullPath);
	}
	else {
		std::cout << "Unsupported MODEL Type: ." << ext << "\n";
		return;
	}
	// vertex buffer will always be in the same format
	// regardless of format loaded from
	// so we can do all the GL stuff here...
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// give each animation frame per animation its own vbo, we can swap them
	std::vector<std::string> seenAnims;
	for (auto it : animVerts) {
		if (std::find(seenAnims.begin(), seenAnims.end(), it.first) == seenAnims.end()) {
			seenAnims.push_back(it.first);
			vbos.emplace(it.first, std::vector<GLuint>());
		}
	}
	for (auto it : animVerts) {
		for (auto v : it.second) {
			GLuint newvbo;
			glGenBuffers(1, &newvbo);
			glBindBuffer(GL_ARRAY_BUFFER, newvbo);
			glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(GLfloat), &v[0], GL_STATIC_DRAW);
			
			vbos.find(it.first)->second.push_back(newvbo);
			vNum = v.size();
		}
	}
	// vbos now contains a map of every frame of every animation linked to that animation
	animVerts.clear(); // i don't want to play with you anymore
	vbo = vbos.begin()->second[0]; // first frame of first animation as default vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(0);
	this->name = name;
}

void Model::LoadFromOBJ(const std::string& name) {
	// woefully inefficient OBJ loader, objects with lots of verts will take some time
	// todo: improve
	std::ifstream file(name);
	if (!file.is_open()) {
		std::cout << "file " << name << " not found?";
		return;
	}
	std::string line;
	std::vector<GLfloat> indexedverts;
	std::vector<GLfloat> indexeduvs;
	std::vector<GLfloat> indexednorms;
	std::vector<GLuint> indicesverts;
	std::vector<GLuint> indicesuvs;
	std::vector<GLuint> indicesnorms;
	while (std::getline(file, line)) {
		// v line = vertex def
		if (line.find("v ") != std::string::npos) {
			std::stringstream iss(line.substr(2, std::string::npos));
			GLfloat x, y, z;
			iss >> x >> y >> z;
			indexedverts.push_back(x);
			indexedverts.push_back(y);
			indexedverts.push_back(z);
		}
		// vt line = uv def
		else if (line.find("vt") != std::string::npos) {
			std::stringstream iss(line.substr(3, std::string::npos));
			GLfloat u, v;
			iss >> u >> v;
			indexeduvs.push_back(u);
			indexeduvs.push_back(v);
		}
		// vn line = normal def (unused as of now)
		else if (line.find("vn") != std::string::npos) {
			std::stringstream iss(line.substr(3, std::string::npos));
			GLfloat x, y, z;
			iss >> x >> y >> z;
			indexednorms.push_back(x);
			indexednorms.push_back(y);
			indexednorms.push_back(z);
		}
		// f = face def, contains indices to pos, uv and norms
		else if (line.find("f ") != std::string::npos) {
			std::size_t lastFS = 0;
			std::size_t lastspace = line.find(' ') + 1;
			for (int i = 0; i < 3; i++) {
				line = line.substr(lastspace);
				std::stringstream iss(line);
				GLuint iv;
				GLuint it;
				GLuint in;
				iss >> iv;
				iv -= 1; // zero-indexed
				lastFS = line.find('/') + 1;
				line = line.substr(lastFS);
				std::stringstream iss2(line);
				iss2 >> it;
				it -= 1; // also zero-indexed
				lastFS = line.find('/') + 1;
				line = line.substr(lastFS);
				std::stringstream iss3(line);
				iss3 >> in;
				in -= 1;
				indicesverts.push_back(iv);
				indicesuvs.push_back(it);
				indicesnorms.push_back(in);
				lastspace = line.find(' ') + 1;
			}
		}
	}
	// OpenGL expects indices in a different format to what OBJ stores
	// (OpenGL expects an index to a FULL vertex def (PPPUVNNN) whereas
	// OBJ stores them as separate indices
	// for now, we just unpack these indices into an array
	std::vector<GLfloat> verts;
	for (std::size_t i = 0; i < indicesverts.size(); i++) {
		for (int a = 0; a < 3; a++) {
			verts.push_back(indexedverts[(indicesverts[i] * 3) + a]);
		}
		for (int a = 0; a < 2; a++) {
			verts.push_back(indexeduvs[(indicesuvs[i] * 2) + a]);
		}
		for (int a = 0; a < 3; a++) {
			// can't get norms from md2, so i've disabled it here
			//verts.push_back(indexednorms[(indicesnorms[i] * 3) + a]);
		}
	}
	animVerts.emplace("none", std::vector<std::vector<GLfloat>>());
	animVerts.begin()->second.push_back(verts);
	// OBJ files include texture definitions, but they are often wrong path-wise
	// for now we just load this texture for all of them
	tex = Engine::Instance().GetTextureMan().GetTexture("models/level/0.png");
}

void Model::LoadFromMD2(const std::string& name) {
	struct MD2Header
	{
		unsigned int Identifier;
		unsigned int Version;
		unsigned int TextureWidth;
		unsigned int TextureHeight;
		unsigned int FrameSize;
		unsigned int NumTextures;
		unsigned int NumVertices;
		unsigned int NumTextureCoords;
		unsigned int NumTriangles;
		unsigned int NumGLCommands;
		unsigned int NumFrames;
		unsigned int TextureOffset;
		unsigned int TextureCoordsOffset;
		unsigned int TriangleOffset;
		unsigned int FrameOffset;
		unsigned int GLCommandsOffset;
		unsigned int EOFOffset;
	};

	struct MD2Triangle
	{
		unsigned short VertexIndices[3];
		unsigned short TexCoordIndices[3];
	};

	struct MD2Vertex
	{
		unsigned char Vertex[3];
		unsigned char NormalIndex;
	};

	struct MD2Frame
	{
		float Scale[3];
		float Translate[3];
		char Name[16];
		MD2Vertex* Vertices;
	};

	struct MD2Skin
	{
		char Name[64];
	};

	struct MD2TextureCoords
	{
		short S;
		short T;
	};
	MD2Skin* Skins;
	MD2TextureCoords* TextureCoords;
	MD2Triangle* Triangles;
	MD2Frame* Frames;
	MD2Header header;
	std::ifstream file(name, std::ifstream::in | std::ifstream::binary);
	if (!file.is_open()) {
		std::cout << "file " << name << " not found?";
		return;
	}
	// read the header into the header struct
	file.read((char*)& header, sizeof(header));
	if (header.Identifier != 844121161)
	{
		std::cout << "MD2 File is not valid\n";
		return;
	}

	if (header.Version != 8)
	{
		std::cout << "MD2 version not supported\n";
		return;
	}
	// allocate memory to store the data and read them
	Frames = new MD2Frame[header.NumFrames];
	Skins = new MD2Skin[header.NumTextures];
	TextureCoords = new MD2TextureCoords[header.NumTextureCoords];
	Triangles = new MD2Triangle[header.NumTriangles];
	file.seekg(header.TextureOffset, std::ios_base::beg);
	file.read((char*)Skins, sizeof(MD2Skin)* header.NumTextures);
	file.seekg(header.TextureCoordsOffset, std::ios_base::beg);
	file.read((char*)TextureCoords, sizeof(MD2TextureCoords)* header.NumTextureCoords);
	file.seekg(header.TriangleOffset, std::ios_base::beg);
	file.read((char*)Triangles, sizeof(MD2Triangle)* header.NumTriangles);
	file.seekg(header.FrameOffset, std::ios_base::beg);
	// read the vertex defintions for each frame
	for (int i = 0; i < header.NumFrames; i++)
	{
		Frames[i].Vertices = new MD2Vertex[header.NumVertices];
		file.read((char*)Frames[i].Scale, 12);
		file.read((char*)Frames[i].Translate, 12);
		file.read(Frames[i].Name, 16);
		file.read((char*)Frames[i].Vertices, sizeof(MD2Vertex) * header.NumVertices);
		//std::cout << Frames[i].Name << std::endl;
	}
	// unpack the indices and UVs in a format that OpenGL can read happily
	std::vector<GLfloat> verts;
	for (int f = 0; f < header.NumFrames; f++) 
	{
		// each frame is named after the animation is belongs to, followed by frame number
		// we don't care about frame number because they're ordered in the vector
		std::string fname = Frames[f].Name;
		std::string aname = fname.substr(0, fname.find_last_not_of("0123456789")+1);
		// the memset to zero ensures null termination
		memset(&Frames[f].Name, 0, 16);
		memcpy(&Frames[f].Name, aname.c_str(), aname.length());
		for (int i = 0; i < header.NumTriangles; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				MD2Vertex CurrentVertex = Frames[f].Vertices[Triangles[i].VertexIndices[j]];
				// we need to reverse vertex winding order, OGL likes c-clockwise
				// whereas MD2 uses clockwise
				verts.push_back((CurrentVertex.Vertex[1] * Frames[f].Scale[1]) + Frames[f].Translate[1]);
				verts.push_back((CurrentVertex.Vertex[0] * Frames[f].Scale[0]) + Frames[f].Translate[0]);
				verts.push_back((CurrentVertex.Vertex[2] * Frames[f].Scale[2]) + Frames[f].Translate[2]);

				MD2TextureCoords CurrentTexcoords = TextureCoords[Triangles[i].TexCoordIndices[j]];
				verts.push_back((float)CurrentTexcoords.S / header.TextureWidth);
				verts.push_back((float)CurrentTexcoords.T / header.TextureHeight);
			}
		}
		if (animVerts.find(Frames[f].Name) == animVerts.end()) {
			animVerts.emplace(Frames[f].Name, std::vector<std::vector<GLfloat>>());
		}
		animVerts.find(Frames[f].Name)->second.push_back(verts);
		verts.clear();
	}

	// load the first skin defined in the file (if there is none then this will error!!)
	tex = Engine::Instance().GetTextureMan().GetTexture(Skins[0].Name);
	// Z is UP for MD2s, so we have to rotate it
	rotateFactor = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
	scaleFactor = 0.1f; // scale is all weird compared to meter scale we use
	// deallocate all the stuff we allocated earlier
	delete[] Frames;
	delete[] Skins;
	delete[] TextureCoords;
	delete[] Triangles;
}

bool Model::FrameIsValid(const std::string& anim, const GLuint& frame) {
	auto i = vbos.find(anim);
	if (i == vbos.end()) {
		return false;
	}
	if (frame > i->second.size()) {
		return false;
	}
	return true;
}

void Model::SwitchVBOToFrame(std::string anim, const GLuint& frame) {
	auto i = vbos.find(anim);
	if (i == vbos.end()) {
		vbo = vbos.begin()->second[0];
	}
	else {
		if (frame < i->second.size()) {
			vbo = i->second[frame];
		}
		else {
			vbo = i->second.front();
		}
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

glm::vec3 Model::GetRotateFactor() const {
	return rotateFactor;
}

float Model::GetScaleFactor() const {
	return scaleFactor;
}

GLuint Model::GetVNum() const {
	return vNum;
}

GLuint Model::GetModelVAO() const {
	return vao;
}

Texture* Model::GetTex() {
	return tex;
}
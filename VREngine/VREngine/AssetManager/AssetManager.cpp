#include "AssetManager.h"


AssetManager* AssetManager::instance = NULL;

void AssetManager::readConfig(string config) {
	cout << "loading meshes" << endl;
	stringstream configStream;
	configStream.str(config);
	string operation, values1, values2, values3;
	int valuei;
	configStream >> operation;
	while (operation.compare("end") != 0) {
		if (operation.compare("model") == 0) {
			configStream >> values1>>values2>>valuei;
			map<string, pair<int, int> >::iterator tit = meshGroups.find(values1);
			if (tit == meshGroups.end()) {
				if (valuei == 0)
					loadMesh(values1, values2, false);
				else
					loadMesh(values1, values2, true);
			}
			else
				cout << "model name not unique: " << values1 << endl;
		}
		else if (operation.compare("shader") == 0) {
			configStream >> values1>>valuei;
			if (valuei == 2) {
				configStream >> values2 >> values3; 
				map<string, GLuint>::iterator tit = shaderPrograms.find(values1);
				if (tit == shaderPrograms.end()) {
					GLuint vs = Shader::compileShader(values2, Shader::shaderType::vertexShader);
					GLuint fs = Shader::compileShader(values3, Shader::shaderType::fragmentShader);
					this->shaderPrograms[values1] = Shader(vs, fs).Program;
				}
				else {
					cout << "shader program name not unique: " << values1 << endl;
				}
			}
		}
		configStream >> operation;
	}
	cout << "Loaded :" << endl;
	cout << meshGroups.size() << " meshGroups" << endl;
	cout << meshes.size() << " meshes" << endl;
	cout << meterials.size() << " meterials" << endl;
	cout << textures.size() << " textures" << endl;
	cout << shaderPrograms.size() << " shaderPrograms" << endl;
}

void AssetManager::loadMesh(string name, string path, bool isLoadMeterial) {
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	string dir = path.substr(0, path.find_last_of('/'));

	int start = meshes.size(), end;
	// Process ASSIMP's root node recursively
	processNode(dir,scene->mRootNode, scene, isLoadMeterial);
	end = meshes.size();
	meshGroups[name] = make_pair(start, end);
}

void AssetManager::processNode(string path, aiNode* node, const aiScene* scene, bool isLoadMeterial)
{
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->processMesh(path, mesh, scene, isLoadMeterial);
	}
	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(path, node->mChildren[i], scene, isLoadMeterial);
	}

}
void AssetManager::processMesh(string path, aiMesh* mesh, const aiScene* scene, bool isLoadMeterial)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	AABB aabb;
	Mesh *currentMesh = NULL;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		glm::vec3 tp(vector.x, vector.y, vector.z);
		tp.x > aabb.xmax ? aabb.xmax = tp.x : 0;
		tp.x < aabb.xmin ? aabb.xmin = tp.x : 0;
		tp.y > aabb.ymax ? aabb.ymax = tp.y : 0;
		tp.y < aabb.ymin ? aabb.ymin = tp.y : 0;
		tp.z > aabb.zmax ? aabb.zmax = tp.z : 0;
		tp.z < aabb.zmin ? aabb.zmin = tp.z : 0;
		vertex.Position = vector;
		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	currentMesh = new Mesh(vertices, indices, aabb);

	// Process materials
	if (isLoadMeterial)
	{
		if (mesh->mMaterialIndex >= 0) {
			const auto* mat = scene->mMaterials[mesh->mMaterialIndex];

			aiString name;
			mat->Get(AI_MATKEY_NAME, name);
			currentMesh->meterialName = string(name.C_Str());

			// Is the material cached?
			map<string, Meterial>::iterator tit = this->meterials.find(string(name.C_Str()));
			if (tit== meterials.end()) {
				Meterial meterial;
				meterial.name = string(name.C_Str());
				// Get the first texture for each texture type we need
				// since there could be multiple textures per type
				aiColor3D color(0.0f,0.0f,0.0f);
				aiString textPath;
				//diffuse
				mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
				meterial.diffuseColor = glm::vec3(color.r,color.g,color.b);
				if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
					mat->GetTexture(aiTextureType_DIFFUSE, 0, &textPath);
					meterial.diffusePath = path +"/"+ textPath.C_Str();
					meterial.hasDiffuse = true;
				}

				//
				mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
				meterial.ambientColor = glm::vec3(color.r, color.g, color.b);
				if (mat->GetTextureCount(aiTextureType_AMBIENT) > 0) {
					mat->GetTexture(aiTextureType_AMBIENT, 0, &textPath);
					meterial.ambientPath = path + "/" + textPath.C_Str();
					meterial.hasAmbient = true;
				}

				mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
				meterial.specularColor = glm::vec3(color.r, color.g, color.b);
				if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
					mat->GetTexture(aiTextureType_SPECULAR, 0, &textPath);
					meterial.specularPath = path + "/" + textPath.C_Str();
					meterial.hasSpecular = true;
				}

				mat->Get(AI_MATKEY_SHININESS, color.r);
				meterial.shininess = color.r*0.25;
				if (mat->GetTextureCount(aiTextureType_SHININESS) > 0) {
					mat->GetTexture(aiTextureType_SHININESS, 0, &textPath);
					meterial.shininessPath = path + "/" + textPath.C_Str();
					meterial.hasShininess = true;
				}

				if (mat->GetTextureCount(aiTextureType_NORMALS) > 0) {
					mat->GetTexture(aiTextureType_NORMALS, 0, &textPath);
					meterial.normalPath = path + "/" + textPath.C_Str();
					meterial.hasNormal = true;
				}

				if (mat->GetTextureCount(aiTextureType_HEIGHT) > 0) {
					mat->GetTexture(aiTextureType_HEIGHT, 0, &textPath);
					meterial.heightPath = path + "/" + textPath.C_Str();
					meterial.hasHeight = true;
				}

				if (mat->GetTextureCount(aiTextureType_LIGHTMAP) > 0) {
					mat->GetTexture(aiTextureType_LIGHTMAP, 0, &textPath);
					meterial.lightmapPath = path + "/" + textPath.C_Str();
					meterial.hasLightmap = true;
				}

				if (mat->GetTextureCount(aiTextureType_EMISSIVE) > 0) {
					mat->GetTexture(aiTextureType_EMISSIVE, 0, &textPath);
					meterial.emissivePath = path + "/" + textPath.C_Str();
					meterial.hasEmissive = true;
				}
				loadMeterial(meterial);
				this->meterials[name.C_Str()] = meterial;
			}

			currentMesh->meterial = &(this->meterials[name.C_Str()]);
		}
	}

	// Return a mesh object created from the extracted mesh data
	this->meshes.push_back(currentMesh);
}

void AssetManager::loadMeterial(Meterial &m) {
	if (m.hasAmbient) {
		map<string, Texture>::iterator tit = this->textures.find(m.ambientPath);
		if (tit != textures.end())
			m.ambientId = textures[m.ambientPath].id;
		else {
			Texture tt(m.ambientPath);
			m.ambientId = tt.id;
			textures[m.ambientPath] = tt;
		}
	}
	if (m.hasDiffuse) {
		map<string, Texture>::iterator tit = this->textures.find(m.diffusePath);
		if (tit != textures.end())
			m.diffuseId = textures[m.diffusePath].id;
		else {
			Texture tt(m.diffusePath);
			m.diffuseId = tt.id;
			textures[m.diffusePath] = tt;
		}
	}
	if (m.hasSpecular) {
		map<string, Texture>::iterator tit = this->textures.find(m.specularPath);
		if (tit != textures.end())
			m.specularId = textures[m.specularPath].id;
		else {
			Texture tt(m.specularPath);
			m.specularId = tt.id;
			textures[m.specularPath] = tt;
		}
	}
	if (m.hasShininess) {
		map<string, Texture>::iterator tit = this->textures.find(m.shininessPath);
		if (tit != textures.end())
			m.shininessId = textures[m.shininessPath].id;
		else {
			Texture tt(m.shininessPath);
			m.shininessId = tt.id;
			textures[m.shininessPath] = tt;
		}
	}
	if (m.hasNormal) {
		map<string, Texture>::iterator tit = this->textures.find(m.normalPath);
		if (tit != textures.end())
			m.normalId = textures[m.normalPath].id;
		else {
			Texture tt(m.normalPath);
			m.normalId = tt.id;
			textures[m.normalPath] = tt;
		}
	}
	if (m.hasHeight) {
		map<string, Texture>::iterator tit = this->textures.find(m.heightPath);
		if (tit != textures.end())
			m.heightId = textures[m.heightPath].id;
		else {
			Texture tt(m.heightPath);
			m.heightId = tt.id;
			textures[m.heightPath] = tt;
		}
	}
	if (m.hasLightmap) {
		map<string, Texture>::iterator tit = this->textures.find(m.lightmapPath);
		if (tit != textures.end())
			m.lightmapId = textures[m.lightmapPath].id;
		else {
			Texture tt(m.lightmapPath);
			m.lightmapId = tt.id;
			textures[m.lightmapPath] = tt;
		}
	}
	if (m.hasEmissive) {
		map<string, Texture>::iterator tit = this->textures.find(m.emissivePath);
		if (tit != textures.end())
			m.emissiveId = textures[m.emissivePath].id;
		else {
			Texture tt(m.emissivePath);
			m.emissiveId = tt.id;
			textures[m.emissivePath] = tt;
		}
	}
}
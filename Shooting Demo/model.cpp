#include "model.h"

Model::Model(char* path) {
	this->load_model(path);
}

void Model::draw(const Shader& shader) {
	for (unsigned int i = 0; i < this->meshes.size(); i++) {
		meshes.at(i).draw(shader);
	}
}

void Model::draw_instanced(const Shader& shader, glm::mat4* model_matrices, const unsigned int amount) {
	for (unsigned int i = 0; i < this->meshes.size(); i++) {
		this->meshes.at(i).draw_instanced(shader, model_matrices, amount);
	}
}

void Model::load_model(std::string path) {
	Assimp::Importer importer;
	/*
	ReadFile expects a file path as its first argument and several post-processing options are
	available to use as the second argument.

	By setting aiProcess_Triangulate we tell Assimp that if the model does not entirely consist
	of triangles it should transform all the model's primitive shapes into triangles.

	By setting aiProcess_FlipUVs we tell Assimp to flip the texture coordinates on the y-axis
	where necessary during processing (because most images in OpenGL are reversed around the
	y-axis so this option fixes that for us)

	Other useful options:
	aiProcess_GenNormals: Creates normals for each vertex if the model didn't contain normal vectors

	aiProcess_SplitLargeMeshes: splits large meshes into smaller sub-meshes which is useful if your
	rendering has a maximum number of vertices allowed and can only process smaller meshes

	aiProcess_OptimizeMeshes: does the reverse of aiProcess_SplitLargeMeshes (tries to join several
	meshes into one larger mesh, reducing calls for optimization
	*/
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	/*
	check to see if the scene and the root node are null and check a flag to see if the
	returned data is incomplete
	*/
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	// retrieve the directory path of the given file path
	this->directory = path.substr(0, path.find_last_of('/'));

	/*
	because each node might contain a set of children we want to first process the first node,
	then continue processing all of the node's children until the recursive function ends
	*/
	this->process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene) {
	// process all the node's meshes, if any
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(process_mesh(mesh, scene));
	}

	// do the same thing for all of the node's children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene);
	}
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		// I don't really understand why a placeholder is needed but apparently it is
		glm::vec3 placeholder;

		// process vertex positions
		placeholder.x = mesh->mVertices[i].x;
		placeholder.y = mesh->mVertices[i].y;
		placeholder.z = mesh->mVertices[i].z;

		vertex.pos = placeholder;

		// process normals
		placeholder.x = mesh->mNormals[i].x;
		placeholder.y = mesh->mNormals[i].y;
		placeholder.z = mesh->mNormals[i].z;

		vertex.normal = placeholder;

		/*
		process texture coordinates

		Assimp allows a model to have up to 8 different texture coordinates per vertex,
		though we aren't going to utilize that (we're only going to use one) so we access
		only the first row of the 2D array
		*/
		if (mesh->mTextureCoords[0]) {	// if the mesh has texture coordinates
			glm::vec2 temp;

			temp.x = mesh->mTextureCoords[0][i].x;
			temp.y = mesh->mTextureCoords[0][i].y;

			vertex.texCoords = temp;
		} else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		// process tangents
		placeholder.x = mesh->mTangents[i].x;
		placeholder.y = mesh->mTangents[i].y;
		placeholder.z = mesh->mTangents[i].z;

		vertex.tangent = placeholder;

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE,
		Texture::DIFFUSE);
	std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_DIFFUSE,
		Texture::SPECULAR);
	std::vector<Texture> normal_maps = load_material_textures(material, aiTextureType_HEIGHT,
		Texture::NORMAL);
	//std::vector<Texture> height_maps = load_material_textures(material, aiTextureType_AMBIENT,
	//Texture::HEIGHT);
	std::vector<Texture> reflection_maps = load_material_textures(material, aiTextureType_AMBIENT,
		Texture::REFLECTION);

	textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
	textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
	//textures.insert(textures.end(), height_maps.begin(), height_maps.end());
	textures.insert(textures.end(), reflection_maps.begin(), reflection_maps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_textures(aiMaterial* material, aiTextureType type, int type_name) {
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString str;

		material->GetTexture(type, i, &str);

		int index = this->index_of_loaded_texture(str.C_Str());

		// if the texture has not been loaded
		if (index == -1) {
			Texture texture(GL_TEXTURE_2D, type_name);
			texture.set_id(texture_from_file(str.C_Str(), this->directory, texture.get_target()));
			texture.set_path(std::string(str.C_Str()));
			textures.push_back(texture);

			this->loaded_textures.push_back(texture);
		} else {
			textures.push_back(this->loaded_textures.at(index));
		}
	}

	return textures;
}

unsigned int Model::texture_from_file(const char* path, const std::string& directory, int target) {
	std::string filename = directory + "/" + std::string(path);
	unsigned int texture_id;
	int width, height, num_components;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &num_components, 0);

	glGenTextures(1, &texture_id);

	if (data) {
		GLenum format;
		if (num_components == 1) {
			format = GL_RED;
		} else if (num_components == 3) {
			format = GL_RGB;
		} else if (num_components == 4) {
			format = GL_RGBA;
		}

		glBindTexture(target, texture_id);
		glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);

		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		std::cout << "Texture failed to load at path: " << filename << std::endl;
	}

	stbi_image_free(data);

	return texture_id;
}

int Model::index_of_loaded_texture(const char* path) {
	for (unsigned int i = 0; i < this->loaded_textures.size(); i++) {
		// if the texture has been loaded, return the index of the texture in the loaded_textures vector
		if (std::strcmp(this->loaded_textures.at(i).get_path().c_str(), path) == 0) {
			return i;
		}
	}

	// if the texture has not been loaded, return -1
	return -1;
}
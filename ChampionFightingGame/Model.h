#pragma once
#include "utils.h"
#include <vector>
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace glm;

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 tex_coords;
};

struct Texture {
	u32 id;
	string type;
	string path;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures);
	void render(Shader& shader);
private:
	u32 VAO;
	u32 VBO;
	u32 EBO;

	void init();
};

class Model {
public:
	Model(string path);
    void render(Shader& shader);
private:
	vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;

    void load_model(string path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, string type_name);
};
#pragma once
#include "utils.h"
#include <vector>
#include <map>
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace glm;

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 tex_coords;
	vec3 tangent;
	vec3 bitangent;
	int bone_ids[MAX_BONE_INFLUENCE] = { 0 };
	float weights[MAX_BONE_INFLUENCE] = { 0.0 };
};

struct Texture {
	u32 id;
	string type;
	string path;
};

struct Bone {
	string name;
	int parent_id;
	vec3 pos;
	vec4 rot;
	vec3 scale;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	vector<Bone> bones;

	Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures, vector<Bone> bones, string name);
	void render(Shader& shader);
	bool visible = true;
	string name;
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
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	vector<Bone> bones;

	string directory;
	bool gamma_correct;
	int vertex_count = 0;

	int get_mesh_id(string mesh_name);
	int get_bone_id(string bone_name);
	vector<Bone*> find_all_matching_bones(string name);
	void reset_skeleton();
	vector<vector<Bone*>> matching_bones;
private:
	void load_model(string path);
	void load_skeleton(string path);

    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, string type_name);
};
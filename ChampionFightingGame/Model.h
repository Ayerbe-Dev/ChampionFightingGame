#pragma once
#include "utils.h"
#include <vector>
#include <map>
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <gtx/euler_angles.hpp>
#include "Bone.h"
#include "Animation.h"
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

enum {
	TEXTURE_KIND_DIFFUSE,
	TEXTURE_KIND_SPECULAR,
	TEXTURE_KIND_NORMAL,
	TEXTURE_KIND_HEIGHT,

	TEXTURE_KIND_MAX
};

struct Texture {
	u32 id;
	int type;
	string type_name;
	string type_string;
	string path;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	string name;
	bool visible;

	Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures, string name);
	void render(Shader *shader);

	u32 VAO;
	u32 VBO;
	u32 EBO;
private:
	void init();
};

class Model {
public:
	Model();
	Model(string path);
	~Model();
	void load_model(string path);
	void unload_model();
	void set_bones(float frame, Animation* anim_kind);
	void reset_bones();
    void render(Shader *shader);
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;

	mat4 global_transform;

	vector<Bone> bones;

	string directory;
	bool gamma_correct;
	int vertex_count = 0;

	void set_mesh_visibility(string mesh_name, bool visibility);

	int get_mesh_id(string mesh_name);
	int get_bone_id(string bone_name);
private:
	void load_skeleton(string path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
	bool find_missing_bones(aiNode* node, vector<string>& bone_names);
    vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, string type_name);
};
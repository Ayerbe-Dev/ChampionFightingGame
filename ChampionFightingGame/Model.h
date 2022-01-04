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
	bool visible = true;
	string name;

	Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures, string name);
	void render(Shader *shader);
	
private:
	u32 VAO;
	u32 VBO;
	u32 EBO;

	void init();
};

class Model {
public:
	Model(string path);
    void render(Shader *shader);
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	vector<Bone> bones;
	vector<Bone> bones_anim;

	string directory;
	bool gamma_correct;
	int vertex_count = 0;


	int get_mesh_id(string mesh_name);
	int get_bone_id(string bone_name);
	void set_bones(float frame, Animation3D *anim_kind);
	void reset_bones();

private:
	void load_model(string path);
	void load_skeleton(string path);

    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, string type_name);
};
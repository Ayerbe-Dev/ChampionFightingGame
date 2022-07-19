#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/euler_angles.hpp>
#include <unordered_map>

#include "BoneConstants.h"
#include "Bone.h"

class Animation;
class Shader;

enum {
	TEXTURE_KIND_DIFFUSE,
	TEXTURE_KIND_SPECULAR,
	TEXTURE_KIND_NORMAL,
	TEXTURE_KIND_HEIGHT,

	TEXTURE_KIND_MAX
};

struct ModelVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	int bone_ids[MAX_BONE_INFLUENCE] = { 0 };
	int f_bone_ids[MAX_BONE_INFLUENCE] = { 0 };
	float weights[MAX_BONE_INFLUENCE] = { 0.0 };
	int num_weights = { 0 };
};

struct ModelTexture {
	unsigned int id = 0;
	int type;
	std::string type_string;
	std::string filename;
};

class Mesh {
public:
	Mesh();
	Mesh(std::vector<ModelVertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> textures, std::string name);

	Mesh copy(); //This can't be a copy constructor because it will fuck with vectors
	
	void render();
	void render_shadow();

	std::vector<ModelVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ModelTexture> textures;
	std::string name;
	bool visible;


	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
private:
	void init();
};

class Model {
public:
	Model();
	Model(std::string path);

	Model copy();

	~Model();
	void load_model(std::string path);
	void load_textures();
	void load_textures(std::string path);
	void unload_model();
	void unload_textures();
	void set_move(bool move);
	void set_bones(float frame, Animation* anim_kind, bool flip);
	void reset_bones();

    void render(Shader *shader, bool flip);
	void render_shadow(Shader* shader, bool flip);

	std::vector<std::string> texture_names;
	std::vector<unsigned int> textures_loaded;
	std::unordered_map<std::string, std::vector<ModelTexture*>> texture_map;

	std::vector<Mesh> meshes;
	std::unordered_map<std::string, int> mesh_map;

	glm::mat4 global_transform;

	std::vector<Bone> bones;
	std::vector<Bone*> trans_children;
	std::unordered_map<std::string, int> bone_map;

	std::string directory;

	bool has_skeleton;

	void set_mesh_visibility(std::string mesh_name, bool visibility);

	int get_mesh_id(std::string mesh_name);
	int get_bone_id(std::string bone_name, bool verbose = true);
private:
	bool load_skeleton(std::string path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<ModelTexture> load_texture_data(aiMaterial* mat, aiTextureType type, std::string type_name);
	void process_skeleton(aiNode* root);
	void post_process_skeleton();

	glm::mat4 flip_matrix;
	glm::mat4* dummy_matrix;
	glm::vec3* dummy_vec;
	glm::quat* dummy_quat;
	bool move;
	bool tpose;
	size_t trans_id;
};
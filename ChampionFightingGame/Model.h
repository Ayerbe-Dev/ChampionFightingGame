#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/euler_angles.hpp>
#include <unordered_map>

#include "Bone.h"

#define MAX_BONE_INFLUENCE 4

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
	float bone_weights[MAX_BONE_INFLUENCE] = { 0.0 };
	int bone_ids[MAX_BONE_INFLUENCE] = { 0 };
	int num_bone_weights = { 0 };
};

struct ModelTexture {
	unsigned int id = 0;
	int type;
	std::string type_string;
	std::string filename;
};

struct Material {
	Material(std::vector<ModelTexture> textures, std::string name);
	std::vector<ModelTexture> textures;
	std::string name;
};

class Mesh {
public:
	Mesh();
	Mesh(std::vector<ModelVertex> vertices, std::vector<unsigned int> indices, Material* mat, std::string name);
	Mesh(Mesh& other);
	Mesh(const Mesh& other);
	Mesh operator=(Mesh& other);
	Mesh operator=(const Mesh& other);

	void init_gl();

	void bind_materials() const;
	void render() const;

	std::vector<ModelVertex> vertices;
	std::vector<unsigned int> indices;
	Material* mat;
	Shader* shader;
	std::string name;
	bool visible;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

//The base model that is stored by ResourceManager. Does not hold its own textures.
class ModelData {
public:
	ModelData();
	ModelData(std::string path);
	void load_model(std::string path);
	void unload_model();

	void init_gl_meshes();

	glm::mat4 get_global_transform() const;
	std::string get_directory() const;
	std::size_t get_trans_id() const;
	bool has_skeleton() const;

	std::unordered_map<std::string, int> material_map;
	std::unordered_map<std::string, int> mesh_map;
	std::unordered_map<std::string, std::vector<std::pair<int, int>>> texture_map;

	std::vector<Material> material_data;
	std::vector<Mesh> mesh_data;
	std::vector<std::string> texture_names;
	Skeleton skeleton;

	glm::mat4* dummy_matrix;
	glm::vec3* dummy_vec;
	glm::quat* dummy_quat;
private:
	void process_scene(const aiScene* scene);
	void process_node(aiNode* node, const aiScene* scene);
	Mesh process_mesh(aiMesh* mesh);
	std::vector<ModelTexture> load_texture_names(aiMaterial* mat, aiTextureType type, std::string type_name);
	void process_skeleton(aiNode* root);

	glm::mat4 global_transform;
	std::string directory;
	std::size_t trans_id;
	bool skeleton_loaded;
	int skipped_verts;
};

//Data that is specific to a given model, basically this is what actually gets rendered.
class ModelInstance {
public:
	ModelInstance();
	ModelInstance(std::string path);
	~ModelInstance();

	void load_model_instance(std::string path);
	void load_used_model_instance(std::string path);
	void unload_model_instance();

	void set_move(bool move);
	void set_flip(bool flip);
	void set_bones(float frame, Animation* anim_kind);
	void reset_bones();

	void load_textures();
	void load_textures(std::string path);
	void unload_textures();

	void set_mesh_visibility(std::string mesh_name, bool visibility);
	void set_mesh_mat(std::string mesh_name, std::string mat_name);

	int get_mesh_id(std::string mesh_name);
	int get_material_id(std::string material_name);
	int get_bone_id(std::string bone_name) const;
	Skeleton get_skeleton() const;

	bool has_skeleton();
	bool is_loaded() const;

	void render(Shader* shader);
	void render_no_texture(Shader* shader);

	std::vector<Material> materials;
	std::vector<Mesh> meshes;
	std::vector<Bone> bone_data;
	std::vector<Bone*> trans_children;

	std::string texture_dir;
private:
	void process_skeleton_instance();

	ModelData* model;
	bool move;
	bool flip;
};
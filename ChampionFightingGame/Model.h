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

	void render() const;
	void render_no_texture() const;

	std::vector<ModelVertex> vertices;
	std::vector<unsigned int> indices;
	Material* mat;
	Shader* shader;
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
	Model(Model& other);
	Model(const Model& other);
	Model& operator=(Model& other);
	Model& operator=(const Model& other);

	void init(std::string path);
	void destroy();

	void load_model(std::string path); //Checks if this model is already loaded into the ResourceManager,
	//then copies it

	void unload_model(); //Tells the ResourceManager to decrease the user count for this model

	void load_textures(); //Checks if this model's textures are already loaded into the ResourceManager,
	//then copies them and tells all of its meshes
	void load_textures(std::string path); //Same as above, but it will look for textures in the subdirectory
	//passed to the function (E.G. stage models will get all of their textures from one place, player models
	//will get them from subdirs for their respective alts)

	void unload_textures(); //Tells the ResourceManager to decrease the user count for all of these textures
	void unload_texture_resources(); //Tells the ResourceManager to actually unload these textures

	void set_move(bool move);
	void set_bones(float frame, Animation* anim_kind, bool flip);
	void reset_bones();

    void render(Shader *shader, bool flip);
	void render_no_texture(Shader* shader, bool flip);

	void set_mesh_visibility(std::string mesh_name, bool visibility);
	void set_mesh_mat(std::string mesh_name, std::string mat_name);

	int get_mesh_id(std::string mesh_name);
	int get_material_id(std::string material_name);
	int get_bone_id(std::string bone_name, bool verbose = true);

	std::vector<std::string> texture_names;
	std::unordered_map<std::string, std::vector<std::pair<int, int>>> texture_map;

	std::vector<Material> materials;
	std::unordered_map<std::string, int> material_map;

	std::vector<Mesh> meshes;
	std::unordered_map<std::string, int> mesh_map;

	glm::mat4 global_transform;

	std::vector<Bone> bones;
	std::unordered_map<std::string, int> bone_map;
	std::vector<Bone*> trans_children;

	std::string directory;

	bool has_skeleton;
private:
	bool load_skeleton(std::string path);
    void process_node(aiNode* node, const aiScene* scene);
	void process_start(const aiScene* scene);
	Mesh process_mesh(aiMesh* mesh);
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
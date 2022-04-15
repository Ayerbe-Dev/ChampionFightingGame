#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/euler_angles.hpp>

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
	float weights[MAX_BONE_INFLUENCE] = { 0.0 };
};

struct ModelTexture {
	unsigned int id;
	int type;
	std::string type_name;
	std::string type_string;
	std::string path;
};

class Mesh {
public:
	std::vector<ModelVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ModelTexture> textures;
	std::string name;
	bool visible;

	Mesh(std::vector<ModelVertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> textures, std::string name);
	void render(Shader *shader);
	void render_shadow(Shader* shader);

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
	~Model();
	void load_model(std::string path);
	void load_model_no_skeleton(std::string path);
	void unload_model();
	void set_bones(float frame, Animation* anim_kind);
	void reset_bones();

    void render(Shader *shader, bool flip);
	void render_shadow(Shader* shader, bool flip);

	std::vector<ModelTexture> textures_loaded;
	std::vector<Mesh> meshes;

	glm::mat4 global_transform;

	std::vector<Bone> bones;

	std::string directory;
	bool gamma_correct;
	int vertex_count = 0;

	void set_mesh_visibility(std::string mesh_name, bool visibility);

	int get_mesh_id(std::string mesh_name);
	int get_bone_id(std::string bone_name);
private:
	void load_skeleton(std::string path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
	bool find_missing_bones(aiNode* node, std::vector<std::string>& bone_names);
	std::vector<ModelTexture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name);
	glm::mat4 flip_matrix = glm::mat4(1.0, 0.0, 0.0, 0.0, 
		0.0, 1.0, 0.0, 0.0, 
		0.0, 0.0, -1.0, 0.0, 
		0.0, 0.0, 0.0, 1.0
	);
};
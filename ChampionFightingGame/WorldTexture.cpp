#include "WorldTexture.h"
#include "WindowConstants.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "utils.h"

WorldTexture::WorldTexture() {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	this->VAO = 0;
	this->VBO = 0;
#endif
	this->shader = nullptr;
	this->features = 0;
	this->path = "";
	this->texture_orientation = TEXTURE_MID;
	this->anchor = nullptr;
	this->pos = glm::vec3(0.0);
	this->rot = glm::vec3(0.0);
	this->scale = glm::vec3(1.0);
	this->width = 0;
	this->height = 0;
	this->alpha = 255;
	this->colormod = glm::vec3(0.0);
	this->h_flipped = false;
	this->v_flipped = false;
	this->billboard_setting = BILLBOARD_OFF;
	this->sprite = 0;
	this->buffer_updates = 0;
	this->loaded = false;
}

WorldTexture::WorldTexture(std::string path, unsigned char features) : WorldTexture() {
	init(path, features);
}

WorldTexture::WorldTexture(unsigned int texture, unsigned char features, int width, int height) : WorldTexture() {
	std::vector<unsigned int> tex{ texture };
	init(tex, features, width, height);
}

WorldTexture::WorldTexture(std::vector<unsigned int> texture, unsigned char features, int width, int height) : WorldTexture() {
	init(texture, features, width, height);
}

WorldTexture::WorldTexture(WorldTexture& other) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	this->VAO = other.VAO;
	this->VBO = other.VBO;
#endif
	this->v_spec = other.v_spec;
	this->v_pos.resize(v_spec.num_vertices);
	this->v_pos_accessor.resize(v_spec.num_vertices);
	this->v_texcoord.resize(v_spec.num_vertices);
	this->v_texcoord_accessor.resize(v_spec.num_vertices);
	this->v_data_for_gpu.resize(v_spec.num_vertices_internal);
	for (int i = 0; i < v_spec.num_vertices; i++) {
		this->v_pos[i] = other.v_pos[i];
		this->v_texcoord[i] = other.v_texcoord[i];
		this->v_pos_accessor[i] = &this->v_pos[i];
		this->v_texcoord_accessor[i] = &this->v_texcoord[i];
		this->v_data_for_gpu[i] = other.v_data_for_gpu[i];
	}
	for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
		this->v_data_for_gpu[v_spec.vertex_bindings[i].first] = this->v_data_for_gpu[v_spec.vertex_bindings[i].second];
	}
	this->shader = other.shader;
	this->features = other.features;
	this->path = other.path;
	this->texture_orientation = other.texture_orientation;
	this->anchor = other.anchor;
	this->texture = other.texture;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->width = other.width;
	this->height = other.height;
	this->alpha = other.alpha;
	this->colormod = other.colormod;
	this->h_flipped = other.h_flipped;
	this->v_flipped = other.v_flipped;
	this->billboard_setting = other.billboard_setting;
	this->sprite = other.sprite;
	this->buffer_updates = other.buffer_updates;
	if (ResourceManager::get_instance()->is_tex_const_copied(texture[0])) {
		this->loaded = other.loaded;
		other.loaded = false;
	}
	else {
		this->loaded = false;
	}
}

WorldTexture::WorldTexture(const WorldTexture& other) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	this->VAO = other.VAO;
	this->VBO = other.VBO;
#endif
	this->v_spec = other.v_spec;
	this->v_pos.resize(v_spec.num_vertices);
	this->v_pos_accessor.resize(v_spec.num_vertices);
	this->v_texcoord.resize(v_spec.num_vertices);
	this->v_texcoord_accessor.resize(v_spec.num_vertices);
	this->v_data_for_gpu.resize(v_spec.num_vertices_internal);
	for (int i = 0; i < v_spec.num_vertices; i++) {
		this->v_pos[i] = other.v_pos[i];
		this->v_texcoord[i] = other.v_texcoord[i];
		this->v_pos_accessor[i] = &this->v_pos[i];
		this->v_texcoord_accessor[i] = &this->v_texcoord[i];
		this->v_data_for_gpu[i] = other.v_data_for_gpu[i];
	}
	for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
		this->v_data_for_gpu[v_spec.vertex_bindings[i].first] = this->v_data_for_gpu[v_spec.vertex_bindings[i].second];
	}
	this->shader = other.shader;
	this->features = other.features;
	this->path = other.path;
	this->texture_orientation = other.texture_orientation;
	this->anchor = other.anchor;
	this->texture = other.texture;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->width = other.width;
	this->height = other.height;
	this->alpha = other.alpha;
	this->colormod = other.colormod;
	this->h_flipped = other.h_flipped;
	this->v_flipped = other.v_flipped;
	this->billboard_setting = other.billboard_setting;
	this->sprite = other.sprite;
	this->buffer_updates = other.buffer_updates;
	this->loaded = other.loaded;
	ResourceManager::get_instance()->store_const_copy_addr(texture[0], (WorldTexture*)&other);
}

WorldTexture::WorldTexture(WorldTexture&& other) noexcept {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	this->VAO = other.VAO;
	this->VBO = other.VBO;
#endif
	this->v_spec = other.v_spec;
	this->v_pos.resize(v_spec.num_vertices);
	this->v_pos_accessor.resize(v_spec.num_vertices);
	this->v_texcoord.resize(v_spec.num_vertices);
	this->v_texcoord_accessor.resize(v_spec.num_vertices);
	this->v_data_for_gpu.resize(v_spec.num_vertices_internal);
	for (int i = 0; i < v_spec.num_vertices; i++) {
		this->v_pos[i] = other.v_pos[i];
		this->v_texcoord[i] = other.v_texcoord[i];
		this->v_pos_accessor[i] = &this->v_pos[i];
		this->v_texcoord_accessor[i] = &this->v_texcoord[i];
		this->v_data_for_gpu[i] = other.v_data_for_gpu[i];
	}
	for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
		this->v_data_for_gpu[v_spec.vertex_bindings[i].first] = this->v_data_for_gpu[v_spec.vertex_bindings[i].second];
	}
	this->shader = other.shader;
	this->features = other.features;
	this->path = other.path;
	this->texture_orientation = other.texture_orientation;
	this->anchor = other.anchor;
	this->texture = other.texture;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->width = other.width;
	this->height = other.height;
	this->alpha = other.alpha;
	this->colormod = other.colormod;
	this->h_flipped = other.h_flipped;
	this->v_flipped = other.v_flipped;
	this->billboard_setting = other.billboard_setting;
	this->sprite = other.sprite;
	this->buffer_updates = other.buffer_updates;
	this->loaded = other.loaded;
	other.loaded = false;
}

WorldTexture& WorldTexture::operator=(WorldTexture& other) {
	if (this != &other) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
		this->VAO = other.VAO;
		this->VBO = other.VBO;
#endif
		this->v_spec = other.v_spec;
		this->v_pos.resize(v_spec.num_vertices);
		this->v_pos_accessor.resize(v_spec.num_vertices);
		this->v_texcoord.resize(v_spec.num_vertices);
		this->v_texcoord_accessor.resize(v_spec.num_vertices);
		this->v_data_for_gpu.resize(v_spec.num_vertices_internal);
		for (int i = 0; i < v_spec.num_vertices; i++) {
			this->v_pos[i] = other.v_pos[i];
			this->v_texcoord[i] = other.v_texcoord[i];
			this->v_pos_accessor[i] = &this->v_pos[i];
			this->v_texcoord_accessor[i] = &this->v_texcoord[i];
			this->v_data_for_gpu[i] = other.v_data_for_gpu[i];
		}
		for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
			this->v_data_for_gpu[v_spec.vertex_bindings[i].first] = this->v_data_for_gpu[v_spec.vertex_bindings[i].second];
		}
		this->shader = other.shader;
		this->features = other.features;
		this->path = other.path;
		this->texture_orientation = other.texture_orientation;
		this->anchor = other.anchor;
		this->texture = other.texture;
		this->pos = other.pos;
		this->rot = other.rot;
		this->scale = other.scale;
		this->width = other.width;
		this->height = other.height;
		this->alpha = other.alpha;
		this->colormod = other.colormod;
		this->h_flipped = other.h_flipped;
		this->v_flipped = other.v_flipped;
		this->billboard_setting = other.billboard_setting;
		this->sprite = other.sprite;
		this->buffer_updates = other.buffer_updates;
		if (ResourceManager::get_instance()->is_tex_const_copied(texture[0])) {
			this->loaded = other.loaded;
			other.loaded = false;
		}
		else {
			this->loaded = false;
		}
	}
	return *this;
}

WorldTexture& WorldTexture::operator=(const WorldTexture& other) {
	if (this != &other) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
		this->VAO = other.VAO;
		this->VBO = other.VBO;
#endif
		this->v_spec = other.v_spec;
		this->v_pos.resize(v_spec.num_vertices);
		this->v_pos_accessor.resize(v_spec.num_vertices);
		this->v_texcoord.resize(v_spec.num_vertices);
		this->v_texcoord_accessor.resize(v_spec.num_vertices);
		this->v_data_for_gpu.resize(v_spec.num_vertices_internal);
		for (int i = 0; i < v_spec.num_vertices; i++) {
			this->v_pos[i] = other.v_pos[i];
			this->v_texcoord[i] = other.v_texcoord[i];
			this->v_pos_accessor[i] = &this->v_pos[i];
			this->v_texcoord_accessor[i] = &this->v_texcoord[i];
			this->v_data_for_gpu[i] = other.v_data_for_gpu[i];
		}
		for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
			this->v_data_for_gpu[v_spec.vertex_bindings[i].first] = this->v_data_for_gpu[v_spec.vertex_bindings[i].second];
		}
		this->shader = other.shader;
		this->features = other.features;
		this->path = other.path;
		this->texture_orientation = other.texture_orientation;
		this->anchor = other.anchor;
		this->texture = other.texture;
		this->pos = other.pos;
		this->rot = other.rot;
		this->scale = other.scale;
		this->width = other.width;
		this->height = other.height;
		this->alpha = other.alpha;
		this->colormod = other.colormod;
		this->h_flipped = other.h_flipped;
		this->v_flipped = other.v_flipped;
		this->billboard_setting = other.billboard_setting;
		this->sprite = other.sprite;
		this->buffer_updates = other.buffer_updates;
		this->loaded = other.loaded;
		ResourceManager::get_instance()->store_const_copy_addr(texture[0], (WorldTexture*)&other);
	}
	return *this;
}

WorldTexture& WorldTexture::operator=(WorldTexture&& other) noexcept {
	if (this != &other) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
		this->VAO = other.VAO;
		this->VBO = other.VBO;
#endif
		this->v_spec = other.v_spec;
		this->v_pos.resize(v_spec.num_vertices);
		this->v_pos_accessor.resize(v_spec.num_vertices);
		this->v_texcoord.resize(v_spec.num_vertices);
		this->v_texcoord_accessor.resize(v_spec.num_vertices);
		this->v_data_for_gpu.resize(v_spec.num_vertices_internal);
		for (int i = 0; i < v_spec.num_vertices; i++) {
			this->v_pos[i] = other.v_pos[i];
			this->v_texcoord[i] = other.v_texcoord[i];
			this->v_pos_accessor[i] = &this->v_pos[i];
			this->v_texcoord_accessor[i] = &this->v_texcoord[i];
			this->v_data_for_gpu[i] = other.v_data_for_gpu[i];
		}
		for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
			this->v_data_for_gpu[v_spec.vertex_bindings[i].first] = this->v_data_for_gpu[v_spec.vertex_bindings[i].second];
		}
		this->shader = other.shader;
		this->features = other.features;
		this->path = other.path;
		this->texture_orientation = other.texture_orientation;
		this->anchor = other.anchor;
		this->texture = other.texture;
		this->pos = other.pos;
		this->rot = other.rot;
		this->scale = other.scale;
		this->width = other.width;
		this->height = other.height;
		this->alpha = other.alpha;
		this->colormod = other.colormod;
		this->h_flipped = other.h_flipped;
		this->v_flipped = other.v_flipped;
		this->billboard_setting = other.billboard_setting;
		this->sprite = other.sprite;
		this->buffer_updates = other.buffer_updates;
		this->loaded = other.loaded;
		other.loaded = false;
	}
	return *this;
}

WorldTexture::~WorldTexture() {
	if (!ResourceManager::get_instance()->is_tex_const_copied(texture[0], this) && loaded) {
		destroy();
	}
}

WorldTexture&& WorldTexture::init(std::string path, unsigned char features) {
	//This function is identical to ScreenTexture::init, so documentation is there
	this->features = features;
	v_spec = (features & TEX_FEAT_4T5V) ? v_spec_4t5v : v_spec_2t4v;
	set_shader("default");
	shader->use();
	shader->set_int("f_texture", 0);

	ResourceManager* resource_manager = ResourceManager::get_instance();
	if (path.ends_with(".gif")) {
		texture = resource_manager->get_textures(path);
	}
	else {
		texture.push_back(resource_manager->get_texture(path));
	}
#ifdef TEX_IMPL_MODE_VULKAN

#else
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, width.get_val_ptr());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height.get_val_ptr());
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)offsetof(TextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	set_default_vertex_data();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	loaded = true;
	this->path = path;
	return std::move(*this);
}

WorldTexture&& WorldTexture::init(std::vector<unsigned int> texture, unsigned char features, int width, int height) {
	this->features = features;
	v_spec = (features & TEX_FEAT_4T5V) ? v_spec_4t5v : v_spec_2t4v;
	set_shader("default");
	shader->use();
	shader->set_int("f_texture", 0);
#ifdef TEX_IMPL_MODE_VULKAN

#else
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	if (width == -1) {
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	}
	if (height == -1) {
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	}
	width = width;
	height = height;
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)offsetof(TextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	set_default_vertex_data();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	loaded = true;
	this->texture = texture;
	return std::move(*this);
}

void WorldTexture::destroy() {
	if (loaded) {
		if (path != "") {
			ResourceManager::get_instance()->unuse_texture(path);
		}
		else {
#ifdef TEX_IMPL_MODE_VULKAN

#else
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
#endif
		}
		loaded = false;
	}
}

WorldTexture WorldTexture::init_copy() {
	WorldTexture ret;
	if (path != "") {
		ret.init(path, features);
	}
	else {
		ret.init(texture, features);
	}
	return ret;
}

WorldTexture&& WorldTexture::set_shader(std::string frag_shader) {
	this->shader = ShaderManager::get_instance()->get_shader("world_tex", "world_tex_" + frag_shader, "", billboard_setting);
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_features(unsigned char features) {
	if ((this->features & TEX_FEAT_4T5V) != (features & TEX_FEAT_4T5V)) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		v_spec = (features & TEX_FEAT_4T5V) ? v_spec_4t5v : v_spec_2t4v;
		set_default_vertex_data();

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	this->features = features;
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_pause(bool pause) {
	for (int i = 0; i < v_spec.num_vertices_internal; i++) {
		v_pos[i].x.set_pause(pause);
		v_pos[i].y.set_pause(pause);
		v_texcoord[i].x.set_pause(pause);
		v_texcoord[i].y.set_pause(pause);
	}
	pos.set_pause(pause);
	rot.set_pause(pause);
	scale.set_pause(pause);
	width.set_pause(pause);
	height.set_pause(pause);
	alpha.set_pause(pause);
	colormod.set_pause(pause);
	return std::move(*this);
}

std::string WorldTexture::get_path() const {
	return path;
}

WorldTexture&& WorldTexture::set_orientation(int orientation) {
	this->texture_orientation = orientation;
	return std::move(*this);
}

int WorldTexture::get_texture_orientation() const {
	return texture_orientation;
}

WorldTexture&& WorldTexture::set_anchor(TextureAnchor* anchor) {
	this->anchor = anchor;
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_pos(glm::vec3 pos) {
	this->pos = pos;
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_pos(glm::vec3 pos, int frames) {
	this->pos.set_target_val(pos, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::add_pos(glm::vec3 pos) {
	this->pos += pos;
	return std::move(*this);
}

glm::vec3 WorldTexture::get_pos() const {
	return pos.get_val();
}

TargetVar<glm::vec3> WorldTexture::get_pos_target() const {
	return pos;
}

WorldTexture&& WorldTexture::set_rot(glm::vec3 rot) {
	this->rot = rot;
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_rot(glm::vec3 rot, int frames) {
	this->rot.set_target_val(rot, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::add_rot(glm::vec3 rot) {
	this->rot += rot;
	return std::move(*this);
}

glm::vec3 WorldTexture::get_rot() const {
	return rot.get_val();
}

WorldTexture&& WorldTexture::set_width(int new_width) {
	width = new_width;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_width(int new_width, int frames) {
	width.set_target_val(new_width, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::add_width(int width) {
	this->width += width;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

int WorldTexture::get_width() const {
	return width.get_val();
}

WorldTexture&& WorldTexture::set_height(int new_height) {
	height = new_height;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_height(int new_height, int frames) {
	height.set_target_val(new_height, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::add_height(int height) {
	this->height += height;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

int WorldTexture::get_height() const {
	return height.get_val();
}

WorldTexture&& WorldTexture::set_scale(glm::vec3 scale) {
	this->scale = scale;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_scale(glm::vec3 scale, int frames) {
	this->scale.set_target_val(scale, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::add_scale(glm::vec3 scale) {
	this->scale += scale;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

glm::vec3 WorldTexture::get_scale() const {
	return scale.get_val();
}

WorldTexture&& WorldTexture::scale_right_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
	}
	else {
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
	}
	buffer_updates = std::max(buffer_updates, 1);
#endif
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_right_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
	}
	buffer_updates = std::max(buffer_updates, frames);
#endif
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_right_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, percent, 1.0);
		v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, percent, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, percent, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, percent, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_right_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_left_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_left_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_left_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, 1.0 - percent, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, 1.0 - percent, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, 1.0 - percent, 1.0);
		v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, 1.0 - percent, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_left_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_top_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
	}
	else {
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_top_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_top_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, percent, 1.0);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, percent, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
	}
	else {
		v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, percent, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, percent, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_top_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
	}
	else {
		v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_bottom_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_bottom_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_bottom_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, 1.0 - percent, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, 1.0 - percent, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, 1.0 - percent, 1.0);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, 1.0 - percent, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_bottom_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_top_right_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0) * height_scale, 1.0);
	} break;
	case TEX_FLIP_H: {
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0) * height_scale, 1.0);
	} break;
	case TEX_FLIP_V: {
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0) * height_scale, 1.0);
	}break;
	case TEX_FLIP_B: {
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0) * height_scale, 1.0);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_top_right_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	float width_scale = (float)width / (float)WINDOW_WIDTH;
	float height_scale = (float)height / (float)WINDOW_HEIGHT;
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0) * height_scale, 1.0), frames);
	} break;
	case TEX_FLIP_H: {
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0) * height_scale, 1.0), frames);
	} break;
	case TEX_FLIP_V: {
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0) * height_scale, 1.0), frames);
	}break;
	case TEX_FLIP_B: {
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0) * height_scale, 1.0), frames);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_top_right_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, percent_x, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, percent_y, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	case TEX_FLIP_H: {
		v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, percent_x, 1.0);
		v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, percent_y, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	case TEX_FLIP_V: {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, percent_x, 1.0);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, percent_y, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	case TEX_FLIP_B: {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, percent_x, 1.0);
		v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, percent_y, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_top_right_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	case TEX_FLIP_H: {
		v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	case TEX_FLIP_V: {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	}break;
	case TEX_FLIP_B: {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_top_left_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	case TEX_FLIP_H: {
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	case TEX_FLIP_V: {
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	}break;
	case TEX_FLIP_B: {
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_top_left_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	case TEX_FLIP_H: {
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	case TEX_FLIP_V: {
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	}break;
	case TEX_FLIP_B: {
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_top_left_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
		v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, percent_y, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	case TEX_FLIP_H: {
		v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, percent_y, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	case TEX_FLIP_V: {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
		v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, percent_y, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	case TEX_FLIP_B: {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, percent_y, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_top_left_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	case TEX_FLIP_H: {
		v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	case TEX_FLIP_V: {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	}break;
	case TEX_FLIP_B: {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_bottom_right_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	case TEX_FLIP_H: {
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	case TEX_FLIP_V: {
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	}break;
	case TEX_FLIP_B: {
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	}

#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_bottom_right_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	case TEX_FLIP_H: {
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	case TEX_FLIP_V: {
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	}break;
	case TEX_FLIP_B: {
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_bottom_right_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, percent_x, 1.0);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	case TEX_FLIP_H: {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, percent_x, 1.0);
		v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	case TEX_FLIP_V: {
		v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, percent_x, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	case TEX_FLIP_B: {
		v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, percent_x, 1.0);
		v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_bottom_right_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	case TEX_FLIP_H: {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	case TEX_FLIP_V: {
		v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	case TEX_FLIP_B: {
		v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_bottom_left_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	case TEX_FLIP_H: {
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	case TEX_FLIP_V: {
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	}break;
	case TEX_FLIP_B: {
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::scale_bottom_left_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	case TEX_FLIP_H: {
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	case TEX_FLIP_V: {
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	}break;
	case TEX_FLIP_B: {
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_bottom_left_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
		v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	case TEX_FLIP_H: {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
	} break;
	case TEX_FLIP_V: {
		v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
		v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	case TEX_FLIP_B: {
		v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::crop_bottom_left_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
	case TEX_FLIP_N: {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	case TEX_FLIP_H: {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
	} break;
	case TEX_FLIP_V: {
		v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	}break;
	case TEX_FLIP_B: {
		v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
	} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_corners(glm::vec2 c1, glm::vec2 c2) {
	glm::vec2 scale_vec = glm::vec2(19.2, 10.8);
	c1 /= scale_vec;
	c2 /= scale_vec;
	v_pos_accessor[v_spec.bottom_left_idx]->x = c1.x;
	v_pos_accessor[v_spec.bottom_left_idx]->y = c1.y;
	v_pos_accessor[v_spec.top_left_idx]->x = c1.x;
	v_pos_accessor[v_spec.top_left_idx]->y = c2.y;
	v_pos_accessor[v_spec.bottom_right_idx]->x = c2.x;
	v_pos_accessor[v_spec.bottom_right_idx]->y = c1.y;
	v_pos_accessor[v_spec.top_right_idx]->x = c2.x;
	v_pos_accessor[v_spec.top_right_idx]->y = c2.y;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_alpha(unsigned char alpha) {
	this->alpha = alpha;
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_alpha(unsigned char alpha, int frames) {
	this->alpha.set_target_val(alpha, frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::add_alpha(unsigned char alpha) {
	this->alpha += alpha;
	return std::move(*this);
}

unsigned char WorldTexture::get_alpha() const {
	return alpha.get_val();
}

WorldTexture&& WorldTexture::set_colormod(glm::vec3 color) {
	colormod = color / glm::vec3(255.0f);
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_colormod(glm::vec3 color, int frames) {
	colormod.set_target_val(color / glm::vec3(255.0f), frames);
	return std::move(*this);
}

WorldTexture&& WorldTexture::add_colormod(glm::vec3 color) {
	colormod += color / glm::vec3(255.0f);
	return std::move(*this);
}

glm::vec3 WorldTexture::get_colormod() const {
	return colormod.get_val();
}

WorldTexture&& WorldTexture::flip_h() {
	h_flipped = !h_flipped;
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_h_flipped(bool h_flipped) {
	this->h_flipped = h_flipped;
	return std::move(*this);
}

bool WorldTexture::is_h_flipped() const {
	return h_flipped;
}

WorldTexture&& WorldTexture::flip_v() {
	v_flipped = !v_flipped;
	return std::move(*this);
}

WorldTexture&& WorldTexture::set_v_flipped(bool v_flipped) {
	this->v_flipped = v_flipped;
	return std::move(*this);
}

bool WorldTexture::is_v_flipped() const {
	return v_flipped;
}

int WorldTexture::get_flipped() const {
	return (int)h_flipped | (((int)v_flipped) * 2);
}

WorldTexture&& WorldTexture::set_billboard_setting(int billboard_setting) {
	this->shader = ShaderManager::get_instance()->get_shader_switch_features(shader, this->billboard_setting, billboard_setting);
	this->billboard_setting = billboard_setting;
	return std::move(*this);
}

int WorldTexture::get_billboard_setting() const {
	return billboard_setting;
}

WorldTexture&& WorldTexture::set_sprite(unsigned int sprite) {
	if (sprite < texture.size()) {
		this->sprite = sprite;
	}
	return std::move(*this);
}

WorldTexture&& WorldTexture::next_sprite() {
	if (sprite == texture.size() - 1) {
		sprite -= texture.size();
	}
	sprite++;
	return std::move(*this);
}

WorldTexture&& WorldTexture::prev_sprite() {
	if (sprite == 0) {
		sprite = texture.size();
	}
	sprite--;
	return std::move(*this);
}

unsigned int WorldTexture::get_sprite() const {
	return sprite;
}

void WorldTexture::render() {
	glm::vec3 scale_vec = glm::vec3(
		WINDOW_WIDTH / 100.0f,
		WINDOW_HEIGHT / 100.0f,
		WINDOW_DEPTH / 100.0f
	);
	glm::vec3 render_pos = pos.get_val();
	render_pos /= scale_vec;
	if (texture_orientation & TEXTURE_LEFT) {
		render_pos.x += scale_vec.x;
	}
	else if (texture_orientation & TEXTURE_RIGHT) {
		render_pos.x -= scale_vec.x;
	}
	if (texture_orientation & TEXTURE_TOP) {
		render_pos.y -= scale_vec.y;
	}
	else if (texture_orientation & TEXTURE_BOTTOM) {
		render_pos.y += scale_vec.y;
	}
	glm::mat4 matrix = glm::scale(glm::mat4(1.0f), scale.get_val());
	matrix = glm::translate(matrix, render_pos);
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().x), glm::vec3(1.0, 0.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().y), glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().z), glm::vec3(0.0, 0.0, 1.0));
	
	if (anchor) {
		matrix = anchor->world_mat * matrix;
	}
#ifdef TEX_IMPL_MODE_VULKAN

#else
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	if (buffer_updates) {
		update_buffer_data();
		buffer_updates--;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[sprite]);
	shader->use();
	shader->set_vec3("f_colormod", colormod.get_val());
	shader->set_float("f_alphamod", alpha.get_val() / 255.0f);
	shader->set_mat4("matrix", matrix);
	switch (billboard_setting) {
		case BILLBOARD_OFF:
		default: {

		} break;
		case BILLBOARD_ON: {
		} break;
		case BILLBOARD_ON_FIXED_SIZE: {

		} break;
	}
	glDepthMask(GL_FALSE);
	if (glIsEnabled(GL_CULL_FACE)) {
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, v_spec.num_vertices_internal);
		glEnable(GL_CULL_FACE);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, v_spec.num_vertices_internal);
	}
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

void WorldTexture::set_default_vertex_data() {
	v_pos.resize(v_spec.num_vertices);
	v_pos_accessor.resize(v_spec.num_vertices);
	v_texcoord.resize(v_spec.num_vertices);
	v_texcoord_accessor.resize(v_spec.num_vertices);
	v_data_for_gpu.resize(v_spec.num_vertices_internal);
	v_pos[v_spec.top_left_idx] = glm::vec2(-1.0, 1.0);
	v_pos[v_spec.top_right_idx] = glm::vec2(1.0, 1.0);
	v_pos[v_spec.bottom_left_idx] = glm::vec2(-1.0, -1.0);
	v_pos[v_spec.bottom_right_idx] = glm::vec2(1.0, -1.0);

	v_texcoord[v_spec.top_left_idx] = glm::vec2(0.0, 1.0);
	v_texcoord[v_spec.top_right_idx] = glm::vec2(1.0, 1.0);
	v_texcoord[v_spec.bottom_left_idx] = glm::vec2(0.0, 0.0);
	v_texcoord[v_spec.bottom_right_idx] = glm::vec2(1.0, 0.0);

	if (v_spec.num_vertices == 5) {
		v_pos[TC_4T5V_MIDDLE] = glm::vec2(0.0, 0.0);
		v_texcoord[TC_4T5V_MIDDLE] = glm::vec2(0.5, 0.5);
	}
	for (int i = 0; i < v_spec.num_vertices; i++) {
		v_pos_accessor[i] = &v_pos[i];
		v_texcoord_accessor[i] = &v_texcoord[i];
		v_data_for_gpu[i] = { v_pos[i], v_texcoord[i] };
	}
	for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
		v_data_for_gpu[v_spec.vertex_bindings[i].first] = v_data_for_gpu[v_spec.vertex_bindings[i].second];
	}

	glBufferData(GL_ARRAY_BUFFER, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data(), GL_DYNAMIC_DRAW);
}

void WorldTexture::update_buffer_data() {
	bool update = false;
	for (int i = 0; i < v_spec.num_vertices; i++) {
		if (v_data_for_gpu[i].pos != v_pos[i]
		|| v_data_for_gpu[i].tex_coord != v_texcoord[i]) {
			if (v_spec.num_vertices == 5) {
				if (v_pos[TC_4T5V_TOP_LEFT].x > 0.0f != h_flipped
					|| v_pos[TC_4T5V_BOTTOM_LEFT].x > 0.0f != h_flipped) {
					v_pos[TC_4T5V_MIDDLE].x = (h_flipped ?
						std::min(v_pos[TC_4T5V_TOP_LEFT].x.get_val(), v_pos[TC_4T5V_BOTTOM_LEFT].x.get_val())
						: std::max(v_pos[TC_4T5V_TOP_LEFT].x.get_val(), v_pos[TC_4T5V_BOTTOM_LEFT].x.get_val()));
				}
				else if (v_pos[TC_4T5V_TOP_RIGHT].x < 0.0f != h_flipped
					|| v_pos[TC_4T5V_BOTTOM_RIGHT].x < 0.0f != h_flipped) {
					v_pos[TC_4T5V_MIDDLE].x = (h_flipped ?
						std::max(v_pos[TC_4T5V_TOP_RIGHT].x.get_val(), v_pos[TC_4T5V_BOTTOM_RIGHT].x.get_val())
						: std::min(v_pos[TC_4T5V_TOP_RIGHT].x.get_val(), v_pos[TC_4T5V_BOTTOM_RIGHT].x.get_val()));
				}
				else {
					v_pos[TC_4T5V_MIDDLE].x = 0.0f;
				}

				if (v_texcoord[TC_4T5V_TOP_LEFT].x > 0.5f
					|| v_texcoord[TC_4T5V_BOTTOM_LEFT].x > 0.5f) {
					v_texcoord[TC_4T5V_MIDDLE].x = std::max(v_texcoord[TC_4T5V_TOP_LEFT].x.get_val(), v_texcoord[TC_4T5V_BOTTOM_LEFT].x.get_val());
				}
				else if (v_texcoord[TC_4T5V_TOP_RIGHT].x < 0.5f
					|| v_texcoord[TC_4T5V_BOTTOM_RIGHT].x < 0.5f) {
					v_texcoord[TC_4T5V_MIDDLE].x = std::min(v_texcoord[TC_4T5V_TOP_RIGHT].x.get_val(), v_texcoord[TC_4T5V_BOTTOM_RIGHT].x.get_val());
				}
				else {
					v_texcoord[TC_4T5V_MIDDLE].x = 0.5f;
				}

				if (v_pos[TC_4T5V_BOTTOM_LEFT].y > 0.0f != v_flipped
					|| v_pos[TC_4T5V_BOTTOM_RIGHT].y > 0.0f != v_flipped) {
					v_pos[TC_4T5V_MIDDLE].y = (v_flipped ?
						std::min(v_pos[TC_4T5V_BOTTOM_LEFT].y.get_val(), v_pos[TC_4T5V_BOTTOM_RIGHT].y.get_val())
						: std::max(v_pos[TC_4T5V_BOTTOM_LEFT].y.get_val(), v_pos[TC_4T5V_BOTTOM_RIGHT].y.get_val()));
				}
				else if (v_pos[TC_4T5V_TOP_LEFT].y < 0.0f != v_flipped
					|| v_pos[TC_4T5V_TOP_RIGHT].y < 0.0f != v_flipped) {
					v_pos[TC_4T5V_MIDDLE].y = (v_flipped ?
						std::max(v_pos[TC_4T5V_TOP_LEFT].y.get_val(), v_pos[TC_4T5V_TOP_RIGHT].y.get_val())
						: std::min(v_pos[TC_4T5V_TOP_LEFT].y.get_val(), v_pos[TC_4T5V_TOP_RIGHT].y.get_val()));
				}
				else {
					v_pos[TC_4T5V_MIDDLE].y = 0.0f;
				}

				if (v_texcoord[TC_4T5V_BOTTOM_LEFT].y > 0.5f
					|| v_texcoord[TC_4T5V_BOTTOM_RIGHT].y > 0.5f) {
					v_texcoord[TC_4T5V_MIDDLE].y = std::max(v_texcoord[TC_4T5V_BOTTOM_LEFT].y.get_val(), v_texcoord[TC_4T5V_BOTTOM_RIGHT].y.get_val());
				}
				else if (v_texcoord[TC_4T5V_TOP_LEFT].y < 0.5f
					|| v_texcoord[TC_4T5V_TOP_RIGHT].y < 0.5f) {
					v_texcoord[TC_4T5V_MIDDLE].y = std::min(v_texcoord[TC_4T5V_TOP_LEFT].y.get_val(), v_texcoord[TC_4T5V_TOP_RIGHT].y.get_val());
				}
				else {
					v_texcoord[TC_4T5V_MIDDLE].y = 0.5f;
				}
			}
			for (int i = 0; i < v_spec.num_vertices; i++) {
				v_data_for_gpu[i] = { v_pos[i], v_texcoord[i] };
			}
			for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
				v_data_for_gpu[v_spec.vertex_bindings[i].first] = v_data_for_gpu[v_spec.vertex_bindings[i].second];
			}
			glBufferSubData(GL_ARRAY_BUFFER, 0, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data());
			return;
		}
	}
}
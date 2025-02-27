#include "ScreenTexture.h"
#include "WindowConstants.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "utils.h"

/// <summary>
/// Default constructor for ScreenTexture
/// </summary>
ScreenTexture::ScreenTexture() {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	this->VAO = 0;
	this->VBO = 0;
#endif
	this->shader = nullptr;
	this->features = 0;
	this->path = "";
	this->screen_orientation = TEXTURE_MID;
	this->texture_orientation = TEXTURE_MID;
	this->anchor = nullptr;
	this->pos = glm::vec3(0.0);
	this->rot = glm::vec3(0.0);
	this->magnitude = glm::vec3(0.0);
	this->base_width = 0;
	this->width_scale = 1.0;
	this->base_height = 0;
	this->height_scale = 1.0;
	this->alpha = 255;
	this->colormod = glm::vec3(0.0);
	this->h_flipped = false;
	this->v_flipped = false;
	this->sprite = 0;
	this->buffer_updates = 0;
	this->loaded = false;
}

/// <summary>
/// Parameterized constructor for ScreenTexture.
/// </summary>
/// <param name="path">The path to the image which this texture will render.</param>
/// <param name="features">0, or TEX_FEAT_4T5V for textures which need to provide 4 triangles 
/// of vertex info (I.E. textures which crop the bottom right or top left corners)</param>
ScreenTexture::ScreenTexture(std::string path, unsigned char features) : ScreenTexture() {
	this->init(path, features);
}

ScreenTexture::ScreenTexture(unsigned int texture, unsigned char features, int width, int height) : ScreenTexture() {
	this->init(texture, features, width, height);
}

ScreenTexture::ScreenTexture(std::vector<unsigned int> texture, unsigned char features, int width, int height) : ScreenTexture() {
	this->init(texture, features, width, height);
}

ScreenTexture::ScreenTexture(ScreenTexture& other) {
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
	this->texture = other.texture;
	this->screen_orientation = other.screen_orientation;
	this->texture_orientation = other.texture_orientation;
	this->anchor = other.anchor;
	this->pos = other.pos;
	this->rot = other.rot;
	this->magnitude = other.magnitude;
	this->base_width = other.base_width;
	this->width_scale = other.width_scale;
	this->base_height = other.base_height;
	this->height_scale = other.height_scale;
	this->alpha = other.alpha;
	this->colormod = other.colormod;
	this->h_flipped = other.h_flipped;
	this->v_flipped = other.v_flipped;
	this->buffer_updates = other.buffer_updates;
	this->sprite = other.sprite;
	if (!texture.empty() && ResourceManager::get_instance()->is_tex_const_copied(texture[0])) {
		this->loaded = other.loaded;
		other.loaded = false;
	}
	else {
		this->loaded = false;
	}
}

ScreenTexture::ScreenTexture(const ScreenTexture& other) {
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
	this->texture = other.texture;
	this->screen_orientation = other.screen_orientation;
	this->texture_orientation = other.texture_orientation;
	this->anchor = other.anchor;
	this->pos = other.pos;
	this->rot = other.rot;
	this->magnitude = other.magnitude;
	this->base_width = other.base_width;
	this->width_scale = other.width_scale;
	this->base_height = other.base_height;
	this->height_scale = other.height_scale;
	this->alpha = other.alpha;
	this->colormod = other.colormod;
	this->h_flipped = other.h_flipped;
	this->v_flipped = other.v_flipped;
	this->sprite = other.sprite;
	this->buffer_updates = other.buffer_updates;
	this->loaded = other.loaded;
	if (!texture.empty()) {
		ResourceManager::get_instance()->store_const_copy_addr(texture[0], (ScreenTexture*)&other);
	}
}

ScreenTexture::ScreenTexture(ScreenTexture&& other) noexcept {
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
	this->texture = other.texture;
	this->screen_orientation = other.screen_orientation;
	this->texture_orientation = other.texture_orientation;
	this->anchor = other.anchor;
	this->pos = other.pos;
	this->rot = other.rot;
	this->magnitude = other.magnitude;
	this->base_width = other.base_width;
	this->width_scale = other.width_scale;
	this->base_height = other.base_height;
	this->height_scale = other.height_scale;
	this->alpha = other.alpha;
	this->colormod = other.colormod;
	this->h_flipped = other.h_flipped;
	this->v_flipped = other.v_flipped;
	this->sprite = other.sprite;
	this->buffer_updates = other.buffer_updates;
	this->loaded = other.loaded;
	other.loaded = false;
}

ScreenTexture& ScreenTexture::operator=(ScreenTexture& other) {
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
		this->texture = other.texture;
		this->screen_orientation = other.screen_orientation;
		this->texture_orientation = other.texture_orientation;
		this->anchor = other.anchor;
		this->pos = other.pos;
		this->rot = other.rot;
		this->magnitude = other.magnitude;
		this->base_width = other.base_width;
		this->width_scale = other.width_scale;
		this->base_height = other.base_height;
		this->height_scale = other.height_scale;
		this->alpha = other.alpha;
		this->colormod = other.colormod;
		this->h_flipped = other.h_flipped;
		this->v_flipped = other.v_flipped;
		this->sprite = other.sprite;
		this->buffer_updates = other.buffer_updates;
		if (!texture.empty() && ResourceManager::get_instance()->is_tex_const_copied(texture[0])) {
			this->loaded = other.loaded;
			other.loaded = false;
		}
		else {
			this->loaded = false;
		}
	}
	return *this;
}

ScreenTexture& ScreenTexture::operator=(const ScreenTexture& other) {
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
		this->texture = other.texture;
		this->screen_orientation = other.screen_orientation;
		this->texture_orientation = other.texture_orientation;
		this->anchor = other.anchor;
		this->pos = other.pos;
		this->rot = other.rot;
		this->magnitude = other.magnitude;
		this->base_width = other.base_width;
		this->width_scale = other.width_scale;
		this->base_height = other.base_height;
		this->height_scale = other.height_scale;
		this->alpha = other.alpha;
		this->colormod = other.colormod;
		this->h_flipped = other.h_flipped;
		this->v_flipped = other.v_flipped;
		this->sprite = other.sprite;
		this->buffer_updates = other.buffer_updates;
		this->loaded = other.loaded;
		if (!texture.empty()) {
			ResourceManager::get_instance()->store_const_copy_addr(texture[0], (ScreenTexture*)&other);
		}
	}
	return *this;
}

ScreenTexture& ScreenTexture::operator=(ScreenTexture&& other) noexcept {
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
		this->texture = other.texture;
		this->screen_orientation = other.screen_orientation;
		this->texture_orientation = other.texture_orientation;
		this->anchor = other.anchor;
		this->pos = other.pos;
		this->rot = other.rot;
		this->magnitude = other.magnitude;
		this->base_width = other.base_width;
		this->width_scale = other.width_scale;
		this->base_height = other.base_height;
		this->height_scale = other.height_scale;
		this->alpha = other.alpha;
		this->colormod = other.colormod;
		this->h_flipped = other.h_flipped;
		this->v_flipped = other.v_flipped;
		this->sprite = other.sprite;
		this->buffer_updates = other.buffer_updates;
		this->loaded = other.loaded;
		other.loaded = false;
	}
	return *this;
}

ScreenTexture::~ScreenTexture() {
	if ((texture.empty() || !ResourceManager::get_instance()->is_tex_const_copied(texture[0], this)) && loaded) {
		destroy();
	}
}

ScreenTexture&& ScreenTexture::init(std::string path, unsigned char features) {
	//Setting up shaders is the same between OpenGL and Vulkan
	this->features = features;
	v_spec = (features & TEX_FEAT_4T5V) ? v_spec_4t5v : v_spec_2t4v;
	set_shader("default");
	shader->use();
	shader->set_int("f_texture", 0);
	
	//Texture loading is technically different within the ResourceManager, but both impls still use
	//a texture ID system so this part is also the same

	ResourceManager* resource_manager = ResourceManager::get_instance();
	if (path.ends_with(".gif")) {
		texture = resource_manager->get_textures(path);
}
	else {
		texture.push_back(resource_manager->get_texture(path));
	}
#ifdef TEX_IMPL_MODE_VULKAN

#else
	//Extracting GL texture info is obviously different, though tbh this could also be queried from 
	//the ResourceManager itself

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, base_width.get_val_ptr());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, base_height.get_val_ptr());
	glBindTexture(GL_TEXTURE_2D, 0);

	//Tell the GPU how to interpret our vertex data. Vulkan uses VBOs but not VAOs

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoord), (void*)offsetof(TextureCoord, tex_coord));
	glEnableVertexAttribArray(1);

	set_default_vertex_data();

	if (path == "resource/scene/battle/ui/meter/health.png") {
		buffer_updates = 1;
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	loaded = true;
	this->path = path;
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::init(unsigned int texture, unsigned char features, int width, int height) {
	this->features = features;
	v_spec = (features & TEX_FEAT_4T5V) ? v_spec_4t5v : v_spec_2t4v;
	set_shader("default");
	shader->use();
	shader->set_int("f_texture", 0);
#ifdef TEX_IMPL_MODE_VULKAN

#else
	glBindTexture(GL_TEXTURE_2D, texture);
	if (width == -1) {
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	}
	if (height == -1) {
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	}
	base_width = width;
	base_height = height;
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
	this->texture.push_back(texture);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::init(std::vector<unsigned int> texture, unsigned char features, int width, int height) {
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
	base_width = width;
	base_height = height;
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

void ScreenTexture::destroy() {
	if (loaded) {
		if (path != "") {
			//If we were given a path, then we should properly unuse the texture from the 
			//ResourceManager regardless of our implementation
			ResourceManager::get_instance()->unuse_texture(path);
		}
		else {
			//If not, then that means our ScreenTexture instance either:
			//A. Is referencing a target texture from a Framebuffer, in which case the texture will
			//	 properly unload itself when the Framebuffer is destroyed
			//B. Has no texture at all, in which case there's no image to unuse or destroy

			//(We still need to destroy the VAO/VBO if we're using GL though)
#ifdef TEX_IMPL_MODE_VULKAN

#else
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
#endif
		}
		loaded = false;
	}
}

ScreenTexture ScreenTexture::init_copy() const {
	ScreenTexture ret;
	if (path != "") {
		ret.init(path, features);
	}
	else {
		ret.init(texture, features);
	}
	return ret;
}

ScreenTexture&& ScreenTexture::set_shader(std::string frag_shader) {
	this->shader = ShaderManager::get_instance()->get_shader("screen_tex", "screen_tex_" + frag_shader, "", 0);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_features(unsigned char features) {
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

ScreenTexture&& ScreenTexture::set_pause(bool pause) {
	for (int i = 0; i < v_spec.num_vertices_internal; i++) {
		v_pos[i].x.set_pause(pause);
		v_pos[i].y.set_pause(pause);
		v_texcoord[i].x.set_pause(pause);
		v_texcoord[i].y.set_pause(pause);
	}
	pos.set_pause(pause);
	rot.set_pause(pause);
	base_width.set_pause(pause);
	base_height.set_pause(pause);
	width_scale.set_pause(pause);
	height_scale.set_pause(pause);
	alpha.set_pause(pause);
	colormod.set_pause(pause);
	return std::move(*this);
}

std::string ScreenTexture::get_path() const {
	return path;
}

ScreenTexture&& ScreenTexture::set_screen_orientation(int orientation) {
	this->screen_orientation = orientation;
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_texture_orientation(int orientation) {
	this->texture_orientation = orientation;
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_orientation(int orientation) {
	this->screen_orientation = orientation;
	this->texture_orientation = orientation;
	return std::move(*this);
}

int ScreenTexture::get_screen_orientation() const {
	return screen_orientation;
}

int ScreenTexture::get_texture_orientation() const {
	return texture_orientation;
}

ScreenTexture&& ScreenTexture::set_anchor(TextureAnchor* anchor) {
	this->anchor = anchor;
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_pos(glm::vec3 pos) {
	this->pos = pos;
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_pos(glm::vec3 pos, int frames) {
	this->pos.set_target_val(pos, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::add_pos(glm::vec3 pos) {
	this->pos += pos;
	return std::move(*this);
}

glm::vec3 ScreenTexture::get_pos() const {
	return pos.get_val();
}

TargetVar<glm::vec3> ScreenTexture::get_pos_target() const {
	return pos;
}

ScreenTexture&& ScreenTexture::set_rot(glm::vec3 rot) {
	this->rot = rot;
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_rot(glm::vec3 rot, int frames) {
	this->rot.set_target_val(rot, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::add_rot(glm::vec3 rot) {
	this->rot += rot;
	return std::move(*this);
}

glm::vec3 ScreenTexture::get_rot() const {
	return rot.get_val();
}

ScreenTexture&& ScreenTexture::set_magnitude(glm::vec3 magnitude) {
	this->magnitude = magnitude;
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_magnitude(glm::vec3 magnitude, int frames) {
	this->magnitude.set_target_val(magnitude, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::add_magnitude(glm::vec3 magnitude) {
	this->magnitude += magnitude;
	return std::move(*this);
}

glm::vec3 ScreenTexture::get_magnitude() const {
	return magnitude.get_val();
}

ScreenTexture&& ScreenTexture::set_base_width(int new_width) {
	base_width = new_width;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_base_width(int new_width, int frames) {
	base_width.set_target_val(new_width, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::add_base_width(int width) {
	base_width += width;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

int ScreenTexture::get_base_width() const {
	return base_width.get_val();
}

ScreenTexture&& ScreenTexture::set_base_height(int new_height) {
	base_height = new_height;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_base_height(int new_height, int frames) {
	base_height.set_target_val(new_height, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::add_base_height(int height) {
	base_height += height;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

int ScreenTexture::get_base_height() const {
	return base_height.get_val();
}

float ScreenTexture::get_width() const {
	return width_scale * base_width;
}

float ScreenTexture::get_height() const {
	return height_scale * base_height;
}

ScreenTexture&& ScreenTexture::set_width_scale(float scale) {
	width_scale = scale;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_width_scale(float scale, int frames) {
	width_scale.set_target_val(scale, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::add_width_scale(float scale) {
	width_scale += scale;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

float ScreenTexture::get_width_scale() const {
	return width_scale.get_val();
}

ScreenTexture&& ScreenTexture::set_height_scale(float scale) {
	height_scale = scale;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_height_scale(float scale, int frames) {
	height_scale.set_target_val(scale, frames);
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::add_height_scale(float scale) {
	height_scale += scale;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

float ScreenTexture::get_height_scale() const {
	return height_scale.get_val();
}

ScreenTexture&& ScreenTexture::set_scale(float scale) {
	set_width_scale(scale);
	set_height_scale(scale);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_scale(float scale, int frames) {
	set_width_scale(scale, frames);
	set_height_scale(scale, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::scale_right_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
	}
	else {
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::scale_right_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::crop_right_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (h_flipped) {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, percent, 1.0);
		v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, percent, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, percent, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, percent, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::crop_right_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (h_flipped) {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::scale_left_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::scale_left_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::crop_left_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (h_flipped) {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, 1.0 - percent, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, 1.0 - percent, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, 1.0 - percent, 1.0);
		v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, 1.0 - percent, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::crop_left_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (h_flipped) {
		v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::scale_top_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (v_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
	}
	else {
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::scale_top_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (v_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::crop_top_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (v_flipped) {
		v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, percent, 1.0);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, percent, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
	}
	else {
		v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, percent, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, percent, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::crop_top_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (v_flipped) {
		v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
	}
	else {
		v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, percent, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::scale_bottom_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (v_flipped) {
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::scale_bottom_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (v_flipped) {
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::crop_bottom_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (v_flipped) {
		v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, 1.0 - percent, 1.0);
		v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, 1.0 - percent, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0), 1.0);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, 1.0 - percent, 1.0);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, 1.0 - percent, 1.0);
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0), 1.0);
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::crop_bottom_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent < 0.0 || percent > 1.0) {
		return std::move(*this);
	}
	if (v_flipped) {
		v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0), 1.0), frames);
	}
	else {
		v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent, 1.0), frames);
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0), 1.0), frames);
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::scale_top_right_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
		case TEX_FLIP_N: {
			v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
		case TEX_FLIP_H: {
			v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
		case TEX_FLIP_V: {
			v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		}break;
		case TEX_FLIP_B: {
			v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::scale_top_right_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN

#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return std::move(*this);
	}
	switch (get_flipped()) {
		case TEX_FLIP_N: {
			v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
		case TEX_FLIP_H: {
			v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
		case TEX_FLIP_V: {
			v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		}break;
		case TEX_FLIP_B: {
			v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
	}
#endif
	buffer_updates = std::max(buffer_updates, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::crop_top_right_corner(float percent_x, float percent_y) {
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

ScreenTexture&& ScreenTexture::crop_top_right_corner(float percent_x, float percent_y, int frames) {
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

ScreenTexture&& ScreenTexture::scale_top_left_corner(float percent_x, float percent_y) {
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

ScreenTexture&& ScreenTexture::scale_top_left_corner(float percent_x, float percent_y, int frames) {
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

ScreenTexture&& ScreenTexture::crop_top_left_corner(float percent_x, float percent_y) {
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

ScreenTexture&& ScreenTexture::crop_top_left_corner(float percent_x, float percent_y, int frames) {
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

ScreenTexture&& ScreenTexture::scale_bottom_right_corner(float percent_x, float percent_y) {
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

ScreenTexture&& ScreenTexture::scale_bottom_right_corner(float percent_x, float percent_y, int frames) {
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

ScreenTexture&& ScreenTexture::crop_bottom_right_corner(float percent_x, float percent_y) {
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

ScreenTexture&& ScreenTexture::crop_bottom_right_corner(float percent_x, float percent_y, int frames) {
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

ScreenTexture&& ScreenTexture::scale_bottom_left_corner(float percent_x, float percent_y) {
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

ScreenTexture&& ScreenTexture::scale_bottom_left_corner(float percent_x, float percent_y, int frames) {
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

ScreenTexture&& ScreenTexture::crop_bottom_left_corner(float percent_x, float percent_y) {
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

ScreenTexture&& ScreenTexture::crop_bottom_left_corner(float percent_x, float percent_y, int frames) {
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

ScreenTexture&& ScreenTexture::set_alpha(unsigned char alpha) {
	this->alpha = alpha;
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_alpha(unsigned char alpha, int frames) {
	this->alpha.set_target_val(alpha, frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::add_alpha(unsigned char alpha) {
	this->alpha += alpha;
	return std::move(*this);
}

unsigned char ScreenTexture::get_alpha() const {
	return alpha.get_val();
}

ScreenTexture&& ScreenTexture::set_colormod(glm::vec3 color) {
	colormod = color / glm::vec3(255.0f);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_colormod(glm::vec3 color, int frames) {
	colormod.set_target_val(color / glm::vec3(255.0f), frames);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::add_colormod(glm::vec3 color) {
	colormod += color / glm::vec3(255.0f);
	return std::move(*this);
}

glm::vec3 ScreenTexture::get_colormod() const {
	return colormod.get_val();
}

ScreenTexture&& ScreenTexture::flip_h() {
	std::swap(v_pos[v_spec.bottom_left_idx].x.get_val_ref(), v_pos[v_spec.bottom_right_idx].x.get_val_ref());
	std::swap(v_pos[v_spec.top_left_idx].x.get_val_ref(), v_pos[v_spec.top_right_idx].x.get_val_ref());
	std::swap(v_pos_accessor[v_spec.bottom_left_idx]->x.get_val_ref(), v_pos_accessor[v_spec.bottom_right_idx]->x.get_val_ref());
	std::swap(v_texcoord_accessor[v_spec.bottom_left_idx]->x.get_val_ref(), v_texcoord_accessor[v_spec.bottom_right_idx]->x.get_val_ref());
	std::swap(v_pos_accessor[v_spec.top_left_idx]->x.get_val_ref(), v_pos_accessor[v_spec.top_right_idx]->x.get_val_ref());
	std::swap(v_texcoord_accessor[v_spec.top_left_idx]->x.get_val_ref(), v_texcoord_accessor[v_spec.top_right_idx]->x.get_val_ref());
	h_flipped = !h_flipped;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_h_flipped(bool h_flipped) {
	if (this->h_flipped != h_flipped) {
		return flip_h();
	}
	return std::move(*this);
}

bool ScreenTexture::is_h_flipped() const {
	return h_flipped;
}

ScreenTexture&& ScreenTexture::flip_v() {
	std::swap(v_pos[v_spec.bottom_left_idx].y.get_val_ref(), v_pos[v_spec.top_left_idx].y.get_val_ref());
	std::swap(v_pos[v_spec.bottom_right_idx].y.get_val_ref(), v_pos[v_spec.top_right_idx].y.get_val_ref());
	std::swap(v_pos_accessor[v_spec.bottom_left_idx]->y.get_val_ref(), v_pos_accessor[v_spec.top_left_idx]->y.get_val_ref());
	std::swap(v_texcoord_accessor[v_spec.bottom_left_idx]->y.get_val_ref(), v_texcoord_accessor[v_spec.top_left_idx]->y.get_val_ref());
	std::swap(v_pos_accessor[v_spec.bottom_right_idx]->y.get_val_ref(), v_pos_accessor[v_spec.top_right_idx]->y.get_val_ref());
	std::swap(v_texcoord_accessor[v_spec.bottom_right_idx]->y.get_val_ref(), v_texcoord_accessor[v_spec.top_right_idx]->y.get_val_ref());
	v_flipped = !v_flipped;
	buffer_updates = std::max(buffer_updates, 1);
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::set_v_flipped(bool v_flipped) {
	if (this->v_flipped != v_flipped) {
		return flip_v();
	}
	return std::move(*this);
}

bool ScreenTexture::is_v_flipped() const {
	return v_flipped;
}

int ScreenTexture::get_flipped() const {
	return (int)h_flipped | (((int)v_flipped) * 2);
}

ScreenTexture&& ScreenTexture::set_sprite(unsigned int sprite) {
	if (sprite < texture.size()) {
		this->sprite = sprite;
	}
	return std::move(*this);
}
	

ScreenTexture&& ScreenTexture::next_sprite() {
	if (sprite == texture.size() - 1) {
		sprite -= texture.size();
	}
	sprite++;
	return std::move(*this);
}

ScreenTexture&& ScreenTexture::prev_sprite() {
	if (sprite == 0) {
		sprite = texture.size();
	}
	sprite--;
	return std::move(*this);
}

unsigned int ScreenTexture::get_sprite() const {
	return sprite;
}

void ScreenTexture::render() {
	if (texture.empty()) return;
	glm::vec3 render_pos = pos.get_val();
	float window_width = WINDOW_WIDTH;
	float window_height = WINDOW_HEIGHT;
	if (anchor) {
		window_width *= anchor->w;
		window_height *= anchor->h;
	}
	if (screen_orientation & TEXTURE_LEFT) {
		render_pos.x -= window_width;
	}
	else if (screen_orientation & TEXTURE_RIGHT) {
		render_pos.x *= -1.0;
		render_pos.x += window_width;
	}
	
	if (screen_orientation & TEXTURE_TOP) {
		render_pos.y *= -1.0;
		render_pos.y += window_height;
	}
	else if (screen_orientation & TEXTURE_BOTTOM) {
		render_pos.y -= window_height;
	}
	
	if (texture_orientation & TEXTURE_LEFT) {
		render_pos.x += get_width();
	}
	else if (texture_orientation & TEXTURE_RIGHT) {
		render_pos.x -= get_width();
	}
	if (texture_orientation & TEXTURE_TOP) {
		render_pos.y -= get_height();
	}
	else if (texture_orientation & TEXTURE_BOTTOM) {
		render_pos.y += get_height();
	}
	render_pos.x /= WINDOW_WIDTH;
	render_pos.y /= WINDOW_HEIGHT;
#ifdef TEX_IMPL_MODE_VULKAN

#else
	glm::mat4 matrix = glm::translate(glm::mat4(1.0), render_pos);
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().x), glm::vec3(1.0, 0.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().y), glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().z), glm::vec3(0.0, 0.0, 1.0));
	matrix = glm::translate(matrix, magnitude.get_val() / glm::vec3(WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f));
	if (anchor) {
		matrix = anchor->screen_mat * matrix;
	}
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
	if (glIsEnabled(GL_CULL_FACE)) {
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, v_spec.num_vertices_internal);
		glEnable(GL_CULL_FACE);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, v_spec.num_vertices_internal);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

void ScreenTexture::set_default_vertex_data() {
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
	glm::vec2 v_pos_scaler = glm::vec2(
		get_width() / WINDOW_WIDTH,
		get_height() / WINDOW_HEIGHT
	);
	for (int i = 0; i < v_spec.num_vertices; i++) {
		v_pos_accessor[i] = &v_pos[i];
		v_texcoord_accessor[i] = &v_texcoord[i];
		v_data_for_gpu[i] = { (glm::vec2)v_pos[i] * v_pos_scaler, v_texcoord[i] };
	}
	for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
		v_data_for_gpu[v_spec.vertex_bindings[i].first] = v_data_for_gpu[v_spec.vertex_bindings[i].second];
	}

	glBufferData(GL_ARRAY_BUFFER, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data(), GL_DYNAMIC_DRAW);
}

void ScreenTexture::update_buffer_data() {
	glm::vec2 v_pos_scaler = glm::vec2(
		get_width() / WINDOW_WIDTH,
		get_height() / WINDOW_HEIGHT
	);
	for (int i = 0; i < v_spec.num_vertices; i++) {
		if (v_data_for_gpu[i].pos != (glm::vec2)v_pos[i] * v_pos_scaler
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
				v_data_for_gpu[i] = { (glm::vec2)v_pos[i] * v_pos_scaler, v_texcoord[i] };
			}
			for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
				v_data_for_gpu[v_spec.vertex_bindings[i].first] = v_data_for_gpu[v_spec.vertex_bindings[i].second];
			}
			glBufferSubData(GL_ARRAY_BUFFER, 0, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data());
			return;
		}
	}
}
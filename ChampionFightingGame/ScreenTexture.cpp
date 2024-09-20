#include "ScreenTexture.h"
#include "WindowConstants.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "utils.h"

ScreenTexture::ScreenTexture() {
#ifdef TEX_IMPL_MODE_VULKAN
	this->top_right_corner_scale = 100.0f;
	this->top_right_corner_crop = 100.0f;
	this->top_left_corner_scale = 100.0f;
	this->top_left_corner_crop = 100.0f;
	this->bottom_right_corner_scale = 100.0f;
	this->bottom_right_corner_crop = 100.0f;
	this->bottom_left_corner_scale = 100.0f;
	this->bottom_left_corner_crop = 100.0f;
#else
	this->VAO = 0;
	this->VBO = 0;
#endif
	this->shader = nullptr;
	this->features = 0;
	this->path = "";
	this->screen_orientation = TEXTURE_MID;
	this->texture_orientation = TEXTURE_MID;
	this->pos = glm::vec3(0.0);
	this->rot = glm::vec3(0.0);
	this->base_width = 0;
	this->width_scale = 1.0;
	this->base_height = 0;
	this->height_scale = 1.0;
	this->alpha = 255;
	this->colormod = glm::vec3(0.0);
	this->h_flipped = false;
	this->v_flipped = false;
	this->sprite = 0;
	this->loaded = false;
}

ScreenTexture::ScreenTexture(std::string path, unsigned char features) : ScreenTexture() {
	this->init(path, features);
}

ScreenTexture::ScreenTexture(std::vector<unsigned int> texture, unsigned char features, int width, int height) : ScreenTexture() {
	this->init(texture, features, width, height);
}

ScreenTexture::ScreenTexture(ScreenTexture& other) {
#ifdef TEX_IMPL_MODE_VULKAN
	this->top_right_corner_scale = other.top_right_corner_scale;
	this->top_right_corner_crop = other.top_right_corner_crop;
	this->top_left_corner_scale = other.top_left_corner_scale;
	this->top_left_corner_crop = other.top_left_corner_crop;
	this->bottom_right_corner_scale = other.bottom_right_corner_scale;
	this->bottom_right_corner_crop = other.bottom_right_corner_crop;
	this->bottom_left_corner_scale = other.bottom_left_corner_scale;
	this->bottom_left_corner_crop = other.bottom_left_corner_crop;
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
		this->v_data_for_gpu[i] = { this->v_pos[i], this->v_texcoord[i]};
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
	this->pos = other.pos;
	this->rot = other.rot;
	this->base_width = other.base_width;
	this->width_scale = other.width_scale;
	this->base_height = other.base_height;
	this->height_scale = other.height_scale;
	this->alpha = other.alpha;
	this->colormod = other.colormod;
	this->h_flipped = other.h_flipped;
	this->v_flipped = other.v_flipped;
	this->sprite = other.sprite;
	this->loaded = false;
}

ScreenTexture::ScreenTexture(ScreenTexture&& other) noexcept {
#ifdef TEX_IMPL_MODE_VULKAN
	this->top_right_corner_scale = other.top_right_corner_scale;
	this->top_right_corner_crop = other.top_right_corner_crop;
	this->top_left_corner_scale = other.top_left_corner_scale;
	this->top_left_corner_crop = other.top_left_corner_crop;
	this->bottom_right_corner_scale = other.bottom_right_corner_scale;
	this->bottom_right_corner_crop = other.bottom_right_corner_crop;
	this->bottom_left_corner_scale = other.bottom_left_corner_scale;
	this->bottom_left_corner_crop = other.bottom_left_corner_crop;
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
		this->v_data_for_gpu[i] = { this->v_pos[i], this->v_texcoord[i] };
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
	this->pos = other.pos;
	this->rot = other.rot;
	this->base_width = other.base_width;
	this->width_scale = other.width_scale;
	this->base_height = other.base_height;
	this->height_scale = other.height_scale;
	this->alpha = other.alpha;
	this->colormod = other.colormod;
	this->h_flipped = other.h_flipped;
	this->v_flipped = other.v_flipped;
	this->sprite = other.sprite;
	this->loaded = other.loaded;
	other.loaded = false;
}

ScreenTexture& ScreenTexture::operator=(ScreenTexture& other) {
	if (this != &other) {
#ifdef TEX_IMPL_MODE_VULKAN
		this->top_right_corner_scale = other.top_right_corner_scale;
		this->top_right_corner_crop = other.top_right_corner_crop;
		this->top_left_corner_scale = other.top_left_corner_scale;
		this->top_left_corner_crop = other.top_left_corner_crop;
		this->bottom_right_corner_scale = other.bottom_right_corner_scale;
		this->bottom_right_corner_crop = other.bottom_right_corner_crop;
		this->bottom_left_corner_scale = other.bottom_left_corner_scale;
		this->bottom_left_corner_crop = other.bottom_left_corner_crop;
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
			this->v_data_for_gpu[i] = { this->v_pos[i], this->v_texcoord[i] };
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
		this->pos = other.pos;
		this->rot = other.rot;
		this->base_width = other.base_width;
		this->width_scale = other.width_scale;
		this->base_height = other.base_height;
		this->height_scale = other.height_scale;
		this->alpha = other.alpha;
		this->colormod = other.colormod;
		this->h_flipped = other.h_flipped;
		this->v_flipped = other.v_flipped;
		this->sprite = other.sprite;
		this->loaded = false;
	}
	return *this;
}

ScreenTexture& ScreenTexture::operator=(ScreenTexture&& other) noexcept {
	if (this != &other) {
#ifdef TEX_IMPL_MODE_VULKAN
		this->top_right_corner_scale = other.top_right_corner_scale;
		this->top_right_corner_crop = other.top_right_corner_crop;
		this->top_left_corner_scale = other.top_left_corner_scale;
		this->top_left_corner_crop = other.top_left_corner_crop;
		this->bottom_right_corner_scale = other.bottom_right_corner_scale;
		this->bottom_right_corner_crop = other.bottom_right_corner_crop;
		this->bottom_left_corner_scale = other.bottom_left_corner_scale;
		this->bottom_left_corner_crop = other.bottom_left_corner_crop;
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
			this->v_data_for_gpu[i] = { this->v_pos[i], this->v_texcoord[i] };
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
		this->pos = other.pos;
		this->rot = other.rot;
		this->base_width = other.base_width;
		this->width_scale = other.width_scale;
		this->base_height = other.base_height;
		this->height_scale = other.height_scale;
		this->alpha = other.alpha;
		this->colormod = other.colormod;
		this->h_flipped = other.h_flipped;
		this->v_flipped = other.v_flipped;
		this->sprite = other.sprite;
		this->loaded = other.loaded;
		other.loaded = false;
	}
	return *this;
}

ScreenTexture::~ScreenTexture() {
	if (loaded) {
		destroy();
	}
}

void ScreenTexture::init(std::string path, unsigned char features) {
	//Setting up shaders is the same between OpenGL and Vulkan
	this->features = features;
	v_spec = (features & TEX_FEAT_4T5V) ? v_spec_4t5v : v_spec_2t4v;
	set_default_vertex_data();
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

	for (int i = 0; i < v_spec.num_vertices; i++) {
		v_data_for_gpu[i].pos.x *= get_width() / (float)WINDOW_WIDTH;
		v_data_for_gpu[i].pos.y *= get_height() / (float)WINDOW_HEIGHT;
	}
	for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
		v_data_for_gpu[v_spec.vertex_bindings[i].first] = v_data_for_gpu[v_spec.vertex_bindings[i].second];
	}

	glBufferData(GL_ARRAY_BUFFER, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	loaded = true;
	this->path = path;
}

void ScreenTexture::init(std::vector<unsigned int> texture, unsigned char features, int width, int height) {
	this->features = features;
	v_spec = (features & TEX_FEAT_4T5V) ? v_spec_4t5v : v_spec_2t4v;
	set_default_vertex_data();
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

	for (int i = 0; i < v_spec.num_vertices; i++) {
		v_data_for_gpu[i].pos.x *= get_width() / (float)WINDOW_WIDTH;
		v_data_for_gpu[i].pos.y *= get_height() / (float)WINDOW_HEIGHT;
	}
	for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
		v_data_for_gpu[v_spec.vertex_bindings[i].first] = v_data_for_gpu[v_spec.vertex_bindings[i].second];
	}

	glBufferData(GL_ARRAY_BUFFER, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	loaded = true;
	this->texture = texture;
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

ScreenTexture& ScreenTexture::set_shader(std::string frag_shader) {
	this->shader = ShaderManager::get_instance()->get_shader("screen_tex", "screen_tex_" + frag_shader, "", 0);
	return *this;
}

std::string ScreenTexture::get_path() const {
	return path;
}

ScreenTexture& ScreenTexture::set_screen_orientation(int orientation) {
	this->screen_orientation = orientation;
	return *this;
}

ScreenTexture& ScreenTexture::set_texture_orientation(int orientation) {
	this->texture_orientation = orientation;
	return *this;
}

ScreenTexture& ScreenTexture::set_orientation(int screen_orientation, int texture_orientation) {
	this->screen_orientation = screen_orientation;
	this->texture_orientation = texture_orientation;
	return *this;
}

int ScreenTexture::get_screen_orientation() const {
	return screen_orientation;
}

int ScreenTexture::get_texture_orientation() const {
	return texture_orientation;
}

ScreenTexture& ScreenTexture::set_pos(glm::vec3 pos) {
	this->pos = pos;
	return *this;
}

ScreenTexture& ScreenTexture::set_pos(glm::vec3 pos, int frames) {
	this->pos.set_target_val(pos, frames);
	return *this;
}

ScreenTexture& ScreenTexture::add_pos(glm::vec3 pos) {
	this->pos += pos;
	return *this;
}

glm::vec3 ScreenTexture::get_pos() const {
	return pos.get_val();
}

ScreenTexture& ScreenTexture::set_rot(glm::vec3 rot) {
	this->rot = rot;
	return *this;
}

ScreenTexture& ScreenTexture::set_rot(glm::vec3 rot, int frames) {
	this->rot.set_target_val(rot, frames);
	return *this;
}

ScreenTexture& ScreenTexture::add_rot(glm::vec3 rot) {
	this->rot += rot;
	return *this;
}

glm::vec3 ScreenTexture::get_rot() const {
	return rot.get_val();
}

ScreenTexture& ScreenTexture::set_base_width(int new_width) {
	float prev_width_scaler = get_width() / WINDOW_WIDTH;
	base_width = new_width;
	float curr_width_scaler = get_width() / WINDOW_WIDTH;
	for (int i = 0; i < v_spec.num_vertices; i++) {
		float val = v_pos[i].x.get_target_val() / prev_width_scaler * curr_width_scaler;
		int frames = v_pos[i].x.get_frames();
		if (frames) {
			v_pos[i].x.set_target_val(val, frames);
		}
		else {
			v_pos[i].x = val;
		}
	}
	return *this;
}

ScreenTexture& ScreenTexture::set_base_width(int new_width, int frames) {
	float prev_width_scaler = get_width() / WINDOW_WIDTH;
	float curr_width_scaler = (width_scale * new_width) / WINDOW_WIDTH;
	base_width.set_target_val(new_width, frames);
	for (int i = 0; i < v_spec.num_vertices; i++) {
		v_pos[i].x.set_target_val(v_pos[i].x / prev_width_scaler * curr_width_scaler, frames);
	}
	return *this;
}

ScreenTexture& ScreenTexture::add_base_width(int width) {
	float prev_width_scaler = get_width() / (float)WINDOW_WIDTH;
	base_width += width;
	float curr_width_scaler = get_width() / (float)WINDOW_WIDTH;
	for (int i = 0; i < v_spec.num_vertices; i++) {
		float val = v_pos[i].x.get_target_val() / prev_width_scaler * curr_width_scaler;
		int frames = v_pos[i].x.get_frames();
		if (frames) {
			v_pos[i].x.set_target_val(val, frames);
		}
		else {
			v_pos[i].x = val;
		}
	}
	return *this;
}

int ScreenTexture::get_base_width() const {
	return base_width.get_val();
}

ScreenTexture& ScreenTexture::set_base_height(int new_height) {
	float prev_height_scaler = get_height() / WINDOW_HEIGHT;
	base_height = new_height;
	float curr_height_scaler = get_height() / WINDOW_HEIGHT;
	for (int i = 0; i < v_spec.num_vertices; i++) {
		float val = v_pos[i].y.get_target_val() / prev_height_scaler * curr_height_scaler;
		int frames = v_pos[i].y.get_frames();
		if (frames) {
			v_pos[i].y.set_target_val(val, frames);
		}
		else {
			v_pos[i].y = val;
		}
	}
	return *this;
}

ScreenTexture& ScreenTexture::set_base_height(int new_height, int frames) {
	float prev_height_scaler = get_height() / WINDOW_HEIGHT;
	float curr_height_scaler = (height_scale * new_height) / WINDOW_HEIGHT;
	base_height.set_target_val(new_height, frames);
	for (int i = 0; i < v_spec.num_vertices; i++) {
		v_pos[i].y.set_target_val(v_pos[i].x / prev_height_scaler * curr_height_scaler, frames);
	}
	return *this;
}

ScreenTexture& ScreenTexture::add_base_height(int height) {
	float prev_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	base_height += height;
	float curr_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	for (int i = 0; i < v_spec.num_vertices; i++) {
		float val = v_pos[i].y.get_target_val() / prev_height_scaler * curr_height_scaler;
		int frames = v_pos[i].y.get_frames();
		if (frames) {
			v_pos[i].y.set_target_val(val, frames);
		}
		else {
			v_pos[i].y = val;
		}
	}
	return *this;
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

ScreenTexture& ScreenTexture::set_width_scale(float scale) {
	float prev_width_scaler = get_width() / WINDOW_WIDTH;
	width_scale = scale;
	float curr_width_scaler = get_width() / WINDOW_WIDTH;
	for (int i = 0; i < v_spec.num_vertices; i++) {
		float val = v_pos[i].x.get_target_val() / prev_width_scaler * curr_width_scaler;
		int frames = v_pos[i].x.get_frames();
		if (frames) {
			v_pos[i].x.set_target_val(val, frames);
		}
		else {
			v_pos[i].x = val;
		}
	}
	return *this;
}

ScreenTexture& ScreenTexture::set_width_scale(float scale, int frames) {
	float prev_width_scaler = get_width() / WINDOW_WIDTH;
	float curr_width_scaler = ((float)base_width * scale) / WINDOW_WIDTH;
	width_scale.set_target_val(scale, frames);
	for (int i = 0; i < v_spec.num_vertices; i++) {
		v_pos[i].x.set_target_val(v_pos[i].x / prev_width_scaler * curr_width_scaler, frames);
	}
	return *this;
}

ScreenTexture& ScreenTexture::add_width_scale(float scale) {
	float prev_width_scaler = get_width() / (float)WINDOW_WIDTH;
	width_scale += scale;
	float curr_width_scaler = get_width() / (float)WINDOW_WIDTH;
	for (int i = 0; i < v_spec.num_vertices; i++) {
		float val = v_pos[i].x.get_target_val() / prev_width_scaler * curr_width_scaler;
		int frames = v_pos[i].x.get_frames();
		if (frames) {
			v_pos[i].x.set_target_val(val, frames);
		}
		else {
			v_pos[i].x = val;
		}
	}
	return *this;
}

float ScreenTexture::get_width_scale() const {
	return width_scale.get_val();
}

ScreenTexture& ScreenTexture::set_height_scale(float scale) {
	float prev_height_scaler = get_height() / WINDOW_HEIGHT;
	height_scale = scale;
	float curr_height_scaler = get_height() / WINDOW_HEIGHT;
	for (int i = 0; i < v_spec.num_vertices; i++) {
		float val = v_pos[i].y.get_target_val() / prev_height_scaler * curr_height_scaler;
		int frames = v_pos[i].y.get_frames();
		if (frames) {
			v_pos[i].y.set_target_val(val, frames);
		}
		else {
			v_pos[i].y = val;
		}
	}
	return *this;
}

ScreenTexture& ScreenTexture::set_height_scale(float scale, int frames) {
	float prev_height_scaler = get_height() / WINDOW_HEIGHT;
	float curr_height_scaler = ((float)base_height * scale) / WINDOW_HEIGHT;
	height_scale.set_target_val(scale, frames);
	for (int i = 0; i < v_spec.num_vertices; i++) {
		v_pos[i].y.set_target_val(v_pos[i].x / prev_height_scaler * curr_height_scaler, frames);
	}
	return *this;
}

ScreenTexture& ScreenTexture::add_height_scale(float scale) {
	float prev_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	height_scale += scale;
	float curr_height_scaler = get_height() / (float)WINDOW_HEIGHT;
	for (int i = 0; i < v_spec.num_vertices; i++) {
		float val = v_pos[i].y.get_target_val() / prev_height_scaler * curr_height_scaler;
		int frames = v_pos[i].y.get_frames();
		if (frames) {
			v_pos[i].y.set_target_val(val, frames);
		}
		else {
			v_pos[i].y = val;
		}
	}
	return *this;
}

float ScreenTexture::get_height_scale() const {
	return height_scale.get_val();
}

ScreenTexture& ScreenTexture::set_scale(float scale) {
	set_width_scale(scale);
	set_height_scale(scale);
	return *this;
}

ScreenTexture& ScreenTexture::set_scale(float scale, int frames) {
	set_width_scale(scale, frames);
	set_height_scale(scale, frames);
	return *this;
}

ScreenTexture& ScreenTexture::scale_right_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_scale = percent;
	bottom_right_corner_scale = percent;
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float width_scale = (float)base_width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
	}
	else {
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_right_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_scale.set_target_val(percent, frames);
	bottom_right_corner_scale.set_target_val(percent, frames);
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float width_scale = (float)base_width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_right_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_crop = percent;
	bottom_right_corner_crop = percent;
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float width_scale = (float)base_width / (float)WINDOW_WIDTH;
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
	return *this;
}

ScreenTexture& ScreenTexture::crop_right_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_crop.set_target_val(percent, frames);
	bottom_right_corner_crop.set_target_val(percent, frames);
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float width_scale = (float)base_width / (float)WINDOW_WIDTH;
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
	return *this;
}

ScreenTexture& ScreenTexture::scale_left_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_left_corner_scale = percent;
	bottom_left_corner_scale = percent;
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float width_scale = (float)base_width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
		v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0);
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_left_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_left_corner_scale.set_target_val(percent, frames);
	bottom_left_corner_scale.set_target_val(percent, frames);
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float width_scale = (float)base_width / (float)WINDOW_WIDTH;
	if (h_flipped) {
		v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * width_scale, 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * width_scale, 1.0), frames);
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_left_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_left_corner_crop = percent;
	bottom_left_corner_crop = percent;
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float width_scale = (float)base_width / (float)WINDOW_WIDTH;
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
	return *this;
}

ScreenTexture& ScreenTexture::crop_left_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_left_corner_crop.set_target_val(percent, frames);
	bottom_left_corner_crop.set_target_val(percent, frames);
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float width_scale = (float)base_width / (float)WINDOW_WIDTH;
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
	return *this;
}

ScreenTexture& ScreenTexture::scale_top_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_scale = percent;
	top_left_corner_scale = percent;
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float height_scale = (float)base_height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
	}
	else {
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_top_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_scale.set_target_val(percent, frames);
	top_left_corner_scale.set_target_val(percent, frames);
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float height_scale = (float)base_height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_top_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_crop = percent;
	top_left_corner_crop = percent;
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float height_scale = (float)base_height / (float)WINDOW_HEIGHT;
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
	return *this;
}

ScreenTexture& ScreenTexture::crop_top_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_crop.set_target_val(percent, frames);
	top_left_corner_crop.set_target_val(percent, frames);
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float height_scale = (float)base_height / (float)WINDOW_HEIGHT;
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
	return *this;
}

ScreenTexture& ScreenTexture::scale_bottom_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_right_corner_scale = percent;
	bottom_left_corner_scale = percent;
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float height_scale = (float)base_height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
		v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0);
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_bottom_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_right_corner_scale.set_target_val(percent, frames);
	bottom_left_corner_scale.set_target_val(percent, frames);
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float height_scale = (float)base_height / (float)WINDOW_HEIGHT;
	if (v_flipped) {
		v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * 2.0) * height_scale, 1.0), frames);
	}
	else {
		v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
		v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent - 0.5) * -2.0) * height_scale, 1.0), frames);
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_bottom_edge(float percent) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_right_corner_crop = percent;
	bottom_left_corner_crop = percent;
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float height_scale = (float)base_height / (float)WINDOW_HEIGHT;
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
	return *this;
}

ScreenTexture& ScreenTexture::crop_bottom_edge(float percent, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_right_corner_crop.set_target_val(percent, frames);
	bottom_left_corner_crop.set_target_val(percent, frames);
#else
	if (percent < 0.0 || percent > 1.0) {
		return *this;
	}
	float height_scale = (float)base_height / (float)WINDOW_HEIGHT;
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
	return *this;
}

/*
  _______ ____  _____   ____  
 |__   __/ __ \|  __ \ / __ \ 
    | | | |  | | |  | | |  | |
    | | | |  | | |  | | |  | |
    | | | |__| | |__| | |__| |
    |_|  \____/|_____/ \____/ 
                              
Most 3D Rendering Implementations don't support quads, including modern OpenGL and
(probably) Fez's Vulkan impl as well. As a result, a "rectangle" is actually two right
triangles stacked on top of each other, likeso:

				  C1								 C2, C5
					************************************
					*:::::::::::::::::::::::::::::****:*
					*:::::::::::::::::::::::::****:::::*
					*:::::::::::::::::::::****:::::::::*
					*:::::::::::::::::****:::::::::::::*
					*:::::::::::::****:::::::::::::::::*
					*:::::::::****:::::::::::::::::::::*
					*:::::****:::::::::::::::::::::::::*
					*:****:::::::::::::::::::::::::::::*
					************************************
				  C3, C6								C4

The first triangle is (C1, C2, C3), and the second is (C4, C5, C6). In practice, we can effectively 
treat this as a quad and just copy the values of C2 and C3 to C5 and C6 respectively whenever they
change. However, since the GPU still sees two separate triangles, moving the corner (such as in a crop)
of one triangle doesn't necessarily mean the other one will be affected. 

For edge cropping, this problem because we always move a corner which is part of both triangles, but
for corner crops that involve C1 or C4 specifically, there won't be any visual difference between
cropping 50% and cropping 100%. Getting around this issue likely involves compensating by moving C2 and
C3 in someway, but we need to figure out what that math looks like.

Alternative Idea: 4 triangles, 12 vertices. 2 for each corner, 4 for the middle:
- If two adjacent corners cross the middle on the axis that they share (E.G. Top Left and Bottom Left
  both have an x pos > 0.0 or an x texcoord > 0.5), move the middle's position for that axis to the 
  lower of the corners' positions
- If one corner crosses the middle on both axes, move the middle to wherever that corner is

*/
ScreenTexture& ScreenTexture::scale_top_right_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_scale = percent_x;
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	float width_scale = (float)base_width / (float)WINDOW_WIDTH;
	float height_scale = (float)base_height / (float)WINDOW_HEIGHT;
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0) * width_scale, 1.0);
			v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0) * height_scale, 1.0);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0) * width_scale, 1.0);
			v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0) * height_scale, 1.0);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0) * width_scale, 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0) * height_scale, 1.0);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0) * width_scale, 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0) * height_scale, 1.0);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_top_right_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_scale.set_target_val(percent_x, frames);
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	float width_scale = (float)base_width / (float)WINDOW_WIDTH;
	float height_scale = (float)base_height / (float)WINDOW_HEIGHT;
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0) * width_scale, 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0) * height_scale, 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0) * width_scale, 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0) * height_scale, 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0) * width_scale, 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0) * height_scale, 1.0), frames);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0) * width_scale, 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0) * height_scale, 1.0), frames);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_top_right_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_crop = percent_x;
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, percent_x, 1.0);
			v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, percent_y, 1.0);
			v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, percent_x, 1.0);
			v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, percent_y, 1.0);
			v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, percent_x, 1.0);
			v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, percent_y, 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_B: {
			v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, percent_x, 1.0);
			v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, percent_y, 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_top_right_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_right_corner_crop.set_target_val(percent_x, frames);
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_top_left_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_left_corner_scale = percent_x;
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_top_left_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_left_corner_scale.set_target_val(percent_x, frames);
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_top_left_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_left_corner_crop = percent_x;
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
			v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, percent_y, 1.0);
			v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
			v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, percent_y, 1.0);
			v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
			v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, percent_y, 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_B: {
			v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
			v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, percent_y, 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_top_left_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	top_left_corner_crop.set_target_val(percent_x, frames);
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, percent_y, 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_bottom_right_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_right_corner_scale = percent_x;
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
	}

#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_bottom_right_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_right_corner_scale.set_target_val(percent_x, frames);
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_bottom_right_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_right_corner_crop = percent_x;
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, percent_x, 1.0);
			v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, percent_x, 1.0);
			v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, percent_x, 1.0);
			v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
			v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_B: {
			v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, percent_x, 1.0);
			v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
			v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_bottom_right_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_right_corner_crop.set_target_val(percent_x, frames);
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_B: {
			v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_bottom_left_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_left_corner_scale = percent_x;
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::scale_bottom_left_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_left_corner_scale.set_target_val(percent_x, frames);
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_bottom_left_corner(float percent_x, float percent_y) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_left_corner_crop = percent_x;
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_texcoord_accessor[v_spec.bottom_left_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
			v_texcoord_accessor[v_spec.bottom_left_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.bottom_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_texcoord_accessor[v_spec.bottom_right_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
			v_texcoord_accessor[v_spec.bottom_right_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.bottom_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_texcoord_accessor[v_spec.top_left_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
			v_texcoord_accessor[v_spec.top_left_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
			v_pos_accessor[v_spec.top_left_idx]->x = clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0);
			v_pos_accessor[v_spec.top_left_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
		case SCREEN_TEX_FLIP_B: {
			v_texcoord_accessor[v_spec.top_right_idx]->x = clampf(0.0, 1.0 - percent_x, 1.0);
			v_texcoord_accessor[v_spec.top_right_idx]->y = clampf(0.0, 1.0 - percent_y, 1.0);
			v_pos_accessor[v_spec.top_right_idx]->x = clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0);
			v_pos_accessor[v_spec.top_right_idx]->y = clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::crop_bottom_left_corner(float percent_x, float percent_y, int frames) {
#ifdef TEX_IMPL_MODE_VULKAN
	bottom_left_corner_crop.set_target_val(percent_x, frames);
#else
	if (percent_x < 0.0 || percent_x > 1.0 || percent_y < 0.0 || percent_y > 1.0) {
		return *this;
	}
	switch (get_flipped()) {
		case SCREEN_TEX_FLIP_N: {
			v_texcoord_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_H: {
			v_texcoord_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.bottom_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * -2.0), 1.0), frames);
		} break;
		case SCREEN_TEX_FLIP_V: {
			v_texcoord_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * -2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_left_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		}break;
		case SCREEN_TEX_FLIP_B: {
			v_texcoord_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(0.0, 1.0 - percent_x, 1.0), frames);
			v_texcoord_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(0.0, 1.0 - percent_y, 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->x.set_target_val(clampf(-1.0, ((percent_x - 0.5) * 2.0), 1.0), frames);
			v_pos_accessor[v_spec.top_right_idx]->y.set_target_val(clampf(-1.0, ((percent_y - 0.5) * 2.0), 1.0), frames);
		} break;
	}
#endif
	return *this;
}

ScreenTexture& ScreenTexture::set_alpha(unsigned char alpha) {
	this->alpha = alpha;
	return *this;
}

ScreenTexture& ScreenTexture::set_alpha(unsigned char alpha, int frames) {
	this->alpha.set_target_val(alpha, frames);
	return *this;
}

ScreenTexture& ScreenTexture::add_alpha(unsigned char alpha) {
	this->alpha += alpha;
	return *this;
}

unsigned char ScreenTexture::get_alpha() const {
	return alpha.get_val();
}

ScreenTexture& ScreenTexture::set_colormod(glm::vec3 color) {
	colormod = color;
	return *this;
}

ScreenTexture& ScreenTexture::set_colormod(glm::vec3 color, int frames) {
	colormod.set_target_val(color, frames);
	return *this;
}

ScreenTexture& ScreenTexture::add_colormod(glm::vec3 color) {
	colormod += color;
	return *this;
}

glm::vec3 ScreenTexture::get_colormod() const {
	return colormod.get_val();
}

ScreenTexture& ScreenTexture::flip_h() {
	std::swap(v_pos[v_spec.bottom_left_idx].x.get_val_ref(), v_pos[v_spec.bottom_right_idx].x.get_val_ref());
	std::swap(v_pos[v_spec.top_left_idx].x.get_val_ref(), v_pos[v_spec.top_right_idx].x.get_val_ref());
	std::swap(v_pos_accessor[v_spec.bottom_left_idx], v_pos_accessor[v_spec.bottom_right_idx]);
	std::swap(v_texcoord_accessor[v_spec.bottom_left_idx], v_texcoord_accessor[v_spec.bottom_right_idx]);
	std::swap(v_pos_accessor[v_spec.top_left_idx], v_pos_accessor[v_spec.top_right_idx]);
	std::swap(v_texcoord_accessor[v_spec.top_left_idx], v_texcoord_accessor[v_spec.top_right_idx]);
	h_flipped = !h_flipped;
	return *this;
}

ScreenTexture& ScreenTexture::set_h_flipped(bool h_flipped) {
	this->h_flipped = h_flipped;
	return *this;
}

bool ScreenTexture::is_h_flipped() const {
	return h_flipped;
}

ScreenTexture& ScreenTexture::flip_v() {
	v_flipped = !v_flipped;
	return *this;
}

ScreenTexture& ScreenTexture::set_v_flipped(bool v_flipped) {
	this->v_flipped = v_flipped;
	return *this;
}

bool ScreenTexture::is_v_flipped() const {
	return v_flipped;
}

int ScreenTexture::get_flipped() const {
	return (int)h_flipped | (((int)v_flipped) * 2);
}

ScreenTexture& ScreenTexture::set_sprite(unsigned int sprite) {
	if (sprite < texture.size()) {
		this->sprite = sprite;
	}
	return *this;
}
	

ScreenTexture& ScreenTexture::next_sprite() {
	if (sprite == texture.size() - 1) {
		sprite -= texture.size();
	}
	sprite++;
	return *this;
}

ScreenTexture& ScreenTexture::prev_sprite() {
	if (sprite == 0) {
		sprite = texture.size();
	}
	sprite--;
	return *this;
}

unsigned int ScreenTexture::get_sprite() const {
	return sprite;
}

void ScreenTexture::render() {
	glm::vec3 render_pos = pos.get_val();

	if (!(screen_orientation & TEXTURE_H_MID)) {
		if (screen_orientation & TEXTURE_LEFT) {
			render_pos.x -= WINDOW_WIDTH;
		}
		else {
			render_pos *= -1.0;
			render_pos.x += WINDOW_WIDTH;
		}
	}
	if (!(screen_orientation & TEXTURE_V_MID)) {
		if (screen_orientation & TEXTURE_TOP) {
			render_pos *= -1.0;
			render_pos.y += WINDOW_HEIGHT;
		}
		else {
			render_pos.y -= WINDOW_HEIGHT;
		}
	}
	if (!(texture_orientation & TEXTURE_H_MID)) {
		if (texture_orientation & TEXTURE_LEFT) {
			render_pos.x += get_width() / 2;
		}
		else {
			render_pos.x -= get_width() / 2;
		}
	}
	if (!(texture_orientation & TEXTURE_V_MID)) {
		if (texture_orientation & TEXTURE_TOP) {
			render_pos.y -= get_height() / 2;
		}
		else {
			render_pos.y += get_height() / 2;
		}
	}
	render_pos.x /= (float)WINDOW_WIDTH;
	render_pos.y /= (float)WINDOW_HEIGHT;
#ifdef TEX_IMPL_MODE_VULKAN

#else
	glm::mat4 matrix = glm::translate(glm::mat4(1.0), render_pos);
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().x), glm::vec3(1.0, 0.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().y), glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(rot.get_val().z), glm::vec3(0.0, 0.0, 1.0));
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	update_buffer_data();
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
		v_pos[ST_4T5V_MIDDLE] = glm::vec2(0.0, 0.0);
		v_texcoord[ST_4T5V_MIDDLE] = glm::vec2(0.5, 0.5);
	}
	for (int i = 0; i < v_spec.num_vertices; i++) {
		v_pos_accessor[i] = &v_pos[i];
		v_texcoord_accessor[i] = &v_texcoord[i];
		v_data_for_gpu[i] = { v_pos[i], v_texcoord[i] };
	}
	for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
		v_data_for_gpu[v_spec.vertex_bindings[i].first] = v_data_for_gpu[v_spec.vertex_bindings[i].second];
	}
}

void ScreenTexture::update_buffer_data() {
	bool update = false;
	glm::vec2 v_pos_scaler = glm::vec2(
		get_width() / (float)WINDOW_WIDTH,
		get_height() / (float)WINDOW_HEIGHT
	);
	for (int i = 0; i < v_spec.num_vertices; i++) {
		if (v_data_for_gpu[i].pos != (glm::vec2)v_pos[i] * v_pos_scaler
		|| v_data_for_gpu[i].tex_coord != v_texcoord[i]) {
			v_data_for_gpu[i] = { (glm::vec2)v_pos[i] * v_pos_scaler, v_texcoord[i] };
			update = true;
		}
	}

	if (update) {
		if (v_spec.num_vertices == 5) {
			if ((v_data_for_gpu[ST_4T5V_TOP_LEFT].pos.x > 0.0
			|| v_data_for_gpu[ST_4T5V_BOTTOM_LEFT].pos.x > 0.0) != h_flipped) {
				v_data_for_gpu[ST_4T5V_MIDDLE].pos.x = (h_flipped ? 
					std::min(v_data_for_gpu[ST_4T5V_TOP_LEFT].pos.x, v_data_for_gpu[ST_4T5V_BOTTOM_LEFT].pos.x) 
					: std::max(v_data_for_gpu[ST_4T5V_TOP_LEFT].pos.x, v_data_for_gpu[ST_4T5V_BOTTOM_LEFT].pos.x));
			}
			else if ((v_data_for_gpu[ST_4T5V_TOP_RIGHT].pos.x < 0.0
			|| v_data_for_gpu[ST_4T5V_BOTTOM_RIGHT].pos.x < 0.0) != h_flipped) {
				v_data_for_gpu[ST_4T5V_MIDDLE].pos.x = (h_flipped ?
					std::max(v_data_for_gpu[ST_4T5V_TOP_RIGHT].pos.x, v_data_for_gpu[ST_4T5V_BOTTOM_RIGHT].pos.x)
					: std::min(v_data_for_gpu[ST_4T5V_TOP_RIGHT].pos.x, v_data_for_gpu[ST_4T5V_BOTTOM_RIGHT].pos.x));
			}
			if ((v_data_for_gpu[ST_4T5V_BOTTOM_LEFT].pos.y > 0.0
				|| v_data_for_gpu[ST_4T5V_BOTTOM_RIGHT].pos.y > 0.0) != v_flipped) {
				v_data_for_gpu[ST_4T5V_MIDDLE].pos.y = (v_flipped ? 
					std::min(v_data_for_gpu[ST_4T5V_BOTTOM_LEFT].pos.y, v_data_for_gpu[ST_4T5V_BOTTOM_RIGHT].pos.y)
					: std::max(v_data_for_gpu[ST_4T5V_BOTTOM_LEFT].pos.y, v_data_for_gpu[ST_4T5V_BOTTOM_RIGHT].pos.y));
			}
			else if ((v_data_for_gpu[ST_4T5V_TOP_LEFT].pos.y < 0.0
				|| v_data_for_gpu[ST_4T5V_TOP_RIGHT].pos.y < 0.0) != v_flipped) {
				v_data_for_gpu[ST_4T5V_MIDDLE].pos.y = (v_flipped ?
					std::max(v_data_for_gpu[ST_4T5V_TOP_LEFT].pos.y, v_data_for_gpu[ST_4T5V_TOP_RIGHT].pos.y)
					: std::min(v_data_for_gpu[ST_4T5V_TOP_LEFT].pos.y, v_data_for_gpu[ST_4T5V_TOP_RIGHT].pos.y));
			}

			if (v_data_for_gpu[ST_4T5V_TOP_LEFT].tex_coord.x > 0.5
				|| v_data_for_gpu[ST_4T5V_BOTTOM_LEFT].tex_coord.x > 0.5) {
				v_data_for_gpu[ST_4T5V_MIDDLE].tex_coord.x = std::max(v_data_for_gpu[ST_4T5V_TOP_LEFT].tex_coord.x, v_data_for_gpu[ST_4T5V_BOTTOM_LEFT].tex_coord.x);
			}
			else if (v_data_for_gpu[ST_4T5V_TOP_RIGHT].tex_coord.x < 0.5
				|| v_data_for_gpu[ST_4T5V_BOTTOM_RIGHT].tex_coord.x < 0.5) {
				v_data_for_gpu[ST_4T5V_MIDDLE].tex_coord.x = std::min(v_data_for_gpu[ST_4T5V_TOP_RIGHT].tex_coord.x, v_data_for_gpu[ST_4T5V_BOTTOM_RIGHT].tex_coord.x);
			}
			if (v_data_for_gpu[ST_4T5V_BOTTOM_LEFT].tex_coord.y > 0.5
				|| v_data_for_gpu[ST_4T5V_BOTTOM_RIGHT].tex_coord.y > 0.5) {
				v_data_for_gpu[ST_4T5V_MIDDLE].tex_coord.y = std::max(v_data_for_gpu[ST_4T5V_BOTTOM_LEFT].tex_coord.y, v_data_for_gpu[ST_4T5V_BOTTOM_RIGHT].tex_coord.y);
			}
			else if (v_data_for_gpu[ST_4T5V_TOP_LEFT].tex_coord.y < 0.5
				|| v_data_for_gpu[ST_4T5V_TOP_RIGHT].tex_coord.y < 0.5) {
				v_data_for_gpu[ST_4T5V_MIDDLE].tex_coord.y = std::min(v_data_for_gpu[ST_4T5V_TOP_LEFT].tex_coord.y, v_data_for_gpu[ST_4T5V_TOP_RIGHT].tex_coord.y);
			}
		}
		for (size_t i = 0, max = v_spec.vertex_bindings.size(); i < max; i++) {
			v_data_for_gpu[v_spec.vertex_bindings[i].first] = v_data_for_gpu[v_spec.vertex_bindings[i].second];
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, v_data_for_gpu.size() * sizeof(TextureCoord), v_data_for_gpu.data());
	}
}
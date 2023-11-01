#include "WorldTexture.h"
#include "utils.h"

WorldTexture::WorldTexture() {
	path = "";
	pos = glm::vec3(0.0);
	rot = glm::vec3(0.0);
	right_edge_scale = 100.0;
	right_edge_crop = 100.0;
	left_edge_scale = 100.0;
	left_edge_crop = 100.0;
	top_edge_scale = 100.0;
	top_edge_crop = 100.0;
	bottom_edge_scale = 100.0;
	bottom_edge_crop = 100.0;
	width = 0;
	width_scale = 1.0;
	height = 0;
	height_scale = 1.0;
	alpha = 255;
	colormod = glm::vec3(0.0);
	h_flipped = false;
	v_flipped = false;
	sprite = 0;
	loaded = false;
}

WorldTexture::WorldTexture(std::string path) {
	init(path);
}

WorldTexture::WorldTexture(std::vector<std::vector<char>> texture, int width, int height) {
	init(texture, width, height);
}

WorldTexture::WorldTexture(WorldTexture& other) {
	path = other.path;
	texture = other.texture;
	pos = other.pos;
	rot = other.rot;
	right_edge_scale = other.right_edge_scale;
	right_edge_crop = other.right_edge_crop;
	left_edge_scale = other.left_edge_scale;
	left_edge_crop = other.left_edge_crop;
	top_edge_scale = other.top_edge_scale;
	top_edge_crop = other.top_edge_crop;
	bottom_edge_scale = other.bottom_edge_scale;
	bottom_edge_crop = other.bottom_edge_crop;
	width = other.width;
	width_scale = other.width_scale;
	height = other.height;
	height_scale = other.height_scale;
	alpha = other.alpha;
	colormod = other.colormod;
	h_flipped = other.h_flipped;
	v_flipped = other.v_flipped;
	sprite = other.sprite;
	loaded = false;
}

WorldTexture::WorldTexture(WorldTexture&& other) noexcept {
	path = other.path;
	texture = other.texture;
	pos = other.pos;
	rot = other.rot;
	right_edge_scale = other.right_edge_scale;
	right_edge_crop = other.right_edge_crop;
	left_edge_scale = other.left_edge_scale;
	left_edge_crop = other.left_edge_crop;
	top_edge_scale = other.top_edge_scale;
	top_edge_crop = other.top_edge_crop;
	bottom_edge_scale = other.bottom_edge_scale;
	bottom_edge_crop = other.bottom_edge_crop;
	width = other.width;
	width_scale = other.width_scale;
	height = other.height;
	height_scale = other.height_scale;
	alpha = other.alpha;
	colormod = other.colormod;
	h_flipped = other.h_flipped;
	v_flipped = other.v_flipped;
	sprite = other.sprite;
	loaded = other.loaded;
	other.loaded = false;
}

WorldTexture& WorldTexture::operator=(WorldTexture& other) {
	if (this != &other) {
		path = other.path;
		texture = other.texture;
		pos = other.pos;
		rot = other.rot;
		right_edge_scale = other.right_edge_scale;
		right_edge_crop = other.right_edge_crop;
		left_edge_scale = other.left_edge_scale;
		left_edge_crop = other.left_edge_crop;
		top_edge_scale = other.top_edge_scale;
		top_edge_crop = other.top_edge_crop;
		bottom_edge_scale = other.bottom_edge_scale;
		bottom_edge_crop = other.bottom_edge_crop;
		width = other.width;
		width_scale = other.width_scale;
		height = other.height;
		height_scale = other.height_scale;
		alpha = other.alpha;
		colormod = other.colormod;
		h_flipped = other.h_flipped;
		v_flipped = other.v_flipped;
		sprite = other.sprite;
		loaded = false;
	}
	return *this;
}

WorldTexture& WorldTexture::operator=(WorldTexture&& other) noexcept {
	if (this != &other) {
		path = other.path;
		texture = other.texture;
		pos = other.pos;
		rot = other.rot;
		right_edge_scale = other.right_edge_scale;
		right_edge_crop = other.right_edge_crop;
		left_edge_scale = other.left_edge_scale;
		left_edge_crop = other.left_edge_crop;
		top_edge_scale = other.top_edge_scale;
		top_edge_crop = other.top_edge_crop;
		bottom_edge_scale = other.bottom_edge_scale;
		bottom_edge_crop = other.bottom_edge_crop;
		width = other.width;
		width_scale = other.width_scale;
		height = other.height;
		height_scale = other.height_scale;
		alpha = other.alpha;
		colormod = other.colormod;
		h_flipped = other.h_flipped;
		v_flipped = other.v_flipped;
		sprite = other.sprite;
		loaded = other.loaded;
		other.loaded = false;
	}
	return *this;
}

WorldTexture::~WorldTexture() {
	if (loaded) {
		destroy();
	}
}

void WorldTexture::init(std::string path) {
	//Load Image, call some vulkan stuff probably
}

void WorldTexture::init(std::vector<std::vector<char>> texture, int width, int height) {

}

void WorldTexture::destroy() {
	//Call some Vulkan stuff and tell the ResourceManager to unuse one instance of this texture info
}

WorldTexture WorldTexture::init_copy() {
	WorldTexture ret;
	if (path != "") {
		ret.init(path);
	}
	else {
		ret.init(texture);
	}
	return ret;
}

std::string WorldTexture::get_path() const {
	return path;
}

void WorldTexture::set_pos(glm::vec3 pos) {
	this->pos = pos;
}

void WorldTexture::set_pos(glm::vec3 pos, int frames) {
	this->pos.set_target_val(pos, frames);
}

void WorldTexture::add_pos(glm::vec3 pos) {
	this->pos += pos;
}

glm::vec3 WorldTexture::get_pos() const {
	return pos.get_val();
}

void WorldTexture::set_rot(glm::vec3 rot) {
	this->rot = rot;
}

void WorldTexture::set_rot(glm::vec3 rot, int frames) {
	this->rot.set_target_val(rot, frames);
}

void WorldTexture::add_rot(glm::vec3 rot) {
	this->rot += rot;
}

glm::vec3 WorldTexture::get_rot() const {
	return rot.get_val();
}

void WorldTexture::scale_right_edge(float percent) {
	right_edge_scale = percent;
}

void WorldTexture::scale_right_edge(float percent, int frames) {
	right_edge_scale.set_target_val(percent, frames);
}

void WorldTexture::crop_right_edge(float percent) {
	right_edge_crop = percent;
}

void WorldTexture::crop_right_edge(float percent, int frames) {
	right_edge_crop.set_target_val(percent, frames);
}

void WorldTexture::scale_left_edge(float percent) {
	left_edge_scale = percent;
}

void WorldTexture::scale_left_edge(float percent, int frames) {
	left_edge_scale.set_target_val(percent, frames);
}

void WorldTexture::crop_left_edge(float percent) {
	left_edge_crop = percent;
}

void WorldTexture::crop_left_edge(float percent, int frames) {
	left_edge_crop.set_target_val(percent, frames);
}

void WorldTexture::scale_top_edge(float percent) {
	top_edge_scale = percent;
}

void WorldTexture::scale_top_edge(float percent, int frames) {
	top_edge_scale.set_target_val(percent, frames);
}

void WorldTexture::crop_top_edge(float percent) {
	top_edge_crop = percent;
}

void WorldTexture::crop_top_edge(float percent, int frames) {
	top_edge_crop.set_target_val(percent, frames);
}

void WorldTexture::scale_bottom_edge(float percent) {
	bottom_edge_scale = percent;
}

void WorldTexture::scale_bottom_edge(float percent, int frames) {
	bottom_edge_scale.set_target_val(percent, frames);
}

void WorldTexture::crop_bottom_edge(float percent) {
	bottom_edge_crop = percent;
}

void WorldTexture::crop_bottom_edge(float percent, int frames) {
	bottom_edge_crop.set_target_val(percent, frames);
}

void WorldTexture::set_width(int new_width) {
	width = new_width;
}

void WorldTexture::set_width(int new_width, int frames) {
	width.set_target_val(new_width, frames);
}

void WorldTexture::set_width_scale(float scale) {
	width_scale = scale;
}

void WorldTexture::set_width_scale(float scale, int frames) {
	width_scale.set_target_val(scale, frames);
}

void WorldTexture::set_height(int new_height) {
	height = new_height;
}

void WorldTexture::set_height(int new_height, int frames) {
	height.set_target_val(new_height, frames);
}

void WorldTexture::set_height_scale(float scale) {
	height_scale = scale;
}

void WorldTexture::set_height_scale(float scale, int frames) {
	height_scale.set_target_val(scale, frames);
}

int WorldTexture::get_base_width() const {
	return width.get_val();
}

int WorldTexture::get_base_height() const {
	return height.get_val();
}

float WorldTexture::get_width_scale() const {
	return width_scale.get_val();
}

float WorldTexture::get_height_scale() const {
	return height_scale.get_val();
}

float WorldTexture::get_width() {
	return width_scale * width;
}

float WorldTexture::get_height() {
	return height_scale * height;
}

void WorldTexture::set_alpha(unsigned char alpha) {
	this->alpha = alpha;
}

void WorldTexture::set_alpha(unsigned char alpha, int frames) {
	this->alpha.set_target_val(alpha, frames);
}

void WorldTexture::add_alpha(unsigned char alpha) {
	this->alpha += alpha;
}

unsigned char WorldTexture::get_alpha() const {
	return alpha.get_val();
}

void WorldTexture::set_colormod(glm::vec3 color) {
	colormod = color;
}

void WorldTexture::set_colormod(glm::vec3 color, int frames) {
	colormod.set_target_val(color, frames);
}

void WorldTexture::add_colormod(glm::vec3 color) {
	colormod += color;
}

glm::vec3 WorldTexture::get_colormod() const {
	return colormod.get_val();
}

void WorldTexture::flip_h() {
	h_flipped = !h_flipped;
}

void WorldTexture::set_h_flipped(bool h_flipped) {
	this->h_flipped = h_flipped;
}

bool WorldTexture::is_h_flipped() const {
	return h_flipped;
}

void WorldTexture::flip_v() {
	v_flipped = !v_flipped;
}

void WorldTexture::set_v_flipped(bool v_flipped) {
	this->v_flipped = v_flipped;
}

bool WorldTexture::is_v_flipped() const {
	return v_flipped;
}

void WorldTexture::set_billboarded(bool billboarded) {
	this->billboarded = billboarded;
}

bool WorldTexture::is_billboarded() const {
	return billboarded;
}

void WorldTexture::set_sprite(unsigned int sprite) {
	if (sprite < texture.size()) {
		this->sprite = sprite;
	}
}

void WorldTexture::next_sprite() {
	if (sprite == texture.size() - 1) {
		sprite -= texture.size();
	}
	sprite++;
}

void WorldTexture::prev_sprite() {
	if (sprite == 0) {
		sprite = texture.size();
	}
	sprite--;
}

unsigned int WorldTexture::get_sprite() const {
	return sprite;
}

void WorldTexture::render() {
	//Vulkan things	
}
#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/scene.h>

aiMatrix4x4 ass_converter(const glm::mat4& from);
glm::mat4 ass_converter(const aiMatrix4x4& from);
glm::vec3 ass_converter(const aiVector3D& from);
glm::quat ass_converter(const aiQuaternion& from);
glm::vec3 lazy_q2v(const aiQuaternion& from);
glm::vec3 lazy_q2v(const glm::quat& from);
glm::vec3 get_circular_pos(glm::vec3 origin_point, float magnitude, float angle);
glm::vec3 rotate(const glm::vec3& v, const glm::quat& q);
glm::vec3 calc_rotation(glm::vec3 base, glm::vec3 angle);
void print_vec(std::ostream& stream, glm::vec4 vec, std::string extra_chars = "");
void print_vec(std::ostream& stream, glm::vec3 vec, std::string extra_chars = "");
void print_vec(std::ostream& stream, glm::vec2 vec, std::string extra_chars = "");
void print_vec(std::ostream& stream, aiVector3D vec, std::string extra_chars = "");
void print_vec(std::ostream& stream, aiVector2D vec, std::string extra_chars = "");
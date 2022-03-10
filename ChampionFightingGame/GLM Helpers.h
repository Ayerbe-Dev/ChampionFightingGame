#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/scene.h>

aiMatrix4x4 glm_to_assimp(const glm::mat4& from);
glm::mat4 ass_converter(const aiMatrix4x4& from);
glm::vec3 ass_converter(const aiVector3D& from);
glm::quat ass_converter(const aiQuaternion& from);
glm::vec3 get_circular_pos(glm::vec3 origin_point, float magnitude, float angle);
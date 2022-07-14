#include "GLM Helpers.h"

glm::mat4 ass_converter(const aiMatrix4x4& from) {
	glm::mat4 to;
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}

glm::vec3 ass_converter(const aiVector3D& from) {
	return glm::vec3(from.x, from.y, from.z);
}

glm::quat ass_converter(const aiQuaternion& from) {
	return glm::quat(from.w, from.x, from.y, from.z);
}

aiMatrix4x4 ass_converter(const glm::mat4& from) {
	aiMatrix4x4 to;
	to.a1 = from[0][0]; to.a2 = from[1][0]; to.a3 = from[2][0]; to.a4 = from[3][0];
	to.b1 = from[0][1]; to.b2 = from[1][1]; to.b3 = from[2][1]; to.b4 = from[3][1];
	to.c1 = from[0][2]; to.c2 = from[1][2]; to.c3 = from[2][2]; to.c4 = from[3][2];
	to.d1 = from[0][3]; to.d2 = from[1][3]; to.d3 = from[2][3]; to.d4 = from[3][3];
	return to;
}

glm::vec3 lazy_q2v(const aiQuaternion& from) {
	//Converts an Assimp Quat into GLM's format, then turn it into a Matrix, convert it
	//back into Assimp's format, and use it to extract our rotation as a vector. Then
	//convert the result back back into GLM's format and return

	aiVector3D temp;
	aiVector3D ai_rot;
	ass_converter(glm::toMat4(ass_converter(from))).Decompose(temp, ai_rot, temp);
	return ass_converter(ai_rot);
}

glm::vec3 lazy_q2v(const glm::quat& from) {
	//The above, with one less conversion

	aiVector3D temp;
	aiVector3D ai_rot;
	ass_converter(glm::toMat4(from)).Decompose(temp, ai_rot, temp);
	return ass_converter(ai_rot);
}

glm::vec3 get_circular_pos(glm::vec3 origin_point, float magnitude, float angle) {
	glm::vec3 ret = origin_point;
	float new_x = magnitude * cos(angle);
	float new_y = magnitude * sin(angle);
	ret.x += new_x;
	ret.y += new_y;
	return ret;
}

glm::vec3 rotate(const glm::vec3& v, const glm::quat& q) {
	glm::vec3 u(q.x, q.y, q.z);
	float s = q.w;

	return 2.0f * dot(u, v) * u
		+ (s * s - dot(u, u)) * v
		+ 2.0f * s * cross(u, v);
}

void print_vec(std::ostream& stream, glm::vec4 vec, std::string extra_chars) {
	stream << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << extra_chars << "\n";
}

void print_vec(std::ostream& stream, glm::vec3 vec, std::string extra_chars) {
	stream << vec.x << ", " << vec.y << ", " << vec.z << extra_chars << "\n";
}

void print_vec(std::ostream& stream, glm::vec2 vec, std::string extra_chars) {
	stream << vec.x << ", " << vec.y << extra_chars << "\n";
}

void print_vec(std::ostream& stream, aiVector3D vec, std::string extra_chars) {
	stream << vec.x << ", " << vec.y << ", " << vec.z << extra_chars << "\n";
}

void print_vec(std::ostream& stream, aiVector2D vec, std::string extra_chars) {
	stream << vec.x << ", " << vec.y << extra_chars << "\n";
}
#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 5) in ivec4 v_boneids;
layout (location = 6) in vec4 v_weights;


uniform mat4 camera_matrix;
uniform mat4 model_matrix;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bone_matrix[MAX_BONES];

void main()
{
    mat4 bone_transform = mat4(0.0);
    float total_weights = 0.0;
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        bone_transform += bone_matrix[v_boneids[i]] * v_weights[i];
        total_weights += v_weights[i];
    }
    if (total_weights < 1.0) {
        bone_transform += bone_matrix[v_boneids[0]] * (1.0 - total_weights);
    }

    vec4 total_pos = bone_transform * vec4(vertex_position, 1.0);


    gl_Position = camera_matrix * (model_matrix * total_pos);

    //gl_Position = camera_matrix * model_matrix * vec4(vertex_position, 1.0);
}  
#version 330 core

// global input data (shader initialization)
layout (location = 0) in vec3 in_position;

// other uniform variables and functions
layout (location = 1) in vec2 in_texCoord;
layout (location = 2) in float in_light;

out vec2 f_texCoord;
out float f_light;
uniform mat4 model_matrix;
uniform mat4 projView;

void main(){
    // the main function of the shader
    f_texCoord = in_texCoord;
    f_light = in_light;
    gl_Position = projView * model_matrix * vec4(in_position, 1.0);
}
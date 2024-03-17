#version 330 core

// the final value of the pixel color
out vec4 fragColor;

// global input data (shader initialization)
uniform vec2 u_resolution;

// other uniform variables and functions
in vec2 f_texCoord;
in float f_light;
uniform sampler2D u_texture0;


void main() {
    // the main function of the shader
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    vec4 color = vec4(vec3(f_light), 1.0);
    
    // change color
    color = color * texture(u_texture0, f_texCoord);

    // data output
    fragColor = color;
}

#version 330 core

// the final value of the pixel color
layout (location = 0) out vec4 fragColor;

// global input data (shader initialization)
uniform vec2 u_resolution;
uniform vec3 backGround_color = vec3(0.1, 0.2, 0.2);
uniform int AA = 1;
uniform float GAMMA = 1.0;

// other uniform variables and functions
in vec2 f_texCoord;
in float f_light;
uniform sampler2D u_texture0;


vec2 getUV(in vec2 offset) {
    /* issues a uv vector from offset */
    return (2.0 * (gl_FragCoord.xy + offset) - u_resolution.xy) / u_resolution.y;
}


vec4 render(in vec2 uv) {
    /* main color render function */
    vec4 color = vec4(vec3(f_light), 1.0);
    /* change color */
    color = color * texture(u_texture0, f_texCoord);
    /* out color */
    return color;
}


/* anti-aliasing render functions */
vec4 renderAAx1() {
    return render(getUV(vec2(0)));
}

vec4 renderAAx2() {
    float bxy = int(gl_FragCoord.x + gl_FragCoord.y) & 1;
    float nbxy = 1.0 - bxy;
    vec4 colAA = (render(getUV(vec2(0.33 * nbxy, 0.))) + render(getUV(vec2(0.33 * bxy, 0.66))));
    return colAA / 2.0;
}

vec4 renderAAx3() {
    float bxy = int(gl_FragCoord.x + gl_FragCoord.y) & 1;
    float nbxy = 1. - bxy;
    vec4 colAA = (render(getUV(vec2(0.66 * nbxy, 0.))) +
    render(getUV(vec2(0.66 * bxy, 0.66))) +
    render(getUV(vec2(0.33, 0.33))));
    return colAA / 3.0;
}

vec4 renderAAx4() {
    vec4 e = vec4(0.125, -0.125, 0.375, -0.375);
    vec4 colAA = render(getUV(e.xz)) + render(getUV(e.yw)) + render(getUV(e.wx)) + render(getUV(e.zy));
    return colAA /= 4.0;
}

vec4 gammaCorrection(inout vec4 col) {
    /* gamma correction using GAMMA */
    return pow(col, vec4(GAMMA));
}


void main() {
    /* the main function of the shader */
    vec4 color = (AA == 1) ? renderAAx1() : (AA == 2) ? renderAAx2() : (AA == 3) ? renderAAx3(): renderAAx4();
    // gamma correction
    color = gammaCorrection(color);
    // data output
    fragColor = color;
}
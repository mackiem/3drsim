#version 330

in vec3 vert;
in vec3 vertColor;

out vec3 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    // Pass the tex coord straight through to the fragment shader
	fragColor = vertColor;
    
    gl_Position = proj * view * model * vec4(vert, 1);
}
#version 150

in vec3 vert;
in vec3 vertColor;
in vec2 vertTexCoord;

out vec2 fragTexCoord;
out vec3 fragColor;
out vec4 fragProjTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 projView;
uniform mat4 projProj;
uniform mat4 texMapping;

void main() {
    // Pass the tex coord straight through to the fragment shader
    fragTexCoord = vertTexCoord;
	fragColor = vertColor;

	fragProjTexCoord = texMapping * projProj * projView * model * vec4(vert, 1);
    
    gl_Position = proj * view * model * vec4(vert, 1);
    //gl_Position = vec4(vert, 1);
}
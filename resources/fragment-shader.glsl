#version 150

uniform sampler2D projTex;
uniform sampler2D tex;

in vec2 fragTexCoord;
in vec3 fragColor;
in vec4 fragProjTexCoord;

out vec4 finalColor;

void main() {
	vec4 color = vec4(fragColor, 1);
    finalColor = texture(tex, fragTexCoord);

	vec4 projTexColor = vec4(0.f, 0.f, 0.f, 0.f);
	if (fragProjTexCoord.w > 0) {
		projTexColor = textureProj(projTex, fragProjTexCoord.xyzw);
		if (fragProjTexCoord.z <= 0.f) {
			vec4 projTexColor = vec4(0.f, 0.f, 0.f, 0.f);
		}
	} 


    finalColor += color + projTexColor;
}
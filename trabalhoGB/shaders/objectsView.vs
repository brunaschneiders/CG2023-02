#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
// coordenada de textura
layout (location = 2) in vec2 tcoord;
layout (location = 3) in vec3 normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 finalColor;
out vec3 scaledNormal;
out vec2 texcoord;
out vec3 fragpos;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	
	scaledNormal = mat3(transpose(inverse(model))) * normal;
	texcoord = vec2(tcoord.x, 1 - tcoord.y);
	fragpos = vec3(model * vec4(position, 1.0));
}
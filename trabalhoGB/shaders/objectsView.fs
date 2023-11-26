#version 450

in vec3 finalColor;
in vec3 scaledNormal;
in vec2 texcoord;

// coordenada do ponto sem a proje��o da camera
in vec3 fragpos;

uniform float ka;
uniform float kd;
uniform float ks;
uniform float q;
// cor da luz => entre 0 e 1s
uniform vec3 lightColor;
// posi��o da fonte de luz pontual
uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform sampler2D tex_buffer;

out vec4 color;

void main()
{
	//Parcela ambiente
	vec3 ambient = ka * lightColor;

	//Parcela difusa
	vec3 N = normalize(scaledNormal); // vetor normal escalado para ficar com m�dulo (comprimento) 1
	vec3 L = normalize(lightPos - fragpos);
	vec3 diffuse = kd * max(dot(N,L),0.0) * lightColor;

	//Parcela especular
	vec3 V = normalize(cameraPos - fragpos);
	vec3 R = normalize(reflect(-L,N));
	vec3 specular = ks * pow(max(dot(R,V),0.0),q) * lightColor;

	vec3 texColor = texture(tex_buffer, texcoord).xyz;

	vec3 result = (ambient + diffuse) * texColor + specular;
	
	color = vec4(result,1.0);
}
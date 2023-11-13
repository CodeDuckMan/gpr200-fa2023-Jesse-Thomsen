#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
}fs_in;

uniform sampler2D _Texture;

struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];


struct Material {
	float ambientK;// (0-1)
	float diffuseK;// (0-1)	
	float specular;// (0-1)
	float shininess;// (0-1)
};
uniform Material _Material;

uniform vec3 _Camerapose;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 lightColor = vec3(0.0,0.0,0.0);
	
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		
		vec3 w = normalize(_Lights[i].position - fs_in.WorldPosition);
		vec3 v = normalize(_Camerapose - fs_in.WorldPosition);
		vec3 h = normalize((w + v)); // half vector
		vec3 specular  = _Lights[i].color * pow(max(dot(h,normal),0),_Material.shininess * _Material.specular);
		vec3 diffuse = _Lights[i].color * _Material.diffuseK * max(dot(w, normal),0);
		vec3 ambient = _Lights[i].color * _Material.ambientK;
		lightColor += ambient + diffuse + specular;
	}
	vec3 finalColor = texture(_Texture,fs_in.UV).rgb * lightColor;
	FragColor = vec4(finalColor, 1);
}
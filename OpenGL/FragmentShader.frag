#version 400 core

in vec3 outColor;
in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D textureSampler;
uniform bool isTexture = true;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 lightColor = vec3(1,1,1);

	float ambientFactor = 0.3f;
	vec3 ambient = lightColor * ambientFactor;
	//Diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float NDotL = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightColor * NDotL;
	//Specular

	float specularFactor = 0.8f;
	float shininess = 32.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = lightColor * specularFactor * pow(NDotH, shininess);
	vec3 result = (ambient + diffuse + specular) ;

	if(isTexture){
		vec4 texel = texture( textureSampler, UV );
		color =  vec4(result, 1.0f) * texel;
	}else{
		color =  vec4(result, 1.0f);
	}

}
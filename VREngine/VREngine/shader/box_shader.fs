// Fragment shader:
// ================
#version 330 core

struct Material {
	vec3 specularColor, diffuseColor, ambientColor;
	float shininess;
	int hasSpecular, hasDiffuse, hasAmbient, hasShininess;
	int hasNormal, hasHeight, hasLightmap, hasEmissive;
	sampler2D specularTexture, diffuseTexture, ambientTexture, shininessTexture;
    sampler2D normalTexture, heightTexture, lightmapTexture, emissiveTexture;
}; 
/* Note: because we now use a material struct again you want to change your
mesh class to bind all the textures using material.texture_diffuseN instead of
texture_diffuseN. */

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 2

in vec3 fragPosition;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

void main()
{    
    vec3 result = vec3(0,0,0);
    vec3 viewDir = normalize(viewPos - fragPosition );
	vec3 norm = vec3(0,0,0);
	if(material.hasNormal==1){
		norm = texture(material.normalTexture, TexCoords).rgb;
		norm = normalize(norm * 2.0 - 1.0);  
	}
	else
		norm = normalize(Normal);
    

	if(material.hasAmbient==1)
		result += vec3(texture(material.ambientTexture, TexCoords));
	else
		result += material.ambientColor;
	if(material.hasDiffuse==1)
		result += vec3(texture(material.diffuseTexture, TexCoords));
	else
		result += material.diffuseColor;
	if(material.hasSpecular==1)
		result += vec3(texture(material.specularTexture, TexCoords));
	else
		result += material.specularColor;
	color = 0.5f * vec4(result, 1.0f);
}
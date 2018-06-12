// Vertex shader:
// ================
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;


out vec2 TexCoords;
out vec3 fragPosition;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 eyeMat;
uniform mat4 projection;

uniform vec3 posInWorld;
uniform float radius;
uniform int isRedirection;

vec4 curve(vec4 pos);

void main()
{	
	vec4 pos = model*vec4(position,1.0f);
	
	if(abs(radius) > 1e-5 || isRedirection > 0){
		pos/=pos.w;
		pos -= vec4(posInWorld,0);
		pos = curve(pos);
		pos += vec4(posInWorld,0);
	}
	
	gl_Position = projection*eyeMat*view*pos;

	
    fragPosition = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoords;
}


vec4 curve(vec4 pos)
{
	vec4 realPos = pos;
	float correction = 0.0f;
	float threshold = radians(radius/(-1.0f/6*36+2*6+(3+1.0f/6*9-3*2)));

	
	float displacement = 0.0f;
	
	float r = sqrt(realPos.x*realPos.x+realPos.z*realPos.z);
	float theta = 0.0f;
	if(realPos.x>0)
		theta = asin(realPos.z/r);
	else
		theta = radians(180.0f)-asin(realPos.z/r);
	
	float rn,thetan;
	rn = r;
	if(r<3)
		thetan = theta+r*threshold-(-1.0f/6*36+2*6+(3+1.0f/6*9-3*2))*threshold;
	else if(r<6)
		thetan = theta+(-1.0f/6*r*r+2*r+(3+1.0f/6*9-3*2))*threshold-(-1.0f/6*36+2*6+(3+1.0f/6*9-3*2))*threshold;
	else
		thetan = theta+(-1.0f/6*36+2*6+(3+1.0f/6*9-3*2))*threshold-(-1.0f/6*36+2*6+(3+1.0f/6*9-3*2))*threshold;

	realPos.z = rn*sin(thetan);
	realPos.x = rn*cos(thetan);
	
	return realPos;

}


#version 150

in vec4 worldPosInGeom;

out vec4 fragmentColor;

uniform vec4 objectColor;
uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;

void main() 
{
	vec3 normal = normalize(cross(dFdx(worldPosInGeom).xyz,dFdy(worldPosInGeom).xyz));

	vec3 lightDirection = normalize(vec3(0.5,1,-1));

	float intensity = dot(normal, lightDirection);

	vec4 baseLight = normalize(objectColor) * 0.3;

	fragmentColor = vec4(((objectColor * intensity + baseLight).xyz),1);
}

#version 150

in vec4 Color;
out vec4 fragmentColor;

in vec4 worldPos;

void main() 
{
	vec3 normal = normalize(cross(dFdx(worldPos).xyz,dFdy(worldPos).xyz));

	vec3 lightDirection = normalize(vec3(0.5,1,-1));

	float intensity = dot(normal, lightDirection);

	vec3 lightColor = vec3(0.5,0,0);

	vec3 baseLight = vec3(0.6,0,0);

    fragmentColor = vec4(lightColor * intensity + vec3(0.5,0,0),1);
}

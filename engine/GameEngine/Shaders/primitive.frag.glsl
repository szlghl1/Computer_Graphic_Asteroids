#version 150

in vec4 Color;
out vec4 fragmentColor;

in vec4 worldPos;

void main() 
{
	vec3 normal = normalize(cross(dFdx(worldPos).xyz,dFdy(worldPos).xyz));

	vec3 lightDirection = normalize(vec3(0.5,1,-1));

	float intensity = dot(normal, lightDirection);

	//vec3 lightColor = vec3(0.5,0,0);

	vec4 baseLight = normalize(Color) * 0.3;

    //fragmentColor = vec4(lightColor * intensity + vec3(0.5,0,0),1);
	fragmentColor = vec4(((Color * intensity + baseLight).xyz),1);
}

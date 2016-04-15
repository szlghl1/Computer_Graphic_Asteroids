#version 150

in vec4 Normal;
out vec4 fragmentColor;

uniform vec4 objectColor;
uniform float GameTimeTotalSeconds;
uniform int Exploding = 0;

void main() 
{
	vec3 lightDirection = normalize(vec3(0.5,1,-1));

	float intensity = dot(Normal.xyz, lightDirection);

	if(Exploding == 0)
	{
		vec4 baseLight = normalize(objectColor) * 0.3;

		fragmentColor = vec4(((objectColor * intensity + baseLight).xyz),1);
	}
	else
	{
		if(int(GameTimeTotalSeconds*2) %2 == 0)
		{
			vec4 baseLight = vec4(0.3,0,0,0);

			fragmentColor = vec4(((vec4(1,0,0,0) * intensity + baseLight).xyz),1);
		}
		else
		{
			vec4 baseLight = vec4(0,0.3,0,0);

			fragmentColor = vec4(((vec4(0,1,0,0) * intensity + baseLight).xyz),1);
		}
	}

}

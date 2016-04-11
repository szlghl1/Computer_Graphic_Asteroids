/*
#version 150

layout(triangles) in;

layout(triangle_strip, max_vertices = 3) out; // type of primitive we're outputting. In this case, we're making a single triangle.

in gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];

void main() 
{
	for(int i = 0; i < 3; ++i)
	{
		gl_in[i].gl_Position *= 2;
	}
}
*/

#version 150
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float ExplosionFactor = 0.f;
uniform float GameTimeTotalSeconds = 0.f;


in gl_PerVertex
{
    vec4 gl_Position;

    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position * 2;

        EmitVertex();
    }

    EndPrimitive();

}
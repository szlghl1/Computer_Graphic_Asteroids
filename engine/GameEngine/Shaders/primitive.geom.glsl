#version 150
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float ExplosionFactor = 0.4f;
uniform float GameTimeTotalSeconds = 0.f;
uniform int Exploding = 0;

in vec4 worldPosInVert[];
out vec4 worldPosInGeom;

in gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];

void main()
{
    vec4 center;
    center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3;
    if(Exploding == 0)
    {
        for (int i = 0; i < 3; ++i)
        {
            gl_Position = gl_in[i].gl_Position;
        
            EmitVertex();
        
            worldPosInGeom = worldPosInVert[i];
        }
    }
    else
    {
        for (int i = 0; i < 3; ++i)
        {
            vec4 shrinkDirect = normalize(gl_in[i].gl_Position - center);

            gl_Position = gl_in[i].gl_Position - shrinkDirect * ExplosionFactor;
        
            EmitVertex();
        
            worldPosInGeom = worldPosInVert[i];
        }
    }

    EndPrimitive();
}
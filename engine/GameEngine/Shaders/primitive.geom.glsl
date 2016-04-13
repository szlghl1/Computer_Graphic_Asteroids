#version 150
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float ExplosionFactor = 0.f;
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
    vec4 test[3];
    test[0] = vec4(1,0,0,0);
    test[1] = vec4(0,1,0,0);
    test[2] = vec4(1,1,0,0);
    if(Exploding == 0)
    {
        for (int i = 0; i < 3; ++i)
        {
            gl_Position = gl_in[i].gl_Position;// + test[i];
        
            EmitVertex();
        
            worldPosInGeom = worldPosInVert[i];
        }
    }
    else
    {
        for (int i = 0; i < 3; ++i)
        {
            gl_Position = gl_in[i].gl_Position + test[i];
        
            EmitVertex();
        
            worldPosInGeom = worldPosInVert[i];
        }
    }

    EndPrimitive();
}
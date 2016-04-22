#version 150
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float GameTimeTotalSeconds;
uniform float TimeBeginToExplode;
uniform int Exploding = 0;

out vec4 Normal;

float ExplosionFactor = 0.2f;

in gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];

void main()
{
    vec3 v0 = gl_in[0].gl_Position.xyz;
    vec3 v1 = gl_in[1].gl_Position.xyz;
    vec3 v2 = gl_in[2].gl_Position.xyz;

    Normal = vec4(normalize(cross(v2 - v1, v0 - v1)),1);

    vec4 center;
    center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3;

    for (int i = 0; i < 3; ++i)
    {
        if(Exploding == 0)
        {
            gl_Position = gl_in[i].gl_Position;
        }
        else
        {
            vec4 shrinkDirect = normalize(center - gl_in[i].gl_Position);

            gl_Position = gl_in[i].gl_Position + (shrinkDirect * ExplosionFactor + Normal) * (GameTimeTotalSeconds - TimeBeginToExplode);
        }
        EmitVertex();
    }
    EndPrimitive();
}
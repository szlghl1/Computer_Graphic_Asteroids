#version 150

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;
in vec3 Pos;
out vec4 worldPosInVert;

void main()
{
    vec4 position = vec4(Pos,1);
    
    worldPosInVert = World * position;
    
    gl_Position = Projection * View * World * position;
}
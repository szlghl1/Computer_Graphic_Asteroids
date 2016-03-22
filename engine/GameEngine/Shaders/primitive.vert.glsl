#version 150

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;
in vec3 Pos;
out vec4 worldPos;

void main()
{
    vec4 position = vec4(Pos,1);
    
    worldPos = World * position;
    
    gl_Position = Projection * View * World * position;
}
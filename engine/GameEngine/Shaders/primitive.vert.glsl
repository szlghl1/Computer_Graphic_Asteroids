#version 150

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 translation;
uniform mat4 rotation;


in vec3 Pos;

out vec4 Color;

void main()
{
    vec4 position = vec4(Pos,1);
    
    position =  Projection *  View * World * translation * rotation * position;
    
    Color = vec4(0,1,0,0);
    gl_Position = position;
}

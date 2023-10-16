#shader vertex
#version 330 core

layout (location = 0) in vec4 position; // <vec2 pos, vec2 tex>

uniform mat4 u_MVP;
uniform vec4 u_Zoom;

void main()
{
    gl_Position = u_MVP * (u_Zoom * position);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

uniform vec4 u_Colour;

void main()
{
    colour = u_Colour;
}
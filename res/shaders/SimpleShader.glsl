#shader vertex
#version 330 core

layout (location = 0) in vec4 position; // <vec2 pos, vec2 tex>
layout (location = 1) in vec4 colour; // <vec2 pos, vec2 tex>

out vec4 v_Colour;

uniform mat4 u_MVP;
uniform vec4 u_Zoom;

void main()
{
    v_Colour = colour;
    gl_Position = u_MVP * (u_Zoom * position);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec4 v_Colour;

void main()
{
    colour = v_Colour;
}
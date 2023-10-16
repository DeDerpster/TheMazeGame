#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;

out vec2 v_TexCoord;
out float v_TexIndex;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
   v_TexIndex = texIndex;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;
in float v_TexIndex;

uniform vec4 u_Colour;
uniform sampler2D u_Textures[32];

void main()
{
    colour = vec4( 1.0, 0, 0, 1 );
}
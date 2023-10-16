#shader vertex
#version 330 core
layout (location = 0) in vec4 position; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 texCoord; //
layout (location = 2) in vec4 textColour; //
layout (location = 3) in float texIndex; //

out vec2 v_TexCoords;
out vec4 v_TextColour;
out float v_TexIndex;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoords = texCoord;
    v_TextColour = textColour;
    v_TexIndex = texIndex;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoords;
in vec4 v_TextColour;
in float v_TexIndex;


uniform sampler2D u_Textures[32];

void main()
{
    int index = int(v_TexIndex);
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Textures[index], v_TexCoords).r);
    colour = v_TextColour * sampled;
}
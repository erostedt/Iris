#version 330 core
layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_texture_coordinate;
uniform sampler2D u_texture;

void main()
{
    color = texture(u_texture, v_texture_coordinate);
    //color = v_color;
}

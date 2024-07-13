#version 460 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 in_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 Color;
void main()
{
    gl_Position = u_projection * u_view * u_model * position;
    Color = in_color;
}

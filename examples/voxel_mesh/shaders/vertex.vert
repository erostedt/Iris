#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_normals;
layout(location = 2) in vec2 a_texture_coordinate;

out vec2 v_texture_coordinate;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
    v_texture_coordinate = a_texture_coordinate;
}

#type vertex

#version 330 core

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0f);
}

#type fragment

#version 330 core
out vec4 color;

uniform vec3 u_Color;

void main() {
    color = vec4(u_Color, 1.0);
}

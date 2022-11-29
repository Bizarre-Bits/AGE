#type vertex
#version 410 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) out float v_TexIndex;

void main() {
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * vec4(a_Pos, 1.0f);
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
}

#type fragment
#version 410 core

layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in float v_TexIndex;

uniform sampler2D u_Textures[32];

out vec4 f_Color;

void main() {
    f_Color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
}

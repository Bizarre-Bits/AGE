#type vertex
#version 410 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TillingFactor;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;

layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) out float v_TexIndex;
layout(location = 3) out float v_TillingFactor;
flat layout(location = 4) out int v_EntityID;

void main() {
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * vec4(a_Pos, 1.0f);
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
    v_TillingFactor = a_TillingFactor;
    v_EntityID = a_EntityID;
}

#type fragment
#version 410 core

layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in float v_TexIndex;
layout(location = 3) in float v_TillingFactor;
flat layout(location = 4) in int v_EntityID;

uniform sampler2D u_Textures[32];

layout(location = 0) out vec4 f_Color;
layout(location = 1) out int f_EntityID;

void main() {
    f_Color = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TillingFactor) * v_Color;
    f_EntityID = v_EntityID;
}

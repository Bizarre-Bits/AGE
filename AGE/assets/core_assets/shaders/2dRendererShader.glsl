#type vertex
#version 450 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

layout (std140, binding = 0) uniform Camera {
    mat4 ViewProjection;
} u_Camera;

struct VertexOutput {
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

layout (location = 0) out VertexOutput vertexOutput;
layout (location = 4) out flat int v_EntityID;

void main() {
    vertexOutput.Color = a_Color;
    vertexOutput.TexCoord = a_TexCoord;
    vertexOutput.TexIndex = a_TexIndex;
    vertexOutput.TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;

    gl_Position = u_Camera.ViewProjection * vec4(a_Pos, 1.0f);
}

#type fragment
#version 450 core

struct VertexOutput {
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

layout(location = 0) in VertexOutput vertexOutput;
layout(location = 4) in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_Textures[32];

layout(location = 0) out vec4 f_Color;
layout(location = 1) out int f_EntityID;

void main() {
    f_Color = texture(u_Textures[int(vertexOutput.TexIndex)], vertexOutput.TexCoord * vertexOutput.TilingFactor) * vertexOutput.Color;
    f_EntityID = v_EntityID;
}

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;

void main() {
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * vec4(a_Pos, 1.0f);
    v_Color = a_Color;
}

#type fragment
#version 330 core

in vec2 v_TexCoord;
out vec4 f_Color;

uniform sampler2D u_Texture;
in vec4 v_Color;

void main() {
    //    f_Color = texture(u_Texture, v_TexCoord) * u_Color;
    f_Color = v_Color;
}

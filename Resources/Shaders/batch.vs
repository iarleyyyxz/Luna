#version 330 core
layout (location = 0) in vec3 aPos;      // Posição (x, y, z)
layout (location = 1) in vec4 aColor;    // Cor (r, g, b, a)
layout (location = 2) in vec2 aTexCoord; // Coordenadas de textura (u, v)
layout (location = 3) in float aTexID;   // ID da textura (para múltiplos binds)

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexID;

uniform mat4 u_Projection; // Matriz de projeção

void main() {
    gl_Position = u_Projection * vec4(aPos, 1.0);
    v_Color = aColor;
    v_TexCoord = aTexCoord;
    v_TexID = aTexID;
}
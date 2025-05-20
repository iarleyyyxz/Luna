#version 330 core
layout (location = 0) in vec3 aPos;      // Posi��o do v�rtice
layout (location = 1) in vec2 aTexCoord; // Coordenadas de textura do v�rtice

out vec2 TexCoord; // Passa as coordenadas de textura para o fragment shader

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

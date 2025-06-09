#version 330 core
layout (location = 0) in vec3 aPos;  // Posi��o do v�rtice
layout (location = 1) in vec4 aColor; // Cor do v�rtice

out vec4 fColor; // A cor que ser� passada para o fragment shader

uniform mat4 u_Projection; // Matriz de proje��o (inclui view e proje��o, ou apenas proje��o)

void main() {
    fColor = aColor;
    gl_Position = u_Projection * vec4(aPos, 1.0); // Transforma a posi��o do v�rtice
}

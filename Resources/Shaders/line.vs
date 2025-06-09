#version 330 core
layout (location = 0) in vec3 aPos;  // Posição do vértice
layout (location = 1) in vec4 aColor; // Cor do vértice

out vec4 fColor; // A cor que será passada para o fragment shader

uniform mat4 u_Projection; // Matriz de projeção (inclui view e projeção, ou apenas projeção)

void main() {
    fColor = aColor;
    gl_Position = u_Projection * vec4(aPos, 1.0); // Transforma a posição do vértice
}

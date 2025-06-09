#version 330 core
in vec4 fColor; // A cor interpolada do vertex shader
out vec4 FragColor; // A cor final do fragmento

void main() {
    FragColor = fColor; // Usa a cor do vértice para colorir o fragmento
}

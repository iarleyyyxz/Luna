#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // Recebe as coordenadas de textura do vertex shader
uniform sampler2D ourTexture; // O sampler para a nossa textura

void main() {
    FragColor = texture(ourTexture, TexCoord); // Amostra a textura usando as coordenadas
}

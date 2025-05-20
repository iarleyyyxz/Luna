#version 330 core
out vec4 FragColor;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexID;

uniform sampler2D u_Textures[32]; // Array de samplers de textura (MAX_TEXTURE_SLOTS)

void main() {
    vec4 texColor = vec4(1.0);
    if (v_TexID > 0.0) {
        int textureIndex = int(v_TexID);
        if (textureIndex < 32) { // Certifique-se de que o índice está dentro dos limites
            texColor = texture(u_Textures[textureIndex], v_TexCoord);
        } else {
            texColor = vec4(1.0, 0.0, 1.0, 1.0); // Cor de erro se o ID estiver fora dos limites
        }
    } else { // v_TexID == 0.0 (textura branca padrão)
        texColor = texture(u_Textures[0], v_TexCoord);
    }

    FragColor = texColor * v_Color; // Multiplica a cor da textura pela cor do vértice (tintColor)
}
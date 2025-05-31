#ifndef RENDERER2D_HPP
#define RENDERER2D_HPP

#include <GL/glew.h>
#include <glm.hpp>
#include <vector>
#include <array>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Sprite.hpp" // NOVO: Incluir a classe Sprite

// Estrutura para um v�rtice de quad 2D para renderiza��o em lotes
struct QuadVertex {
    glm::vec3 position; // Posi��o (x, y, z)
    glm::vec4 color;    // Cor (r, g, b, a)
    glm::vec2 texCoord; // Coordenadas de textura (u, v)
    float texID;        // ID da textura (para m�ltiplos binds)
};

class Renderer2D {
public:
    Renderer2D();
    ~Renderer2D();

    // Inicializa o Renderer2D (configura VAO, VBO, shaders)
    bool Init(float screenWidth, float screenHeight);
    // Encerra o Renderer2D (libera recursos OpenGL)
    void Shutdown();

    // Inicia uma nova cena de renderiza��o em lotes
    void beginScene(const glm::mat4& projection);
    // Finaliza a cena, desenhando todos os quads em lote
    void endScene();

    // Desenha um quad com cor s�lida
    void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    // Desenha um quad com textura
    void drawQuad(const glm::vec2& position, const glm::vec2& size, Texture& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
    // Desenha um quad com textura e coordenadas de textura espec�ficas
    void drawQuad(const glm::vec2& position, const glm::vec2& size, Texture& texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax, const glm::vec4& tintColor = glm::vec4(1.0f));
    // Desenha uma line com cordenadas especificas
    void drawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color);
    // NOVO: Desenha um Sprite
    void drawSprite(const glm::vec2& position, const glm::vec2& size, Sprite& sprite, const glm::vec4& tintColor = glm::vec4(1.0f));

private:
    // Constantes para renderiza��o em lotes
    static const GLuint MAX_QUADS = 10000; // N�mero m�ximo de quads por lote
    static const GLuint MAX_VERTICES = MAX_QUADS * 4; // 4 v�rtices por quad
    static const GLuint MAX_INDICES = MAX_QUADS * 6;   // 6 �ndices por quad (2 tri�ngulos)
    static const GLuint MAX_TEXTURE_SLOTS = 32; // Slots de textura para m�ltiplos binds (limite OpenGL)

    // Recursos OpenGL
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    // Dados do lote (buffer de CPU e ponteiro para escrita)
    QuadVertex* quadBuffer;
    QuadVertex* quadBufferPtr;
    GLuint indexCount;

    // Shaders
    Shader* batchShader;

    // Texturas
    // Vari�veis est�ticas para slots de textura e �ndice para que sejam compartilhadas entre inst�ncias
    // e para que o compilador n�o reclame de inicializa��o de membros est�ticos no construtor.
    static std::array<Texture*, MAX_TEXTURE_SLOTS> textureSlots;
    static GLuint textureSlotIndex;
    Texture* defaultWhiteTexture; // Textura branca padr�o para quads de cor s�lida

    // Matrizes
    glm::mat4 projectionMatrix;

    // Fun��es auxiliares
    void initOpenGLResources(); // Configura VAO, VBO, EBO
    void flush(); // Envia o lote para a GPU e desenha
    float getTextureSlot(Texture& texture); // Obt�m o slot de textura para uma textura

    // ... outras membros ...
    Shader* lineShader; // Novo shader para linhas
    GLuint lineVAO, lineVBO;
    void initLineRendering();
};

#endif // RENDERER2D_HPP
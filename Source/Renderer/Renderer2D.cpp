#include "Renderer2D.hpp"
#include <matrix_transform.hpp> // Para glm::ortho
#include <type_ptr.hpp>         // Para glm::value_ptr
#include <array> // Já incluído no .hpp, mas bom ter aqui também

// Variáveis estáticas da classe (inicialização fora da classe)
std::array<Texture*, Renderer2D::MAX_TEXTURE_SLOTS> Renderer2D::textureSlots;
GLuint Renderer2D::textureSlotIndex = 0;

Renderer2D::Renderer2D() : VAO(0), VBO(0), EBO(0), quadBuffer(nullptr), quadBufferPtr(nullptr), indexCount(0), batchShader(nullptr), defaultWhiteTexture(nullptr) {
    std::cout << "Renderer2D construído!" << std::endl;
}

Renderer2D::~Renderer2D() {
    Shutdown();
    std::cout << "Renderer2D destruído!" << std::endl;
}

bool Renderer2D::Init(float screenWidth, float screenHeight) {
    std::cout << "Inicializando Renderer2D..." << std::endl;

    // Configurações básicas de renderização OpenGL para 2D
    glEnable(GL_BLEND); // Habilitar blending para transparência
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Função de blending padrão

    // Inicializar recursos OpenGL (VAO, VBO, EBO)
    initOpenGLResources();

    // Criar o shader para renderização em lotes
    // Você precisará criar estes arquivos de shader:
    // Resources/Shaders/batch.vs e Resources/Shaders/batch.fs
    batchShader = new Shader("Resources/Shaders/batch.vs", "Resources/Shaders/batch.fs");
    if (!batchShader->ID) {
        std::cerr << "Falha ao criar o shader de lote." << std::endl;
        return false;
    }

   // lineShader = new Shader("Resources/Shaders/line.vert", "Resources/Shaders/frag.vert");
   // if (!lineShader->ID) {
     //   std::cerr << "Falha ao criar o shader de linhas." << std::endl;
       // return false;
   // }

    // Criar uma textura branca 1x1 padrão para quads de cor sólida
    unsigned char whitePixel[] = { 255, 255, 255, 255 }; // RGBA branco
    // Certifique-se de que o construtor Texture(int, int, unsigned char*, GLenum) existe e que Texture.hpp está atualizado!
    // Se você estiver a ter um erro aqui, limpe e reconstrua o seu projeto.
    defaultWhiteTexture = new Texture(1, 1, whitePixel); // Construtor de textura a partir de dados
    if (!defaultWhiteTexture->getID()) {
        std::cerr << "Falha ao criar a textura branca padrão." << std::endl;
        return false;
    }

    // Configurar a projeção ortográfica (pode ser ajustada dinamicamente)
    projectionMatrix = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);

    initLineRendering();

    std::cout << "Renderer2D inicializado com sucesso." << std::endl;
    return true;
}

void Renderer2D::Shutdown() {
    std::cout << "Encerrando Renderer2D..." << std::endl;

    // Liberar recursos OpenGL
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);

    // Liberar o buffer de vértices
    if (quadBuffer) {
        delete[] quadBuffer;
        quadBuffer = nullptr;
    }

    // Liberar o shader
    if (batchShader) {
        delete batchShader;
        batchShader = nullptr;
    }

    // Liberar a textura branca padrão
    if (defaultWhiteTexture) {
        delete defaultWhiteTexture;
        defaultWhiteTexture = nullptr;
    }

    // ... shutdown existente ...
    if (lineVAO) glDeleteVertexArrays(1, &lineVAO);
    if (lineVBO) glDeleteBuffers(1, &lineVBO);

    std::cout << "Renderer2D encerrado." << std::endl;
}

void Renderer2D::initOpenGLResources() {
    // Gerar VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Alocar memória para o VBO, mas não preencher ainda
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

    // Configurar layout dos atributos do vértice
    // Posição (layout 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, position));
    glEnableVertexAttribArray(0);
    // Cor (layout 1)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, color));
    glEnableVertexAttribArray(1);
    // Coordenadas de Textura (layout 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, texCoord));
    glEnableVertexAttribArray(2);
    // ID da Textura (layout 3)
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, texID));
    glEnableVertexAttribArray(3);

   

    // Gerar índices para os quads (eles são sempre os mesmos)
    // 0, 1, 2, 2, 3, 0 para cada quad
    GLuint* quadIndices = new GLuint[MAX_INDICES];
    GLuint offset = 0;
    for (GLuint i = 0; i < MAX_INDICES; i += 6) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDICES * sizeof(GLuint), quadIndices, GL_STATIC_DRAW);
    delete[] quadIndices;

    // Desvincular VAO
    glBindVertexArray(0);

    // Alocar o buffer de CPU para os vértices
    quadBuffer = new QuadVertex[MAX_VERTICES];
}

void Renderer2D::initLineRendering()
{
    glGenVertexArrays(1, &lineVAO);
    glBindVertexArray(lineVAO);

    glGenBuffers(1, &lineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW); // Apenas posições agora

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer2D::drawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color)
{
    lineShader->use();
    glm::mat4 projection = projectionMatrix;
    lineShader->setMat4("u_Projection", projection);
    lineShader->setVec4("u_Color", color);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

    std::array<glm::vec3, 2> lineVertices = {
        glm::vec3(p1, 0.0f),
        glm::vec3(p2, 0.0f)
    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineVertices), lineVertices.data());

    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
    lineShader->unuse();
}

void Renderer2D::beginScene(const glm::mat4& projMatrix) {
    projectionMatrix = projMatrix;
    quadBufferPtr = quadBuffer; // Resetar o ponteiro do buffer
    indexCount = 0;             // Resetar a contagem de índices
    textureSlotIndex = 1;       // Slot 0 reservado para a textura branca padrão
    textureSlots[0] = defaultWhiteTexture; // Sempre ter a textura branca no slot 0
}

void Renderer2D::endScene() {
    flush(); // Desenhar todos os quads restantes
}

void Renderer2D::flush() {
    if (indexCount == 0) return; // Nada para desenhar
    
    // Calcular o tamanho dos dados a serem enviados para o VBO
    GLsizeiptr size = (uint8_t*)quadBufferPtr - (uint8_t*)quadBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, quadBuffer); // Atualizar apenas a parte usada do VBO

    batchShader->use();
    batchShader->setMat4("u_Projection", projectionMatrix); // Passar a matriz de projeção

    // Ativar e vincular as texturas usadas no lote
    for (GLuint i = 0; i < textureSlotIndex; ++i) {
        if (textureSlots[i]) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textureSlots[i]->getID());
        }
    }

    // Passar os samplers de textura para o shader
    GLint samplers[MAX_TEXTURE_SLOTS];
    for (GLuint i = 0; i < MAX_TEXTURE_SLOTS; ++i) {
        samplers[i] = i;
    }
    batchShader->setInt("u_Textures", MAX_TEXTURE_SLOTS, samplers); // Usa o método setInt para arrays

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr); // Desenhar o lote
    glBindVertexArray(0);

    batchShader->unuse();

    // Resetar para o próximo lote
    quadBufferPtr = quadBuffer;
    indexCount = 0;
    textureSlotIndex = 1; // Slot 0 reservado para a textura branca padrão
}

float Renderer2D::getTextureSlot(Texture& texture) {
    float texID = 0.0f; // ID 0.0f para a textura branca padrão

    if (texture.getID() == defaultWhiteTexture->getID()) {
        return 0.0f; // Textura branca padrão sempre no slot 0
    }

    // Procurar se a textura já está em um slot
    for (GLuint i = 1; i < textureSlotIndex; ++i) { // Começar do slot 1
        if (textureSlots[i] && textureSlots[i]->getID() == texture.getID()) {
            texID = (float)i;
            break;
        }
    }

    // Se a textura não foi encontrada, atribuir um novo slot
    if (texID == 0.0f) { // Se ainda for 0.0f, significa que não foi encontrada
        if (textureSlotIndex >= MAX_TEXTURE_SLOTS) {
            // Lote cheio, precisa desenhar o lote atual e começar um novo
            flush();
            // Após o flush, o textureSlotIndex é resetado para 1, então podemos adicionar
            textureSlots[textureSlotIndex] = &texture;
            texID = (float)textureSlotIndex;
            textureSlotIndex++;
        }
        else {
            textureSlots[textureSlotIndex] = &texture;
            texID = (float)textureSlotIndex;
            textureSlotIndex++;
        }
    }
    return texID;
}


void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    // Usar a textura branca padrão
    drawQuad(position, size, *defaultWhiteTexture, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), color);
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, Texture& texture, const glm::vec4& tintColor) {
    // Usar coordenadas de textura padrão (0,0) a (1,1)
    drawQuad(position, size, texture, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), tintColor);
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, Texture& texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax, const glm::vec4& tintColor) {
    if (indexCount >= MAX_INDICES) {
        flush(); // Lote cheio, desenha e reinicia
    }

    float texID = getTextureSlot(texture);

    // Definir os 4 vértices do quad
    // Top-left
    quadBufferPtr->position = glm::vec3(position.x, position.y + size.y, 0.0f);
    quadBufferPtr->color = tintColor;
    quadBufferPtr->texCoord = glm::vec2(texCoordMin.x, texCoordMax.y);
    quadBufferPtr->texID = texID;
    quadBufferPtr++;

    // Top-right
    quadBufferPtr->position = glm::vec3(position.x + size.x, position.y + size.y, 0.0f);
    quadBufferPtr->color = tintColor;
    quadBufferPtr->texCoord = glm::vec2(texCoordMax.x, texCoordMax.y);
    quadBufferPtr->texID = texID;
    quadBufferPtr++;

    // Bottom-right
    quadBufferPtr->position = glm::vec3(position.x + size.x, position.y, 0.0f);
    quadBufferPtr->color = tintColor;
    quadBufferPtr->texCoord = glm::vec2(texCoordMax.x, texCoordMin.y);
    quadBufferPtr->texID = texID;
    quadBufferPtr++;

    // Bottom-left
    quadBufferPtr->position = glm::vec3(position.x, position.y, 0.0f);
    quadBufferPtr->color = tintColor;
    quadBufferPtr->texCoord = glm::vec2(texCoordMin.x, texCoordMin.y);
    quadBufferPtr->texID = texID;
    quadBufferPtr++;

    indexCount += 6; // Adicionar 6 índices para este quad
}

void Renderer2D::drawQuad(const glm::mat4& transform, Texture& texture, const glm::vec4& tintColor)
{
    if (indexCount >= MAX_INDICES)
    {
        flush();
    }

    float texID = getTextureSlot(texture);

    // Os quatro vértices do quad no espaço do modelo
    glm::vec4 vertices[4] = {
        glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f),  // Top-left
        glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),   // Top-right
        glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),  // Bottom-right
        glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f)   // Bottom-left
    };

    // Coordenadas de textura correspondentes
    glm::vec2 texCoords[4] = {
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        { 0.0f, 0.0f }
    };

    for (int i = 0; i < 4; ++i)
    {
        glm::vec3 transformedPos = transform * vertices[i];
        quadBufferPtr->position = transformedPos;
        quadBufferPtr->color = tintColor;
        quadBufferPtr->texCoord = texCoords[i];
        quadBufferPtr->texID = texID;
        quadBufferPtr++;
    }

    indexCount += 6;
}

// NOVO: Implementação do drawSprite
void Renderer2D::drawSprite(const glm::vec2& position, const glm::vec2& size, Sprite& sprite, const glm::vec4& tintColor) {
    // Reutiliza o método drawQuad, passando a textura e as coordenadas de textura do sprite
    if (sprite.getTexture()) {
        drawQuad(position, size, *sprite.getTexture(), sprite.getTexCoordMin(), sprite.getTexCoordMax(), tintColor);
    }
    else {
        std::cerr << "ERRO::RENDERER2D::DRAW_SPRITE: Sprite com textura nula. Desenhando quad de cor sólida." << std::endl;
        drawQuad(position, size, tintColor); // Desenha um quad de cor sólida como fallback
    }
}
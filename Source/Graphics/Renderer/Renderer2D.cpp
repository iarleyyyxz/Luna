#include "Renderer2D.hpp"
#include <matrix_transform.hpp> // Para glm::ortho
#include <type_ptr.hpp>         // Para glm::value_ptr
#include <array> // J� inclu�do no .hpp, mas bom ter aqui tamb�m

// Vari�veis est�ticas da classe (inicializa��o fora da classe)
std::array<Texture*, Renderer2D::MAX_TEXTURE_SLOTS> Renderer2D::textureSlots;
GLuint Renderer2D::textureSlotIndex = 0;

Renderer2D::Renderer2D() : VAO(0), VBO(0), EBO(0), quadBuffer(nullptr), quadBufferPtr(nullptr), indexCount(0), batchShader(nullptr), defaultWhiteTexture(nullptr) {
    std::cout << "Renderer2D constru�do!" << std::endl;
}

Renderer2D::~Renderer2D() {
    Shutdown();
    std::cout << "Renderer2D destru�do!" << std::endl;
}

bool Renderer2D::Init(float screenWidth, float screenHeight) {
    std::cout << "Inicializando Renderer2D..." << std::endl;

    // Configura��es b�sicas de renderiza��o OpenGL para 2D
    glEnable(GL_BLEND); // Habilitar blending para transpar�ncia
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Fun��o de blending padr�o

    // Inicializar recursos OpenGL (VAO, VBO, EBO)
    initOpenGLResources();

    // Criar o shader para renderiza��o em lotes
    // Voc� precisar� criar estes arquivos de shader:
    // Resources/Shaders/batch.vs e Resources/Shaders/batch.fs
    batchShader = new Shader("Resources/Shaders/batch.vs", "Resources/Shaders/batch.fs");
    if (!batchShader->ID) {
        std::cerr << "Falha ao criar o shader de lote." << std::endl;
        return false;
    }

    // Corrigindo a inicializa��o do lineShader
    lineShader = new Shader("Resources/Shaders/line.vs", "Resources/Shaders/line.fs"); // Certifique-se que o VS e FS para linha s�o diferentes
    if (!lineShader->ID) {
        std::cerr << "Falha ao criar o shader de linhas." << std::endl;
        return false;
    }

    // Criar uma textura branca 1x1 padr�o para quads de cor s�lida
    unsigned char whitePixel[] = { 255, 255, 255, 255 }; // RGBA branco
    // Certifique-se de que o construtor Texture(int, int, unsigned char*, GLenum) existe e que Texture.hpp est� atualizado!
    // Se voc� estiver a ter um erro aqui, limpe e reconstrua o seu projeto.
    defaultWhiteTexture = new Texture(1, 1, whitePixel); // Construtor de textura a partir de dados
    if (!defaultWhiteTexture->getID()) {
        std::cerr << "Falha ao criar a textura branca padr�o." << std::endl;
        return false;
    }

    // Configurar a proje��o ortogr�fica (pode ser ajustada dinamicamente)
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

    // Liberar o buffer de v�rtices
    if (quadBuffer) {
        delete[] quadBuffer;
        quadBuffer = nullptr;
    }

    // Liberar o shader
    if (batchShader) {
        delete batchShader;
        batchShader = nullptr;
    }

    if (lineShader) { // Liberar o lineShader tamb�m
        delete lineShader;
        lineShader = nullptr;
    }

    // Liberar a textura branca padr�o
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
    // Alocar mem�ria para o VBO, mas n�o preencher ainda
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

    // Configurar layout dos atributos do v�rtice
    // Posi��o (layout 0)
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

   

    // Gerar �ndices para os quads (eles s�o sempre os mesmos)
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

    // Alocar o buffer de CPU para os v�rtices
    quadBuffer = new QuadVertex[MAX_VERTICES];
}

void Renderer2D::initLineRendering()
{
    glGenVertexArrays(1, &lineVAO);
    glBindVertexArray(lineVAO);

    glGenBuffers(1, &lineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW); // Apenas posi��es agora

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer2D::beginScene(const glm::mat4& projMatrix) {
    projectionMatrix = projMatrix;
    quadBufferPtr = quadBuffer; // Resetar o ponteiro do buffer
    indexCount = 0;             // Resetar a contagem de �ndices
    textureSlotIndex = 1;       // Slot 0 reservado para a textura branca padr�o
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
    batchShader->setMat4("u_Projection", projectionMatrix); // Passar a matriz de proje��o

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
    batchShader->setInt("u_Textures", MAX_TEXTURE_SLOTS, samplers); // Usa o m�todo setInt para arrays

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr); // Desenhar o lote
    glBindVertexArray(0);

    batchShader->unuse();

    // Resetar para o pr�ximo lote
    quadBufferPtr = quadBuffer;
    indexCount = 0;
    textureSlotIndex = 1; // Slot 0 reservado para a textura branca padr�o
}

float Renderer2D::getTextureSlot(Texture& texture) {
    float texID = 0.0f; // ID 0.0f para a textura branca padr�o

    if (texture.getID() == defaultWhiteTexture->getID()) {
        return 0.0f; // Textura branca padr�o sempre no slot 0
    }

    // Procurar se a textura j� est� em um slot
    for (GLuint i = 1; i < textureSlotIndex; ++i) { // Come�ar do slot 1
        if (textureSlots[i] && textureSlots[i]->getID() == texture.getID()) {
            texID = (float)i;
            break;
        }
    }

    // Se a textura n�o foi encontrada, atribuir um novo slot
    if (texID == 0.0f) { // Se ainda for 0.0f, significa que n�o foi encontrada
        if (textureSlotIndex >= MAX_TEXTURE_SLOTS) {
            // Lote cheio, precisa desenhar o lote atual e come�ar um novo
            flush();
            // Ap�s o flush, o textureSlotIndex � resetado para 1, ent�o podemos adicionar
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
    // Usar a textura branca padr�o
    drawQuad(position, size, *defaultWhiteTexture, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), color);
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, Texture& texture, const glm::vec4& tintColor) {
    // Usar coordenadas de textura padr�o (0,0) a (1,1)
    drawQuad(position, size, texture, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), tintColor);
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, Texture& texture, const glm::vec2& texCoordMin, const glm::vec2& texCoordMax, const glm::vec4& tintColor) {
    if (indexCount >= MAX_INDICES) {
        flush(); // Lote cheio, desenha e reinicia
    }

    float texID = getTextureSlot(texture);

    // Definir os 4 v�rtices do quad
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

    indexCount += 6; // Adicionar 6 �ndices para este quad
}

void Renderer2D::DrawLine(glm::vec2 p1, glm::vec2 p2, glm::vec4 color, float thickness) {
    // ATEN��O: Para realmente desenhar linhas com espessura > 1, voc� precisaria de um geometry shader
    // que expanda a linha para um quad, ou desenhar um quad fino diretamente no CPU.
    // Esta implementa��o utiliza o pipeline GL_LINES, que pode ser limitado pela GPU para espessura.
    // A vari�vel 'thickness' � passada para o shader como um uniform.
    // SEU SHADER DE LINHA PRECISA DE UM UNIFORM 'u_Thickness' E LIDAR COM ELE.

    lineShader->use();
    // Passe a matriz de proje��o para o shader de linha
    lineShader->setMat4("u_Projection", projectionMatrix);
    lineShader->setVec4("u_Color", color); // A cor pode vir do uniforme ou do pr�prio v�rtice
    lineShader->setFloat("u_Thickness", thickness); // Passe a espessura para o shader de linha

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

    // Dados dos v�rtices da linha
    LineVertex lineVertices[2] = {
        {glm::vec3(p1.x, p1.y, 0.0f), color},
        {glm::vec3(p2.x, p2.y, 0.0f), color}
    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineVertices), lineVertices); // Atualiza os dados do VBO

    glDrawArrays(GL_LINES, 0, 2); // Desenha a linha

    glBindVertexArray(0);
    lineShader->unuse();
}

void Renderer2D::DrawFilledRect(glm::vec2 center, glm::vec2 size, glm::vec4 color, float rotationRadians) {
    // Para desenhar um ret�ngulo preenchido rotacionado, a melhor abordagem �
    // construir uma matriz de transforma��o (modelo) e pass�-la para o shader.
    // Em seguida, use o m�todo drawQuad que aceita uma matriz de transforma��o.

    // 1. Criar a matriz de modelo: Escala -> Rota��o -> Transla��o
    glm::mat4 model = glm::mat4(1.0f);

    // Transla��o (move o objeto para o centro do ret�ngulo)
    model = glm::translate(model, glm::vec3(center.x, center.y, 0.0f));

    // Rota��o (em torno do eixo Z para 2D)
    model = glm::rotate(model, rotationRadians, glm::vec3(0.0f, 0.0f, 1.0f));

    // Escala (ajusta o tamanho do quad de -0.5 a 0.5 para o tamanho 'size')
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

    // Agora, chame o seu m�todo drawQuad que aceita uma matriz de transforma��o.
    // Usaremos a textura branca padr�o, j� que � um ret�ngulo de cor s�lida.
    drawQuad(model, *defaultWhiteTexture, color);
}



void Renderer2D::drawQuad(const glm::mat4& transform, Texture& texture, const glm::vec4& tintColor)
{
    if (indexCount >= MAX_INDICES)
    {
        flush();
    }

    float texID = getTextureSlot(texture);

    // Os quatro v�rtices do quad no espa�o do modelo
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

// NOVO: Implementa��o do drawSprite
void Renderer2D::drawSprite(const glm::vec2& position, const glm::vec2& size, Sprite& sprite, const glm::vec4& tintColor) {
    // Reutiliza o m�todo drawQuad, passando a textura e as coordenadas de textura do sprite
    if (sprite.getTexture()) {
        drawQuad(position, size, *sprite.getTexture(), sprite.getTexCoordMin(), sprite.getTexCoordMax(), tintColor);
    }
    else {
        std::cerr << "ERRO::RENDERER2D::DRAW_SPRITE: Sprite com textura nula. Desenhando quad de cor s�lida." << std::endl;
        drawQuad(position, size, tintColor); // Desenha um quad de cor s�lida como fallback
    }
}
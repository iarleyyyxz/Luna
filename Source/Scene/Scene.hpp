#ifndef SCENE_HPP
#define SCENE_HPP

#include <GLFW/glfw3.h> // Para GLFWwindow*
#include <glm.hpp>   // Para glm::vec2, etc.

// Forward declarations para evitar inclusões circulares e dependências desnecessárias
class Renderer2D;
class Keyboard;
class Mouse;
class Camera2D;
class Spritesheet;
class Animation;
class Sprite;

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    // Métodos virtuais puros que cada cena concreta deve implementar
    // window: Ponteiro para a janela GLFW
    // screenWidth, screenHeight: Dimensões da janela
    // renderer: Referência ao Renderer2D para desenhar
    // keyboard, mouse: Referências aos gerenciadores de input
    virtual bool Init(GLFWwindow* window, float screenWidth, float screenHeight,
        Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse) = 0;

    // Atualiza a lógica da cena
    // deltaTime: Tempo decorrido desde a última atualização
    virtual void Update(float deltaTime) = 0;

    // Renderiza o conteúdo da cena
    virtual void Render() = 0;

    // Encerra a cena e libera seus recursos
    virtual void Shutdown() = 0;
};

#endif // SCENE_HPP
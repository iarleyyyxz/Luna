#ifndef SCENE_HPP
#define SCENE_HPP

#include <GLFW/glfw3.h> // Para GLFWwindow*
#include <glm.hpp>   // Para glm::vec2, etc.

// Forward declarations para evitar inclus�es circulares e depend�ncias desnecess�rias
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

    // M�todos virtuais puros que cada cena concreta deve implementar
    // window: Ponteiro para a janela GLFW
    // screenWidth, screenHeight: Dimens�es da janela
    // renderer: Refer�ncia ao Renderer2D para desenhar
    // keyboard, mouse: Refer�ncias aos gerenciadores de input
    virtual bool Init(GLFWwindow* window, float screenWidth, float screenHeight,
        Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse) = 0;

    // Atualiza a l�gica da cena
    // deltaTime: Tempo decorrido desde a �ltima atualiza��o
    virtual void Update(float deltaTime) = 0;

    // Renderiza o conte�do da cena
    virtual void Render() = 0;

    // Encerra a cena e libera seus recursos
    virtual void Shutdown() = 0;
};

#endif // SCENE_HPP
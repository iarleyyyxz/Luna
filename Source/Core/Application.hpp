#ifndef APPLICATION_H
#define APPLICATION_H

#include <functional>
#include <string>

#include "Source/Events/EventManager.hpp"

class Application
{
public:
    Application();
    ~Application();

    bool Init();
    void Run();
    void Shutdown();

    bool IsRunning() const { return isRunning; }

    // Event delegates
    std::function<void()> OnStep;
    std::function<void()> OnExit;
    std::function<void(const std::string&)> OnNewScene;

    // Acesso ao EventManager
    EventManager& GetEventManager() { return eventManager; }

private:
    bool isRunning;
    EventManager eventManager; // Instância do EventManager dentro da Application
};

#endif // APPLICATION_H
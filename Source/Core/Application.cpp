#include "Application.hpp"

#include <iostream>

Application::Application() : isRunning(false)
{
    std::cout << "Application construída!" << std::endl;
    // A instância de eventManager é automaticamente construída aqui
}

Application::~Application()
{
    std::cout << "Application destruída!" << std::endl;
}

bool Application::Init()
{
    std::cout << "Inicializando a aplicação..." << std::endl;
    isRunning = true;
    // Aqui você colocaria a lógica de inicialização de outros sistemas
    return true;
}

void Application::Run()
{
    std::cout << "Executando o loop principal..." << std::endl;
    while (isRunning)
    {
        // Lógica de cada frame do jogo aqui
        if (OnStep)
        {
            OnStep();
        }

        // Simulação de saída
        static int frameCount = 0;
        frameCount++;
        if (frameCount > 10)
        {
            isRunning = false;
        }
    }
    std::cout << "Saindo do loop principal." << std::endl;
    if (OnExit)
    {
        OnExit();
    }
}

void Application::Shutdown()
{
    std::cout << "Encerrando a aplicação..." << std::endl;
    // Aqui você colocaria a lógica de shutdown de outros sistemas
}
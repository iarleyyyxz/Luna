#include "Application.hpp"

#include <iostream>

Application::Application() : isRunning(false)
{
    std::cout << "Application constru�da!" << std::endl;
    // A inst�ncia de eventManager � automaticamente constru�da aqui
}

Application::~Application()
{
    std::cout << "Application destru�da!" << std::endl;
}

bool Application::Init()
{
    std::cout << "Inicializando a aplica��o..." << std::endl;
    isRunning = true;
    // Aqui voc� colocaria a l�gica de inicializa��o de outros sistemas
    return true;
}

void Application::Run()
{
    std::cout << "Executando o loop principal..." << std::endl;
    while (isRunning)
    {
        // L�gica de cada frame do jogo aqui
        if (OnStep)
        {
            OnStep();
        }

        // Simula��o de sa�da
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
    std::cout << "Encerrando a aplica��o..." << std::endl;
    // Aqui voc� colocaria a l�gica de shutdown de outros sistemas
}
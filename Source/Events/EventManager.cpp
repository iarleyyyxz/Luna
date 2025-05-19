#include "EventManager.hpp"
#include <iostream> // Para demonstra��o

void EventManager::Subscribe(const std::string& eventType, EventCallback callback)
{
    listeners[eventType].push_back(callback);
    std::cout << "Listener inscrito para o evento: " << eventType << std::endl;
}

void EventManager::Unsubscribe(const std::string& eventType, const EventCallback& callback)
{
    auto it = listeners.find(eventType);
    if (it != listeners.end())
    {
        auto& callbacks = it->second;
        for (auto it_callback = callbacks.begin(); it_callback != callbacks.end(); ++it_callback)
        {
            // Note: Comparar std::function diretamente pode n�o funcionar como esperado
            // Dependendo da implementa��o, lambdas podem ser diferentes.
            // Uma forma mais robusta seria exigir um ID para cada subscription.
            if (*it_callback = callback)
            {
                callbacks.erase(it_callback);
                std::cout << "Listener removido do evento: " << eventType << std::endl;
                return;
            }
        }
        std::cout << "Callback n�o encontrado para o evento: " << eventType << std::endl;
    }
    else
    {
        std::cout << "Evento n�o encontrado: " << eventType << std::endl;
    }
}

void EventManager::Dispatch(Event* event)
{
    // Por simplicidade, vamos usar o nome da classe do evento como tipo.
    // Em um sistema mais complexo, voc� poderia ter um m�todo GetType virtual.
    const std::string eventType = typeid(*event).name();

    auto it = listeners.find(eventType);
    if (it != listeners.end())
    {
        for (const auto& callback : it->second)
        {
            callback(event);
        }
        std::cout << "Evento '" << eventType << "' despachado para " << it->second.size() << " listeners." << std::endl;
    }
    else
    {
        std::cout << "Nenhum listener inscrito para o evento: " << eventType << std::endl;
    }
    delete event; // O respons�vel por disparar o evento geralmente aloca e o EventManager notifica.
    // Aqui estamos simplificando a gest�o da mem�ria. Em um sistema real,
    // voc� poderia ter outras estrat�gias (como usar smart pointers ou um pool de eventos).
}
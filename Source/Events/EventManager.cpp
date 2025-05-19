#include "EventManager.hpp"
#include <iostream> // Para demonstração

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
            // Note: Comparar std::function diretamente pode não funcionar como esperado
            // Dependendo da implementação, lambdas podem ser diferentes.
            // Uma forma mais robusta seria exigir um ID para cada subscription.
            if (*it_callback = callback)
            {
                callbacks.erase(it_callback);
                std::cout << "Listener removido do evento: " << eventType << std::endl;
                return;
            }
        }
        std::cout << "Callback não encontrado para o evento: " << eventType << std::endl;
    }
    else
    {
        std::cout << "Evento não encontrado: " << eventType << std::endl;
    }
}

void EventManager::Dispatch(Event* event)
{
    // Por simplicidade, vamos usar o nome da classe do evento como tipo.
    // Em um sistema mais complexo, você poderia ter um método GetType virtual.
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
    delete event; // O responsável por disparar o evento geralmente aloca e o EventManager notifica.
    // Aqui estamos simplificando a gestão da memória. Em um sistema real,
    // você poderia ter outras estratégias (como usar smart pointers ou um pool de eventos).
}
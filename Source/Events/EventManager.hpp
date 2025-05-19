#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <functional>
#include <map>
#include <vector>
#include <iostream>

// Definição base para um evento (pode ser estendida posteriormente com dados específicos)
class Event
{
public:
    virtual ~Event() = default;
};

class EventManager
{
public:
    using EventCallback = std::function<void(Event*)>;

    void Subscribe(const std::string& eventType, EventCallback callback);
    void Unsubscribe(const std::string& eventType, const EventCallback& callback);
    void Dispatch(Event* event);

private:
    std::map<std::string, std::vector<EventCallback>> listeners;
};

#endif // EVENT_MANAGER_H
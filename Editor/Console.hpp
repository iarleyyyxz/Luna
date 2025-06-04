#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <string>
#include <vector>
#include <imgui.h>
#include <memory>
#include "Source/Core/Log.hpp" // Inclua o cabeçalho do sistema de log

// A classe Console gere e exibe mensagens de log dentro da interface ImGui.
class Console {
public:
    Console();
    ~Console() = default;

    // Adiciona uma nova mensagem de log (agora recebe uma LogMessage).
    // @param logMessage A estrutura LogMessage a ser adicionada.
    void AddLog(const Luna::LogMessage& logMessage);

    // Limpa todas as mensagens do console.
    void Clear();

    // Desenha a janela do console usando ImGui.
    void OnGui();

private:
    std::vector<Luna::LogMessage> m_messages; // Armazena mensagens de log estruturadas.
    bool m_scrollToBottom;                   // Flag para rolar automaticamente.
    char m_inputBuffer[256];                 // Buffer para a linha de entrada.

    // Função auxiliar para processar comandos (opcional).
    void ProcessCommand(const std::string& command);

    // Retorna a cor ImVec4 para um determinado nível de log.
    ImVec4 GetColorForLogLevel(Luna::LogLevel level) const;
};

#endif // CONSOLE_HPP
#include "Console.hpp"
#include <iostream>
#include <algorithm>

// Construtor do Console.
Console::Console() : m_scrollToBottom(true) {
    std::fill(m_inputBuffer, m_inputBuffer + sizeof(m_inputBuffer), 0);

    // Adiciona algumas mensagens de boas-vindas usando o novo sistema de log.
    AddLog(Luna::LogMessage(Luna::LogLevel::Info, "Bem-vindo ao Console Luna!"));
    AddLog(Luna::LogMessage(Luna::LogLevel::Info, "Digite 'help' para ver os comandos disponiveis."));
}

// Adiciona uma nova mensagem de log ao console.
void Console::AddLog(const Luna::LogMessage& logMessage) {
    m_messages.push_back(logMessage);
    m_scrollToBottom = true;
}

// Limpa todas as mensagens do console.
void Console::Clear() {
    m_messages.clear();
    m_scrollToBottom = true;
}

// Retorna a cor ImVec4 para um determinado nível de log.
ImVec4 Console::GetColorForLogLevel(Luna::LogLevel level) const {
    switch (level) {
    case Luna::LogLevel::Trace:    return ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Cinza
    case Luna::LogLevel::Debug:    return ImVec4(0.0f, 0.7f, 0.0f, 1.0f); // Verde
    case Luna::LogLevel::Info:     return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Branco
    case Luna::LogLevel::Warning:  return ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Amarelo
    case Luna::LogLevel::Error:    return ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Vermelho
    case Luna::LogLevel::Critical: return ImVec4(1.0f, 0.0f, 1.0f, 1.0f); // Magenta
    default:                       return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Padrão: Branco
    }
}

// Desenha a janela do console usando ImGui.
void Console::OnGui() {
    ImGui::Begin("Console");

    // Botões de controlo
    if (ImGui::Button("Limpar")) {
        Clear();
    }
    ImGui::SameLine();
    if (ImGui::Button("Rolar para Baixo")) {
        m_scrollToBottom = true;
    }
    ImGui::SameLine();
    // Exemplo de como usar o logger para adicionar uma mensagem de teste
    if (ImGui::Button("Teste Log")) {
        static int counter = 0;
        switch (counter % 6) {
        case 0: Luna::GLogger.Trace("Mensagem de Trace de teste " + std::to_string(counter)); break;
        case 1: Luna::GLogger.Debug("Mensagem de Debug de teste " + std::to_string(counter)); break;
        case 2: Luna::GLogger.Info("Mensagem de Info de teste " + std::to_string(counter)); break;
        case 3: Luna::GLogger.Warn("Mensagem de Warning de teste " + std::to_string(counter)); break;
        case 4: Luna::GLogger.Error("Mensagem de Error de teste " + std::to_string(counter)); break;
        case 5: Luna::GLogger.Critical("Mensagem de Critical de teste " + std::to_string(counter)); break;
        }
        counter++;
    }


    ImGui::Separator();

    // Área de texto para as mensagens
    // Adiciona uma borda e padding para um visual mais limpo
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Fundo escuro para a área de mensagens

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2), true, ImGuiWindowFlags_HorizontalScrollbar);

    // Exibe cada mensagem com sua cor correspondente
    for (const auto& logMessage : m_messages) {
        ImGui::PushStyleColor(ImGuiCol_Text, GetColorForLogLevel(logMessage.level));
        ImGui::Text("[%s] [%s] %s", logMessage.timestamp.c_str(),
            (logMessage.level == Luna::LogLevel::Trace ? "TRACE" :
                logMessage.level == Luna::LogLevel::Debug ? "DEBUG" :
                logMessage.level == Luna::LogLevel::Info ? "INFO" :
                logMessage.level == Luna::LogLevel::Warning ? "WARN" :
                logMessage.level == Luna::LogLevel::Error ? "ERROR" : "CRITICAL"),
            logMessage.message.c_str());
        ImGui::PopStyleColor(); // Remove a cor para o próximo texto
    }

    if (m_scrollToBottom && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
        m_scrollToBottom = false;
    }

    ImGui::EndChild();
    ImGui::PopStyleColor(); // Remove a cor de fundo
    ImGui::PopStyleVar(2); // Remove as variáveis de estilo (ChildRounding, ChildBorderSize)

    ImGui::Separator();

    // Linha de entrada de comando
    bool reclaim_focus = false;
    if (ImGui::InputText("Entrada", m_inputBuffer, sizeof(m_inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        std::string command = m_inputBuffer;
        if (!command.empty()) {
            // Loga o comando digitado usando o sistema de log
            Luna::GLogger.Info("> " + command);
            ProcessCommand(command);
            std::fill(m_inputBuffer, m_inputBuffer + sizeof(m_inputBuffer), 0);
        }
        reclaim_focus = true;
    }

    ImGui::SetItemDefaultFocus();
    if (reclaim_focus) {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
}

// Função auxiliar para processar comandos (exemplo simples).
void Console::ProcessCommand(const std::string& command) {
    if (command == "help") {
        Luna::GLogger.Info("Comandos disponiveis:");
        Luna::GLogger.Info("- help: Mostra esta mensagem.");
        Luna::GLogger.Info("- clear: Limpa o console.");
        Luna::GLogger.Info("- echo <texto>: Repete o texto.");
    }
    else if (command.rfind("echo ", 0) == 0) {
        Luna::GLogger.Info(command.substr(5));
    }
    else {
        Luna::GLogger.Warn("Comando desconhecido: '" + command + "'");
    }
}
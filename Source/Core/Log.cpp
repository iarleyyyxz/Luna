#include "Log.hpp"

namespace Luna {

    // Inicializa a inst�ncia global do Logger.
    Logger GLogger;

    // Construtor do Logger.
    Logger::Logger() : m_logCallback(nullptr) {
        // Pode adicionar alguma inicializa��o aqui, se necess�rio.
    }

    // Implementa��o do m�todo principal de log.
    void Logger::Log(LogLevel level, const std::string& message) {
        LogMessage logMsg(level, message);

        // Se um callback de log estiver definido, chame-o.
        if (m_logCallback) {
            m_logCallback(logMsg);
        }

        // Opcional: Imprimir para a sa�da padr�o para depura��o imediata.
        // Em uma aplica��o real, isso seria gerenciado por "sinks" espec�ficos.
        std::cout << "[" << logMsg.timestamp << "] ";
        switch (logMsg.level) {
        case LogLevel::Trace:    std::cout << "[TRACE]   "; break;
        case LogLevel::Debug:    std::cout << "[DEBUG]   "; break;
        case LogLevel::Info:     std::cout << "[INFO]    "; break;
        case LogLevel::Warning:  std::cout << "[WARNING] "; break;
        case LogLevel::Error:    std::cout << "[ERROR]   "; break;
        case LogLevel::Critical: std::cout << "[CRITICAL]"; break;
        }
        std::cout << logMsg.message << std::endl;
    }

    // M�todos de conveni�ncia que chamam o m�todo Log principal.
    void Logger::Trace(const std::string& message) {
        Log(LogLevel::Trace, message);
    }

    void Logger::Debug(const std::string& message) {
        Log(LogLevel::Debug, message);
    }

    void Logger::Info(const std::string& message) {
        Log(LogLevel::Info, message);
    }

    void Logger::Warn(const std::string& message) {
        Log(LogLevel::Warning, message);
    }

    void Logger::Error(const std::string& message) {
        Log(LogLevel::Error, message);
    }

    void Logger::Critical(const std::string& message) {
        Log(LogLevel::Critical, message);
    }

    // Define o callback para o log.
    void Logger::SetLogCallback(std::function<void(const LogMessage&)> callback) {
        m_logCallback = callback;
    }

} // namespace Luna
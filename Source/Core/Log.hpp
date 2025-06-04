#ifndef LUNA_LOG_HPP
#define LUNA_LOG_HPP

#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <functional>
#include <ctime> // Para time_t, tm, localtime_s

namespace Luna {

    enum class LogLevel {
        Trace = 0, Debug, Info, Warning, Error, Critical
    };

    struct LogMessage {
        LogLevel level;
        std::string message;
        std::string timestamp;

        LogMessage(LogLevel lvl, const std::string& msg) : level(lvl), message(msg) {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            // Use localtime_s para segurança
            std::tm buf;
            if (localtime_s(&buf, &in_time_t) == 0) { // localtime_s retorna 0 em sucesso
                std::stringstream ss;
                ss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
                timestamp = ss.str();
            }
            else {
                timestamp = "ERRO_TIMESTAMP"; // Fallback em caso de falha
            }
        }
    };

    class Logger {
    public:
        Logger();
        void Log(LogLevel level, const std::string& message);
        void Trace(const std::string& message);
        void Debug(const std::string& message);
        void Info(const std::string& message);
        void Warn(const std::string& message);
        void Error(const std::string& message);
        void Critical(const std::string& message);
        void SetLogCallback(std::function<void(const LogMessage&)> callback);
    private:
        std::function<void(const LogMessage&)> m_logCallback;
    };

    extern Logger GLogger;

} // namespace Luna

#endif // LUNA_LOG_HPP

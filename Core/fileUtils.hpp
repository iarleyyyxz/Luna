#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Para usar em funções como readJson
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace Core {

    class FileUtils {
    public:
        // Construtor e destrutor padrão
        FileUtils() = default;
        ~FileUtils() = default;

        // Função para ler arquivos de texto
        static std::string readTextFile(const std::string& filePath);

        // Função para ler arquivos binários (como .bmp)
        static std::vector<unsigned char> readBinaryFile(const std::string& filePath);

        // Função para escrever arquivos de texto
        static void writeTextFile(const std::string& filePath, const std::string& content);

        // Função para escrever arquivos binários
        static void writeBinaryFile(const std::string& filePath, const std::vector<unsigned char>& data);

        // Função para ler arquivos JSON
        static rapidjson::Document readJsonFile(const std::string& filePath);

        // Função para escrever arquivos JSON
        static void writeJsonFile(const std::string& filePath, const rapidjson::Document& document);
    };

} // namespace Core

#endif // FILE_UTILS_HPP

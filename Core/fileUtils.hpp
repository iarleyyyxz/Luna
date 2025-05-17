#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Para usar em fun��es como readJson
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace Core {

    class FileUtils {
    public:
        // Construtor e destrutor padr�o
        FileUtils() = default;
        ~FileUtils() = default;

        // Fun��o para ler arquivos de texto
        static std::string readTextFile(const std::string& filePath);

        // Fun��o para ler arquivos bin�rios (como .bmp)
        static std::vector<unsigned char> readBinaryFile(const std::string& filePath);

        // Fun��o para escrever arquivos de texto
        static void writeTextFile(const std::string& filePath, const std::string& content);

        // Fun��o para escrever arquivos bin�rios
        static void writeBinaryFile(const std::string& filePath, const std::vector<unsigned char>& data);

        // Fun��o para ler arquivos JSON
        static rapidjson::Document readJsonFile(const std::string& filePath);

        // Fun��o para escrever arquivos JSON
        static void writeJsonFile(const std::string& filePath, const rapidjson::Document& document);
    };

} // namespace Core

#endif // FILE_UTILS_HPP

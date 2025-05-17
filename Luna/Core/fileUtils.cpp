#include "fileUtils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip> // Para formatação de saída

namespace Core {

    std::string FileUtils::readTextFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Não foi possível abrir o arquivo de texto: " << filePath << std::endl;
            return ""; // Retorna uma string vazia em caso de erro
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::vector<unsigned char> FileUtils::readBinaryFile(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Não foi possível abrir o arquivo binário: " << filePath << std::endl;
            return {}; // Retorna um vetor vazio em caso de erro
        }
        // Obtém o tamanho do arquivo
        file.seekg(0, std::ios::end);
        std::size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> buffer(fileSize);
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
        return buffer;
    }

    void FileUtils::writeTextFile(const std::string& filePath, const std::string& content) {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Não foi possível criar/abrir o arquivo de texto para escrita: " << filePath << std::endl;
            return;
        }
        file << content;
    }

    void FileUtils::writeBinaryFile(const std::string& filePath, const std::vector<unsigned char>& data) {
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Não foi possível criar/abrir o arquivo binário para escrita: " << filePath << std::endl;
            return;
        }
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
    }

    rapidjson::Document FileUtils::readJsonFile(const std::string& filePath) {
        std::string jsonString = readTextFile(filePath);
        rapidjson::Document document;
        document.Parse(jsonString.c_str());
        if (document.HasParseError()) {
            std::cerr << "Erro ao解析ar o arquivo JSON: " << filePath << std::endl;
            std::cerr << "Erro: " << document.GetParseError() << " Offset: " << document.GetErrorOffset() << std::endl;
            // Cria um documento vazio para evitar problemas
            document.SetObject();
        }
        return document;
    }

   // void FileUtils::writeJsonFile(const std::string& filePath, const rapidjson::Document& document) {
     //   rapidjson::StringBuffer buffer;
     //   rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
     //   document.Accept(writer);
      //  std::string jsonString = buffer.GetString();
      //  writeTextFile(filePath, jsonString);
   // }

} // namespace Core

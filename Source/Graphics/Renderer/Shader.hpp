#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector> // Para std::vector<int>

#include <GL/glew.h>
#include <glm.hpp> // Para glm::mat4

class Shader {
public:
    // O ID do programa shader
    GLuint ID;

    // Construtor que lê e constrói o shader
    Shader(const char* vertexPath, const char* fragmentPath);

    // Usa o programa shader
    void use();
    // Desativa o programa shader
    void unuse();

    // Funções utilitárias para uniformes
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    // Para arrays de inteiros (como samplers de textura)
    void setInt(const std::string& name, GLsizei count, const GLint* value) const;


private:
    // Checa erros de compilação/linkagem do shader
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif // SHADER_HPP

// shader.hpp
#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>        // Adicione esta linha
#include <gtc/type_ptr.hpp> // Adicione esta linha
#include <GL/glew.h>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    unsigned int ID;

    std::string readFile(const char* path) const;
    unsigned int compileShader(unsigned int type, const char* source) const;
};

#endif // SHADER_HPP
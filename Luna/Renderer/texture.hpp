#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <GL/glew.h>

class Texture {
public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& path);
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    unsigned int getID() const { return m_textureID; }

private:
    unsigned int m_textureID;
    int m_width;
    int m_height;
    int m_bpp; // Bits per pixel
};

#endif // TEXTURE_HPP
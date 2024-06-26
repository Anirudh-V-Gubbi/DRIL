#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>
#include "../Logger/logger.h"

enum class TextureFormats
{
    JPG,
    PNG
};

class Texture
{
public:
    GLuint ID = 0;

    Texture(std::string imagePath, TextureFormats format, bool persistentData = false)
    {
        m_textureName = imagePath.substr(imagePath.find('.'));
        this->generateTexture(getTextureFullPath(imagePath).c_str(), format);

        // delete image data and image specs if not required to be persistent
        // ------------------------------------------------------------------
        if (!persistentData)
        {
            if (specs.data)
            {
                stbi_image_free(specs.data);
                specs.data = nullptr;
            }
        }
    }
    // empty texture
    // -------------
    Texture()
    {
        specs.data = NULL;
    }
    ~Texture() {}

    void Bind() const
    {
        stbi_set_flip_vertically_on_load(true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    int GetTexWidth() const
    {
        return specs.width;
    }

    int GetTexHeight() const
    {
        return specs.height;
    }

    bool DeleteImageData()
    {
        if (specs.data != nullptr)
        {
            delete specs.data;
            specs.data = nullptr;
            return true;
        }

        return false;
    }

private:
    struct ImageSpecs
    {
        int width;
        int height;
        int nrChannels;
        unsigned char *data;

        ImageSpecs() : width{0}, height{0}, nrChannels{0}, data{nullptr} {}
    };
    ImageSpecs specs;
    std::string m_textureName;

    void generateTexture(std::string path, TextureFormats format)
    {
        specs.data = stbi_load(path.c_str(), &specs.width, &specs.height, &specs.nrChannels, format == TextureFormats::PNG ? STBI_rgb_alpha : STBI_default);
        // std::cout << specs.data << std::endl;
        if (specs.data)
        {
            glGenTextures(1, &this->ID);
            glBindTexture(GL_TEXTURE_2D, this->ID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, format == TextureFormats::PNG ? GL_RGBA : GL_RGB, specs.width, specs.height, 0, format == TextureFormats::PNG ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, specs.data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << m_textureName;
            // Logger::GetInstance()->error("Failed to load texture", m_textureName);
        }
    }

    std::string getTextureFullPath(std::string path)
    {
        return std::string(RESOURCE_PATH) + "textures/" + path;
    }
};

// empty texture
// -------------
class EmptyTexture : public Texture
{
public:
    ~EmptyTexture() {}

    static EmptyTexture *GetInstance()
    {
        if (s_instance == NULL)
        {
            s_instance = new EmptyTexture();
        }

        return s_instance;
    }

private:
    inline static EmptyTexture *s_instance = NULL;
    EmptyTexture() {}
};
#endif

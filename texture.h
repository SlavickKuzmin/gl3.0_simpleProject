//
// Created by slavickkuzmin on 8/17/16.
//

#ifndef GL3_0_TEXTURE_H
#define GL3_0_TEXTURE_H


#include <string>
#include <GL/glew.h>
#include <ImageMagick-6/Magick++.h>

class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);

    bool Load();

    void Bind(GLenum TextureUnit, GLuint &programID);
    GLuint m_textureObj;
private:
    std::string m_fileName;
    GLenum m_textureTarget;

    Magick::Image m_image;
    Magick::Blob m_blob;

    GLuint TextureID;
};


#endif //GL3_0_TEXTURE_H

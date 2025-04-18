#include <stb_image.h>
#include <iostream>

#include <AssetManager/CubeMap.hpp>
#include <Common/Logger.hpp>
#include <Utils/AssetUtils.hpp>

namespace shkyera {

CubeMap::CubeMap(GLenum minFilter, GLenum magFilter, GLenum wrapS, GLenum wrapT, GLenum wrapR) : _textureID(0) {
  initialize(minFilter, magFilter, wrapS, wrapT, wrapR);
}

void CubeMap::initialize(GLenum minFilter, GLenum magFilter, GLenum wrapS, GLenum wrapT, GLenum wrapR) {
  if (_textureID) {
    glDeleteTextures(1, &_textureID);
  }

  glGenTextures(1, &_textureID);

  bind();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
  unbind();
}

CubeMap::CubeMap(CubeMap&& other) noexcept : _textureID(other._textureID) {
  other._textureID = 0;
}

CubeMap& CubeMap::operator=(CubeMap&& other) noexcept {
  if (this != &other) {
    if (_textureID != 0) {
      glDeleteTextures(1, &_textureID);
    }

    _textureID = other._textureID;
    other._textureID = 0;
  }
  return *this;
}

CubeMap::~CubeMap() {
  if (_textureID) {
    glDeleteTextures(1, &_textureID);
  }
}

void CubeMap::bind() const {
  glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
}

void CubeMap::unbind() const {
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

bool CubeMap::loadFaces(Registry* registry, HandleAndAsset<Image> up, HandleAndAsset<Image> down,
                        HandleAndAsset<Image> left, HandleAndAsset<Image> right, HandleAndAsset<Image> front,
                        HandleAndAsset<Image> back, GLenum internalFormat, GLenum format, GLenum type) {
  std::vector faces = {left, right, up, down, back, front};

  bind();
  for (size_t i = 0; i < 6; i++) {
    auto f = utils::assets::read<Image>(registry, faces[i]);
    auto width = f->getWidth();
    auto height = f->getHeight();
    auto channels = f->getChannels();
    auto* data = f->getData();
    if (data) {
      if (channels == 3)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      else
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
      Logger::ERROR("Image was not loaded. Could not load cubemap");
      return false;
    }
  }
  unbind();
  return true;
}

void CubeMap::setFaceData(GLenum face, GLenum internalFormat, uint32_t width, uint32_t height, GLenum format,
                          GLenum type, const void* data) {
  if (face < GL_TEXTURE_CUBE_MAP_POSITIVE_X || face > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) {
    Logger::ERROR("Invalid Cube Map Face enum");
    return;
  }

  bind();
  glTexImage2D(face, 0, internalFormat, width, height, 0, format, type, data);
  unbind();
}

void CubeMap::activate(GLenum textureUnit) const {
  glActiveTexture(textureUnit);
  bind();
}

}  // namespace shkyera

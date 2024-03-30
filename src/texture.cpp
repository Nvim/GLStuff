#include <iostream>
#include <texture.hpp>
#include <stb_image/stb_image.h>

Texture::Texture(const char *file, const char *textureType, GLenum slot) {
  stbi_set_flip_vertically_on_load(true);

  this->slot = slot;
  this->type = textureType;
  this->path = file;

  int width, height, nrChannels;
  unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
  if (!data) {
    std::cerr << "Failed to load texture" << std::endl;
    return;
  }

  glGenTextures(1, &ID);
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (nrChannels == 3) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  } else if (nrChannels == 4) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
  } else {
    std::cerr << "Failed to load texture: invalid number of channels"
              << std::endl;
    return;
  }
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0); // unbind for safety
}

void Texture::TexUnit(Shader &shader, const char *uniform, unsigned int unit) {
  unsigned int textUni = glGetUniformLocation(shader.ID, uniform);
  shader.use();
  glUniform1i(textUni, unit);
}

void Texture::Bind() {
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture::Delete() { glDeleteTextures(1, &ID); }

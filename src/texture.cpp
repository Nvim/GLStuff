#include <iostream>
#include <texture.hpp>

Texture::Texture(const char *file, GLenum textureType, GLenum slot,
                 GLenum format, GLenum pixel_type) {
  stbi_set_flip_vertically_on_load(true);

  this->type = textureType;
  this->slot = slot;

  int width, height, nrChannels;
  unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
  if (!data) {
    std::cerr << "Failed to load texture" << std::endl;
    return;
  }

  glGenTextures(1, &ID);
  glActiveTexture(slot);
  glBindTexture(type, ID);

  glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(type, 0, GL_RGB, width, height, 0, format, pixel_type, data);
  glGenerateMipmap(type);

  stbi_image_free(data);
  glBindTexture(type, 0); // unbind for safety
}

void Texture::TexUnit(Shader &shader, const char *uniform, unsigned int unit) {
  unsigned int textUni = glGetUniformLocation(shader.ID, uniform);
  shader.use();
  glUniform1i(textUni, unit);
}

void Texture::Bind() {
  glActiveTexture(slot);
  glBindTexture(type, ID);
}

void Texture::Unbind() { glBindTexture(type, 0); }

void Texture::Delete() { glDeleteTextures(1, &ID); }

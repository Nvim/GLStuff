#include <vao.hpp>

VAO::VAO() { glGenVertexArrays(1, &ID); }

void VAO::Bind() { glBindVertexArray(ID); }

void VAO::Unbind() { glBindVertexArray(0); }

void VAO::Delete() { glDeleteVertexArrays(1, &ID); }

void VAO::LinkVBO(VBO &VBO, unsigned int layout, unsigned int numComponents,
                  GLenum type, size_t stride, void *offset) {
  VBO.Bind();
  glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  VBO.Unbind();
}

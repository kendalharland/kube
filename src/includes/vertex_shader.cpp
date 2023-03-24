// Kube - A puzzle video game.
// Copyright (C) 2021 Kendal Harland GPL-3.0-only
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef _VERTEX_SHADER_HPP
#define _VERTEX_SHADER_HPP

#include <shader.h>

#include <glm/glm.hpp>

namespace kube {

class ModelShader {
 public:
  // TODO: The shader files themselves should be the arguments?
  ModelShader()
      : _matrixID(0), _programID(0), _vertexBuffer(0), _colorBuffer(0) {
    _programID = LoadShaders("src/shaders/TransformVertexShader.vertexshader",
                             "src/shaders/ColorFragmentShader.fragmentshader");
    _matrixID = glGetUniformLocation(_programID, "MVP");
    glGenBuffers(1, &_vertexBuffer);
    glGenBuffers(1, &_colorBuffer);
  }

  ~ModelShader() {
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteBuffers(1, &_colorBuffer);
    glDeleteProgram(_programID);
  }

  // TODO: Model and camera objects would cut down on the number of params,
  // here.
  void Draw(glm::mat4 mvp, const GLfloat *vertices, int numVertices,
            const GLfloat *colors, int numColors, const GLubyte *indices,
            int numIndices) {
    glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &mvp[0][0]);
    glUseProgram(_programID);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // 1st attribute buffer : vertices.
    glEnableVertexAttribArray(_vertexArray);  // vertices.
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(_vertexArray, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // 2nd attribute buffer : colors.
    glEnableVertexAttribArray(_colorArray);  // colors.
    glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, numColors, colors, GL_STATIC_DRAW);
    glVertexAttribPointer(_colorArray, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_BYTE, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glDisableVertexAttribArray(_vertexArray);
    glDisableVertexAttribArray(_colorArray);
  }

 private:
  GLuint _programID;
  GLuint _matrixID;
  GLuint _vertexBuffer;
  GLuint _colorBuffer;

  static const GLuint _vertexArray = 0;
  static const GLuint _colorArray = 1;
};

};  // namespace kube

#endif  // _VERTEX_SHADER_HPP
#include <model.hpp>
#include <window.h>

namespace kube {

// TODO: Delete.
const glm::vec3 Model::Center() { return center_; }
const glm::mat4 Model::Rotation() { return rotation_; }
const glm::mat4 Model::Scale() { return scale_; }

void Model::Draw(Window *window) {
  glm::mat4 translation = glm::translate(IDENTITY_MAT4, Center());
  glm::mat4 rotation = Rotation();
  glm::mat4 scale = Scale();
  glm::mat4 mvp = window->GetCamera().MVP(translation * rotation * scale);
  mesh_.Draw(shader_, mvp);
}

void Model::SetRotation(float radians, glm::vec3 axis) {
  rotation_ = glm::rotate(IDENTITY_MAT4, radians, axis);
}

void Model::RotateRight(float t) {
  auto angle = glm::radians(ROTATIONS_PER_SEC) * t;
  rotation_ = glm::rotate(rotation_, angle, -Y_AXIS);
}

void Model::RotateLeft(float t) {
  auto angle = glm::radians(ROTATIONS_PER_SEC) * t;
  rotation_ = glm::rotate(rotation_, angle, Y_AXIS);
}

}; // namespace kube

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "tiny_gltf.h"

#include "Window.h"
#include "Shader.h"
#include "Mesh.h"
#include "GLTFUtils.h"
#include "AnimationUtils.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>

int main() {
  Window window(1920, 1080, "Skeletal Shenanigans");
  InputHandler::initialize(window);

  Camera camera;

  Shader shader("resources/shaders/Model.vert", "resources/shaders/Model.frag");

  Model mdl("resources/models/test.gltf");

  std::size_t animationIndex = 0;

  while (!window.shouldClose()) {
    window.update();
    camera.update(window, window.getDeltaTime());

    if (InputHandler::isKeyPressed(Key::UP)) {
      mdl.setAnimationIndex(++animationIndex);
    } else if (InputHandler::isKeyPressed(Key::DOWN)) {
      mdl.setAnimationIndex(--animationIndex);
    }

    mdl.update(window.getDeltaTime());

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), static_cast<float>(window.getWidth()) / window.getHeight(), 0.01f, 1000.0f);

    glm::mat4 model = glm::scale(glm::mat4(1.0f), { 0.01f, 0.01f, 0.01f } );
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, -5.0f));

    shader.use();
    shader.setMat4("u_model", model);
    shader.setMat4("u_view", view);
    shader.setMat4("u_projection", projection);

    mdl.draw(shader);
  }

  return 0;
}
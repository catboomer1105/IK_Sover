#pragma once

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include "camera.h"
#include "model.h"
#include "shader.h"

class EndEffector
{
  public:
    glm::vec3 position;
    float pitch;
    float yaw;
    glm::vec3 scale;

    EndEffector(int x, int y, int z) : objectShader(vertexShaderPath.c_str(), fragShaderPath.c_str())
    {
        position = glm::vec3(x, y, z) + 0.0001f;
        scale = glm::vec3(.05f, .05f, .05f);
        pitch = 0.0f;
        yaw = 0.0f;
    }

    void Render(glm::mat4 view, glm::mat4 proj)
    {
        objectShader.use();

        // Calculate the toWorld matrix for the model
        glm::mat4 model;
        glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
        glm::mat4 R = glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1, 0, 0));
        R = glm::rotate(R, yaw, glm::vec3(0, 0, 1));
        model = T * R * S;

        objectShader.setMat4("model", model);
        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", proj);

        objectModel.Draw(objectShader);
    }

    void ProcessTranslation(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = 2.0f * deltaTime;
        if (direction == UP)
            this->position.y += 1.0f * velocity;
        if (direction == DOWN)
            this->position.y -= 1.0f * velocity;
        if (direction == LEFT)
            this->position.x -= 1.0f * velocity;
        if (direction == RIGHT)
            this->position.x += 1.0f * velocity;
        if (direction == FORWARD)
            this->position.z += 1.0f * velocity;
        if (direction == BACKWARD)
            this->position.z -= 1.0f * velocity;
    }

  private:
    Model objectModel = Model("resources/sphere.off");
    std::string vertexShaderPath = "shader/sphere.vs";
    std::string fragShaderPath = "shader/sphere.fs";
    Shader objectShader;
};
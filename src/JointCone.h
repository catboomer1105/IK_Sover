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

#include "model.h"
#include "shader.h"

class JointCone
{
  public:
    JointCone(glm::vec3 position, glm::vec3 end_position, float magnitude, glm::quat dir)
        : coneShader(VertexShaderPath.c_str(), FragmentShaderPath.c_str())
    {
        Set(position, end_position, magnitude, dir);
    }

    void Set(glm::vec3 position, glm::vec3 end_position, float magnitude, glm::quat dir)
    {
        this->position = position;
        this->end_position = end_position;
        this->magnitude = magnitude;
        this->quat = dir;
    }

    void Render(glm::mat4 view, glm::mat4 proj)
    {
        coneShader.use();

        coneShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        coneShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        coneShader.setVec3("lightPos", 2.0f, 0.0f, 4.0f);
        coneShader.setVec3("viewPos", 0.0f, 0.0f, 3.0f);

        glm::mat4 model;
        glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 PT = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.5));
        glm::mat4 PS = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.2f, 0.05f));
        glm::mat4 PR = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
        glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, magnitude));
        glm::mat4 R = glm::toMat4(quat);

        model = T * R * S * PT * PR* PS;
        coneShader.setMat4("model", model);
        coneShader.setMat4("view", view);
        coneShader.setMat4("projection", proj);

        coneModel.Draw(coneShader);
    }

    glm::vec3 position;
    glm::vec3 end_position;
    glm::quat quat;
    float magnitude;

  private:
    Model coneModel = Model("resources/cone.obj");
    std::string VertexShaderPath = "shader/shader.vs";
    std::string FragmentShaderPath = "shader/shader.fs";
    Shader coneShader;
};

#pragma once

#include "EndEffector.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

#include "EndEffector.h"
#include "JointCone.h"

class Chain
{
  public:
    Chain(std::vector<glm::vec3> joints, EndEffector *t)
    {
        vector<float> lengths;
        vector<glm::quat> directions;
        CalculateLinks(joints, lengths, directions);

        for (int i = 0; i < lengths.size(); ++i)
        {
            cones.push_back(JointCone(joints[i], joints[i + 1], lengths[i], directions[i]));
            total_length += lengths[i];
        }

        target = t;
        size = joints.size();
        this->joints = joints;
    }

    // Fill the lengths and directions vectors via joint positions
    void CalculateLinks(vector<glm::vec3> joints, vector<float> &lengths, vector<glm::quat> &directions)
    {
        origin = *joints.begin();
        end = *(joints.end() - 1);

        for (auto it = joints.begin(); it != joints.end() - 1; ++it)
        {

            glm::vec3 current = *it;
            glm::vec3 next = *(it + 1);

            glm::vec3 link_vector = next - current;

            // Get magnitude of link
            lengths.push_back(glm::length(link_vector));

            // Get rotation of link
            glm::vec3 cross = glm::cross(ref_rot_vector, link_vector);
            glm::quat q;
            q.w = sqrt(pow(glm::length(link_vector), 2) * pow(glm::length(ref_rot_vector), 2)) +
                  glm::dot(link_vector, ref_rot_vector);
            q.x = cross.x;
            q.y = cross.y;
            q.z = cross.z;

            directions.push_back(glm::normalize(q));
        }
    }

    void Solve()
    {
        float current_distance = glm::length(target->position - origin);

        // Target is out of reach; fully extend the arm
        if (current_distance > total_length)
        {
            for (int i = 0; i < joints.size() - 1; ++i)
            {
                float r = glm::length(target->position - joints[i]);
                float l = cones[i].magnitude / r;
                joints[i + 1] = (1 - l) * joints[i] + l * target->position;
            }
        }
        else
        { // Target is in reach, use FABRIK to solve both back and forwards
            int count = 0;
            float difference = glm::length(joints[joints.size() - 1] - target->position);
            // while(difference > tolerance || count < 1) {
            while (difference > tolerance)
            {
                Backward();
                Forward();
                // std::cout << count << std::endl;
                difference = glm::length(joints[joints.size() - 1] - target->position);
                count++;

                if (count > 10)
                    break;
            }
        }

        SetCones();
    }

    void SetCones()
    {
        vector<float> lengths;
        vector<glm::quat> directions;
        CalculateLinks(joints, lengths, directions);

        for (int i = 0; i < lengths.size(); ++i)
        {
            cones[i].Set(joints[i], joints[i + 1], lengths[i], directions[i]);
        }
    }

    void Backward()
    {
        auto end = joints.end() - 1;
        *end = target->position;
        for (int i = int(joints.size() - 2); i >= 0; --i)
        {
            float r = glm::length(joints[i + 1] - joints[i]);
            float l = cones[i].magnitude / r;
            joints[i] = (1 - l) * joints[i + 1] + l * joints[i];
        }
    }

    void Forward()
    {
        auto beg = joints.begin();
        *beg = origin;
        for (int i = 0; i < joints.size() - 1; ++i)
        {
            float r = glm::length(joints[i + 1] - joints[i]);
            float l = cones[i].magnitude / r;

            glm::vec3 new_point = (1 - l) * joints[i] + l * joints[i + 1];
            joints[i + 1] = new_point;
        }
    }

    void Render(glm::mat4 view, glm::mat4 proj)
    {
        for (auto it = cones.begin(); it != cones.end(); ++it)
        {
            it->Render(view, proj);
        }
    }

    unsigned long size;
    float total_length;
    glm::vec3 origin;
    glm::vec3 end;
    EndEffector *target;

  private:
    std::vector<glm::vec3> joints;
    std::vector<JointCone> cones; // The cones that connect the joints
    float tolerance = 0.01f;
    const glm::vec3 ref_rot_vector = glm::vec3(0, 0, -1.0f);
};
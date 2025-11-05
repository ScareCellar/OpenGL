#pragma once
#include "GUI/GUI.h"
#include "Core/Serializable.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace neu {

    struct Transform : public ISerializable, GUI {
        glm::vec3 position{ 0, 0, 0 };
        glm::quat rotation{ glm::identity<glm::quat>() };
        glm::vec3 scale{ 1, 1, 1 };

        Transform() = default;
        Transform(const glm::vec3& position, const glm::vec3& rotation = { 0, 0, 0 }, const glm::vec3& scale = { 1, 1, 1 }) :
            position{ position },
            scale{ scale }
        {
            SetRotation(rotation);
        }

        void SetRotation(const glm::vec3& rotation) {
            rotation = glm::quat(glm::radians(rotation));
        }

        void SetRotationQuat(const glm::quat& rotationq) {
            this->rotation = rotationq;
        }

        glm::vec3 GetRotation() const {
            return glm::degrees(glm::eulerAngles(rotation));
        }

        glm::mat4 GetMatrix() const {
            return glm::translate(glm::mat4(1.0f), position) *
                glm::toMat4(rotation) *
                glm::scale(glm::mat4(1.0f), scale);
        }

        operator glm::mat4() const {
            return GetMatrix();
        }

        glm::vec3 Forward() const { return rotation * glm::vec3{ 0, 0, -1 }; }
        glm::vec3 Up() const { return rotation * glm::vec3{ 0, 1, 0 }; }
        glm::vec3 Right() const { return rotation * glm::vec3{ 1, 0, 0 }; }


        void Read(const serial_data_t& value) override;
        void UpdateGUI() override;
    };
}
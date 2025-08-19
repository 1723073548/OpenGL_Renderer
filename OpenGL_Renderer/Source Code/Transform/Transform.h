#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    Transform();

private: // Private-Fields
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f); // 欧拉角（X/Y/Z轴旋转）
    glm::vec3 m_scale = glm::vec3(1.0f);

    mutable bool m_dirty = true;              // 标记矩阵是否需要重新计算
    mutable glm::mat4 m_modelMatrix = glm::mat4(1.0f); // 缓存的计算结果

public: 
    // 设置变换参数
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& eulerAngles); // 欧拉角（弧度制）
    void SetScale(const glm::vec3& scale);

    // 获取变换参数
    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::vec3 GetScale() const;

    // 获取模型矩阵（用于渲染）
    glm::mat4 GetModelMatrix() const;
};
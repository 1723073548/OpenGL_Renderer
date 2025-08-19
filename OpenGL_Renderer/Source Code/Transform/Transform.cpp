#include "Transform.h"

Transform::Transform(){
    m_modelMatrix = glm::mat4(1.0f);
}

glm::mat4 Transform::GetModelMatrix() const {
    if (m_dirty) {
        // 计算新的模型矩阵
        m_modelMatrix = glm::mat4(1.0f);
        m_modelMatrix = glm::translate(m_modelMatrix, m_position);
        m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
        m_dirty = false;
    }
    return m_modelMatrix;
}

void Transform::SetPosition(const glm::vec3& position) {
    m_position = position;
    m_dirty = true;
}

void Transform::SetRotation(const glm::vec3& eulerAngles) {
    m_rotation = eulerAngles;
	m_dirty = true;
}

void Transform::SetScale(const glm::vec3& scale) {
    m_scale = scale;
    m_dirty = true;
}

glm::vec3 Transform::GetPosition() const {
    return m_position;
}

glm::vec3 Transform::GetRotation() const {
    return m_rotation;
}

glm::vec3 Transform::GetScale() const {
    return m_scale;
}

#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: Position(position), Rotation(rotation), Scale(scale)
{
}

glm::mat4 Transform::GetModelMatrix()
{
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));
	return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
}

#include "Camera.h"

Ref<Frustum> Camera::GetFrustum()
{
	return Ref<Frustum>();
}

Ray Camera::ScreenPointToRay(glm::vec2 position)
{
	glm::mat4 inversedViewProjection = glm::inverse(GetViewProjectionMatrix());
	glm::vec2 halfScreenSize = GetAspectRatio() / 2.0f;

	glm::vec4 near = glm::vec4(
		(position.x - halfScreenSize.x) / halfScreenSize.x,
		-1.0f * (position.y - halfScreenSize.y) / halfScreenSize.y,
		-1.0f,
		 1.0f
	);

	glm::vec4 far = glm::vec4(
		(position.x - halfScreenSize.x) / halfScreenSize.x,
		-1.0f * (position.y - halfScreenSize.y) / halfScreenSize.y,
		 1.0f,
		 1.0f
	);

	glm::vec4 nearResult = inversedViewProjection * near;
	glm::vec4 farResult = inversedViewProjection * far;

	nearResult /= nearResult.w;
	farResult /= farResult.w;

	glm::vec3 direction = glm::vec3(farResult - nearResult);

	return Ray(GetWorldPosition(), glm::normalize(direction));
}

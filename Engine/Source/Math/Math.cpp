#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

// From matrix_decompose.inl
bool Math::DecomposeMatrix(const glm::mat4 modelMatrix, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
{
	using namespace glm;
	using T = float;

	mat4 LocalMatrix(modelMatrix);

	// Normalize the matrix.
	if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
		return false;

	// First, isolate perspective.  This is the messiest.
	if (
		epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
		epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
		epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
	{
		// Clear the perspective partition
		LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
		LocalMatrix[3][3] = static_cast<T>(1);
	}

	// Next take care of translation (easy).
	translation = vec3(LocalMatrix[3]);
	LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

	vec3 Row[3], Pdum3;

	// Now get scale and shear.
	for (length_t i = 0; i < 3; ++i)
		for (length_t j = 0; j < 3; ++j)
			Row[i][j] = LocalMatrix[i][j];

	// Compute X scale factor and normalize first row.
	scale.x = length(Row[0]);// v3Length(Row[0]);
	Row[0] = detail::scale(Row[0], static_cast<T>(1));

	// Now, compute Y scale and normalize 2nd row.
	scale.y = length(Row[1]);
	Row[1] = detail::scale(Row[1], static_cast<T>(1));

	// Next, get Z scale and normalize 3rd row.
	scale.z = length(Row[2]);
	Row[2] = detail::scale(Row[2], static_cast<T>(1));

#if 0
	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
	Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
	if (dot(Row[0], Pdum3) < 0)
	{
		for (length_t i = 0; i < 3; i++)
		{
			scale[i] *= static_cast<T>(-1);
			Row[i] *= static_cast<T>(-1);
		}
	}
#endif

	 rotation.y = asin(-Row[0][2]);
	 if (cos(rotation.y) != 0) {
	     rotation.x = atan2(Row[1][2], Row[2][2]);
	     rotation.z = atan2(Row[0][1], Row[0][0]);
	 } else {
	     rotation.x = atan2(-Row[2][0], Row[1][1]);
	     rotation.z = 0;
	 }

	return true;
}

// Implementation based on: https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
glm::vec3 Math::Transform(const glm::vec3& v, const glm::quat& q)
{
	glm::vec3 u = { q.x, q.y, q.z };
	float s = q.w;

	return 2.0f * glm::dot(u, v) * u
		+ (s * s - glm::dot(u, u)) * v
		+ 2.0f * s * glm::cross(u, v);
}

glm::vec3 Math::Direction(const glm::vec3& a, const glm::vec3& b)
{
	return glm::vec3(b.x - a.x, b.y - a.y, b.z - a.z);
}

float Math::Lerp(float a, float b, float alpha)
{
	return a + alpha * (b - a);
}

glm::vec3 Math::Lerp(glm::vec3 a, glm::vec3 b, float alpha)
{
	return a + alpha * (b - a);
}

glm::vec4 Math::Lerp(glm::vec4 a, glm::vec4 b, float alpha)
{
	return a + alpha * (b - a);
}

float Math::Smoothstep(float a, float b, float alpha)
{
	return a + alpha * alpha * (3.0f - alpha * 2.0f) * (b - a);
}

float Math::Magnitude(const glm::vec3& v)
{
	return glm::sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float Math::Distance(const glm::vec3& a, const glm::vec3& b)
{
	return glm::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z));
}

bool Math::IsNearlyEqual(float a, float b, float errorTolerance)
{
	float c = glm::abs(a - b);
	return c < errorTolerance;
}

bool Math::IsNearlyEqual(const glm::vec3& a, const glm::vec3& b, float errorTolerance)
{
	glm::vec3 c = glm::abs(a - b);
	return c.x < errorTolerance 
		&& c.y < errorTolerance 
		&& c.z < errorTolerance;
}

glm::vec3 Math::Normalize(const glm::vec3& v)
{
	return v / Magnitude(v);
}

glm::vec2 Math::RandomGradient(int ix, int iy)
{
	uint32_t w = 8 * sizeof(uint32_t);
	uint32_t s = w / 2;

	uint32_t a = ix;
	uint32_t b = iy;

	a *= 3284157443;
	b ^= a << s | a >> w - s;
	b *= 1911520717;
	a ^= b << s | b >> w - s;
	a *= 2048419325;

	float random = a * (glm::pi<float>() / ~(~0u >> 1));

	glm::vec2 result;
	result.x = glm::cos(random);
	result.y = glm::sin(random);

	return result;
}

float Math::DotGridGradient(int ix, int iy, float x, float y)
{
	glm::vec2 gradient = RandomGradient(ix, iy);

	float dx = x - (float)ix;
	float dy = y - (float)iy;

	return dx * gradient.x + dy * gradient.y;
}

float Math::PerlinNoise(int x, int y)
{
	int x0 = (int)glm::floor(x);
	int x1 = x0 + 1;
	int y0 = (int)glm::floor(y);
	int y1 = y0 + 1;

	float sx = x - (float)x0;
	float sy = y - (float)y0;

	float n0, n1, ix0, ix1;
	
	n0 = DotGridGradient(x0, y0, x, y);
	n1 = DotGridGradient(x1, y0, x, y);
	ix0 = Smoothstep(n0, n1, sx);

	n0 = DotGridGradient(x0, y1, x, y);
	n1 = DotGridGradient(x1, y1, x, y);
	ix1 = Smoothstep(n0, n1, sx);

	return Smoothstep(ix0, ix1, sy);
}

std::vector<glm::vec3> Math::WorleyPoints(int cellsPerAxis, uint64_t seed)
{
	srand(seed);

	std::vector<glm::vec3> points;
	float cellSize = 1.0f / cellsPerAxis;

	for (int x = 0; x < cellsPerAxis; ++x)
	{
		for (int y = 0; y < cellsPerAxis; ++y)
		{
			for (int z = 0; z < cellsPerAxis; ++z)
			{
				glm::vec3 offset = glm::vec3(rand() & 0xff);
				glm::vec3 pos = (glm::vec3(x, y, z) + offset) * cellSize;
				
				points.push_back(pos);
			}
		}
	}

	return points;
}

float Math::WorleyNoise(std::vector<glm::vec3> points, glm::vec3 position)
{

	return 0.0f;
}

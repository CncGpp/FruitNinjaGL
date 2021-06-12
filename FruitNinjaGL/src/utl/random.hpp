#pragma once
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

/** @brief Utility per i numeri casuali. */
namespace fn::rand {

	template <int N = 1>
	static constexpr inline decltype(auto) uniform(const float a = 0.0f, const float b = 1.0f)
	{
		static_assert(N >= 1 && N <= 4);
		if constexpr (N == 1) 
			return glm::linearRand(a, b);
		else if constexpr (N == 2)
			return glm::vec2(glm::linearRand(a, b), glm::linearRand(a, b));
		else if constexpr (N == 3)
			return glm::vec3(glm::linearRand(a, b), glm::linearRand(a, b), glm::linearRand(a, b));
		else if constexpr (N == 4)
			return glm::vec4(glm::linearRand(a, b), glm::linearRand(a, b), glm::linearRand(a, b), glm::linearRand(a, b));
		else
			return N;
	}
}
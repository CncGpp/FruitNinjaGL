#pragma once
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/color.h>

#include"utl/print.hpp"
#include"utl/log.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "glm/gtx/string_cast.hpp"

#include <chrono>
#include <memory>
#include <filesystem>
#include <random>

#include <iostream>
namespace fs = std::filesystem;
template<typename T> using sp = std::shared_ptr<T>;
template<typename T> using up = std::unique_ptr<T>;

#include<fn/core/input/Mouse.hpp>
#include"ecs/ecs.hpp"
#include"fn/engine/Engine.hpp"

/**
 * @brief Root namespace.
 */
namespace fn {
	extern up<fn::Engine> engine;
}
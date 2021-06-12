#pragma once
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/color.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <chrono>
#include <memory>
#include <filesystem>
#include <random>

#include <iostream>
namespace fs = std::filesystem;
template<typename T> using sp = std::shared_ptr<T>;
template<typename T> using up = std::unique_ptr<T>;

#include <fn/core/Camera.hpp>
#include <fn/core/error_check.hpp>
/*
#include <fn/core/gl/Shader.hpp>
#include <fn/core/gl/Texture.hpp>
#include <fn/core/gl/Mesh.hpp>
#include <fn/core/gl/Model.hpp>
#include <fn/core/assets/AssetManager.hpp>
#include <fn/core/assets/assets.hpp>
*/


/**
 * Namespace per le funzioni core.
 */
namespace core {
	using seconds = std::chrono::duration<double>;          

	/** Namaespace per le funzioni o variabili legate ad OpenGL */
	namespace gl {
		// Dimensioni del display virtuale (conta solo l'aspect ratio, tutte le misure sono relative a vw e vh)
		static constexpr inline const glm::vec2 displayport_size() {
			return { 1280.0, 720.0 };
		}
		static constexpr inline const glm::vec2 displayport_center() {
			return displayport_size() * 0.5f;
		}

		static inline const glm::vec2 viewport_size() {
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			return { viewport[2], viewport[3] };
		}

		// Unità CSS-like corrispondono all'1% delle dimensioni di width ed height della display port
		static constexpr inline float vw = displayport_size().x / 100.0f; 
		static constexpr inline float vh = displayport_size().y / 100.0f;
	}
}

namespace gl = core::gl;
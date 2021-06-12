#pragma once
#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/color.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace core {
    /**
     * \brief Callback di debug OpenGL.
     * 
     * La funzione è richiamata automaticamente da OpenGL ogni qualvolta si verifica un evento di debug.
     * Mostrando sullo stderr Sorgente, tipo, gravità non che il contenuto dell'evento. 
     * Tale evento non sempre è un errore ma può essere un messaggio, un hint o un'informazione.
     * 
     * \param source
     * \param type
     * \param id
     * \param severity
     * \param length
     * \param msg
     * \param data
     * 
     * \see https://gist.github.com/liam-middlebrook/c52b069e4be2d87a6d2f
     */
    static void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data) {
        // Messaggio di debug di NVIDIA non è un errore 
        // Buffer detailed info : Buffer object 1 (bound to GL_ELEMENT_ARRAY_BUFFER_ARB, usage hint is GL_ENUM_88e4) will use VIDEO memory as the source for buffer object operations.
        if (id == 131185 || id == 131218) { 
            return;                                                        
        }
        std::string _source;
        std::string  _type;
        std::string  _severity;

        switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;

        case GL_DEBUG_SOURCE_OTHER:
            _source = "UNKNOWN";
            break;

        default:
            _source = "UNKNOWN";
            break;
        }

        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UDEFINED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;

        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;

        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;

        default:
            _type = "UNKNOWN";
            break;
        }

        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
            break;

        default:
            _severity = "UNKNOWN";
            break;
        }

        fmt::print(stderr, fg(fmt::color::crimson) | fmt::emphasis::bold,
            "{}: {} of {} severity, raised from {}: {}\n",
            id, _type, _severity, _source, msg
        );
    }

    static void GL_CLEAR_ERROR() {
        while (glGetError() != GL_NO_ERROR);
    }
    static bool GL_ERROR_CHECK(const char* stmt, const char* file, int line) {
        GLenum err = glGetError();
        std::string err_str;
        switch (err) {
        case GL_INVALID_VALUE:
            err_str = "GL_INVALID_VALUE";
            break;
        default:
            err_str = fmt::format("{:08x}", err);
            break;
        }

        // Accorcio il path
        auto fname = std::string(file);
        auto pos = fname.rfind("\\FruitNinja\\");

        if (err != GL_NO_ERROR) {
            fmt::print(stderr, "[OpenGL error ");
            fmt::print(stderr, fg(fmt::color::red) | fmt::emphasis::bold, "{}", err_str);
            fmt::print(stderr, "] call ");
            fmt::print(stderr, fg(fmt::color::light_yellow) | fmt::emphasis::bold, "{}", stmt);
            fmt::print(stderr, " @ ");
            fmt::print(stderr, fg(fmt::color::white) | fmt::emphasis::bold, ".\\{}:{}\n", file + pos, line);
            return false;
        }

        return true;
    }
}

#define break_assert(x) if(!(x)) __debugbreak();

#ifdef _DEBUG
/**
 * \brief Semplice macro per il debug degli errori OpenGL.
 * 
 * La macro è usata per wrappare una qualsiasi chiamata OpenGL, in caso
 * di errore: Viene stampato il messaggio di errore, il file e la linea
 * successivamente si forza l'interruzione dell'esecuzione con un breakpoint dinamico.
 * 
 * La macro è abilitata solamente nelle configurazioni di DEBUG.
 */
#define GL_CHECK(stmt) do { \
            core::GL_CLEAR_ERROR(); \
            stmt; \
            break_assert(core::GL_ERROR_CHECK(#stmt, __FILE__, __LINE__)); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif
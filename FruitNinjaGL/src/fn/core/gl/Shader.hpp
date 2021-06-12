#pragma once

#include <fn/fn.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader;
using ShaderSP = std::shared_ptr<Shader>;

/**
 * @brief Astrazione dello shader di OpenGL.
 * 
 * La classe consente di gestire in maniera semplificata alcuni aspetti degli Shader di OpenGL.
 * In particolare:
 * - Compilazione e Link del file sorgente .glsl della coppia vertex shader e fragment shader.
 * - Attivazione (e disattivazione) degli shader
 * - Set delle uniform con un'interfaccia polimorfica basata su template.
 */
class Shader{
public:
    /**
     * @brief Costruttore.
     * 
     * Carica, compila ed in fine linka una shader.
     * 
     * \param vertexShaderPath path al file con il codice del vertex shader
     * \param fragmentShaderPath path al file con il codice del fragment shader
     */
    Shader(const fs::path& vertexShaderPath, const fs::path& fragmentShaderPath);

    /** @brief Attiva lo shader */
    void active();

    /** @brief Disattiva lo shader */
    void deactive();
    
    /**
     * @brief Setta il valore di una uniform nello shader.
     * 
     * La specializzazione del template implementa il set con funzioni di 
     * OpenGL per quello specifico data-type.
     * @tparam T data-type dell'uniform che deve essere settato nello shader.
     * \param name nome della uniform
     * \param value valore da settare
     * 
     * @m_class{m-block m-warning}
     * 
     * @par 
     *      Al momento sono state implementate soltanto le specializzazioni per 
     *      @code{.cpp} int @endcode, @code{.cpp} unsigned int @endcode, @code{.cpp} float @endcode e @code{.cpp} glm::mat4 @endcode
     * 
     */
    template<typename T> void set(const std::string& name, T value);
    /**
     * @brief  Setta il un array di valori in una uniform nello shader.
     * 
     * La specializzazione del template implementa il set con funzioni di
     * OpenGL per quello specifico data-type.
     * @tparam T data-type dell'uniform che deve essere settato nello shader.
     * \param name nome della uniform
     * \param value puntatore all'array
     * \param count numero di valori
     * 
     * @m_class{m-block m-warning}
     *
     * @par 
     *      Al momento sono state implementate soltanto le specializzazioni per
     *      @code{.cpp} int @endcode, @code{.cpp} unsigned int @endcode, @code{.cpp} float @endcode e @code{.cpp} glm::mat4 @endcode
     * 
     */
    template<typename T> void set(const std::string& name, T* value, size_t count);

    /** \return id del programma restituito da OpenGL. */
    [[nodiscard]] inline GLuint program() const { return m_program_id; }

private:
    GLuint m_program_id;
    fs::path m_vShaderPth, m_fShaderPth;
    std::string m_vShaderSrc, m_fShaderSrc;
    [[nodiscard]] bool load();
    [[nodiscard]] bool compile();
};

template<>
inline void Shader::set(const std::string& name, int value){
    const GLint location = glGetUniformLocation(this->m_program_id, name.c_str());
    glUniform1i(location, value);
}

template<>
inline void Shader::set(const std::string& name, unsigned int value) {
    const GLint location = glGetUniformLocation(this->m_program_id, name.c_str());
    glUniform1ui(location, value);
}

template<>
inline void Shader::set(const std::string& name, float value) {
    const GLint location = glGetUniformLocation(this->m_program_id, name.c_str());
    glUniform1f(location, value);
}

template<>
inline void Shader::set(const std::string& name, glm::mat4 value) {
    const GLint location = glGetUniformLocation(this->m_program_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
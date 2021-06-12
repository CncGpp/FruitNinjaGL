#include "Shader.hpp"

Shader::Shader(const fs::path& vertexShaderPath, const fs::path& fragmentShaderPath)
{
	this->m_vShaderPth = vertexShaderPath;
	this->m_fShaderPth = fragmentShaderPath;

    if (!this->load()) {
        fn::log::fatal("Il loading degli shader è fallito.");
        std::terminate();
    }
        

    if (!this->compile()) {
        fn::log::fatal("La compilazione degli shader è fallita.");
        std::terminate();
    }
}

void Shader::active()
{
    GL_CHECK( glUseProgram(this->m_program_id) );
}

void Shader::deactive()
{
    glUseProgram(0);
}

bool Shader::load(){
	try{
		std::ifstream v(this->m_vShaderPth);
		this->m_vShaderSrc = std::string((std::istreambuf_iterator<char>(v)),
										 (std::istreambuf_iterator<char>()));
        std::ifstream f(this->m_fShaderPth);
        this->m_fShaderSrc = std::string((std::istreambuf_iterator<char>(f)),
                                         (std::istreambuf_iterator<char>()));
	}
	catch (const std::ifstream::failure&){
		fn::log::error(
			"[ERRORE::SHADER] Errore durante la lettura degli shader <vShaderPth={}, fShaderPth={}>\n", 
            this->m_vShaderPth.string(), this->m_fShaderPth.string()
		);
		return false;
	}
	return true;
}

bool Shader::compile(){
    // Compilo il codice sorgente degli shader
    const GLchar* vShaderSrc = m_vShaderSrc.c_str();
    const GLchar* fShaderSrc = m_fShaderSrc.c_str();

    GLint success;
    GLchar compile_errors_info[512];

    // 1. Compilazione del VertexShader
    const GLuint vs_id = glCreateShader(GL_VERTEX_SHADER);
    GL_CHECK( glShaderSource(vs_id, 1, &vShaderSrc, NULL) );
    GL_CHECK( glCompileShader(vs_id) );
    // Se la compilazione non è riuscita....
    glGetShaderiv(vs_id, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vs_id, 512, NULL, compile_errors_info);
        fn::log::error(
            "[ERRORE::SHADER] Errore durante la compilazione del Vertex Shader <shaderPath={}>\n[LOG] {}\n", this->m_vShaderSrc, compile_errors_info
        );
    }

    // 2. Compilazione del FragmentShader
    const GLuint fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    GL_CHECK( glShaderSource(fs_id, 1, &fShaderSrc, NULL) );
    GL_CHECK( glCompileShader(fs_id) );
    // Se la compilazione non è riuscita....
    glGetShaderiv(fs_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs_id, 512, NULL, compile_errors_info);
        fn::log::error(
            "[ERRORE::SHADER] Errore durante la compilazione del Fragment Shader <shaderPath={}>\n[LOG] {}\n", this->m_fShaderSrc, compile_errors_info
        );
    }

    // 3. Creo il Program linkando gli shader
    GL_CHECK( this->m_program_id = glCreateProgram() );
    GL_CHECK(glAttachShader(this->m_program_id, vs_id));
    GL_CHECK(glAttachShader(this->m_program_id, fs_id));
    GL_CHECK(glLinkProgram(this->m_program_id));
    // Se il linking non è riuscito....
    glGetProgramiv(this->m_program_id, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(this->m_program_id, 512, NULL, compile_errors_info);
        fn::log::error(
            "[ERRORE::SHADER] Errore durante il linking degli Shader.\n[LOG] {}\n", compile_errors_info
        );
    }
    // Cancello gli shader, non sono più necessari dal momento che sono stati linkati
    glDeleteShader(vs_id);
    glDeleteShader(fs_id);
	return true;
}

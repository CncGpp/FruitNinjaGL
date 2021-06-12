#define STB_IMAGE_IMPLEMENTATION
#include "Texture.hpp"

Texture::Texture(const fs::path& texpath, const Texture::Type type, const Texture::Parameteri& parameteri) : m_path(texpath)
{
	// Determino il tipo
	switch (type){
	case Texture::Type::diffuse:
		this->m_type = "texture_diffuse";
		break;
	case Texture::Type::specular:
		this->m_type = "texture_specular";
		break;
	default: {
			fmt::print("Errore tipo della texture non riconosciuto");
			this->m_type = "texture_diffuse";
			break;
		}
	}

	// Aggiorno i parametri di default
	for (auto& [key, value]  : parameteri) {
		this->m_parameteri[key] = value;
	}

	this->load();
}

Texture::Texture(const fs::path& texpath, const Texture::Parameteri& parameteri) : Texture(texpath, Texture::Type::diffuse, parameteri) {}

Texture::~Texture()
{
	glDeleteTextures(1, &this->m_id);
}

void Texture::load()
{
	stbi_set_flip_vertically_on_load(true);

	auto f = this->m_path.string();
	unsigned char* image = stbi_load(f.c_str(), &m_width, &m_height, 0, STBI_rgb_alpha);

	if (image == nullptr) {
		fn::log::error("[ERRORE] Errore durante il load della texture {} <image=nullptr>\n", f);
	}
	//if (stbi_failure_reason())
	//	fn::log::error("[ERRORE::TEXTURE] Errore durante il load della texture: {}.\n", stbi_failure_reason());


	GL_CHECK(glGenTextures(1, &m_id));
	// Eseguo il binding della texture
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));

	// Setto i parametri 
	for (auto& [key, value] : m_parameteri) {
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, key, value));
	}

	// Carico i dati della texture
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image));
	// Genero eventuali mipmap
	GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));

	// Pulizia e free
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	stbi_image_free(image);
}

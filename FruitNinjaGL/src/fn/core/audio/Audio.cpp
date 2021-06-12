#include "Audio.hpp"

ISoundEngine* Audio::s_audio_engine;

Audio::Audio(const fs::path& audiopath)
{
	if (this->Audio::s_audio_engine == nullptr)
		Audio::s_audio_engine = createIrrKlangDevice();

	if (!this->Audio::s_audio_engine) {
		fn::log::error("[ERRORE::IrrKlang] Errore durante la creazione dell'engine audio <s_audio_engine=nullptr>\n");
	}
	
	this->loadAudio(audiopath);
}

void Audio::play2D(float volume, bool loop)
{
	m_soundSource->setDefaultVolume(volume);
	s_audio_engine->play2D(m_soundSource, loop);
}

void Audio::play3D(glm::vec3 position, float volume, bool loop)
{
	irrklang::vec3df _position(position.x, position.y, position.z);
	m_soundSource->setDefaultVolume(volume);
	s_audio_engine->play3D(m_soundSource, _position,  loop);
}

void Audio::loadAudio(const fs::path& audiopath)
{
	this->m_soundSource = s_audio_engine->addSoundSourceFromFile(audiopath.string().c_str());
}

#include "LoadState.hpp"
#include <thread>
#include <fn\engine\states\PlayState.hpp>
#include <fn\core\assets\AssetManager.hpp>

LoadState::LoadState()
{
	m_splash_screen = std::make_shared<Sprite>(assets::res::splash_screen, glm::vec3{ 0.0f, 0.0f, 0.0f }, gl::viewport_size(), 1.0f);
	m_background = std::make_shared<Sprite>(assets::res::backgrounds[2], glm::vec3{ 0.0f, 0.0f, -200.0f }, gl::viewport_size(), 1.0f);
	AssetManager::loadAudio(assets::res::music::soundtrack)->play2D();
}
void LoadState::notifyEvent(Event* e){}

void LoadState::update(core::seconds delta_t)
{
	static bool once = false;
	if (!once && m_splash_elapsed.count() > 1.0) {
		this->loadAudios();
		this->loadShaders();
		this->loadTextures();
		this->loadModels();
		once = true;
	}
	m_splash_elapsed += delta_t;

	if (m_splash_lifetime < m_splash_elapsed) {
		Audio::stopAllSounds();
		//auto& currState = fn::engine->topState();
		//fn::engine->popState();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		auto state = std::make_unique<PlayState>();
		fn::engine->pushState(state);
	}
}

void LoadState::render()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float t = (float) m_splash_elapsed.count() / (float) m_splash_lifetime.count();
	t = t * 0.7f + 0.3f;

	if (t >= 0.6f) m_background->draw(m_camera);
	m_splash_screen->setAlpha(assets::res::anim::cosine(t));
	m_splash_screen->draw(m_camera);

	glfwSwapBuffers(fn::engine->window());
}


void LoadState::loadModels()
{
	// CARICAMENTO DEI MODELLI NELL'ASSETMANAGER
	for (auto& f : Fruits::list()) {
		fn::log::info("Loading del frutto {}.\n", f->name);
		AssetManager::loadModel(f->modelpath(Fruits::Model::whole));
		AssetManager::loadModel(f->modelpath(Fruits::Model::half_back));
		AssetManager::loadModel(f->modelpath(Fruits::Model::half_front));
	}
}

void LoadState::loadTextures()
{
	fn::log::info("Loading del font bitmap.\n");
	for (int i = (int)'0'; i <= (int)'9'; i++) {
		AssetManager::loadTexture(assets::res::ui::alphanum[i]);
	}

	fn::log::info("Loading del delle texture UI.\n");
	AssetManager::loadTexture(assets::res::ui::score_icon);
	AssetManager::loadTexture(assets::dir::sprites / "combo" / "arrow.png");
	AssetManager::loadTexture(assets::dir::sprites / "combo" / "blitz5.png");
	AssetManager::loadTexture(assets::dir::sprites / "combo" / "blitz10.png");
	AssetManager::loadTexture(assets::dir::sprites / "combo" / "blitz15.png");
	AssetManager::loadTexture(assets::dir::sprites / "combo" / "combo3.png");
	AssetManager::loadTexture(assets::dir::sprites / "combo" / "combo4.png");
	AssetManager::loadTexture(assets::dir::sprites / "combo" / "combo5.png");
	AssetManager::loadTexture(assets::dir::sprites / "combo" / "combo6.png");
}

void LoadState::loadShaders()
{
	fn::log::info("Loading e compilazione degli shader.\n");
	AssetManager::loadShader(assets::dir::shaders / "blades" / "default.vglsl", 
		                     assets::dir::shaders / "blades" / "default.fglsl");
	AssetManager::loadShader(assets::dir::shaders / "fruits" / "default.vglsl",
							 assets::dir::shaders / "fruits" / "default.fglsl");
	AssetManager::loadShader(assets::dir::shaders / "picking" / "default.vglsl",
		                     assets::dir::shaders / "picking" / "color_picking.fglsl");
	AssetManager::loadShader(assets::dir::shaders / "sprites" / "default.vglsl",
		                     assets::dir::shaders / "sprites" / "default.fglsl");
}

void LoadState::loadAudios()
{
	fn::log::info("Loading delle musiche.\n");
	AssetManager::loadAudio(assets::res::music::soundtrack);

	fn::log::info("Loading dei suoni.\n");
	AssetManager::loadAudio(assets::res::sound::game_start);
	for(auto& x : assets::res::sound::combo)
		AssetManager::loadAudio(x);
	for (auto& x : assets::res::sound::blitz)
		AssetManager::loadAudio(x);

}

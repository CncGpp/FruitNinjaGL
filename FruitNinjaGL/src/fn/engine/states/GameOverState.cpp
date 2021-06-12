#include "GameOverState.hpp"

GameOverState::GameOverState(unsigned int score)
{
	m_background = std::make_shared<Sprite>(assets::res::game_over_screen, glm::vec3{ 0.0f, 0.0f, -199.0f }, gl::displayport_size(), 1.0f);
	m_hud.setScore(score);
}

void GameOverState::notifyEvent(Event* e){}

void GameOverState::update(core::seconds delta_t)
{
	//if (Mouse::buttonLeftPressed()) {fn::print("test");}
}

void GameOverState::render()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_background->draw(m_camera);
	m_hud.draw(m_camera);
	glfwSwapBuffers(fn::engine->window());
}

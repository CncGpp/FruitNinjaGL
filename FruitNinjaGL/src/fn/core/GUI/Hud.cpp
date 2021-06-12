#include "Hud.hpp"

Hud::Hud()
{
	constexpr auto vh = gl::vh;
	constexpr auto vw = gl::vw;
	constexpr auto k = 2.0f;

	this->m_score_icon = std::make_shared<Sprite>(
		assets::res::ui::score_icon,
		glm::vec3{ k * vh,  k * vh, 0.0f },
		glm::vec2{ -1.0f, 10.0f * vh },
		1.0f
		);

	this->m_score_text = std::make_shared<Text>(
		std::to_string(0),
		glm::vec3{ k * vh + 0.6f * vw + this->m_score_icon->width(), k * vh, 0.0f },
		2.0f
		);

	this->m_timer_text = std::make_shared<Text>(
		"0:00",
		glm::vec3{ vw * 86, k * vh, 0.0f },
		2.0f
		);
}

void Hud::setScore(const unsigned int score)
{
	this->m_score_text->setText(std::to_string(score));
}

void Hud::setTimer(const core::seconds time)
{
	if (time < core::seconds(0)) {
		this->m_timer_text->setText(fmt::format("{:d}:{:02d}", 0, 0));
		return;
	}
	const unsigned int mm = (unsigned int)(time.count() / 60.0);
	const unsigned int ss = (unsigned int)(time.count() - mm * 60.0);
	this->m_timer_text->setText(fmt::format("{:d}:{:02d}", mm, ss));
}

void Hud::draw(const fn::Camera& camera)
{
	this->m_score_icon->draw(camera);
	this->m_score_text->draw(camera);
	this->m_timer_text->draw(camera);
}



void Hud::onResize()
{
	m_score_icon->onResize();
	this->m_score_text->onResize();
}

glm::vec3 Hud::position()
{
	return glm::vec3();
}

glm::vec2 Hud::shape()
{
	return gl::displayport_size();
}

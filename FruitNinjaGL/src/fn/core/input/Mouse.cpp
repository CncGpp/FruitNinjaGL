#include "Mouse.hpp"

Mouse Mouse::m_mouse = Mouse{};

void Mouse::init(const fn::Engine* engine)
{
	glfwSetCursorPosCallback(engine->window(), Mouse::cursor_position_callback);
	m_mouse.m_engine = engine;
}

void Mouse::pop_back(const size_t count) {
	if (count >= m_mouse.m_positions.size()) {
		m_mouse.m_positions.clear();
	}
	else
		for (int i = 0; i < count; i++)
			m_mouse.m_positions.pop_back();
}

void Mouse::update(core::seconds delta_t)
{
	// Calcolo quanti punti del mouse devo rimuovere es il X al secondo? Farlo dipendente dalla lunghezza?
	constexpr auto tr = core::seconds{ 1 } / 60;
	static core::seconds cum_mouse = core::seconds{ 0 };
	cum_mouse += delta_t;
	if (cum_mouse > tr) {
		cum_mouse = core::seconds{ 0 };
		Mouse::pop_back();
	}
}

void Mouse::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	const auto state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state != GLFW_PRESS) return;

	if (m_mouse.m_positions.size() >= m_mouse.m_max_buffer_size) {
		m_mouse.m_positions.pop_back();
	}

	m_mouse.m_positions.push_front({ xpos, ypos });
}

action Mouse::getAction(scancode button)
{
	return glfwGetMouseButton(m_mouse.m_engine->window(), button);
}
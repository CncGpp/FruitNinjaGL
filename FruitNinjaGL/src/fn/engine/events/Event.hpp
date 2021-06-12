#pragma once
#include<functional>

class Event {
public:
	enum struct Type {
		BLADE,		// Quando si muove la blade sullo schermo
		SPLIT,		// Quando un frutto viene diviso
		COMBO,      // Quando si genera una combo
		BLITZ,		// Quando si entra in un blitz
	};

	Event(Event::Type type) : m_type(type) {}
	Event(Event::Type type, std::function<void()> lambda) : m_type(type),  m_lambda(lambda) {}
	virtual void execute(){
		if (m_lambda) m_lambda();
	}

	virtual Event::Type type() const { return m_type; }

private:
	Event::Type m_type;
	std::function<void()> m_lambda;
};

struct ComboEvent : public Event {
public:
	ComboEvent(int comboLevel) : Event(Event::Type::COMBO), comboLevel(comboLevel) {}
	int comboLevel;
};

struct BlitzEvent : public Event {
public:
	BlitzEvent(int blitzLevel) : Event(Event::Type::BLITZ), blitzLevel(blitzLevel) {}
	int blitzLevel;
};
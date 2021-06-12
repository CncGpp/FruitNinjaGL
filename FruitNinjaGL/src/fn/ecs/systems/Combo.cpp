#include "Combo.hpp"
#include<numeric>
#include <fn\ecs\components\components.hpp>

namespace S {
	Combo::Combo(State* state, fn::Database& database) : m_state(state), m_database(database)
	{
		this->m_arrowSprite1 = std::make_shared<Sprite>(assets::dir::sprites / "combo" / "arrow.png", glm::vec3(0.0f, 0.0f, -199.0), gl::displayport_size());
		auto h = this->m_arrowSprite1->height();
		this->m_arrowSprite2 = std::make_shared<Sprite>(assets::dir::sprites / "combo" / "arrow.png", glm::vec3(0.0f, h, -199.0), gl::displayport_size());

		m_arrowSprite1->setAlpha(0.00f);
		m_arrowSprite2->setAlpha(0.00f);
		// Creo le entità da mostrare quando si fa il blitz
		fn::Entity e = m_database.create_entity();
		e.set<C::Sprite>({});
		e.get<C::Sprite>()->sprite = m_arrowSprite1;

		fn::Entity e2 = m_database.create_entity();
		e2.set<C::Sprite>({});
		e2.get<C::Sprite>()->sprite = m_arrowSprite2;
	}

	void Combo::update(core::seconds delta_t)
	{
		this->m_lastSplit += delta_t;
		this->m_lastCombo += delta_t;
		this->m_lastBlitz += delta_t;

		if (m_lastSplit > m_splitTimeout || m_splitCount >= MAX_COMBO_COUNT || !Mouse::buttonLeftPressed()) {
			if (m_splitCount >= 3) {
				this->onCombo();
				m_comboCount++;
				m_lastCombo = core::seconds(0);
			}
			m_splitCount = 0;
		}

		if (m_lastCombo > m_comboTimeout) {
			m_comboCount = 0;
		}

		if (m_lastBlitz > m_blitzTimeout) {
			m_lastBlitzLevel = BlitzLevel::none;
		}

		if (m_lastBlitzLevel < this->blitzLevel()) {
			this->onBlitz();
			fn::log::trace("(combo) Blitz!\n");
			m_lastBlitz = core::seconds(0);
		}
		else if (m_lastBlitzLevel == MAX_BLITZ_LEVEL && m_lastBlitz > m_blitzDelay) {
			this->onBlitz();
			fn::log::trace("(combo) Blitz, livello massimo!\n");
			m_lastBlitz = core::seconds(0);
		}

		this->updateParticles(delta_t);
		this->updateArrowBlitz(delta_t);

	}

	Combo::BlitzLevel Combo::blitzLevel() const
	{
		if (m_comboCount < 3) return BlitzLevel::none;
		else if (m_comboCount < 6) return BlitzLevel::combo;
		else if (m_comboCount < 9) return BlitzLevel::great;
		else if (m_comboCount < 12) return BlitzLevel::awesome;

		return MAX_BLITZ_LEVEL;
	}

	// Funzione chiamata quando viene effettuata una combo
	void Combo::onCombo()
	{
		// Fase 1: determino la combo (3, 4, 5, 6)
		m_splitCount = std::min(m_splitCount, MAX_COMBO_COUNT);
		fn::log::trace("(combo) Combo da {} frutti!\n", m_splitCount);

		// Fase 2: In base al punto medio delle Mouse::positions determino il quadrante di apparizione
		auto mean = std::accumulate(Mouse::positions().begin(), Mouse::positions().end(), glm::vec2());
		if (Mouse::positions().empty())
			mean = gl::displayport_center();
		else
			mean = mean / (float)Mouse::positions().size();

		// Fase 2: Genero lo sprite TODO fare un component message con fade e animazione
		auto s = std::make_shared<Sprite>(assets::dir::sprites / "combo" / fmt::format("combo{}.png", m_splitCount),
			glm::vec3(mean, 0.0f), glm::vec2(-1.0f, 30.0 * gl::vh));
		// Mi assicuro che lo sprite non sia disegnato fuori
		if (s->position().x + s->width() > gl::displayport_size().x)
			s->setPosition({ gl::displayport_size().x - s->width() - 10.0f * gl::vw, s->position().y, s->position().z });

		if (s->position().y + s->height() > gl::displayport_size().y)
			s->setPosition({ s->position().x, gl::displayport_size().y - s->height() - 10.0f * gl::vh, s->position().z });

		// Fase 4: Genero l'entità che apparirà a schermo
		fn::Entity e = m_database.create_entity();
		e.set<C::Sprite>({});
		e.get<C::Sprite>()->sprite = s;
		e.set<C::Particle>({ .lifetime = PARTICLE_LIFETIME, .elapsed = core::seconds(0), .interpolator = assets::res::anim::cosine });

		// Fase 5: Creo l'evanto di combo
		m_state->notifyEvent(new ComboEvent(m_splitCount));

	}

	void Combo::onBlitz()
	{
		m_lastBlitzLevel = this->blitzLevel();
		if (m_lastBlitzLevel == BlitzLevel::none) return;

		// Fase 1: Genero lo sprite
		auto s = std::make_shared<Sprite>(assets::dir::sprites / "combo" / fmt::format("blitz{}.png", m_lastBlitzLevel), glm::vec3(0.0), glm::vec2(-1.0f, 40.0 * gl::vh));
		s->setPosition({ gl::displayport_center() - 0.5f * s->shape(), 0.5f });

		// Fase 2: Genero l'entità che apparirà a schermo
		fn::Entity e = m_database.create_entity();
		e.set<C::Sprite>({});
		e.get<C::Sprite>()->sprite = s;
		e.set<C::Particle>({ .lifetime = PARTICLE_LIFETIME, .elapsed = core::seconds(0), .interpolator = assets::res::anim::cubic_bezier });

		// Fase 5: Creo l'evanto di blitz
		m_state->notifyEvent(new BlitzEvent((int)m_lastBlitzLevel));
	}

	void Combo::updateParticles(core::seconds delta_t)
	{
		m_database.for_each<C::Sprite, C::Particle>([&](fn::Eid eid, C::Sprite& s, C::Particle& p) {
			const float t = (float)p.elapsed.count() / (float)p.lifetime.count();
			if(p.interpolator)
				s.sprite->setAlpha(p.interpolator(t));

			p.elapsed += delta_t;
			if (p.elapsed > p.lifetime)
				m_database.destroy_entity(eid);
		});
	}

	void Combo::updateArrowBlitz(core::seconds delta_t)
	{
		// Aggiorno lo sprite 1
		{
			auto pos = this->m_arrowSprite1->position();
			this->m_arrowSprite1->setPosition(pos + glm::vec3(0.0f, -150.0f, 0.0f) * (float)delta_t.count());
			if (pos.y + this->m_arrowSprite1->height() < 0.0f) {
				this->m_arrowSprite1->setPosition({ pos.x, 0.0f, pos.z });
			}
		}

		// Aggiorno lo sprite 2
		{
			auto pos = this->m_arrowSprite2->position();
			this->m_arrowSprite2->setPosition(pos + glm::vec3(0.0f, -150.0f, 0.0f) * (float)delta_t.count());
			if (pos.y < 0.0f) {
				this->m_arrowSprite2->setPosition({ pos.x, this->m_arrowSprite2->height(), pos.z });
			}
		}

		// Aggiorno l'alpha in base al tempo
		if (m_lastBlitzLevel != BlitzLevel::none) {
			float t = 1.0f - (float)m_lastBlitz.count() / (float)m_blitzTimeout.count();
			this->m_arrowSprite1->setAlpha(t);
			this->m_arrowSprite2->setAlpha(t);
		}
		else {
			this->m_arrowSprite1->setAlpha(0.0f);
			this->m_arrowSprite2->setAlpha(0.0f);
		}
	}


}
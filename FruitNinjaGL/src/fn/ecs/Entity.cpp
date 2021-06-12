#include "Entity.hpp"
#include "Database.hpp"

namespace fn {
	void Entity::m_update_signature()
	{
		m_signature = m_database->signature(m_eid);
	}
}


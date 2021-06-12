#pragma once
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace fn {
	/**
	 * @brief Camera di gioco
	 * 
	 * Classe che astrae il concetto di 'Camera' definendo:
	 *  - projection matrix: proiezione prospettica
	 *  - view matrix: calcolata a patrie dalla posizione della camera
	 * 
	 * Tutti i parametri della camera come fovy, aspect, zNear, zFar
	 */
	class Camera {
	public:
		static inline constexpr float fovy = 45.0;								//!< Valore, in gradi del field of view della proiezione rispetto l'asse y
		static inline constexpr float aspect = 16.0f / 9.0f;					//!< Aspect ratio della proiezione
		static inline constexpr float zNear = 1.0f;                             //!< zNear della proiezione
		static inline constexpr float zFar = 150.0f;							//!< zFar della proiezione

		/**
		 * \brief Costruisce la camera.
		 * 
		 * Durante la costruzione della camera vengono calcolate m_projection
		 * e m_view, tali matrici non saranno più modificate.
		 * *NOTE* La camera punterà sempre verso {0,0,-1} con up {0,1,0}
		 * \param position posizione nello spazio dove collocare la camera.
		 */
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 20.0f)) : m_position(position)
		{
			this->m_projection = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
			this->m_view = glm::lookAt(m_position, m_position + m_center, m_up);
		}
		virtual ~Camera() = default;

		[[nodiscard]] const glm::vec3& position() const { return m_position; }
		[[nodiscard]] const glm::mat4& projection() const { return m_projection; }
		[[nodiscard]] const glm::mat4& view() const { return m_view; }

	protected:
		const glm::vec3 m_center = glm::vec3(0.0f, 0.0f, -1.0f);   //!<  Punto nello spazio dove la camera deve guardare (equivale al front)
		const glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);        //!<  Versore al up della camera, serve a definire l'asse y
		glm::vec3 m_position;                                      //!<  Punto nello spazio dove la camera è posizionata (equivale all'eye)
		glm::mat4 m_projection;                                    //!<  Matrice che definisce la proiezione prospettica
		glm::mat4 m_view;										   //!<  Matrice che definisce la trasformazione di vista
	};
}
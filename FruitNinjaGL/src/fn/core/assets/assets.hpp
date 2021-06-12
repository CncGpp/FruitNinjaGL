#pragma once
#include"../../fn.hpp"

/** @file
 * @brief Dichiarazioni di variabili e percorsi legati agli asset di gioco.
 */

/**
 * @brief Raccolta di percorsi e variabili relative agli asset.
 */
namespace assets {

	/**  @brief Directory di base alle varie risorse e asset di gioco. */
	namespace dir {
		static inline const fs::path models = "res/models";
		static inline const fs::path shaders = "res/shaders";
		static inline const fs::path textures = "res/textures";
		static inline const fs::path sprites = "res/textures/sprites";
		static inline const fs::path sounds = "res/sounds";
		static inline const fs::path musics = "res/musics";
	}

	/**  @brief Mapping delle risorse di gioco (in particolare sprite e texture). */
	namespace res {
		static inline const fs::path splash_screen = assets::dir::textures / "splash" / "splash.png";
		static inline const fs::path game_over_screen = assets::dir::textures / "backgrounds" / "game_over.jpg";

		/**
		 * @brief Array dei possibili background.
		 */
		static const fs::path backgrounds[] = {
			assets::dir::textures / "backgrounds" / "background_0.jpg",
			assets::dir::textures / "backgrounds" / "background_1.jpg",
			assets::dir::textures / "backgrounds" / "background_2.jpg",
		};

		/**
		 * @brief Array delle possibili texture da applicare alle blade.
		 */
		static const fs::path blades[] = {
			assets::dir::textures / "blades" / "blade_0.png",											// Lama Nera
			assets::dir::textures / "blades" / "blade_1.png",											// Lama viola
			assets::dir::textures / "blades" / "blade_2.png",											// Lama bicolore bianco-grigio
			assets::dir::textures / "blades" / "blade_3.png",
		};

		/** @brief Texture e risorse dedicate alle ui e alla gui. */
		namespace ui {
			static inline const fs::path score_icon = assets::dir::sprites / "half_watermelon.png";
			/**
			 * @brief Texture da associare ai vari caratteri dello spritefont.
			 * 
			 * Piuttosto che creare un array coin i possibili caratteri è stata utilizzata una struct
			 * con operator[] custom che compone il path alla texture dal codice ASCII del richiesto. 
			 */
			struct { 
				inline fs::path operator[](char c) {
					return (assets::dir::sprites / "font") / fmt::format("{:03d}.png", (int)c);
				}
			} alphanum;
		}

		/**
		 * @brief Funzioni di interpolazione delle animazioni.
		 * 
		 * Il namespace contiene tutta una serie di funzioni utilizzate per interpolare varie animazioni
		 * in particolare quelle di fade-in o fade-out degli sprite.
		 * 
		 * Una funzione @f$ f @f$ affinchè possa essere utilizzata come interpolante deve: @f[ 
		 * f:[0, 1] \mapsto [0, 1], \qquad f(0) = f(1) = 0  @f] ovvero, mappare valori da 0 a 1 fra 0 e 1 ed 
		 * assumere valore nulli agli estremi del dominio.
		 */
		namespace anim {
			/**
			 * @brief Curva gaussiana.
			 * 
			 * \param t indica il tempo @f$ t\in[0,1] @f$ da trasformare.
			 * \return valore interpolato.
			 */
			static inline float gaussian(const float t) {
				constexpr float u = 0.5f;
				constexpr float s = 0.15f;
				const auto a = (1 / (s * glm::sqrt(2 * glm::pi<float>()))) * exp(-0.5f * glm::pow((t - u) / s, 2.0f));
				return std::min(0.5f * a, 1.0f);
			};

			/**
			 * @brief Curva cubica di bezier.
			 * 
			 * Viene utilizzate una [curva di Bézier](https://en.wikipedia.org/wiki/B%C3%A9zier_curve)
			 * cubica composta da 4 punti di controllo. La curva è visualizzabile su [desmos](https://www.desmos.com/calculator/66vnmrcgnc).
			 * I punti sono stati scelti in modo da avere un rapito incremento a 1 e una decrescita a 0 più dolce.
			 * 
			 * @param t indica il tempo @f$ t\in[0,1] @f$ da trasformare.
			 * @return valore interpolato.
			 */
			static inline float cubic_bezier(const float t) {
				constexpr glm::vec2 P0{ 0.0f, 0.0f };
				constexpr glm::vec2 P1{ 0.0f, 1.618f };
				constexpr glm::vec2 P2{ 1.0f, 1.0f };
				constexpr glm::vec2 P3{ 1.0f, 0.0f };
				const auto p = glm::pow(1 - t, 3.0f) * P0 + 3 * t * glm::pow(1 - t, 2.0f) * P1 + 3 * t * t * (1 - t) * P2 + t * t * t * P3;
				return p.y;
			}

			/**
			 * @brief Curva coseno.
			 *
			 * Viene utilizzate una potenza sesta della funzione coseno, opportunamente contratta per avere semi-periodo unitario.
			 * La potenza è stata opportunamente scelta per dare una forma più quadra alla curva.
			 *
			 * @param t indica il tempo @f$ t\in[0,1] @f$ da trasformare.
			 * @return valore interpolato.
			 */
			static inline float cosine(const float t) {
				return 1.0f - glm::pow(glm::cos(t * glm::pi<float>()), 6.0f);
			}
		}

		/**
		 * @brief Musica di sottofondo.
		 */
		namespace music {
			static inline const fs::path soundtrack = assets::dir::musics / "music-original.ogg";
		}

		/**
		 * @brief Suoni di gioco.
		 */
		namespace sound {
			static inline const fs::path game_start = assets::dir::sounds / "Game-start.ogg";
			static inline const fs::path game_end = assets::dir::sounds / "Game-over.ogg";

			/**
			 * @brief Suoni utilizzabili per l'impatto con i frutti.
			 * 
			 * @m_class{m-block m-warning}

				@par 
					Il suono è preso casualmente fra quelli disponibili. Non sono implementati suoni specifici per frutto.
			 */
			static inline const fs::path impact[] = {
				assets::dir::sounds / "Impact-Orange.ogg",
				assets::dir::sounds / "Impact-Pineapple.ogg",
				assets::dir::sounds / "Impact-Plum.ogg",
				assets::dir::sounds / "Impact-Watermelon.ogg",
			};

			/** @brief Suoni utilizzabili per le combo. */
			static inline const fs::path combo[] = {
				assets::dir::sounds / "combo-1.ogg",
				assets::dir::sounds / "combo-2.ogg",
				assets::dir::sounds / "combo-3.ogg",
				assets::dir::sounds / "combo-4.ogg",
				assets::dir::sounds / "combo-5.ogg",
				assets::dir::sounds / "combo-6.ogg",
				assets::dir::sounds / "combo-7.ogg",
				assets::dir::sounds / "combo-8.ogg",
			};

			/** @brief Suoni utilizzabili per i blitz. */
			static inline const fs::path blitz[] = {
				assets::dir::sounds / "combo-blitz-1.ogg",
				assets::dir::sounds / "combo-blitz-2.ogg",
				assets::dir::sounds / "combo-blitz-3.ogg",
				assets::dir::sounds / "combo-blitz-4.ogg",
				assets::dir::sounds / "combo-blitz-5.ogg",
				assets::dir::sounds / "combo-blitz-6.ogg",
			};
		}

	}
}
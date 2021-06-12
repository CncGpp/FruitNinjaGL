#pragma once
#include<fn/fn.hpp>
#include <fmt/color.h>
#include <fmt/chrono.h>

namespace fn {
	/** @brief Utility per il log */
	class log{
	public:
		enum struct level : unsigned int{
			off =    0,		 // Il livello più basso possibile, viene usato per disattivare i log.
			fatal =  1,      // Errore importante che causa un prematuro termine dell'esecuzione. 
			error =  2,		 // Un errore di esecuzione o una condizione imprevista.
			warn =   4,		 // Usato per ogni condizione inaspettata o anomalia di esecuzione, che però non necessariamente ha comportato un errore.
			info =   8,      // Usato per segnalare eventi di esecuzione (esempio: startup/shutdown). 
			debug = 16,		 // Usato nella fase di debug del programma. Viene riportato nel file di log.
			trace = 32,      // Alcune informazioni dettagliate.
			all =   64,      // Il livello più alto possibile, tutti i log sono attivi.
		};

		// Bitmap utilizzata per mascherare alcuni livelli di log
		using mask = unsigned int;

		template<level l, typename ...Args>
		static void base_logging(fmt::text_style style, const std::string& S, Args ...args) {
			if constexpr (l <= s_logging_level && ((unsigned int)l & s_logging_mask) > 0) {
				std::time_t t = std::time(nullptr);
				fmt::print(style, s_datetime_fmt + S, fmt::localtime(t), args...);
			}
		}

		template<typename ...Args>
		static constexpr void fatal(const std::string& S, Args ...args) {
			base_logging<log::level::fatal, Args...>(
				fg(fmt::color::white_smoke) | bg(fmt::color::dark_red) | fmt::emphasis::bold,
				S, args...
			);
		}

		template<typename ...Args>
		static constexpr void error(const std::string& S, Args ...args) {
			base_logging<log::level::error, Args...>( fg(fmt::color::crimson) | fmt::emphasis::bold, S, args...);
		}

		template<typename ...Args>
		static constexpr void warn(const std::string& S, Args ...args) {
			base_logging<log::level::warn, Args...>(fg(fmt::color::peach_puff) | fmt::emphasis::bold, S, args...);
		}

		template<typename ...Args>
		static constexpr void info(const std::string& S, Args ...args) {
			base_logging<log::level::info, Args...>(fg(fmt::color::light_golden_rod_yellow) | fmt::emphasis::italic, "[INFO] " + S, args...);
		}

		template<typename ...Args>
		static constexpr void trace(const std::string& S, Args ...args) {
			base_logging<log::level::trace, Args...>(fg(fmt::color::blanched_almond) | fmt::emphasis::italic, "[TRACE] " + S, args...);
		}

	private:
		static inline const std::string s_datetime_fmt = "[{:%Y-%m-%d %H:%M:%S}] ";
		static inline constexpr level s_logging_level = level::all;
		static inline constexpr mask s_logging_mask = 0xFFFFFFFF;
	};
}


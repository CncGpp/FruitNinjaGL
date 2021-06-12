#pragma once
#include <fn\core\core.hpp>
#include <fn/engine/events/Event.hpp>

/**
 * @brief Interfaccia ad uno Stato di gioco.
 * 
 * La classe State è un interfaccia astratta allo stato interno del gioco in un dato momento.
 * Con il passare del tempo, e a seconda delle scelte dell'utente il gioco può trovarsi im
 * stati interni differenti. Esempi possono essere:
 * - LoadState: Ovvero la fase iniziale, quando il gioco si apre. Viene mostrata la splash screen ed
 *              avviene il caricamento degli asset principali.
 * - PlayState: Stato di gioco effettivo, vengono lanciati i frutti ed il tempo diminuisce.
 * - PauseState (non implementato): Quando il gioco è messo in pausa.
 * - GameOverState: Stato nel quale il gioco termina. Viene mostrata la schermata di game Over.
 *
 * Tutti gli stati condividono un interfaccia e quindi un comportamento comune:
 * 1. handle di input ed eventi, attraverso State::handleInputs e State::handleEvents
 * 2. Notifica di nuovi eventi, mediante State::notifyEvent
 * 3. update e render
 * 4. Callback specifiche, ad esempio al resize dello schermo.
 * 
 * Ogni stato concreto può scegliere come vuole di implementare queste funzioni, a secoda
 * delle necessità.
 */
class State {
public:
	/** @brief costruttore */
	State() = default;

	/** @brief distruttore. */
	virtual ~State() = default;

	/** @brief Gestisce gli input (mouse e tastiera) */
	virtual void handleInputs() {}
	/** @brief  Gestisce gli eventi di gioco.*/
	virtual void handleEvents() {}
	/** @brief  Notifica lo stato di un nuovo evento.*/
	virtual void notifyEvent(Event* e) = 0;
	/** @brief  Aggiorna lo stato interno. */
	virtual void update(core::seconds delta_t) = 0;
	/** @brief  Renderizza lo stato attuale.*/
	virtual void render() {}

	// Callback in risposta agli eventi
	// virtual void onCreate() {}		//Non usata
	// virtual void onAttached() {}     //Non usata

	/** @brief  Callback, richiamata al resize della finestra.*/
	virtual void onResize() {}
};
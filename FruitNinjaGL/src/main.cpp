#include <time.h>
#include<iostream>
#include"fn/fn.hpp"
#include <fn\engine\states\PlayState.hpp>
#include <fn\engine\states\LoadState.hpp>

// Abilito i device dedicati di Nvidia e AMD https://github.com/glfw/glfw/issues/1545
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int main()
{
	srand((unsigned int)time(0));

	// Creo ed inizializzo l'istanza globale del motore
	fn::engine = std::make_unique<fn::Engine>(1280, 720);

	// Creo e abilito lo stato di partenza
	auto state = std::make_unique<LoadState>();
	fn::engine->pushState(state);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(fn::engine->window())) {
		fn::engine->loop();
	}
	
	return 0;
}
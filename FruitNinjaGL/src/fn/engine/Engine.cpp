#include "Engine.hpp"
#include <utl\log.hpp>


namespace fn {

	Engine::Engine(int window_width, int window_height) noexcept {
		this->m_window_height = window_height;
		this->m_window_width = window_width;
		this->last_t = core::seconds(0);
		this->delta_t = core::seconds(0);

		this->init();
	}

	Engine::~Engine() {
		glfwDestroyWindow(this->m_window);
		this->m_window = nullptr;
		glfwTerminate();
	}


	void Engine::loop() {
		// Determino il tempo trascorso
		auto now = core::seconds(glfwGetTime());
		this->delta_t = now - last_t;
		last_t = now;

		// Aggiorno l'input
		Mouse::update(this->delta_t);

		if (m_states.empty()) return;
		auto& state = *m_states.top();					// Prendo lo stato corrente

		// Handle inputs
		glfwPollEvents();
		state.handleInputs();

		// Update
		state.update(this->delta_t);

		// Render
		state.render();

		// Handle Events
		state.handleEvents();

		if (Engine::resized) {
			state.onResize();
			Engine::resized = false;
		}
	}



	void Engine::init() {
		// Fase 1. Inizializzo il contesto di Glfw
		fn::log::info("Inizializzazione del contesto di GLFW...\n");
		if (!this->init_context()) std::exit(1);

		// Fase 2. Creo la finestra
		fn::log::info("Inizializzazione e creazione della finestra...\n");
		if (!this->init_window()) std::exit(1);

		// Fase 3. Inizializzo il loader opengl
		fn::log::info("Inizializzazione di GLEW ...\n");
		if (!this->init_gl_loader()) std::exit(1);

		// Fase 4. Inizializzo OpenGL
		fn::log::info("Inizializzazione del contesto di OpenGL ...\n");
		if (!this->init_gl()) std::exit(1);

		// Fase 5. Inizializzo la view
		fn::log::info("Inizializzazione della view-port e callback di resize ...\n");
		if (!this->init_view()) std::exit(1);


		// Inizializzo lo stato intero di Game
		this->last_t = core::seconds(glfwGetTime());
		this->delta_t = core::seconds(0);
		fn::log::info("Inizializzazione del motore completa!\n");

		// Inizializzo mouse e tastiera
		Mouse::init(this);
	}


	bool Engine::init_context()
	{
		// 1. Definisco la Callback da utilizzare in caso di errori con GLFW.
		glfwSetErrorCallback([](int error, const char* description) {
			fprintf(stderr, "Glfw Error %d: %s\n", error, description);
			});

		// 2. Inizializzo il contesto di glfw
		if (!glfwInit())  return false;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->VERSION_MINOR);
		if (this->VERSION_MAJOR > 3 && this->VERSION_MINOR > 2)
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		if (this->VERSION_MAJOR > 3 && this->VERSION_MINOR > 0)
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// 3.0+ only

		return true;
	}

	bool Engine::init_window() {
		// Creo la finestra GLFW
		this->m_window = glfwCreateWindow(this->m_window_width, this->m_window_height, "Fruit Ninja GL - Giuseppe Cianci Pio", NULL, NULL);
		if (this->m_window == nullptr)
			return false;
		glfwMakeContextCurrent(this->m_window);			
		
		// Configuro le proprietà della finestra e di OpenGL
		glfwSwapInterval(1);										// Abilito il V-sync						
		glfwSetWindowAspectRatio(this->m_window, 16, 9);			// Forzo l'aspect ratio a 16:9
		GL_CHECK( glEnable(GL_MULTISAMPLE) );						// Abilito l'anti-aliasing

		return true;
	}

	bool Engine::init_gl_loader() {
		// Setto questo valore a true, cosi GLEW può utilizzare un approccio moderno per recuperare i puntatori alle funzioni ed estenzioni.
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize OpenGL loader!\n");
			return false;
		}
		else return true;
	}

	bool Engine::init_gl() {
		GL_CHECK(glEnable(GL_BLEND));
		GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GL_CHECK(glEnable(GL_DEPTH_TEST));

		// Abilito le callback degli errori
		GL_CHECK(glEnable(GL_DEBUG_OUTPUT));
		GL_CHECK(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
		glDebugMessageCallback(core::GLDebugMessageCallback, nullptr);

		return true;
	}

	bool Engine::init_view()
	{
		// Inizializzo la view
		glViewport(0, 0, this->m_window_width, this->m_window_height);
		glfwSetWindowSizeCallback(this->window(), [](GLFWwindow* window, int width, int height) {
			std::cout << "New w: " << width << "  new h:" << height << std::endl;
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			Engine::m_window_height = display_h;
			Engine::m_window_width = display_w;
			Engine::resized = true;
			});

		return true;
	}
}

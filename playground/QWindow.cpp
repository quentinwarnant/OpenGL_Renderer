#include "QWindow.hpp"


QWindow::QWindow(GLuint windowWidth, GLuint windowHeight, const char* title)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;

    m_title = title;
}

QWindow::~QWindow()
{
	// Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_mainWindow);
		
	glfwTerminate();

}

int QWindow::Init()
{
    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return 1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x multisampling
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	m_mainWindow = glfwCreateWindow( m_windowWidth, m_windowHeight, m_title, NULL, NULL);
	if( m_mainWindow == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(m_mainWindow, &m_windowBufferWidth, &m_windowBufferHeight);

	glfwMakeContextCurrent(m_mainWindow);
	glewExperimental=true; // Needed in core profile


	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwDestroyWindow(m_mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0,0,m_windowBufferWidth, m_windowBufferHeight);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_mainWindow, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.4f, 0.0f, 0.2f, 0.0f);

	return 0;
}
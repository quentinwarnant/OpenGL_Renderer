#include "QWindow.hpp"


QWindow::QWindow(GLuint windowWidth, GLuint windowHeight, const char* title)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;

    m_title = title;

	//Input
	for (size_t i = 0; i < 1024; i++)
	{
		m_keys[i] = false;
	}

	m_mousePosChangeX = m_mousePosChangeY = 0;
	m_mouseHasMoved = false;
	
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

	//input
	glfwSetWindowUserPointer( m_mainWindow, this);
	glfwSetInputMode(m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	InitCallbacks();

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
	glClearColor(0.4f, 0.4f, 0.5f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	return 0;
}


void QWindow::InitCallbacks()
{
	glfwSetKeyCallback(m_mainWindow, HandleKeyboardInput);
	glfwSetCursorPosCallback(m_mainWindow, HandleMouseInput);
}

void QWindow::PreUpdate()
{
}

void QWindow::Update()
{

}

void QWindow::PostUpdate()
{
}

void QWindow::HandleKeyboardInput(GLFWwindow* window, int key, int code, int action, int mode)
{
	QWindow* thisQWindow = static_cast<QWindow*>(glfwGetWindowUserPointer(window)); 

	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if( key >= 0 && key < 1024)
	{
		if( action == GLFW_PRESS )
		{
			thisQWindow->m_keys[key] = true;
			printf("pressed key %d\n ", key);
		}
		else if(  action == GLFW_RELEASE && thisQWindow->m_keys[key] ) // not pressing & key was pressed before
		{
			thisQWindow->m_keys[key] = false;
			printf("released key %d\n ", key);

		}
	}
}

void QWindow::HandleMouseInput(GLFWwindow* window, double mousePosX, double mousePosY)
{
	QWindow* thisQWindow = static_cast<QWindow*>(glfwGetWindowUserPointer(window)); 

	if( thisQWindow->m_mouseHasDoneInitialPositioning == false )
	{
		//Doubles into float?
		thisQWindow->m_mouseLastPosX = mousePosX;
		thisQWindow->m_mouseLastPosY = mousePosY;
		thisQWindow->m_mouseHasDoneInitialPositioning = true;
	}

	//Calculate diff since last call
	thisQWindow->m_mousePosChangeX = mousePosX - thisQWindow->m_mouseLastPosX;
	thisQWindow->m_mousePosChangeY = mousePosY - thisQWindow->m_mouseLastPosY;

	// Set mouse pos values ahead of next call
	thisQWindow->m_mouseLastPosX = mousePosX;
	thisQWindow->m_mouseLastPosY = mousePosY;

	thisQWindow->m_mouseHasMoved = true;
}

bool QWindow::IsKeyPressed(int keyId)
{
	return m_keys[keyId];
}

void QWindow::GetMouseChange(GLfloat& mousePosChangeX, GLfloat& mousePosChangeY, bool consume)
{
	if(m_mouseHasMoved)
	{
		mousePosChangeX = *(&m_mousePosChangeX);
		mousePosChangeY = *(&m_mousePosChangeY);
	}
	else
	{
		mousePosChangeX = 0;
		mousePosChangeY = 0;
	}

	if( consume )
	{
		m_mouseHasMoved = false;
	}
}
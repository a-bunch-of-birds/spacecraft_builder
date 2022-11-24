
#include <iostream>
#include <fstream>
#include <vector>
#include "glfw_class.h"

using namespace std;

gl_object::gl_object(int width, int height, const char* title)
{
	this->width = width;
	this->height = height;
	this->title = title;
	this->fps = 60;
	this->running = true;

	glfwInit();
	if (!glfwInit())
	{
		cout << "Failed to initialize GLFW" << endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DEBUG
	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	window = glfwCreateWindow(width, height, title, 0, 0);
	if (!window)
	{
		cout << "GLFW window open failure" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return;
	};

	glfwSetWindowTitle(window, "Spacecraft Builder - Grimm MSc Project");
	
}

gl_object::~gl_object() {
	glfwTerminate();
}

GLFWwindow* gl_object::getWindow()
{
	return window;
}

void gl_object::DisplayVersion()
{
	cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "Version: " << glGetString(GL_VERSION) << endl;
	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
}

int gl_object::eventLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		renderer();

		glfwSwapBuffers(window);
		glfwPollEvents();

		
	}

	glfwTerminate();
	return 0;
}

void gl_object::setErrorCallBack(void(*func)(int error, const char* description))
{
	glfwSetErrorCallback(func);
}

void gl_object::setRenderer(void(*func)())
{
	this->renderer = func;
}

void gl_object::setReshapeCallback(void(*func)(GLFWwindow* window, int w, int h))
{
	glfwSetFramebufferSizeCallback(window, func);
}

void gl_object::setKeyCallBack(void(*func)(GLFWwindow* window, int key, int scancode, int action, int mods))
{
	glfwSetKeyCallback(window, func);
}


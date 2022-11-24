#ifndef GLFW_CLASS_H
#define GLFW_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class gl_object {
private:
	int width;
	int height;
	const char* title;
	double fps;
	void (*renderer)();
	bool running;
	GLFWwindow* window;

public:
	gl_object(int width, int height, const char* title);
	~gl_object();

	void setFPS(double fps) {
		this->fps = fps;
	}

	void DisplayVersion();

	void setRenderer(void(*f)());
	void setReshapeCallback(void(*f)(GLFWwindow* window, int w, int h));
	void setKeyCallBack(void(*f)(GLFWwindow* window, int key, int scancode, int action, int mods));
	void setErrorCallBack(void(*f)(int error, const char* description));


	int eventLoop();
	GLFWwindow* getWindow();
};

#endif
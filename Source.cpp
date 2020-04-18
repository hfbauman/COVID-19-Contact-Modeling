//Graphics libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Allows output messages
#include <iostream>

//Compile-time replacements:
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	//Intialize GLFW (our window and graphics control interface)
	glfwInit();

	//Tell GLFW what the version of OpenGL that we want to use is. I'm using 3.3, as its the oldest with the core profile so that I am sure that everything I write will have maximum portability.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Use the core profile of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	
	//Recognizes if the window didn't generate properly. If it didn't, we clean up GLFW and return an error.
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Makes the window the current place to draw stuff. This tells OpenGL where the image data that it is about to render should go.
	glfwMakeContextCurrent(window);

	//Checks to make sure that GLAD has appropriately loaded before we try to do anything with OpenGL calls.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Sets the viewport for OpenGL. For our purposes right now, we want it to fill the entire window, so we match the viewport to the window.
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//References our program to link OpenGL with the instructions on what to do in the event of a window resize 
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Event loop. This contains what the program should do every frame.
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//Sets the clear color (i.e. the color that is written over everything at the beginning of a new frame render).
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//Writes everything over with the color that we just specified.
		glClear(GL_COLOR_BUFFER_BIT);

		//Finished with rendering, display the image on the screen.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clean up nicely after ourselves, once everything is done.
	glfwTerminate();
	return 0;
}


//Tells OpenGL what to do in the event of a window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//A function that allows for keyboard input. I don't use much of its functionality, but it's useful to have for future reference.
void processInput(GLFWwindow* window)
{
	//Window closes on the press of the escape key
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
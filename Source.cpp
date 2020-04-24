//Graphics libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Allows output messages
#include <iostream>

//Compile-time replacements:
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

//Tells VS that these will be functions that I will define at some point in the future
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Source code for the vertex shader. This program is written for OpenGL and describes how to transform the vertex data to put it on the screen
const char *vertexShaderSource = "#version 330 core\n"
"layout (location=0) in vec3 position;\n" //Specifies that the position vector should be put in location 0
"void main()\n"
"{\n"
"	gl_Position=vec4(position.x,position.y,position.z,1.0);\n"
"}\0";

//Source code for the fragment shader. This program is also written for OpenGL and describes how to color shapes that we are passing in. It colors everything the same color.
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\n\0";

int main()
{
	//Intialize GLFW (our window and graphics control interface)
	glfwInit();

	//Tell GLFW what the version of OpenGL that we want to use is. I'm using 3.3, as its the oldest with the core profile so that I am sure that everything I write will have maximum portability.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Use the core profile of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creates a pointer to a window object with GLFW
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Compile and build the vertex shader program
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Check if the vertex shader actually built properly. It would be bad to try to render with it if it doesn't work.
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//Print out any error messages
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Compile and build the fragment shader program
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Check if the fragment shader built properly
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	//Print any error messages
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Now we need to link the two shaders into one program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Check if the program built properly
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Since the shaders have been built into a program, we can now delete them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Defines the vertex data that I'd like to use to make a triangle
	float verticies[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top 
	};

	//Create a spot in memory for a Vertex Array Object. This will bind together all the calls necessary to send our data to the GPU and interpret it, so that it's easier to call later in the program
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);

	//Creates a spot in memory for a handle to the Vertex Buffer Object
	unsigned int VBO;

	//Creates the Vertex Buffer Object and stores the handle in the spot in memory that we specify
	glGenBuffers(1, &VBO);

	//Sets the Vertex Array Object, so that further calls describing the buffer and interpretation are stored within that instance.
	glBindVertexArray(VAO);

	//Binds the GL array buffer to the buffer object that we just created
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Sends the vertex data to the data buffer and tells it that we won't be changing this data often (which affects how the graphics card stores the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	//Defines how to process the data we sent in
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Now that we've finished making all of those definitions, tell OpenGL to stop writing things to those objects so that future statements don't accidentally modify them.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//Event loop. This contains what the program should do every frame.
	while (!glfwWindowShouldClose(window))
	{
		//Processes any input that has happened since the last frame
		processInput(window);

		//Sets the clear color (i.e. the color that is written over everything at the beginning of a new frame render).
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//Writes everything over with the color that we just specified.
		glClear(GL_COLOR_BUFFER_BIT);

		//Tells OpenGL to use the shaders that we custom made
		glUseProgram(shaderProgram);

		//Tells OpenGL how to get the data properly transmitted
		glBindVertexArray(VAO);

		//Draw the triangle. Yay!
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
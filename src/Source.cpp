//Graphics libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Allows output messages
#include <iostream>

//Allows use of vector objects
#include <vector>

//Gives random number generation
#include<cstdlib>
#include <time.h>

//Inclusion of Dear imgui, an easy-to-use GUI library
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

//Circle class
#include "Circle.h"

using namespace std;

//Tells VS that these will be functions that I will define at some point in the future
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void drawInSquareViewport(GLFWwindow* window);
void generateCircles(vector<Circle> &circles);
void createCircles(vector<Circle> &circles, int VAO);
void circleMotion(vector<Circle> &circles, bool immunity, float infection_chance, float average_recovery);
void circleCollision(vector<Circle> &circles, bool immunity, float infection_chance, float average_recovery);
void drawCircles(vector<Circle> &circles, int shaderProgram);

//Sets program parameters
#define PI 3.14159265358979323846
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define NUM_CIRCLE_VERTICES 100
#define CIRCLE_RADIUS 0.05
#define CIRCLE_SPEED 0.01
#define FRAMERATE 60

int num_circles = 30;
float sim_speed = 1;

//Sets virus parameters
//Whether the population is capable of being reinfected by the disease
bool immunity = true;
//Chance for a susceptible circle to be infected by an infected circle
float infection_chance = 1.0;
//Amount of time (in some unit, who knows) for a circle to recover
float average_recovery = 5.0;



//Source code for the vertex shader. This program is written for OpenGL and describes how to transform the vertex data to put it on the screen
const char *vertexShaderSource = "#version 330 core\n"
"layout (location=0) in vec3 position;\n" //Specifies that the position vector should be put in location 0

//This matrix will hold the scaling and translational data for the drawing of the entire circle
"uniform mat4 mvMatrix;\n"

//This will hold the rgb color data
"uniform vec3 color;\n"

"out VS_OUT {\n"
"	vec4 color;\n"
"} vs_out;\n"


"void main()\n"
"{\n"
"	gl_Position=mvMatrix*vec4(position,1.0);\n"
"	vs_out.color=vec4(color, 1.0);\n"
"}\0";

//Source code for the fragment shader. This program is also written for OpenGL and describes how to color shapes that we are passing in. It colors everything the same color.
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
	"in VS_OUT{\n"
	"	vec4 color;\n"
	"} fs_in;\n"
    "void main()\n"
    "{\n"
    "   FragColor = fs_in.color;\n"
	"}\0";


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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Contact Modeling", NULL, NULL);
	
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

	//Sets the viewport for OpenGL. For circles to be drawn properly, it needs to be square
	drawInSquareViewport(window);

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

	//initialize IMGUI
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		ImGui_ImplOpenGL3_Init("#version 330");
	}

	//Generate array of circles
	vector<Circle> *circles = new vector<Circle>(num_circles);
	generateCircles(*circles);

	//Saves the time for framerate comparisons
	double time_at_beginning_of_previous_frame = glfwGetTime();

	bool simulationRunning = false;
	bool settingUpSim = true;


	//Event loop. This contains what the program should do every frame.
	while (!glfwWindowShouldClose(window))
	{
		if (simulationRunning)
		{
			//Checks to see if enough time has passed to bother rendering another frame
			if (glfwGetTime() < time_at_beginning_of_previous_frame + 1.0 / FRAMERATE)
			{
				continue;
			}
			//Saves the current time to reference on the next iterations of the loop
			time_at_beginning_of_previous_frame = glfwGetTime();

			//Processes any input that has happened since the last frame
			processInput(window);

			//Processes the movement of the circle
			circleMotion(*circles,immunity,infection_chance,average_recovery);

		}
		//Clears and resizes the window appropriately
		drawInSquareViewport(window);
		if (!settingUpSim)
		{

			//Tells OpenGL to use the shaders that we custom made
			glUseProgram(shaderProgram);

			drawCircles(*circles, shaderProgram);
		}

		//imgui information
		{

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//The dragable window that will contain options
			ImGui::Begin("Simulation Control");
			{
				//This reads as:
				//If this button is clicked, switch the simulation running variable and set setting up sim to false.
				//The expression in parenthesis of the ImGui::Button says "If the simulationRunning variable is true, display Pause, else display Start"
				if (ImGui::Button(simulationRunning ? "Pause" : "Start"))
				{
					simulationRunning = !simulationRunning;
					if (settingUpSim)
						settingUpSim = false;
				}

				//A button to restart the simulation with new randomly generated circles, positions, and velocities
				if (ImGui::Button("Restart")) {
					generateCircles(*circles);
				}

				//A checkbox for the immunity boolean
				ImGui::Checkbox("Immunity", &immunity);
				
				//Allows the user to change the number of circles in realtime
				if (ImGui::InputInt("Number of Circles/People", &num_circles, 1, 100, ImGuiInputTextFlags_AutoSelectAll)) {
					delete(circles);
					circles = new vector<Circle>(num_circles);
					generateCircles(*circles);
					simulationRunning = false;
				}

				//A slider for the infection chance variable. Bounds are from 0.0 to 1.0
				ImGui::SliderFloat("Infection Chance", &infection_chance, 0.0f, 1.0f);

				//A slider for the recovery time variable. Bounds are from 0.0 to 20.0
				ImGui::SliderFloat("Recovery Time", &average_recovery, 0.0f, 20.0f);

				//A slider for the simulation speed. Bounds are between 0.0 and 5.0
				ImGui::SliderFloat("Simulation Speed", &sim_speed, 0.0f, 5.0f);
				ImGui::End();
			}

			// Rendering
			ImGui::Render();
			ImGuiIO& io = ImGui::GetIO();
			glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		//Finished with rendering, display the image on the screen.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clean up nicely after ourselves, once everything is done.

	// imgui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//Glfw cleanup
	glfwDestroyWindow(window);
	glfwTerminate();

	//Variable cleanup
	delete(circles);
	return 0;
}


//Tells OpenGL what to do in the event of a window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	drawInSquareViewport(window);
}

//A function that allows for keyboard input. I don't use much of its functionality, but it's useful to have for future reference.
void processInput(GLFWwindow* window)
{
	//Window closes on the press of the escape key
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void drawInSquareViewport(GLFWwindow *window)
{
	//Sets the viewport to be the whole screen
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//Sets the clear color for the borders (grey)
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Gets the current width and height of the window
	int width;
	int height;
	glfwGetFramebufferSize(window,&width,&height);

	//Find the minimum dimension
	int min;
	if (width < height)
	{
		min = width;
	}
	else
	{
		min = height;
	}

	//Draws on just a portion of the screen to clear the "viewing" area to black
	glEnable(GL_SCISSOR_TEST);
	glScissor(int((width - min) / 2.0), int((height - min) / 2.0), min, min);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	//Defines the square to use for future OpenGL calls
	glViewport(int((width - min) / 2.0), int((height - min) / 2.0), min, min);


}

void generateCircles(vector<Circle> &circles)
{
	//Defines the vertex data that I'd like to use using vector objects
	vector<double> circle((NUM_CIRCLE_VERTICES + 2) * 3);
	//The center is (0,0,0) since I'll use the vertex shader to define translations

	//The angle as measured from (0,1,0) clockwise
	double angle;

	//Makes a ring of vertices to draw with TRIANGLEFAN
	for (int i = 1;i < NUM_CIRCLE_VERTICES + 2;i++) {
		angle = (i - 1) * (2 * PI) / NUM_CIRCLE_VERTICES;
		circle[3 * i] = sin(angle);
		circle[(3 * i) + 1] = cos(angle);
	}


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

	//Sends the vertex data to the data buffer and tells it that we won't be changing this data often (which affects how the graphics card stores the data)
	glBufferData(GL_ARRAY_BUFFER, circle.size() * sizeof(double), circle.data(), GL_STATIC_DRAW);

	//Defines how to process the data we sent in
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
	glEnableVertexAttribArray(0);

	//Now that we've finished making all of those definitions, tell OpenGL to stop writing things to those objects so that future statements don't accidentally modify them.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	createCircles(circles, VAO);
}

void createCircles(vector<Circle> &circles, int VAO)
{
	vector<double> position(2);
	vector<double> velocity(2);
	vector<float> color(3);
	double angle;
	double max=RAND_MAX;

	srand(time(NULL));

	//Make the uninfected color blue
	color[2] = 1.0;

	for (int i = 0;i < circles.size();i++) {

		//Calculate random position
		position[0] = (rand() / max) * 2 - 1;
		position[1] = (rand() / max) * 2 - 1;

		//Create circle object
		circles[i] = Circle(position, CIRCLE_RADIUS, VAO);

		//Calculate random velocity angle
		angle = (rand() / max) * 2 * PI;

		//Calculate Cartesian components of velocity
		velocity[0] = cos(angle);
		velocity[1] = sin(angle);

		//Set velocity result
		circles[i].setVelocity(velocity);

		//Set color to be uninfected;
		circles[i].setColor(color);
	}

	//Check for circle overlap before the program starts
	circleCollision(circles, false, 0.0, 0.0);

	color[2] = 0.0;
	color[0] = 1.0;

	//Start an infection. Note that I've done this after the collision detection has already run once, so that any circles that were initially overlapping don't infect each other
	circles[0].setColor(color);
}

void circleMotion(vector<Circle> &circles, bool immunity, float infection_chance, float average_recovery)
{
	circleCollision(circles, immunity, infection_chance, average_recovery);

	vector<double> position;
	vector<double> velocity;

	for (int circle = 0;circle < circles.size();circle++) {
		position = circles[circle].getPosition();
		velocity = circles[circle].getVelocity();
		position[0] = position[0] + velocity[0] * sim_speed * CIRCLE_SPEED;
		position[1] = position[1] + velocity[1] * sim_speed * CIRCLE_SPEED;

		circles[circle].setPosition(position);
	}
}

void circleCollision(vector<Circle> &circles, bool immunity, float infection_chance, float average_recovery)
{
	vector<double> position(2);
	vector<double> distance(2);
	vector<double> velocity(2);
	vector<double> other_velocity(2);
	vector<float> red(3);
	vector<float> green(3);
	double radius;
	double overlap;
	double dot;
	double magnitude;

	double max = RAND_MAX;

	red[0] = 1.0;
	green[1] = 1.0;

	for (int circle = 0;circle < circles.size();circle++) {

		//Poll the current attributes of the circle of interest
		position = circles[circle].getPosition();
		velocity = circles[circle].getVelocity();
		radius = circles[circle].getRadius();

		//Check for collisions between circles. By starting the second loop after the current position of the first loop, I don't check for the same collision twice
		for (int other_circle = circle+1;other_circle < circles.size();other_circle++) {

			//Calculates vector between the two circles
			distance[0] = position[0] - circles[other_circle].getPosition()[0];
			distance[1] = position[1] - circles[other_circle].getPosition()[1];

			//The magnitude of the distance vector
			magnitude = sqrt(distance[0] * distance[0] + distance[1] * distance[1]);

			//The amount of overlap between the two circles
			overlap = (circles[circle].getRadius() + circles[other_circle].getRadius())-magnitude;
				
			//Rounding error is in the 1e-17 spot, so this avoids weird rounding errors that might not shift the circles quite all of the way out of each other
			if (overlap>1e-16) {
				//Poll the velocity of the other circle
				other_velocity = circles[other_circle].getVelocity();

				//Convert the displacement vector to a unit vector
				distance[0] = distance[0] / magnitude;
				distance[1] = distance[1] / magnitude;

				//Shift the position to avoid clipping
				position[0] = position[0] + distance[0] * overlap;
				position[1] = position[1] + distance[1] * overlap;

				//Compute the dot product between the velocity and the normal vector to the plane of incidence
				dot = velocity[0] * (-distance[0]) + velocity[1] * (-distance[1]);

				//Adjust the velocity using the reflection formula
				velocity[0] = velocity[0] - 2 * dot * (-distance[0]);
				velocity[1] = velocity[1] - 2 * dot * (-distance[1]);

				//Compute the dot product between the other velocity and the normal vector to the plane of incidence
				dot = other_velocity[0] * distance[0] + other_velocity[1] * distance[1];

				//Adjust the other velocity using the reflection formula
				other_velocity[0] = other_velocity[0] - 2 * dot * distance[0];
				other_velocity[1] = other_velocity[1] - 2 * dot * distance[1];

				//Set the velocity for the other circle
				circles[other_circle].setVelocity(other_velocity);

				//Check for infection transmission
				if ((circles[circle].getColor()[0] + circles[other_circle].getColor()[0] == 1.0)) {
					if (rand() / max < infection_chance) {
						if (immunity) {
							//No chance of reinfection
							if (circles[circle].getColor() != green) {
								circles[circle].setColor(red);
							}
							if (circles[other_circle].getColor() != green) {
								circles[other_circle].setColor(red);
							}
						}
						else {
							circles[circle].setColor(red);
							circles[other_circle].setColor(red);
						}
					}
				}
			}
			
		}


		//Checks for collisions between the circles and the sides of the screen
		//I've intentionally put this last, as I want the circles to stay inside the screen more than I care about them slightly clipping into each other
		if (position[0] < -1.0 + radius) {
			position[0] = -1.0 + radius;
			velocity[0] = -velocity[0];
		}else if (position[0] > 1.0 - radius) {
			position[0] = 1.0 - radius;
			velocity[0] = -velocity[0];
		}

		if (position[1] < -1.0 + radius) {
			position[1] = -1.0 + radius;
			velocity[1] = -velocity[1];
		}else if (position[1] > 1.0 - radius) {
			position[1] = 1.0 - radius;
			velocity[1] = -velocity[1];
		}

		//Set the circle attributes as calculated
		circles[circle].setPosition(position);
		circles[circle].setVelocity(velocity);

		//Check for recovered
		if ((circles[circle].getColor() == red) && rand() / max < 1 / (average_recovery * FRAMERATE) * sim_speed) {
			circles[circle].setColor(green);
		}

	}
}

void drawCircles(vector<Circle> &circles, int shaderProgram) {
	//Generate the model matrix for movement around the screen (i.e. the coordinates of where my object origin should reside)
	//Initialize to the identity matrix to be modified by later object calls
	float model_matrix[4][4];
	
	for (int circle = 0;circle < circles.size();circle++) {
		//Reset model matrix to the identity matrix
		for (int i = 0;i < 4;i++) {
			for (int j = 0;j < 4; j++) {
				if (i == j) {
					model_matrix[i][j] = 1.0;
				}
				else {
					model_matrix[i][j] = 0.0;
				}
			}
		}

		//Tells OpenGL how to get the data properly transmitted
		glBindVertexArray(circles[circle].getVertexData());

		//Update the model matrix
		for (int i = 0;i < 3;i++) {
			model_matrix[i][i] = (float)circles[circle].getRadius();
		}
		model_matrix[3][0] = (float)circles[circle].getPosition()[0];
		model_matrix[3][1] = (float)circles[circle].getPosition()[1];

		//Pass the Model/View matrix into the shader
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvMatrix"), 1, GL_FALSE, *model_matrix);

		//Pass the color from the circle object into the shader
		glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, circles[circle].getColor().data());


		//Draw the circle. Yay!
		glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_CIRCLE_VERTICES + 2);
	}

}
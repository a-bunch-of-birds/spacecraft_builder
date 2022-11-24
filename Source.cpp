

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tiny_loader.h"
#include "tiny_obj_loader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image.h"
#include "object_model.h"
#include "object_model_library.h"
#include "glfw_class.h"
#include "shader_builder.h"
#include "save_point.h"



GLuint vao;
GLuint skybox_vao;
GLuint skybox_vbo;
GLuint vertexShader, fragmentShader, shaderProgram, skyboxShader;

//MVP uniforms
GLuint modelLoc, viewLoc, projectionLoc;
GLuint sbviewLoc, sbprojectionLoc;
//frag shader uniforms

GLuint viewposLoc;

//camera positioning
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

float radius = 5.0f;
float phi = (3.14f / 2);
float theta = 0;

//drawmode parameter for draw function
GLuint drawmode;

//<-- vector picker integer
int counter = 1;
int vector_picker = -1;

//<-- deletion marker for delete function
int deletion_marker = 0;

//No Model Selected variables for ImGui
glm::vec3 no_model_translated(0.0f, 0.0f, 0.0f);
GLfloat no_model_scaled = 0.0f;
GLfloat no_model_rotation_x = glm::radians(0.0f);
GLfloat no_model_rotation_y = glm::radians(0.0f);
GLfloat no_model_rotation_z = glm::radians(0.0f);


//texture IDs
unsigned int texture_one;

//object declarations
library lib;
save_point save;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
};

void processInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	const GLfloat cameraSpeed = 0.1;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)

		radius -= 0.25f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)

		radius += 0.25f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		theta += 0.1f;
	};
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		theta -= 0.1f;
	};
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		phi += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		phi -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		drawmode = 1;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		drawmode = 2;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		drawmode = 0;

};

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
};

static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
};

void init(gl_object* window)
{
	//glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	try
	{
		shaderProgram = LoadShader("panshader.vert", "panshader.frag");
	}
	catch (std::exception& e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	viewLoc = glGetUniformLocation(shaderProgram, "view");

	projectionLoc = glGetUniformLocation(shaderProgram, "projection");

	viewposLoc = glGetUniformLocation(shaderProgram, "viewPos");


	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("metal.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glUseProgram(shaderProgram); //<-- use shader to send uniform to shader

	glUniform1i(glGetUniformLocation(shaderProgram, "texture_one"), 0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true); //we need to access the GLFW* window component of our gl_object* window
	ImGui_ImplOpenGL3_Init("#version 330");


}

void display()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//DEPTH TESTING
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgram);

	glm::vec3 view_position_value = cameraPos;
	glUniform3fv(viewposLoc, 1, &view_position_value[0]);

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	float cam_x = radius * sin(phi) * cos(theta);
	float cam_y = radius * cos(phi);
	float cam_z = radius * sin(phi) * sin(theta);

	view = glm::lookAt(glm::vec3(cam_x, cam_y, cam_z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
		100.0f);

	//glUseProgram(shaderProgram);

	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	glBindTexture(GL_TEXTURE_2D, texture_one);
	lib.current_render(drawmode, modelLoc);
	glBindTexture(GL_TEXTURE_2D, 0);

	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	//ImGui implementation begins here ----->
	//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

	ImGui::Begin("Editor Menu", nullptr, ImGuiWindowFlags_NoCollapse);
	ImGui::Columns(3);
	ImGui::SetColumnOffset(1, 200);
	{
		//<-- this is a self-contained loop that can use indices -->
		//no need to change this to iterators
		for (int i = 0; i < lib.populate_files().size(); i++)
		{
			if (ImGui::Button(lib.populate_files()[i].c_str()))
			{
				lib.init_load_assign(lib.populate_files()[i]); //for some reason this only works with unique filenames
			}
		};
		//<-- this is self-contained and works just fine -->
		if (ImGui::Button("SAVE"))
		{
			save.read_in_obj(lib);
			save.print_to_obj("test_save.obj");
		};
	}

	ImGui::NextColumn();
	{
		if (!lib.seize_lib().empty())
		{
			if (ImGui::Button("DELETE"))
			{

				vector_picker = -1;
				if (lib.seize_lib().size() == 1)
				{
					lib.seize_lib().clear();
				}

				else
				{
					lib.model_indiv_delete(deletion_marker);
				}

			}
			ImGui::SameLine(0.0f, -5.0f);
			if (ImGui::Button("CLEAR ALL"))
			{
				vector_picker = -1;
				lib.seize_lib().clear();
			}
		}

		for (int j = 0; j < lib.seize_lib().size(); j++)
		{
			lib.seize_lib()[j].unique_ID = lib.seize_lib()[j].get_filename().c_str() + std::to_string(j + 1);

			if (ImGui::Button(lib.seize_lib()[j].unique_ID.c_str()))
			{
				vector_picker = j;
				deletion_marker = j;
				//cout << deletion_marker << endl; //<-- for debugging purposes
			}
		}
	}
	ImGui::NextColumn();
	{
		if (ImGui::Button("RETURN VIEW TO ORIGIN"))
		{
			radius = 5.0f;
			phi = (3.14f / 2);
			theta = 0;
		}
		ImGui::SameLine(0.0f, -5.0f);
		if (ImGui::Button("RETURN MODEL TO ORIGIN"))
		{
			if (vector_picker < 0)
			{
				cout << "not a valid index" << endl;
			}
			else
			{
				lib.seize_lib()[vector_picker].get_transl().x = 0.0f;
				lib.seize_lib()[vector_picker].get_transl().y = 0.0f;
				lib.seize_lib()[vector_picker].get_transl().z = 0.0f;
				lib.seize_lib()[vector_picker].rotation_var_x = glm::radians(0.0f);
				lib.seize_lib()[vector_picker].rotation_var_y = glm::radians(0.0f);
				lib.seize_lib()[vector_picker].rotation_var_z = glm::radians(0.0f);
			}
		}

		if (vector_picker < 0)
		{
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("NO MODEL LOADED", tab_bar_flags))
			{
				if (ImGui::BeginTabItem("Translations"))
				{
					ImGui::PushID("translate");
					ImGui::SliderFloat("No Model Selected##1", &no_model_translated.x, -30.0, 30.0); //<---- locally resolving ID conflicts with ##1
					static char buf1[64] = "";
					ImGui::InputText("No Model Selected##4", buf1, 64, ImGuiInputTextFlags_CharsDecimal);
					ImGui::SliderFloat("No Model Selected##2", &no_model_translated.y, -30.0, 30.0);
					static char buf2[64] = "";
					ImGui::InputText("No Model Selected##5", buf2, 64, ImGuiInputTextFlags_CharsDecimal);
					ImGui::SliderFloat("No Model Selected##3", &no_model_translated.z, -30.0, 30.0);
					static char buf3[64] = "";
					ImGui::InputText("No Model Selected##6", buf3, 64, ImGuiInputTextFlags_CharsDecimal);
					ImGui::PopID();

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Scaling"))
				{
					ImGui::PushID("scale");
					ImGui::SliderFloat("No Model Selected", &no_model_scaled, 0.01, 5);
					ImGui::PopID();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Rotations"))
				{
					ImGui::PushID("rotate_x"); //<-- resolving ID conflict with the PushID/PopID system
					ImGui::SliderFloat("No Model Selected", &no_model_rotation_x, 0, 360); //<-- that way, these SliderFloats can have the same label
					ImGui::PopID();
					ImGui::PushID("rotate_y");
					ImGui::SliderFloat("No Model Selected", &no_model_rotation_y, 0, 360);
					ImGui::PopID();
					ImGui::PushID("rotate_z");
					ImGui::SliderFloat("No Model Selected", &no_model_rotation_z, 0, 360);
					ImGui::PopID();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

		}
		else if (vector_picker >= 0)
			//else
		{
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar(lib.seize_lib()[vector_picker].get_filename().c_str(), tab_bar_flags))
			{
				if (ImGui::BeginTabItem("Translations"))
				{
					ImGui::PushID(vector_picker + "translate");
					ImGui::SliderFloat("Translate X-Axis", &lib.seize_lib()[vector_picker].get_transl().x, -30.0, 30.0);
					ImGui::InputFloat("X-Axis Input", &lib.seize_lib()[vector_picker].get_transl().x, 0.01f, 1.0f, "%.3f");
					ImGui::SliderFloat("Translate Y-Axis", &lib.seize_lib()[vector_picker].get_transl().y, -30.0, 30.0);
					ImGui::InputFloat("Y-Axis Input", &lib.seize_lib()[vector_picker].get_transl().y, 0.01f, 1.0f, "%.3f");
					ImGui::SliderFloat("Translate Z-Axis", &lib.seize_lib()[vector_picker].get_transl().z, -30.0, 30.0);
					ImGui::InputFloat("Z-Axis Input", &lib.seize_lib()[vector_picker].get_transl().z, 0.01f, 1.0f, "%.3f");
					ImGui::PopID();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Scaling"))
				{
					ImGui::PushID(vector_picker + "scale");
					ImGui::SliderFloat("Scale", &lib.seize_lib()[vector_picker].scaling_var, 0.01, 5);
					ImGui::InputFloat("Scaling Input", &lib.seize_lib()[vector_picker].scaling_var, 0.01f, 1.0f, "%.3f");
					ImGui::PopID();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Rotations"))
				{
					ImGui::PushID(vector_picker + "rotate_x");
					ImGui::SliderFloat("Rotate (x-axis)", &lib.seize_lib()[vector_picker].rotation_var_x, 0, 360);
					ImGui::InputFloat("X-Axis Rotation Input", &lib.seize_lib()[vector_picker].rotation_var_x, 0.01f, 1.0f, "%.3f");
					ImGui::PopID();
					ImGui::PushID(vector_picker + "rotate_y");
					ImGui::SliderFloat("Rotate (y-axis)", &lib.seize_lib()[vector_picker].rotation_var_y, 0, 360);
					ImGui::InputFloat("Y-Axis Rotation Input", &lib.seize_lib()[vector_picker].rotation_var_y, 0.01f, 1.0f, "%.3f");
					ImGui::PopID();
					ImGui::PushID(vector_picker + "rotate_z");
					ImGui::SliderFloat("Rotate (z-axis)", &lib.seize_lib()[vector_picker].rotation_var_z, 0, 360);
					ImGui::InputFloat("Z-Axis Rotation Input", &lib.seize_lib()[vector_picker].rotation_var_z, 0.01f, 1.0f, "%.3f");
					ImGui::PopID();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

		}
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



	glDisableVertexAttribArray(0);
	glUseProgram(0);


}

int main()
{
	gl_object* window = new gl_object(1024, 768, "window");

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	};



	window->setErrorCallBack(error_callback);
	window->setRenderer(display);
	window->setKeyCallBack(processInput);
	window->setReshapeCallback(reshape);

	init(window);

	window->eventLoop();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	delete(window);

	return 0;

}
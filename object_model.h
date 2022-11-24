#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H
#include "tiny_obj_loader.h"
#include "tiny_loader.h"
//#include <glm\glm.hpp>
#include "glm/glm.hpp"
#include <string>
#include <iostream>

class object_model
{
private:
	TinyObjLoader object;
	glm::mat4 object_model_matrix;
	std::string obj_filename;
	
	bool draw_state;
	//put the variables in public for experimentation
	//GLfloat scaling_var;
	//glm::vec3 translation_var;
	//GLfloat rotation_var;
	//GLfloat x_axis_toggle;
	//GLfloat y_axis_toggle;
	//GLfloat z_axis_toggle;
public:
	//absolute ImGui bullshit
	std::string unique_ID;
	GLfloat scaling_var;
	glm::vec3 translation_var;
	GLfloat rotation_var_x;
	GLfloat rotation_var_y;
	GLfloat rotation_var_z;
	//GLfloat x_axis_toggle;
	//GLfloat y_axis_toggle;
	//GLfloat z_axis_toggle;
	
	object_model();
	~object_model();
	TinyObjLoader get_object();
	glm::mat4& get_object_model_matrix();
	glm::vec3& get_transl();
	void set_transl(glm::vec3 transl);
	std::string get_filename();
	bool get_draw_state();
	void set_draw_state();
	void set_filename(std::string inputfile);
	glm::mat4 translate(glm::vec3 xyz_axes);
	glm::mat4 scale(GLfloat scaling_factor);
	glm::mat4 rotate(GLfloat degrees, GLfloat x_axis, GLfloat y_axis, GLfloat z_axis);
	//glm::mat4 transform(glm::vec3 xyz_axes, GLfloat scaling_factor, GLfloat degrees, GLfloat x_axis, GLfloat y_axis, GLfloat z_axis);
	void transform(object_model& model);
	void exp_transform(glm::vec3 xyz_axes, GLfloat scaling_factor, GLfloat degrees, GLfloat x_axis, GLfloat y_axis, GLfloat z_axis);
	void load();
	void draw_object(int drawmode);
};

#endif
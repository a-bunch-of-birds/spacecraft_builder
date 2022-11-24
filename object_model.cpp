#include "object_model.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <string>
#include <iostream>

object_model::object_model()
{
	this->object = object;
	object_model_matrix = glm::mat4(1.0f);
	this->obj_filename = obj_filename;
	this->unique_ID = unique_ID;
	
	scaling_var = 0.5;
	translation_var = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation_var_x = glm::radians(0.0f);
	rotation_var_y = glm::radians(0.0f);
	rotation_var_z = glm::radians(0.0f);
	
}

object_model::~object_model()
{

}

TinyObjLoader object_model::get_object()
{
	return object;
}

glm::mat4& object_model::get_object_model_matrix()
{
	return object_model_matrix;
}


glm::vec3& object_model::get_transl()
{
	return translation_var;
}

void object_model::set_transl(glm::vec3 transl)
{
	translation_var = transl;
}

std::string object_model::get_filename()
{
	return obj_filename;
}

bool object_model::get_draw_state()
{
	return draw_state;
}

void object_model::set_draw_state()
{
	draw_state = true;
}

void object_model::set_filename(std::string inputfile)
{
	obj_filename = inputfile;
}

glm::mat4 object_model::translate(glm::vec3 xyz_axes)
{
	
	object_model_matrix = glm::translate(object_model_matrix, translation_var);
	
	return object_model_matrix;
}
glm::mat4 object_model::scale(GLfloat scaling_factor)
{
	
	object_model_matrix = glm::scale(object_model_matrix, glm::vec3(scaling_var, scaling_var, scaling_var));
	
	return object_model_matrix;
}

void object_model::transform(object_model& model)
{
	
	glm::mat4 pass_off_trans = glm::mat4(1.0f);
	glm::mat4 pass_off_rot = glm::mat4(1.0f);
	glm::mat4 pass_off_scal = glm::mat4(1.0f);
	
	pass_off_trans = glm::translate(pass_off_trans, model.translation_var);
	
	pass_off_rot = glm::rotate(pass_off_rot, glm::radians(model.rotation_var_x), glm::vec3(1.0f, 0.0f, 0.0f));
	pass_off_rot = glm::rotate(pass_off_rot, glm::radians(model.rotation_var_y), glm::vec3(0.0f, 1.0f, 0.0f));
	pass_off_rot = glm::rotate(pass_off_rot, glm::radians(model.rotation_var_z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	pass_off_scal = glm::scale(pass_off_scal, glm::vec3(model.scaling_var, model.scaling_var, model.scaling_var));
	
	model.object_model_matrix = pass_off_scal * pass_off_trans * pass_off_rot; //rotation goes last so everything rotates around its own axes
	
}

void object_model::exp_transform(glm::vec3 xyz_axes, GLfloat scaling_factor, GLfloat degrees, GLfloat x_axis, GLfloat y_axis, GLfloat z_axis)
{
	
	object_model_matrix = glm::translate(object_model_matrix, xyz_axes);
	object_model_matrix = glm::rotate(object_model_matrix, glm::radians(degrees), glm::vec3(x_axis, y_axis, z_axis));
	object_model_matrix = glm::scale(object_model_matrix, glm::vec3(scaling_factor, scaling_factor, scaling_factor));
}

void object_model::load()
{
	object.load_obj(obj_filename);
}

void object_model::draw_object(int drawmode)
{
	object.drawObject(drawmode);
}
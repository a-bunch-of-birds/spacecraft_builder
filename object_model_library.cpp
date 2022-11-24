#include "object_model_library.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

library::library()
{
	this->obj_library = obj_library;
	this->files = files;
}

library::~library()
{
}

std::vector<object_model>& library::seize_lib()
{
	return obj_library;
}

std::vector<std::string> library::populate_files()
{
	return files;
}

void library::init_load_assign(std::string filename)
{
	object_model object;
	object.set_filename(filename);
	object.load();
	obj_library.push_back(object);
}

void library::current_render(int drawmode, GLuint modelLoc)
{
	for (auto i = 0; i < obj_library.size(); i++)
	{
		obj_library[i].transform(obj_library[i]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &obj_library[i].get_object_model_matrix()[0][0]);
		obj_library[i].draw_object(drawmode);
	}
}

void library::model_indiv_delete(int index)
{	
	auto it = obj_library.begin();
	std::advance(it, index);
	obj_library.erase(it);	
}


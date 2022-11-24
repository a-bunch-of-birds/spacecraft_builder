#pragma once

#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>


#include "glfw_class.h"
#include "object_model.h"
#include "object_model_library.h"

using namespace std;

class save_point
{
private:
	string inbound_file;
	string outbound_file;
	vector<glm::vec3> object_vertices;
	vector<glm::vec2> object_texture_coordinates;
	vector<glm::vec3> object_normals;
	vector<unsigned int> object_faces;
	
	struct face_definition
	{
		int x, y, z;
		int line_index;
	};
	
	
	vector<face_definition> faces;
	
public:
	save_point();
	~save_point();
	vector<glm::vec3> get_vector_data();
	vector<glm::vec2> get_texture_data();
	vector<glm::vec3> get_normals();
	vector<unsigned int> get_indices();
	ifstream stream_val();
	void set_inbound();
	string get_inbound(int mode);
	void set_outbound();
	string get_outbound(int mode);
	//void read_in_obj(std::initializer_list<string> infiles);
	void read_in_obj(library library);
	GLuint commit(glm::mat4 model, glm::mat4 view);
	void print_to_screen();
	void print_to_file();
	void print_to_obj(string outfile);
	int f_parsing_excl();
	int f_offset(bool& not_again);
	void name_file();
	void print_face_definitions();
};


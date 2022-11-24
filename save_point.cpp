
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
#include <cstdarg>
#define GLM_SWIZZLE_XYZW
//#include <glm/glm.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "tiny_obj_loader.h"
#include "save_point.h"
#include "glfw_class.h"
#include "object_model.h"
#include "object_model_library.h"


using namespace std;


save_point::save_point()
{
	
	this->object_vertices = object_vertices;
	this->object_texture_coordinates = object_texture_coordinates;
	this->object_normals = object_normals;
	this->object_faces = object_faces;


}

save_point::~save_point()
{
	/*
	DESTRUCTOR NOT CONSTRUCTED

	... IRONICALLY
	*/
};

vector<glm::vec3> save_point::get_vector_data()
{
	return object_vertices;
};

vector<glm::vec2> save_point::get_texture_data()
{
	return object_texture_coordinates;
};

vector<glm::vec3> save_point::get_normals()
{
	return object_normals;
};

vector<unsigned int> save_point::get_indices()
{
	return object_faces;
};

void save_point::read_in_obj(library library)
{
	int counter_v = 0;
	int counter_vt = 0;
	int counter_vn = 0;
	int counter_f = 0;

	bool first_time_around = false; 
	//see below
	/*
	when it comes to calculating the necessary offset to add to the face definitions of successive objects,
	we need to ensure that we calculate it and only apply it to the files successively ONLY once the first file has been processed.
	We don't want to slap the offset onto the first file's face definitions.  first_time_around is the flag that ensures that the offset is
	calculated but NOT APPLIED until the second file is read in and processed.
	*/

	for (auto param : library.seize_lib()) { //for each of the infile parameters in the std::initializer_list; i.e. process each file (in order)


		vector<glm::vec4> att_v_trans; // vector for storing TRANSFORMED vertices

		glm::mat4 model = glm::mat4(1.0f); //new identity matrix for the model upon which we impose our transformations
		model = param.get_object_model_matrix();
		//model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); //simple test translation to go 1 space right of the origin on the x-axis
		//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0)); //simple test translation to rotate 45 degrees along the y-axis
		//glm::vec3 norm_vert; //vec3 for passing new vertex normals after calculations for transformations
		glm::mat3 normal_matrix = glm::mat3(1.0f); //mat3 normal matrix initialized to an identity matrix 3x3

		string line; //generic string container for each line of our OBJ file
		ifstream instr(param.get_filename()); //set up the ifstream from our initial OBJ file

		while (getline(instr, line)) //while loop to retrieve each line until .eof
		{
			instr.setf(std::ios_base::fixed, std::ios_base::floatfield); //setting parameters for the ifstream to deal with floating point numbers

			if (!line.empty()) //double check that we operate on a line that isn't just whitespace - disregard whitespace lines
			{

				std::string v_x, v_y, v_z; //strings to store each x, y, and z component of the vec3s/vec4s
				std::string vt_x, vt_y; //strings to store each x and y component of the vec2s
				std::string vn_x, vn_y, vn_z; //strings to store each x, y, and z component of the vec3s/vec4s
				std::string::size_type sz; //required size_type string to use std::stof

				if (line.substr(0, 2) == "v ") //first conditional  to find the vertices in the obj file via an identifying substring
				{
					//counter_v++; //<-- old way of calculating offset; abandoned for apparent reasons
					stringstream iss_vertex(line.substr(2)); //start string stream with an offset of the initial substring
					iss_vertex >> v_x >> v_y >> v_z; //push strings matching vertices into the stringstream
					float v_one, v_two, v_three; //create generic containers of type float to store vertices
					glm::vec4 vec_transfer; //create a vec4 container to transfer the substrings converted to floats into the x, y, z components of a vec4
					v_one = std::stof(v_x, &sz); //convert the strings into floats
					v_two = std::stof(v_y, &sz);
					v_three = std::stof(v_z, &sz);
					vec_transfer.x = v_one; //transfer floats into the vec4 container
					vec_transfer.y = v_two;
					vec_transfer.z = v_three;
					vec_transfer.w = 1.0f; //ensure that the vec4s are in homogeneous coordinates

					glm::vec4 result_v = model * vec_transfer; //create vec4 container that will apply the transformation matrix to each vec4

					//EXPERIMENTAL TRYING TO KEEP VEC4s AS VEC3s FOR MESH CLASS
					glm::vec3 result_v_sans_w(result_v);

					object_vertices.push_back(result_v_sans_w); //push back the resulting vec3s into their appropriate vector<glm::vec4>

				}

				else if (line.substr(0, 2) == "vt") //second conditional to find texture coordinates via an identifying substring
				{
					//counter_vt++; //<-- old way of calculating offset
					stringstream iss_texture(line.substr(3)); //start the stringstream with an offset of the initial substring
					iss_texture >> vt_x >> vt_y; //push strings matching tex coords into the stringstream

					float vt_one, vt_two; //create float containers for each tex coord
					glm::vec2 tex_transfer; //create vec2 container to transfer the substrings into x and y components of the vec2s
					vt_one = std::stof(vt_x, &sz); //convert strings to floats
					vt_two = std::stof(vt_y, &sz);
					tex_transfer.x = vt_one; //transfer floats into the appropriate components of the vec2s
					tex_transfer.y = vt_two;
					object_texture_coordinates.push_back(tex_transfer); //push back the vec2s into their appropriate vector<glm::vec2>

				}


				else if (line.substr(0, 2) == "vn") //third conditional to find the normals via an identifying substring
				{
					//counter_vn++; //<-- old way of calculating offset
					stringstream iss_normal(line.substr(3)); //start the stringstream with an offset of the initial substring
					iss_normal >> vn_x >> vn_y >> vn_z; //pull strings matching vertex normals from the stringstream
					float vn_one, vn_two, vn_three; //create float containers for each normal
					glm::vec3 norm_transfer; //create a vec3 container to transfer the substrings into x, y, and z components of the vec3s
					vn_one = std::stof(vn_x, &sz); //convert strings to floats
					vn_two = std::stof(vn_y, &sz);
					vn_three = std::stof(vn_z, &sz);
					norm_transfer.x = vn_one; //transfer floats into the appropriate components of the vec4s
					norm_transfer.y = vn_two;
					norm_transfer.z = vn_three;

					normal_matrix = glm::mat3(transpose(inverse(model))); //create normal matrix as the transpose-inverse of the transformed model matrix
					glm::vec3 result_n = normal_matrix * norm_transfer; //assign the result of multiplying each vec3 normal by the normal matrix

					object_normals.push_back(result_n);
				}

				else if (line.substr(0, 2) == "f ") //fourth conditional to find the face definitions via an identifying substring
				{
					vector<string> face_def_substr; //string vector for storing each face definition as a substring
					string item; //string transfer container
					char space_delimiter = ' '; //space delimiter set for getline() function; splits the line up into substrings by each vector face definition
					stringstream iss_face(line.substr(2)); //start the stringstream iss_face with an offset of 2
					while (getline(iss_face, item, space_delimiter)) //while loop to retrieve a substring from the line per each space_delimiter
					{
						face_def_substr.push_back(item); //push back whole face definition substring into the string vector
						//cout << item << " "; //for testing
					}
					for (auto i = 0; i < face_def_substr.size(); i++) //for loop to process our face def substrings
					{
						int x, y, z; //integers to store vertex indices, vertex texture indices, and vertex normal indices respectively
						string no_texture = "//";
						string slash = "/";
						string::size_type find_slash = face_def_substr[i].find(slash, 0);
						string::size_type find_no_texture = face_def_substr[i].find(no_texture, 0);
						const char* retrieve = face_def_substr[i].c_str(); //retrieve each substring from the string vector and convert into a c-string
						if (find_slash != string::npos)
						{
							if (find_no_texture != string::npos)
							{
								sscanf_s(retrieve, "%i//%i", &x, &z); //process a face definition substring where vertex texture indices are not included

								face_definition f_d;

								if (first_time_around == true) // iff we have already processed the first file, we can now add the offset to successive indices
								{
									f_d.x = x + counter_v;
									f_d.y = -1; //the vertex texture index will have a value of '-1' in the struct for processing later
									f_d.z = z + counter_vn;
									f_d.line_index = counter_f;
								}
								else if (first_time_around == false)
								{
									f_d.x = x;
									f_d.y = -1; //the vertex texture index will have a value of '-1' in the struct for processing later
									f_d.z = z;
									f_d.line_index = counter_f;
								}

								faces.push_back(f_d);
								object_faces.push_back(f_d.x); //EXPERIMENTAL: TRYING TO GET THE VERTEX INDEX TO AN EBO IN MESH_CLASS

							}
							else //else if there are texture indices included in the face definitions
							{
								sscanf_s(retrieve, "%i/%i/%i", &x, &y, &z); //conditional for processing a face definition substring where vertex texture indices are included

								face_definition f_d; //create new face_definition struct

								if (first_time_around == true) // iff we have already processed the first file, we can now apply the offset
								{
									f_d.x = x + counter_v;
									f_d.y = y + counter_vt;
									f_d.z = z + counter_vn;
									f_d.line_index = counter_f;
								}
								else if (first_time_around == false)
								{
									f_d.x = x; //assign vertex index to the struct
									f_d.y = y; //assign vertex texture index to the struct
									f_d.z = z; //assign vertex normal index to the struct
									f_d.line_index = counter_f; //assign the line index so we can keep track of HOW MANY face definitions are per line 
								}

								faces.push_back(f_d); //push back the face_definition struct into the private vector container for faces
								object_faces.push_back(f_d.x);//EXPERIMENTAL: TRYING TO GET THE VERTEX INDEX TO AN EBO IN MESH_CLASS
							}

						}
						else
						{
							sscanf_s(retrieve, "%i", &x); //iff there are only vertex indices
							face_definition f_d; //create new face_definition struct

							if (first_time_around == true)
							{

								f_d.x = x; //assign vertex index to the struct
								f_d.y = -1; //in single face defs, there is no texture index, so value is -1
								f_d.z = -1; //in single face defs, there is no normal index, so value is -1
								f_d.line_index = counter_f; //assign the line index so we can keep track of HOW MANY face definitions are per line

							}
							else if (first_time_around == false)
							{
								f_d.x = x + counter_v;
								f_d.y = -1;
								f_d.z = -1;
								f_d.line_index = counter_f;
							}

							faces.push_back(f_d); //push back the face_definition struct into the private vector container for faces
							object_faces.push_back(f_d.x);//EXPERIMENTAL: TRYING TO GET THE VERTEX INDEX TO AN EBO IN MESH_CLASS
						}
					}

					counter_f++; //the program is done processing the line of text in the obj file.  increment counter.
				}
				else
				{

				}

			}

		};
		instr.clear();
		instr.seekg(0, instr.beg); //go back to the beginning of the file after calculating offset and begin again
		while (getline(instr, line)) //separate while-loop which calculates the offset from the number of vertex indices
									 //placed at the bottom of the function so that offset is not calculated and applied too early
									 //early calculation has led in the past to larger-than-accurate offsets (basically counts
									// the original offset and then would go through and count up the new file and add that 
									// total, which is hilariously higher than necessary
		{
			if (line.substr(0, 2) == "v ")
			{
				counter_v++;
			}
			else if (line.substr(0, 2) == "vt")
			{
				counter_vt++;
			}
			else if (line.substr(0, 2) == "vn")
			{
				counter_vn++;
			}

		};

		first_time_around = true; //we have processed the first file.  the global bool variable is now TRUE, which will trigger the program to include the offsets in the face definition indices

	}

}


void save_point::print_to_obj(string outfile)
{
	//in final function, parameter will be string outfile
	ofstream outbound(outfile);
	outbound << "# save_point class OBJ file" << endl;
	outbound << "# University of Dundee MSc Project" << endl;
	//outbound << "mtllib untitled.mtl" << endl;
	//outbound << "o combined_cube_obj" << endl;
	for (auto i : object_vertices)
	{
		outbound << "v " << i.x << " " << i.y << " " << i.z << endl;
	}
	for (auto j : object_texture_coordinates)
	{
		outbound << "vt " << j.x << " " << j.y << endl;
	}
	for (auto k : object_normals)
	{
		outbound << "vn " << k.x << " " << k.y << " " << k.z << endl;
	}
	outbound << "usemtl None" << endl;
	outbound << "s off" << endl;
	
	int line_sort_counter = 0;  //we have so far processed the face definitions PER VERTEX INDEX; 
								//to output it properly, we need to use the line_index variable in the face_definition structs to print it all out on the same line
	outbound << "f ";
	for (auto p : faces) //for each struct in the faces vector
	{
		
		if (p.line_index > line_sort_counter) //if the line_index is greater than the previous items line_index, end line and print out new line
		{
			line_sort_counter++; //reup the line_sort_counter to account for a line change
			outbound << endl;
			outbound << "f ";
			if (p.y == -1) //determine if the face definition has a vertex texture index
			{
				outbound << p.x << "//" << p.z << " "; //output with correct formatting
			}

			else if (p.y != -1)
			{
				outbound << p.x << "/" << p.y << "/" << p.z << " ";
			}
			continue;
			
	
		}
		else if (p.line_index == line_sort_counter) //if we're still on the same line of face definitions, carry on
		{
			
			if (p.y == -1) //check if the face definition has a vertex texture index
			{
				outbound << p.x << "//" << p.z << " ";
			}

			else if (p.y != -1)
			{
				outbound << p.x << "/" << p.y << "/" << p.z << " ";
			}
			
		}
		
		
	}
	

}

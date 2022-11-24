#ifndef OBJECT_MODEL_LIBRARY_H
#define OBJECT_MODEL_LIBRARY_H
#include "object_model.h"
//#include <tiny_obj_loader.h>
#include "tiny_obj_loader.h"
//#include <tiny_loader.h>
#include "tiny_loader.h"
#include <vector>
#include <string>
#include <iostream>

class library
{
private:
	std::vector<object_model> obj_library;
	std::vector<std::string> files = {"test_save.obj", "capsule.obj", "cassini_body.obj", "deep_space_probe_body.obj", "EM_CIWS.obj", "EM_CIWS_2.obj", "fuel_tank_RCS.obj", "heat_sink_radiator.obj", "ion_drive_nozzle.obj", "jet_fighter.obj", "long_range_antenna.obj", "long_range_dish.obj", "mesh_gantry_mast.obj", "missile.obj", "NE_RCS_gantry.obj", "NW_RCS_gantry.obj", "orbital_satellite_body.obj", "radioisotope_thermoelectric_generator.obj", "RCS.obj", "reentry_platform.obj", "satellite_chassis.obj", "SE_RCS_gantry.obj", "sensor_array.obj", "short_range_dish.obj", "solar_panel.obj", "surface_sensor_array.obj", "SW_RCS_gantry.obj", "ventral_solar_array.obj"};
public:
	library();
	~library();
	std::vector<object_model>& seize_lib();
	std::vector<std::string> populate_files();
	void init_load_assign(std::string filename);
	void current_render(int drawmode, GLuint modelLoc);
	void model_indiv_delete(int index);

};

#endif

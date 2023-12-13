#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp> 
#include <nlohmann/json.hpp>
#include <ObjData/ObjData.hpp>
int main(int argc, char* argv[]) {
	try {
		std::vector<cv::Mat> images;
		std::vector<cv::Mat> slices;
		std::ifstream json_read;
		if (argc == 1) {
			json_read.open(R"(..\..\..\..\prj.cw\tests\test1.json)");
		}
		else if (argc > 1) {
			json_read.open(argv[1]);
		}
		nlohmann::json data = nlohmann::json::parse(json_read);

		int images_count = data["paths"].size();
		double pixel_square = data["pixel_square"];
		double slice_thickness = data["slice_thickness"];
	
		for (int i = 0; i < images_count; i++) {
			cv::Mat image = cv::imread(data["paths"][i], CV_8UC1);
			if (!image.data) {
				std::cout << "Image file not found\n";
				return 1;
			}
			images.push_back(image);
		}

		for (int i = 0; i < images_count; i++) {
			cv::Mat mask = images[i] > 100;
			slices.push_back(mask);
		}
		ObjData object(slices, pixel_square, slice_thickness);
		std::cout << "             GENERAL CALCULATION RESULTS              " << std::endl;
		std::cout << "Object volume: " << object.get_object_volume() << std::endl;
		std::cout << "Material volume: " << object.get_material_volume() << std::endl;
		std::cout << "Absolute porosity(pore's volume): " << object.get_absolute_porosity() << std::endl;
		std::cout << "Porosity: " << object.get_porosity() << std::endl;
		std::cout << std::endl;

		std::cout << "             PORES VOLUME DISTRIBUTION              " << std::endl;
		float x[3] = { 1, 0, 0 };
		float y[3] = { 0, 1, 0 };
		float z[3] = { 0, 0, 1 };
		std::vector<double> axis_x = object.get_porosity_distribution(x);
		std::vector<double> axis_y = object.get_porosity_distribution(y);
		std::vector<double> axis_z = object.get_porosity_distribution(z);
		std::cout << "Axis Z: " << std::endl;
		std::cout << "[ ";
		double vol_z = 0;
		for (int i = 0; i < axis_z.size(); i++) {
			vol_z += axis_z[i];
			if (i == axis_z.size() - 1) {
				std::cout << axis_z[i] << " ]" << std::endl;
			}
			else {
				std::cout << axis_z[i] << ", ";
			}
		}
		std::cout << "Check summary volume on axis z = absolute porosity: " << vol_z << std::endl;

		std::cout << "Axis X: " << std::endl;
		double vol_x = 0;
		for (int i = 0; i < axis_x.size(); i++) {
			vol_x += axis_x[i];
		}
		std::cout << "Check summary volume on axis x = absolute porosity: " << vol_x << std::endl;

		std::cout << "Axis Y: " << std::endl;
		double vol_y = 0;
		for (int i = 0; i < axis_y.size(); i++) {
			vol_y += axis_y[i];
		}
		std::cout << "Check summary volume on axis y = absolute porosity: " << vol_y << std::endl;
		std::cout << std::endl;

		std::cout << "             CALCULATING PARTLY POROSITY              " << std::endl;
		int x1 = data["sub_volume"][0];
		int x2 = data["sub_volume"][1];
		int y1 = data["sub_volume"][2];
		int y2 = data["sub_volume"][3];
		int z1 = data["sub_volume"][4];
		int z2 = data["sub_volume"][5];

		int coordinates[6] = { x1, x2, y1, y2, z1, z2 };
		std::pair<double, double> part = object.get_partial_porosity(coordinates);
		std::cout << "Part absolute porosity: " << part.first << std::endl;
		std::cout << "Part porosity: " << part.second << std::endl;
	}
	catch (const std::exception& err) {
		std::cout << err.what();
	}
}

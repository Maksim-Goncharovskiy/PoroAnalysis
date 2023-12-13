#pragma once
#ifndef POROMARKER_OBJECTDATA_LIB_2373
#define POROMARKER_OBJECTDATA_LIB_2373
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp> 

const std::exception incorrect_input_data("Incorrect input data");

class ObjData {
private:
	std::vector<cv::Mat> slices_;
	double pixel_square_ = 0.01;
	double Slice_thickness_ = 0.1;
	double ObjVol = 0.0;
	double PoreVol = 0.0;
	double MaterialVol = 0.0;
	std::vector<double> xVolDistribution;
	std::vector<double> yVolDistribution;
	std::vector<double> zVolDistribution;
public:
	ObjData() = default;
	~ObjData() = default;

	ObjData(const ObjData& other) = default;
	ObjData& operator=(const ObjData& other) = default;

	ObjData(ObjData&& other) = default;
	ObjData& operator=(ObjData&& other) = default;

    // Конструктор с параметрами, вызывает основные функции подсчёта пористости.
    ObjData(std::vector<cv::Mat>& slices, double pixel_square = 0.01, double Slice_thickness = 0.1);

	// Получение объема тела
    double get_object_volume();
	// Получение абсолютной пористости(объема пор)
    double get_absolute_porosity();
	// Получение объема материала в теле
    double get_material_volume();
	// Получение относительной пористости
    double get_porosity();

	// Распределение пор по осям: x, y, z
    std::vector<double> get_porosity_distribution(float axis[3]);

	// Пористость части тела(кубик)
	// first - абсолютный объем пор в выбранной части
	// second - относительная пористость в выбранной части
    std::pair<double, double> get_partial_porosity(int coordinates[6]);

	std::vector<int> get_sizes();

private:
	// Проверка входных данных
	void Check(double& pixel_square, double& Slice_thickness);
	void Check(int coordinates[6]);

    double delta_porosity(cv::Mat& slice);

    void porosity_volume();

    // Calculates pore distribution in material, axis can be chosen
    std::vector<double> porosity_distribution(float axis[3]);


	double partial_porosity(int x1, int x2, int y1, int y2, int z1, int z2);
};


#endif

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <doctest/doctest.h>
#include <ObjData/ObjData.hpp>
double eps = 0.000001;
TEST_CASE("Correct object creation") {
    cv::Mat image = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test1.png)", CV_8UC1);
    cv::Mat mask = image > 100;
    std::vector<cv::Mat> slices = { mask };
    CHECK_THROWS(ObjData(slices, 0, 1));
    CHECK_THROWS(ObjData(slices, 1, 0));
    CHECK_THROWS(ObjData(slices, -2, 1));
    CHECK_THROWS(ObjData(slices, 1, -1));
}
TEST_CASE("Checking the calculation of pore volumes and material") {
    SUBCASE("Only material") {
        cv::Mat image = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test1.png)", CV_8UC1);
        cv::Mat mask = image > 100;
        std::vector<cv::Mat> slices_1 = { mask };
        ObjData object_1(slices_1);
        std::vector<cv::Mat> slices_2 = { mask, mask, mask, mask, mask, mask };
        ObjData object_2(slices_2);
        CHECK(abs(object_1.get_absolute_porosity()) < eps);
        CHECK(abs(object_2.get_absolute_porosity()) < eps);
        CHECK(abs(object_1.get_porosity()) < eps);
        CHECK(abs(object_2.get_porosity()) < eps);
        CHECK(abs(6 * object_1.get_object_volume() - object_2.get_object_volume()) < eps);
        CHECK(abs(object_1.get_material_volume() - object_1.get_object_volume()) < eps);
        CHECK(abs(object_2.get_material_volume() - object_2.get_object_volume()) < eps);
    }
    SUBCASE("Only pores") {
        cv::Mat image = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test2.png)", CV_8UC1);
        cv::Mat mask = image > 100;
        std::vector<cv::Mat> slices_1 = { mask };
        ObjData object_1(slices_1);
        std::vector<cv::Mat> slices_2 = { mask, mask, mask, mask, mask, mask };
        ObjData object_2(slices_2);

        CHECK(abs(object_1.get_porosity() - 1) < eps);
        CHECK(abs(object_2.get_porosity() - 1) < eps);
        CHECK(abs(object_1.get_material_volume()) < eps);
        CHECK(abs(object_2.get_material_volume()) < eps);
        CHECK(abs(object_1.get_absolute_porosity() - object_1.get_object_volume()) < eps);
        CHECK(abs(object_2.get_absolute_porosity() - object_2.get_object_volume()) < eps);
        CHECK(abs(6 * object_1.get_object_volume() - object_2.get_object_volume()) < eps);
    }
    SUBCASE("A little count of pores") {
        cv::Mat image = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test3.png)", CV_8UC1);
        cv::Mat mask = image > 100;
        std::vector<cv::Mat> slices = { mask };
        ObjData object(slices);
        CHECK(object.get_absolute_porosity() > eps);
        CHECK(object.get_porosity() > eps);
        CHECK(object.get_material_volume() > object.get_absolute_porosity());
        CHECK(abs(object.get_material_volume() + object.get_absolute_porosity() - object.get_object_volume()) < eps);
    }
    SUBCASE("A much more pores") {
        cv::Mat image = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test4.png)", CV_8UC1);
        cv::Mat mask = image > 100;
        std::vector<cv::Mat> slices = { mask };
        ObjData object(slices, 1, 1);
        CHECK(object.get_porosity() > 0.9);
        CHECK(object.get_absolute_porosity() > object.get_material_volume());
        CHECK(abs(object.get_material_volume() + object.get_absolute_porosity() - object.get_object_volume()) < eps);
        CHECK(object.get_object_volume() == 950 * 755);
    }
    SUBCASE("Compare two similar pictures") {
        cv::Mat image_1 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test5_1.png)", CV_8UC1);
        cv::Mat image_2 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test5_2.png)", CV_8UC1);
        cv::Mat mask_1 = image_1 > 100;
        cv::Mat mask_2 = image_2 > 100;
        std::vector<cv::Mat> slices_1 = { mask_1 };
        std::vector<cv::Mat> slices_2 = { mask_2 };
        ObjData object_1(slices_1, 1, 1);
        ObjData object_2(slices_2, 1, 1);
        CHECK(object_1.get_absolute_porosity() > object_2.get_absolute_porosity());
        CHECK(object_1.get_porosity() > object_2.get_porosity());
        CHECK(object_1.get_material_volume() < object_2.get_material_volume());
    }
}

TEST_CASE("Checking distribution function") {
    cv::Mat image_1 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test6.png)", CV_8UC1);
    cv::Mat image_2 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test7.png)", CV_8UC1);
    cv::Mat image_3 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test8.png)", CV_8UC1);
    cv::Mat image_4 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test9.png)", CV_8UC1);
    cv::Mat image_5 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test10.png)", CV_8UC1);
    cv::Mat mask_1 = image_1 > 100;
    cv::Mat mask_2 = image_2 > 100;
    cv::Mat mask_3 = image_3 > 100;
    cv::Mat mask_4 = image_4 > 100;
    cv::Mat mask_5 = image_5 > 100;
    std::vector<cv::Mat> slices = { mask_1, mask_2, mask_3, mask_4, mask_5 };
    ObjData object(slices);
    SUBCASE("AXIS Z") {
        float z[3] = { 0, 0, 1 };
        std::vector<double> distrib_z = object.get_porosity_distribution(z);
        CHECK(distrib_z[0] < distrib_z[1]);
        CHECK(distrib_z[1] < distrib_z[2]);
        CHECK(distrib_z[2] < distrib_z[3]);
        CHECK(distrib_z[3] < distrib_z[4]);
        double vol = 0.0;
        for (int i = 0; i < distrib_z.size(); i++) {
            vol += distrib_z[i];
        }
        CHECK(abs(vol - object.get_absolute_porosity()) < eps);
    }
    SUBCASE("Checking X and Y axises") {
        float x[3] = { 1, 0, 0 };
        float y[3] = { 0, 1, 0 };
        float z[3] = { 0, 0, 1 };
        std::vector<double> axis_x = object.get_porosity_distribution(x);
        std::vector<double> axis_y = object.get_porosity_distribution(y);
        std::vector<double> axis_z = object.get_porosity_distribution(z);

        double vol_x = 0;
        for (int i = 0; i < axis_x.size(); i++) {
            vol_x += axis_x[i];
        }

        double vol_y = 0;
        for (int i = 0; i < axis_y.size(); i++) {
            vol_y += axis_y[i];
        }

        double vol_z = 0;
        for (int i = 0; i < axis_z.size(); i++) {
            vol_z += axis_z[i];
        }

        CHECK(abs(vol_x - vol_z) < eps);
        CHECK(abs(vol_y - vol_z) < eps);
        CHECK(abs(vol_x - object.get_absolute_porosity()) < eps);
    }
}

TEST_CASE("Checking partial porosity calculations") {
    cv::Mat image_1 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test6.png)", CV_8UC1);
    cv::Mat image_2 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test7.png)", CV_8UC1);
    cv::Mat image_3 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test8.png)", CV_8UC1);
    cv::Mat image_4 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test9.png)", CV_8UC1);
    cv::Mat image_5 = cv::imread(R"(..\..\..\..\prj.cw\data\forDoctest\test10.png)", CV_8UC1);
    cv::Mat mask_1 = image_1 > 100;
    cv::Mat mask_2 = image_2 > 100;
    cv::Mat mask_3 = image_3 > 100;
    cv::Mat mask_4 = image_4 > 100;
    cv::Mat mask_5 = image_5 > 100;
    std::vector<cv::Mat> slices = { mask_1, mask_2, mask_3, mask_4, mask_5 };
    ObjData object(slices);
    SUBCASE("Max sizes = main obj") {
        int coordinates[6] = { 1, mask_5.rows, 1, mask_5.cols, 1, slices.size() };
        std::pair<double, double> part = object.get_partial_porosity(coordinates);
        CHECK(object.get_absolute_porosity() == part.first);
        CHECK(object.get_porosity() == part.second);
    }
    SUBCASE("Incorrect input data") {
        int coord_1[6] = { mask_5.rows, 1, 1, mask_5.cols, 1, slices.size() };
        int coord_2[6] = {0, 0, 0, 0, 0, 0};
        int coord_3[6] = {1, 5, -1, 100, 1, 1};
        int coord_4[6] = {1, mask_5.rows + 1, 1, mask_5.cols + 1, 1, 1};
        CHECK_THROWS(object.get_partial_porosity(coord_1));
        CHECK_THROWS(object.get_partial_porosity(coord_2));
        CHECK_THROWS(object.get_partial_porosity(coord_3));
        CHECK_THROWS(object.get_partial_porosity(coord_4));
    }
}
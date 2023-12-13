#include <ObjData/ObjData.hpp>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp> 

ObjData::ObjData(std::vector<cv::Mat>& slices, double pixel_square, double Slice_thickness) {
    Check(pixel_square, Slice_thickness);
    slices_ = slices;
    pixel_square_ = pixel_square;
    Slice_thickness_ = Slice_thickness;

    ObjVol = slices_[0].cols * slices_[0].rows * pixel_square_ * Slice_thickness_ * slices_.size();
    porosity_volume();
    MaterialVol = ObjVol - PoreVol;
    float x[3] = { 1, 0, 0 };
    float y[3] = { 0, 1, 0 };
    float z[3] = { 0, 0, 1 };
    xVolDistribution = porosity_distribution(x);
    yVolDistribution = porosity_distribution(y);
    zVolDistribution = porosity_distribution(z);
}

double ObjData::get_object_volume() {
    return ObjVol;
}

double ObjData::get_absolute_porosity() {
    return PoreVol;
}

double ObjData::get_material_volume() {
    return MaterialVol;
}

double ObjData::get_porosity() {
    return PoreVol / ObjVol;
}

std::vector<double> ObjData::get_porosity_distribution(float axis[3]) {
    if (axis[0] == 1) {
        return xVolDistribution;
    }
    else if (axis[1] == 1) {
        return yVolDistribution;
    }
    else if (axis[2] == 1) {
        return zVolDistribution;
    }
}

std::pair<double, double> ObjData::get_partial_porosity(int coordinates[6]) {
    Check(coordinates);
    int x1 = coordinates[0];
    int x2 = coordinates[1];
    int y1 = coordinates[2];
    int y2 = coordinates[3];
    int z1 = coordinates[4];
    int z2 = coordinates[5];
    double part_volume = (x2 - x1 + 1) * (y2 - y1 + 1) * (z2 - z1 + 1) * pixel_square_ * Slice_thickness_;
    double absolute_porosity = partial_porosity(x1, x2, y1, y2, z1, z2);
    return std::make_pair(absolute_porosity, absolute_porosity / part_volume);
}

void ObjData::Check(double& pixel_square, double& Slice_thickness) {
    if (pixel_square <= 0 || Slice_thickness <= 0) {
        throw incorrect_input_data;
    }
}
void ObjData::Check(int coordinates[6]) {
    if ((coordinates[0] > coordinates[1]) || (coordinates[2] > coordinates[3]) || (coordinates[4] > coordinates[5])) {
        throw incorrect_input_data;
    }
    if ((coordinates[0] <= 0) || (coordinates[1] <= 0) || (coordinates[2] <= 0) || (coordinates[3] <= 0) || (coordinates[4] <= 0) || (coordinates[5] <= 0)) {
        throw incorrect_input_data;
    }
    if ((coordinates[1] > slices_[0].rows) || (coordinates[3] > slices_[0].cols) || (coordinates[5] > slices_.size())) {
        throw incorrect_input_data;
    }
}

double ObjData::delta_porosity(cv::Mat& slice) {
    double pores = 0.0;
    for (int i = 0; i < slice.rows; i++) {
        for (int j = 0; j < slice.cols; j++) {
            if (slice.at<uchar>(i, j) == 0) {
                pores += 1 * pixel_square_;
            }
        }
    }
    return pores;
}

void ObjData::porosity_volume() {
    double poresVolume = 0.0;
    for (int i = 0; i < slices_.size(); i++) {
        double res_i = delta_porosity(slices_[i]);
        poresVolume += res_i * Slice_thickness_;
    }
    PoreVol = poresVolume;
}

std::vector<double> ObjData::porosity_distribution(float axis[3]) {
    std::vector<double> volumes;
    if (axis[2] == 1) {
        for (int i = 0; i < slices_.size(); i++) {
            double volume_i = delta_porosity(slices_[i]) * Slice_thickness_;
            volumes.push_back(volume_i);
        }
        return volumes;
    }
    else if (axis[0] == 1) {
        int rows = slices_[0].rows;
        int cols = slices_[0].cols;
        for (int i = 0; i < rows; i++) {
            int count = 0;
            for (int j = 0; j < slices_.size(); j++) {
                for (int k = 0; k < cols; k++) {
                    if (slices_[j].at<uchar>(i, k) == 0) {
                        count += 1;
                    }
                }
            }
            double volume_i = count * pixel_square_ * Slice_thickness_;
            volumes.push_back(volume_i);
        }
        return volumes;
    }
    else if (axis[1] == 1) {
        int rows = slices_[0].rows;
        int cols = slices_[0].cols;
        for (int i = 0; i < cols; i++) {
            int count = 0;
            for (int j = 0; j < slices_.size(); j++) {
                for (int k = 0; k < rows; k++) {
                    if (slices_[j].at<uchar>(k, i) == 0) {
                        count += 1;
                    }
                }
            }
            double volume_i = count * pixel_square_ * Slice_thickness_;
            volumes.push_back(volume_i);
        }
        return volumes;
    }

}

double ObjData::partial_porosity(int x1, int x2, int y1, int y2, int z1, int z2) {
    double pores_volume = 0.0;
    for (int k = z1 - 1; k <= z2 - 1; k++) {
        double pores = 0.0;
        cv::Mat slice = slices_[k];
        for (int i = x1 - 1; i <= x2 - 1; i++) {
            for (int j = y1 - 1; j <= y2 - 1; j++) {
                if (slice.at<uchar>(i, j) == 0) {
                    pores += 1 * pixel_square_;
                }
            }
        }
        pores_volume += pores * Slice_thickness_;
    }
    return pores_volume;
}

std::vector<int> ObjData::get_sizes() {
    int slices_count = slices_.size();
    std::vector<int> sizes = { slices_[0].rows, slices_[0].cols, slices_count };
    return sizes;
}
#ifndef READBIN_H
#define READBIN_H

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

void loadBinFloatType(const std::string& fileName, cv::Mat* im);

double getTotalMax(const std::vector<std::string>& vFileNames, const int& Height, const int& Width);

void floatConvertTo8U(cv::Mat* src, cv::Mat* dist, const double& totalMax);

#endif

#include "ReadBin.h"

void loadBinFloatType(const std::string& fileName, cv::Mat* im){

    std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
    if(!inFile){
        std::cerr << "Wrong File" << std::endl;
    }

    for(int r = 0; r < im->rows; r++){
        inFile.read((char*)(im->ptr<uchar>(r)), im->cols*im->elemSize());
    }
    inFile.close();
}

double getTotalMax(const std::vector<std::string>& vFileNames, const int& Height, const int& Width){
    cv::Mat im(Height, Width, CV_32F);
    double totalMax = 0;
    double max = 0;
    for (int i = 0; i < vFileNames.size(); i++){
        loadBinFloatType(vFileNames[i], &im);
        cv::minMaxLoc(im, NULL, &max);
        if(max > totalMax){
            totalMax = max;
        }
    }
    return totalMax;
}

void floatConvertTo8U(cv::Mat* src, cv::Mat* dist, const double& totalMax){
    src->convertTo(*dist, CV_8U, 255 / totalMax, 0);
}

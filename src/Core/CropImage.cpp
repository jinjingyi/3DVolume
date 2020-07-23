#include "CropImage.h"
#include <vtkInformation.h>

CropImage::CropImage():
    imageData(vtkSmartPointer<vtkImageData>::New()),
    imageViewer(vtkSmartPointer<vtkImageViewer2>::New())
{
    connectComponents();
}

CropImage::~CropImage() = default;

void CropImage::importImage(const std::string &filename){
    std::string ext = toUpper(getFileExtension(filename));
    if (ext == "BMP") {
        vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
        reader->SetFileName(filename.c_str());
        reader->Update();
        imageData->DeepCopy(reader->GetOutput());
    }
    else if (ext == "PNG") {
        vtkSmartPointer <vtkPNGReader> reader = vtkSmartPointer <vtkPNGReader>::New();
        reader->SetFileName(filename.c_str());
        reader->Update();
        imageData->DeepCopy(reader->GetOutput());
    }
    else if (ext == "JPG") {
        vtkSmartPointer <vtkJPEGReader> reader = vtkSmartPointer <vtkJPEGReader>::New();
        reader->SetFileName(filename.c_str());
        reader->Update();
        imageData->DeepCopy(reader->GetOutput());
    }
    else if (ext == "TIF") {
        vtkSmartPointer <vtkTIFFReader> reader = vtkSmartPointer <vtkTIFFReader>::New();
        reader->SetFileName(filename.c_str());
        reader->Update();
        imageData->DeepCopy(reader->GetOutput());
    }
    else if(ext == "BIN") {
        cv::Mat im(WIDTH, HEIGHT, CV_32F);
        loadBinFloatType(filename, &im);
        cv::Mat dist;
        floatConvertTo8U(&im, &dist, binMaxValue);
        vtkNew<vtkInformation> info ;
        imageData->SetDimensions(dist.cols, dist.rows, 1);
        imageData->SetScalarType(VTK_UNSIGNED_CHAR, info);
        imageData->SetNumberOfScalarComponents(1, info);
        imageData->AllocateScalars(info);
        /*********************** 通过指针遍历元素地址修改灰度值 ************************/
        unsigned char *ptr_vtk = static_cast<unsigned char *>(imageData->GetScalarPointer());
        cv::Mat tempImg;
        cv::flip(dist, tempImg, 0);
        cv::MatIterator_ <uchar> itr_mat = tempImg.begin<uchar>();
        for (; itr_mat != tempImg.end<uchar>(); itr_mat++) {
            *ptr_vtk++ = *itr_mat;
        }
        /**************************************************************************/
    }
}

void CropImage::setViewer(){
    imageViewer->SetColorLevel(127);
	imageViewer->SetColorWindow(256);
    imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
}

void CropImage::connectComponents(){
    imageViewer->SetInputData(imageData);
    setViewer();
}

vtkSmartPointer<vtkImageViewer2> CropImage::getImageViewer(){
    return imageViewer;
}

std::array<int, 2> CropImage::getDimensions(){
    int dims[3];
    imageData->GetDimensions(dims);
    return {dims[0], dims[1]};
}

void CropImage::releaseData(){
    imageData->ReleaseData();
}


void CropImage::setBinWidthHeightMaxValue(const int& WIDTH, const int& HEIGHT, const double& binMaxValue){
    this->WIDTH = WIDTH;
    this->HEIGHT = HEIGHT;
    this->binMaxValue = binMaxValue;
}

double CropImage::getBinMaxValue(){
    return binMaxValue;
}




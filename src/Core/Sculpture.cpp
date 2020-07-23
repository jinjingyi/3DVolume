#include "Sculpture.h"
#include "Util/Utils.h"

Sculpture::Sculpture() :
    imageData(vtkSmartPointer<vtkImageData>::New()),
	volume(vtkSmartPointer<vtkVolume>::New()),
	volumeMapper(vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New()),
	volumeProperty(vtkSmartPointer<vtkVolumeProperty>::New()),
	tf(new TransferFunction()),
    loaded(false)
{
	setProperties();
	connectComponents();
}

Sculpture::~Sculpture() = default;

void Sculpture::connectComponents() {
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
}

void Sculpture::setProperties() {
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->SetGradientOpacity(tf->getGradientFun());
	volumeProperty->SetScalarOpacity(tf->getScalarFun());
	volumeProperty->SetColor(tf->getColorFun());
	volumeProperty->ShadeOn();

}

void Sculpture::setImageFolder(const std::vector<std::string> &vfileNames) {
	loaded = false;

    int fileNamesSize = vfileNames.size();
    vtkSmartPointer<vtkStringArray> fileArray = vtkSmartPointer<vtkStringArray>::New();
    for (int i = 0; i < fileNamesSize; i++){
        fileArray->InsertNextValue(vfileNames[i]);
    }
    std::string ext = toUpper(getFileExtension(vfileNames[0]));
    if (ext == "BMP"){
        vtkSmartPointer <vtkBMPReader> imageReader = vtkSmartPointer< vtkBMPReader>::New();
        imageReader->SetFileNames(fileArray);
        imageReader->SetDataSpacing(1, 1, 1);
        imageReader->SetAllow8BitBMP(8) ;
        imageReader->Allow8BitBMPOn();
        imageReader->Update();
        imageData->DeepCopy(imageReader->GetOutput());
    }
    else if (ext == "PNG") {
        vtkSmartPointer <vtkPNGReader> imageReader = vtkSmartPointer <vtkPNGReader>::New();
        imageReader->SetFileNames(fileArray);
        imageReader->SetDataSpacing(1, 1, 1);
        imageReader->SetDataByteOrderToLittleEndian();
        imageReader->Update();
        imageData->DeepCopy(imageReader->GetOutput());
    }
    else if (ext == "JPG") {
        vtkSmartPointer <vtkJPEGReader> imageReader = vtkSmartPointer <vtkJPEGReader>::New();
        imageReader->SetFileNames(fileArray);
        imageReader->SetDataSpacing(1, 1, 1);
        imageReader->SetDataByteOrderToLittleEndian();
        imageReader->Update();
        imageData->DeepCopy(imageReader->GetOutput());
    }
    else if (ext == "TIF") {
        vtkSmartPointer <vtkTIFFReader> imageReader = vtkSmartPointer <vtkTIFFReader>::New();
        imageReader->SetFileNames(fileArray);
        imageReader->SetDataSpacing(1, 1, 1);
        imageReader->SetDataByteOrderToLittleEndian();
        imageReader->Update();
        imageData->DeepCopy(imageReader->GetOutput());
        
    }

    loadVolume();
	loaded = true;
}

void Sculpture::setVTIFile(const std::string &vtiFileName) {
	loaded = false;

	vtkSmartPointer<vtkXMLImageDataReader> imageReader = vtkSmartPointer<vtkXMLImageDataReader>::New();
	imageReader->SetFileName(vtiFileName.c_str());
    imageReader->Update();
    imageData->DeepCopy(imageReader->GetOutput());
    loadVolume();
	loaded = true;
}

void Sculpture::preSetCVMat(int WIDTH, int HEIGHT, int CHANNELS){
    loaded = false;
    vtkNew<vtkInformation> info ;
    imageData->SetDimensions(WIDTH, HEIGHT, CHANNELS);
    imageData->SetScalarType(VTK_UNSIGNED_CHAR, info);
    imageData->SetNumberOfScalarComponents(1, info);
    imageData->AllocateScalars(info);
}

void Sculpture::postSetCVMat(){
    loadVolume();
    loaded = true;
}

void Sculpture::setCVMat(cv::Mat &src, int idx){
    uchar *ptr_vtk = static_cast<uchar *>(imageData->GetScalarPointer(0, 0, idx));
    //ptr_vtk = ptr_vtk + idx * imageData->GetDimensions()[0] * imageData->GetDimensions()[1];  //可能溢出
    cv::Mat tempImg;
    cv::flip(src, tempImg, 0);
    cv::MatIterator_ <uchar> itr_mat = tempImg.begin<uchar>();
    for (; itr_mat != tempImg.end<uchar>(); itr_mat++) {
        *ptr_vtk++ = *itr_mat;
    }
}


void Sculpture::loadVolume() {
    volumeMapper->SetBlendModeToComposite();
    volumeMapper->SetInputData(imageData);
}

void Sculpture::setMaterial(double ambient, double diffuse, double specular, double power) {
	volumeProperty->SetAmbient(ambient);
	volumeProperty->SetDiffuse(diffuse);
	volumeProperty->SetSpecular(specular); 
	volumeProperty->SetSpecularPower(power);
}


TransferFunction * Sculpture::getTransferFunction() const {
	return tf;
}

vtkSmartPointer<vtkVolume> Sculpture::getVolume() const {
	return volume;
}

vtkSmartPointer<vtkImageData> Sculpture::getImageData() const {
    return imageData;
}


bool Sculpture::getLoaded() const {
	return loaded;
}

double Sculpture::getMinXBound() const {
	return volume->GetMinXBound();
}

double Sculpture::getMinYBound() const {
	return volume->GetMinYBound();
}

double Sculpture::getMinZBound() const {
	return volume->GetMinZBound();
}

double Sculpture::getMaxXBound() const {
	return volume->GetMaxXBound();
}

double Sculpture::getMaxYBound() const {
	return volume->GetMaxYBound();
}

double Sculpture::getMaxZBound() const {
	return volume->GetMaxZBound();
}

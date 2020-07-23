#ifndef SCULPTURE_H
#define SCULPTURE_H

#include <vtkAlgorithm.h>
#include <vtkColorTransferFunction.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkImageData.h>
#include <vtkPiecewiseFunction.h>
#include <vtkSmartPointer.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkXMLImageDataReader.h>
#include <vtkBMPReader.h>
#include <vtkPNGReader.h>
#include <vtkJPEGReader.h>
#include <vtkTIFFReader.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkStringArray.h>
#include <vtkInformation.h>

#include <opencv2/opencv.hpp>

#include "Core/TransferFunction.h"
#include "Util/Measures.h"
#include "Util/Sequence.h"

/**
 * @class Sculpture
 * 体数据类
 */
class Sculpture {
public:
    /**
     * Constructor
     */
    Sculpture();

    /**
     * Destructor
     */
    ~Sculpture();

    /**
     * Get transfer function
     * @return	Transfer function pointer
     */
    TransferFunction *getTransferFunction() const;

    /**
     * Get volume
     * @return	Volume pointer
     */
    vtkSmartPointer<vtkVolume> getVolume() const;

    /**
     * Get 3D matrix with volume data
     * @return	volume data pointer
     */
    vtkSmartPointer<vtkImageData> getImageData() const;

    /**
     * Get the flag to see if a volume is loaded or not
     * @return	Volume is loaded or not
     */
    bool getLoaded() const;

    /**
     * Get volume min x bound
     * @return	Min x bound
     */
    double getMinXBound() const;

    /**
     * Get volume min y bound
     * @return	Min y bound
     */
    double getMinYBound() const;

    /**
     * Get volume min z bound
     * @return	Min z bound
     */
    double getMinZBound() const;

    /**
     * Get volume max x bound
     * @return	Max x bound
     */
    double getMaxXBound() const;

    /**
     * Get volume max y bound
     * @return	Max y bound
     */
    double getMaxYBound() const;

    /**
     * Get volume max z bound
     * @return	Max z bound
     */
    double getMaxZBound() const;

    /**
     * Set folder where (*.BMP, *.PNG, *.JPG, *TIF) files will be read
     * @param	fileNames	picture file path vector
     */
    void setImageFolder(const std::vector<std::string> &fileNames);
    
    /**
     * Set VTI file with volume dataset
     * @param	vtiFileName   VTI file path
     */
    void setVTIFile(const std::string &vtiFileName);

    /**
     * Set volume material
     * @param	ambient		Ambient component
     * @param	diffuse		Diffuse component
     * @param	specular	Specular component
     * @param	power		Specular power component
     */
    void setMaterial(double ambient, double diffuse, double specular, double power);
    
    /**
     * Pre-processing for deliver CV Mat
     * @param   WIDTH       imageData dim[0]
     * @param   HEIGHT      imageData dim[1]
     * @param   CHANNELS    imageData dim[2]
     */    
    void preSetCVMat(int WIDTH, int HEIGHT, int CHANNELS);
    
    /**
     * Post-processing for deliver CV Mat
     */
    void postSetCVMat();
    
    /**
     * Deliver OpenCV Mat(in memory) to vtkImageData
     * @param   src     Mat data
     * @param   idx     Corresponding index, Use to calculate the address pointed by the pointer
     */
    void setCVMat(cv::Mat &src, int idx);

private:
    vtkSmartPointer<vtkImageData> imageData;
    vtkSmartPointer<vtkVolume> volume; /**< Volume */
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper; /**< Mapper that uses DVR GPU Ray-casting */
    vtkSmartPointer<vtkVolumeProperty> volumeProperty; /**< Volume properties */
    TransferFunction *tf; /**< Transfer function */
    bool loaded; /**< Volume is loaded or not */

    /**
     * Set properties of volume
     */
    void setProperties();

    /**
     * Connect components of the pipeline
     */
    void connectComponents();

    /**
     * Load volume from image reader
     * @param imageReader   Image reader with volume info
     */
    void loadVolume();
};

#endif // SCULPTURE_H

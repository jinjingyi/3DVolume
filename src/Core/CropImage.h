#ifndef CROPIMAGE_H
#define CROPIMAGE_H

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkBMPReader.h>
#include <vtkPNGReader.h>
#include <vtkJPEGReader.h>
#include <vtkTIFFReader.h>

#include <opencv2/opencv.hpp>

#include <Util/Utils.h>
#include "Util/Sequence.h"
#include "Util/ReadBin.h"

/**
 * @class   CropImage
 * 基于vtk的图像可视化类
 */ 
class CropImage {
public:
    /**
     * 构造器
     */
    CropImage();
    
    /**
     * 析构函数
     */ 
    ~CropImage();
    
    /**
     * 导入图像文件，支持（bmp, png, jpg, tif, bin）格式
     * @param   filename    图像文件路径，读取到imageData
     */
    void importImage(const std::string &filename);
    
    /**
     * 可视化设置
     */
    void setViewer();
    
    /**
     * 管线连接
     */
    void connectComponents();
    
    /**
     * 获取查看器
     */
    vtkSmartPointer<vtkImageViewer2> getImageViewer();
    
    /**
     * 获取读取数据的维度
     * @return  dims    二维数组表示的宽和高
     */
    std::array<int, 2> getDimensions();
    
    /**
     * 释放数据
     */
    void releaseData();
    
    /**
     * 当导入二进制文件时，需要先确定图像的宽、高以及整个序列的最大灰度值
     * @param   WIDTH       图像的宽
     * @param   HEIGHT      图像的高
     * @param   binMaxValue 整个序列的最大灰度值
     */
    void setBinWidthHeightMaxValue(const int &WIDTH, const int &HEIGHT, const double &binMaxValue);
    
    /**
     * 获取整个序列的最大灰度值(需先设置)
     * @return  binMaxValue 整个序列的最大灰度值
     */
    double getBinMaxValue();
    
private:
    vtkSmartPointer<vtkImageData> imageData; // vtk的图像数据
    vtkSmartPointer<vtkImageViewer2> imageViewer; // vtk的图像查看器
    double binMaxValue; // 整个序列的最大灰度值，仅导入bin文件时有意义
    int WIDTH; // bin文件图像的宽
    int HEIGHT; // bin文件图像的高
};





#endif

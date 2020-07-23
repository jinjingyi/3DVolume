#ifndef LINE_H
#define LINE_H

#include <vtkAlgorithm.h>
#include <vtkSmartPointer.h>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

class Line {
public:
    /**
     * 构造器
     */
    Line();
    
    /**
     * 析构函数
     */
    ~Line();
    
    /**
     * 设置线段起点的空间位置
     * @param   x   x坐标
     * @param   y   y坐标
     * @param   z   z坐标
     */
    void setPoint1(double x, double y, double z);
    
    /**
     * 设置线段终点的空间坐标
     * @param   x   x坐标
     * @param   y   y坐标
     * @param   z   z坐标
     */
    void setPoint2(double x, double y, double z);

    /**
     * 获取线段actor
     * @return  actor   线段actor
     */
    vtkSmartPointer<vtkActor> getActor();
    
private:
    vtkSmartPointer<vtkLineSource> source; // line source
    vtkSmartPointer<vtkPolyDataMapper> mapper; // mapper
    vtkSmartPointer<vtkActor> actor; // actor
    
    /**
     * 管线连接
     */
    void connectComponents();
};


#endif

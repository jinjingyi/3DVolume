#ifndef SPHERE_H
#define SPHERE_H

#include <vtkAlgorithm.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

class Sphere {
public:
    /**
     * constructor
     */
    Sphere();
    
    /**
     * deconstructor
     */
    ~Sphere();
    
    /**
     * set the center of sphere
     * @param   x   x coordinate
     * @param   y   y coordinate
     * @param   z   z coordinate
     */
    void setCenter(double x, double y, double z);
    
    /**
     * set the color of sphere
     * @param   r   red component
     * @param   g   green component
     * @param   b   blue component
     */
    void setColor(double r, double g, double b);
    
    /**
     * set the radius of sphere
     * @param   R   radius
     */
    void setRadius(double R);
    
    /**
     * get sphere actor
     * @return  actor pointer
     */
    vtkSmartPointer<vtkActor> getActor();
    
private:
    vtkSmartPointer<vtkSphereSource> source; // sphere source
    vtkSmartPointer<vtkPolyDataMapper> mapper; // mapper
    vtkSmartPointer<vtkActor> actor; // actor
    
    /**
     * connect components of the pipeline
     */
    void connectComponents(); 
};

#endif

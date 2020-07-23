#ifndef INTERACTORSTYLEPLANEIMAGE_H
#define INTERACTORSTYLEPLANEIMAGE_H

#include <iostream>
#include <QLabel>
#include <QPixmap>
#include <QBitmap>
#include <QObject>
#include <QCoreApplication>

#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCellPicker.h>
#include <vtkRenderWindow.h>

#include "Core/SlicePlane.h"
#include "Core/Sculpture.h"
#include "Core/Sphere.h"
#include "Util/Geometry.h"

#include <vector>

/**
 * @class InteractorStylePlaneImage
 * Class that inherits from vtkInteractorStyleImage and overrides the left button click mouse event to not change the color and window level and 
 * the mouse move event to show density value of a voxel
 */
class InteractorStylePlaneImage : public vtkInteractorStyleImage {
public:
    /**
     * Constructor
     */
    static InteractorStylePlaneImage* New();

    /**
     * Set plane in which will find the value of the voxel selected
     * @param	slicePlane	Plane in which will find the value of the voxel selected
     */
    void SetSlicePlane(SlicePlane* slicePlane);

    /**
     * Set the QLabel where the density value will be shown
     * @param	label	QLabel where the density value will be shown
     */
    void SetLabel(QLabel* label);
    
    void SetRenderWindow(const vtkSmartPointer< vtkRenderWindow > renWin);
    void SetSphere(Sphere * sphere);
    
    void SetShowHoldingPoint(bool showOnOff);
    
    void showHoldingPointOnOff();
    
    std::vector<std::array<double, 3>> getPickedPointPair();
    
    void resetPick();

    /**
     * Mouse move event
     */
    void OnMouseMove() override;

    /**
     * Left button down mouse event
     */
    void OnLeftButtonDown() override;
    
    void OnRightButtonDown() override;

private:
    SlicePlane* slicePlane; /**< Plane in which will find the value of the voxel selected */
    QLabel* label; /**< QLabel where the density value will be shown */
    vtkSmartPointer<vtkRenderWindow> renWin;
    Sphere* sphere;
    std::array<double, 3> pickedPoint;
    std::vector<std::array<double, 3>> pickedPointPair;
    bool showHoldingPoint = true;
    std::vector<Sphere*> vSphere;
    std::array<double, 3> Og;
    std::array<double, 3> P1;
    std::array<double, 3> P2;
    std::array<int, 2> planeSize;
    std::array<int, 2> pointXY;
    std::array<double, 3> holdingPoint;
};

#endif // INTERACTORSTYLEPLANEIMAGE_H

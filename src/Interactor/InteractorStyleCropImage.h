#ifndef INTERACTORSTYLECROPIMAGE_H
#define INTERACTORSTYLECROPIMAGE_H

#include <iostream>
#include <QLabel>
#include <QPixmap>
#include <QBitmap>
#include <QObject>
#include <QCoreApplication>
#include <QPushButton>

#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCellPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>

#include "Util/Geometry.h"
#include "Core/Sphere.h"
#include "Core/Line.h"

#include <vector>

/**
 * @class InteractorStyleImage
 * Class that inherits from vtkInteractorStyleImage and overrides the left button click mouse event to not change the color and window level and 
 * the mouse move event to show density value of a voxel
 */
class InteractorStyleCropImage : public vtkInteractorStyleImage {
public:
    /**
     * Constructor
     */
    static InteractorStyleCropImage* New();

    /**
     * Mouse move event
     */
    void OnMouseMove() override;

    /**
     * Left button down mouse event
     */
    void OnLeftButtonDown() override;
    
    void OnRightButtonDown() override;
    
    void SetRenderWindow(const vtkSmartPointer< vtkRenderWindow > renWin);
    
    std::vector<std::array<int, 2>> getVPickedPoints();
    
    void initialize();
    
    void setExportSequenceButton(QPushButton* exportSequence);
    
    void setDeliverToVolumeButtom(QPushButton* deliverToVolume);

private:
    vtkSmartPointer<vtkRenderWindow> renWin;
    std::array<int, 2> holdPoint;
    Line* line = new Line();
    std::vector<std::array<int, 2>> vPickedPoints;
    std::vector<Line*> vLine;
    
    QPushButton* exportSequence;
    QPushButton* deliverToVolume;
    
    bool isClosed = false;
    bool isFinished = false;
};

#endif // INTERACTORSTYLECROPIMAGE_H

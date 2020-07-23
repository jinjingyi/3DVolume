#include "InteractorStylePlaneImage.h"
#include <vtkRendererCollection.h>

vtkStandardNewMacro(InteractorStylePlaneImage)

void InteractorStylePlaneImage::OnMouseMove() {
    if (slicePlane != nullptr && label != nullptr && this->GetDefaultRenderer() != nullptr) {
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0001);

        int* pos = this->GetInteractor()->GetEventPosition();
        picker->Pick(pos[0], pos[1], pos[2], this->GetDefaultRenderer());

        int* ijk = picker->GetPointIJK();

        if (picker->GetPointId() != -1) { // a voxel has been selected
            float value = slicePlane->getPlane()->GetResliceOutput()->GetScalarComponentAsFloat(ijk[0], ijk[1], ijk[2], 0);
            auto svalue = std::to_string((int)value);
            
            double *og = slicePlane->getOrigin();
            double *p1 = slicePlane->getPoint1();
            double *p2 = slicePlane->getPoint2();
            int dims[3];
            slicePlane->getPlane()->GetResliceOutput()->GetDimensions(dims);

            Og = {og[0], og[1], og[2]};
            P1 = {p1[0], p1[1], p1[2]};
            P2 = {p2[0], p2[1], p2[2]};
            planeSize = {dims[0], dims[1]};
            pointXY = {ijk[0], ijk[1]};
            holdingPoint = computePlanePointCoordinate(Og, P1, P2, planeSize, pointXY);
            
            auto sholdingPoint = "(" + std::to_string((int)holdingPoint[0]) + ", " + std::to_string((int)holdingPoint[1]) + ", " + std::to_string((int)holdingPoint[2]) + ")"; 
            
            sphere->setCenter(holdingPoint[0], holdingPoint[1], holdingPoint[2]);
            renWin->Render();
            
            pickedPoint = holdingPoint;
            
            label->setText(QString::fromStdString("体素: " + svalue + "; " + sholdingPoint));
        } else {
            label->setText(QCoreApplication::translate("MainWindow", "体素:不在范围内"));
        }
    }
    vtkInteractorStyleImage::OnMouseMove(); // Forward events
}

void InteractorStylePlaneImage::OnLeftButtonDown() {
    // idle
    if (slicePlane != nullptr && label != nullptr && this->GetDefaultRenderer() != nullptr) {
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0001);

        int* pos = this->GetInteractor()->GetEventPosition();
        picker->Pick(pos[0], pos[1], pos[2], this->GetDefaultRenderer());

        if (picker->GetPointId() != -1 && showHoldingPoint && vSphere.size() < 2) {
            Sphere *sphere_new = new Sphere();
            sphere_new->setCenter(pickedPoint[0], pickedPoint[1], pickedPoint[2]);
            sphere_new->setColor(0.0, 0.0, 1.0);
            sphere_new->setRadius(1.0/200 * planeSize[0]);
            renWin->GetRenderers()->GetFirstRenderer()->AddActor(sphere_new->getActor());
            renWin->Render();
            vSphere.push_back(sphere_new);
            pickedPointPair.push_back(pickedPoint);
        }
    }
    vtkInteractorStyleImage::OnLeftButtonDown();
}


void InteractorStylePlaneImage::OnRightButtonDown(){
    if (slicePlane != nullptr && label != nullptr && this->GetDefaultRenderer() != nullptr) {
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0001);

        int* pos = this->GetInteractor()->GetEventPosition();
        picker->Pick(pos[0], pos[1], pos[2], this->GetDefaultRenderer());

        if (picker->GetPointId() != -1 && showHoldingPoint) {
            if(vSphere.size()){
                renWin->GetRenderers()->GetFirstRenderer()->RemoveActor(vSphere[vSphere.size() - 1]->getActor());
                renWin->Render();
                vSphere.pop_back();
                pickedPointPair.pop_back();
            }
        }
    }
    vtkInteractorStyleImage::OnRightButtonDown();
}

void InteractorStylePlaneImage::SetSlicePlane(SlicePlane* plane) {
    this->slicePlane = plane;
}

void InteractorStylePlaneImage::SetLabel(QLabel* l) {
    this->label = l;
}

void InteractorStylePlaneImage::SetRenderWindow(const vtkSmartPointer< vtkRenderWindow > renWin){
    this->renWin = renWin;
}

void InteractorStylePlaneImage::SetSphere(Sphere* sphere){
    this->sphere = sphere;
}

void InteractorStylePlaneImage::SetShowHoldingPoint(bool showOnOff){
    showHoldingPoint = showOnOff;
    showHoldingPointOnOff();
}

void InteractorStylePlaneImage::showHoldingPointOnOff(){
    if (showHoldingPoint){
        renWin->GetRenderers()->GetFirstRenderer()->AddActor(sphere->getActor());
        renWin->Render();
        showHoldingPoint = true;
    }
    else{
        renWin->GetRenderers()->GetFirstRenderer()->RemoveActor(sphere->getActor());
        renWin->Render();
        showHoldingPoint = false;
    }
}

std::vector<std::array<double, 3>> InteractorStylePlaneImage::getPickedPointPair(){
    return pickedPointPair;
}

void InteractorStylePlaneImage::resetPick(){
    while (pickedPointPair.size()){
        pickedPointPair.pop_back();
        vSphere.pop_back();
    }
    SetShowHoldingPoint(false);
}

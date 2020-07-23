#include "InteractorStyleCropImage.h"

vtkStandardNewMacro(InteractorStyleCropImage)

void InteractorStyleCropImage::OnMouseMove() {
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.0001);
    
    int* pos = this->GetInteractor()->GetEventPosition();
    picker->Pick(pos[0], pos[1], pos[2], this->GetDefaultRenderer());

    int* ijk = picker->GetPointIJK();

    if (picker->GetPointId() != -1 && ijk[0] != 0 && ijk[1] != 0 && !isFinished) { // a voxel has been selected
        holdPoint = {ijk[0], ijk[1]};
        int pointsNum = vPickedPoints.size();
        if (pointsNum > 2){
            double distance = sqrt(pow(holdPoint[0] - vPickedPoints[0][0], 2) + pow(holdPoint[1] - vPickedPoints[0][1], 2));
            if (distance < 40){
                holdPoint = vPickedPoints[0];
                isClosed = true;
            }
            else{
                isClosed = false;
            }
        }
        if (pointsNum > 0){
            std::array<int, 2> currentPoint = vPickedPoints[pointsNum - 1];
            line->setPoint1(currentPoint[0], currentPoint[1], 1);
            line->setPoint2(holdPoint[0], holdPoint[1], 0);
            renWin->GetRenderers()->GetFirstRenderer()->AddActor(line->getActor());
            renWin->Render();
        }
        
    }
    vtkInteractorStyleImage::OnMouseMove(); // Forward events
}

void InteractorStyleCropImage::OnLeftButtonDown() {
    if(this->GetDefaultRenderer() != nullptr && !isFinished){
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0001);
        
        int* pos = this->GetInteractor()->GetEventPosition();
        picker->Pick(pos[0], pos[1], pos[2], this->GetDefaultRenderer());

        if (picker->GetPointId() != -1) { // a voxel has been selected
            vPickedPoints.push_back(std::array<int, 2>{holdPoint[0], holdPoint[1]});
            if(vPickedPoints.size() > 1){
                std::array<int, 2> previousPoint = vPickedPoints[vPickedPoints.size() - 2];
                std::array<int, 2> currentPoint = vPickedPoints[vPickedPoints.size() - 1];
                Line* newLine = new Line();
                newLine->setPoint1(previousPoint[0], previousPoint[1], 1);
                newLine->setPoint2(currentPoint[0], currentPoint[1], 1);
                renWin->GetRenderers()->GetFirstRenderer()->AddActor(newLine->getActor());
                renWin->Render();
                vLine.push_back(newLine);
            }
            if(isClosed){
                isFinished = true;
                exportSequence->setEnabled(true);
                deliverToVolume->setEnabled(true);
            }
        }
    }
    vtkInteractorStyleImage::OnLeftButtonDown();
}

void InteractorStyleCropImage::OnRightButtonDown(){
    if(this->GetDefaultRenderer() != nullptr){
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0001);
        
        int* pos = this->GetInteractor()->GetEventPosition();
        picker->Pick(pos[0], pos[1], pos[2], this->GetDefaultRenderer());

        if (picker->GetPointId() != -1) { // a voxel has been selected
            int points_num = vPickedPoints.size();
            if (points_num > 1){
                renWin->GetRenderers()->GetFirstRenderer()->RemoveActor(vLine[points_num - 2]->getActor());
                vPickedPoints.pop_back();
                vLine.pop_back();
            }
            else if (points_num == 1){
                vPickedPoints.pop_back();
            }
            renWin->GetRenderers()->GetFirstRenderer()->RemoveActor(line->getActor());
            OnMouseMove();
            if (isFinished){
                isClosed = false;
                isFinished = false;
                exportSequence->setDisabled(true);
                deliverToVolume->setDisabled(true);
            }
        }
    }
    vtkInteractorStyleImage::OnRightButtonDown();
}

void InteractorStyleCropImage::SetRenderWindow(const vtkSmartPointer< vtkRenderWindow > renWin){
    this->renWin = renWin;
}

std::vector<std::array<int, 2> > InteractorStyleCropImage::getVPickedPoints()
{
    return vPickedPoints;
}

void InteractorStyleCropImage::initialize(){
    if(vPickedPoints.size() > 0){
        renWin->GetRenderers()->GetFirstRenderer()->RemoveActor(line->getActor());
    }
    while(vPickedPoints.size()){
        vPickedPoints.pop_back();
    }
    while(vLine.size()){
        renWin->GetRenderers()->GetFirstRenderer()->RemoveActor(vLine[vLine.size() - 1]->getActor());
        vLine.pop_back();
    }
    renWin->Render();
    isClosed = false;
    isFinished = false;
    exportSequence->setDisabled(true);
    deliverToVolume->setDisabled(true);
}

void InteractorStyleCropImage::setExportSequenceButton(QPushButton* exportSequence){
    this->exportSequence = exportSequence;
}

void InteractorStyleCropImage::setDeliverToVolumeButtom(QPushButton* deliverToVolume){
    this->deliverToVolume = deliverToVolume;
}



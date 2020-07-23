#include "Line.h"

Line::Line():
    source(vtkSmartPointer<vtkLineSource>::New()),
    mapper(vtkSmartPointer<vtkPolyDataMapper>::New()),
    actor(vtkSmartPointer<vtkActor>::New())
{
    connectComponents();
}

Line::~Line() = default;

void Line::connectComponents(){
    
    mapper->SetInputConnection(source->GetOutputPort());
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    actor->GetProperty()->SetLineWidth(1);
}

void Line::setPoint1(double x, double y, double z){
    source->SetPoint1(x, y, z);
    source->Update();
}

void Line::setPoint2(double x, double y, double z){
    source->SetPoint2(x, y, z);
    source->Update();
}

vtkSmartPointer<vtkActor> Line::getActor(){
    return actor;
}


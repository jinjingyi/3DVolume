#include "Sphere.h"

Sphere::Sphere():
    source(vtkSmartPointer<vtkSphereSource>::New()),
    mapper(vtkSmartPointer<vtkPolyDataMapper>::New()),
    actor(vtkSmartPointer<vtkActor>::New())
{
    connectComponents();
}

Sphere::~Sphere() = default;

void Sphere::connectComponents(){
    source->SetRadius(30);
    source->SetThetaResolution(50);
    mapper->SetInputConnection(source->GetOutputPort());
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
}

void Sphere::setCenter(double x, double y, double z){
    source->SetCenter(x, y, z);
    source->Update();
}

void Sphere::setColor(double r, double g, double b){
    actor->GetProperty()->SetColor(r, g, b);
}

void Sphere::setRadius(double R){
    source->SetRadius(R);
    source->Update();
}

vtkSmartPointer<vtkActor> Sphere::getActor(){
    return actor;
}

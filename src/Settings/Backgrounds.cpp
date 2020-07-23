#include "Backgrounds.h"

Backgrounds::Backgrounds(QColor volumeBackground) :
    volumeBackground(std::move(volumeBackground)),
    volumeRenderer(nullptr),
    volumeRenderWindow(nullptr)
{}

Backgrounds::~Backgrounds() = default;

void Backgrounds::setVolumeRenderer(const vtkSmartPointer<vtkRenderer> &r) {
    this->volumeRenderer = r;
}

void Backgrounds::setVolumeRenderWindow(const vtkSmartPointer<vtkRenderWindow> &rw) {
    this->volumeRenderWindow = rw;
}

void Backgrounds::setVolumeBackground(const QColor &b) {
    this->volumeBackground = b;
    volumeRenderer->SetBackground(b.redF(), b.greenF(), b.blueF());
    volumeRenderWindow->Render();
}


QColor Backgrounds::getVolumeBackground() const {
    return volumeBackground;
}

#include "ImagePlaneWidget.h"

vtkStandardNewMacro(ImagePlaneWidget)

void ImagePlaneWidget::OnMouseMove() {
    if (moving && viewer != nullptr) {
        viewer->Render();
    }
    vtkImagePlaneWidget::OnMouseMove();
}

void ImagePlaneWidget::OnRightButtonDown() {
    moving = true;
    vtkImagePlaneWidget::OnMiddleButtonDown(); // forward events
}

void ImagePlaneWidget::OnRightButtonUp() {
    moving = false;
    vtkImagePlaneWidget::OnMiddleButtonUp(); // forward events
}

void ImagePlaneWidget::OnMiddleButtonDown() {
    // idle
}

void ImagePlaneWidget::OnMiddleButtonUp() {
    // idle
}

void ImagePlaneWidget::OnLeftButtonDown() {
    // idle
}

void ImagePlaneWidget::OnLeftButtonUp() {
    // idle
}

void ImagePlaneWidget::SetViewer(const vtkSmartPointer<vtkImageViewer2> &v) {
    this->viewer = v;
}


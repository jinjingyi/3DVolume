#include "OpacityTFChart.h"

OpacityTFChart::OpacityTFChart(const vtkSmartPointer<vtkRenderWindow> &figureRenWin, const vtkSmartPointer<vtkRenderWindow> &chartRenWin, vtkSmartPointer<vtkPiecewiseFunction> tf, const std::string &xLabel, const std::string &yLabel, double minBound, double maxBound) :
    chart(vtkSmartPointer<ChartXY>::New()),
    context(vtkSmartPointer<vtkContextView>::New()),
    function(vtkSmartPointer<vtkPiecewiseFunctionItem>::New()),
    controlPoints(vtkSmartPointer<PiecewiseControlPointsItem>::New()),
    tf(std::move(tf))
{
    chart->GetAxis(0)->SetTitle(yLabel);
    chart->GetAxis(1)->SetTitle(xLabel);

    // plots cannot change axis ranges
    chart->GetAxis(0)->SetBehavior(vtkAxis::FIXED);
    chart->GetAxis(1)->SetBehavior(vtkAxis::FIXED);

    function->SetPiecewiseFunction(tf);
    chart->AddPlot(function);

    controlPoints->SetRenderWindow(figureRenWin);
    controlPoints->SetPiecewiseFunction(tf);
    controlPoints->SetUserBounds(minBound, maxBound, 0, 1);
    chart->AddPlot(controlPoints);

    context->SetRenderWindow(chartRenWin);
    context->GetScene()->AddItem(chart);

    defaultRange();
}

OpacityTFChart::~OpacityTFChart() = default;

void OpacityTFChart::defaultRange() {
    chart->GetAxis(0)->SetRange(0, 1); // Y axis range
    chart->GetAxis(1)->SetRange(tf->GetRange()[0], tf->GetRange()[1]); // X axis range
    context->Render();
}

void OpacityTFChart::setRange(double min, double max) {
    chart->GetAxis(1)->SetRange(min, max);
    context->Render();
}

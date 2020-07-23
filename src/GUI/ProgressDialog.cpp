#include "ProgressDialog.h"

ProgressDialog::ProgressDialog(const QString &title, const QString &description) :
    bar(new QProgressBar()),
    progressDialog(new QProgressDialog())
{
    progressDialog->setWindowTitle(title);
    progressDialog->setLabelText(description);
    progressDialog->setWindowIcon(QIcon(":/icons/3DCurator.png"));
    progressDialog->setWindowFlags(progressDialog->windowFlags() & ~Qt::WindowCloseButtonHint);
    progressDialog->setCancelButton(nullptr);
    progressDialog->setModal(true);
    progressDialog->setMinimumWidth(360);
    progressDialog->setBar(bar);
}

ProgressDialog::~ProgressDialog() = default;

void ProgressDialog::show() {
    progressDialog->show();
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

void ProgressDialog::close() {
    bar->close();
    progressDialog->close();
}

void ProgressDialog::setRange(){
    bar->setRange(0, 100);
}

void ProgressDialog::updateProgressValue(double value){
    bar->setValue((int)(value * (bar->maximum() - bar->minimum())));
}

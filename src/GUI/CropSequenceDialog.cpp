#include "CropSequenceDialog.h"
#include "ui_cropSequenceDialog.h"

CropSequenceDialog::CropSequenceDialog(MainWindow* parent):
    QDialog(parent),
    ui(new Ui::CropSequenceDialog),
    cropImage(new CropImage),
    imageStyle(vtkSmartPointer<InteractorStyleCropImage>::New()),
    vfileNames(),
    sequenceSize(),
    mainWindow(parent)
{
    ui->setupUi(this);
    ui->deliverToVolume->setDisabled(true);
    ui->exportSequence->setDisabled(true);
    ui->imageIndex->setDisabled(true);
    
    connectComponents();
}

CropSequenceDialog::~CropSequenceDialog(){
    delete ui;
}
/***************************************信号触发函数********************************************/
void CropSequenceDialog::on_importImg_pressed(){
    importImg();
}

void CropSequenceDialog::on_importBin_pressed(){
    importBin();
}

void CropSequenceDialog::on_resetCropWidget_pressed(){
    resetCropWidget();
}

void CropSequenceDialog::on_clearCropWidget_pressed(){
    // 执行一次可能无法完全清空，执行两次确保清空
    clearCropWidget();
    clearCropWidget();
}

void CropSequenceDialog::on_deliverToVolume_pressed(){
    deliverToVolume();
}

void CropSequenceDialog::on_exportSequence_pressed(){
    exportSequence();
}

void CropSequenceDialog::on_imageIndex_valueChanged(int value){
    showImageByIndex();
}

/**********************************功能函数**************************************/

void CropSequenceDialog::connectComponents(){
    // UI的渲染窗口添加vtk图像查看器的渲染器 <-> vtk图像查看器设置UI的渲染器窗口
    ui->cropWidget->GetRenderWindow()->AddRenderer(cropImage->getImageViewer()->GetRenderer());
    cropImage->getImageViewer()->SetRenderWindow(ui->cropWidget->GetRenderWindow());
    //设置UI渲染窗口的交互器样式 <-> 交互器样式设置UI的渲染窗口
    ui->cropWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(imageStyle);
    imageStyle->SetRenderWindow(ui->cropWidget->GetRenderWindow());    
    //交互器样式设置图像查看器的渲染器
    imageStyle->SetDefaultRenderer(cropImage->getImageViewer()->GetRenderer());
    //在交互样式中控制【数据传送】与【数据保存】的按钮可用状态
    imageStyle->setExportSequenceButton(ui->exportSequence);
    imageStyle->setDeliverToVolumeButtom(ui->deliverToVolume);
}

void CropSequenceDialog::launchWarning(const std::string &message) {
    QPointer<QMessageBox> confirmBox = new QMessageBox();
    confirmBox->setWindowTitle(tr("警告"));
    confirmBox->setWindowIcon(QIcon(":/icons/gooal.ico"));
    confirmBox->setIcon(QMessageBox::Information);
    confirmBox->setText(message.c_str());
    confirmBox->setStandardButtons(QMessageBox::Ok);
    confirmBox->exec();
}

void CropSequenceDialog::removeImage(){
    cropImage->getImageViewer()->RemoveAllObservers();
    cropImage->releaseData();
    ui->cropWidget->GetRenderWindow()->Render();
}

void CropSequenceDialog::drawImage(){
    cropImage->setViewer();
    ui->cropWidget->GetRenderWindow()->Render();
}

/***********************************槽函数**************************************/
void CropSequenceDialog::importImg(){
    QString imageFolder = QFileDialog::getExistingDirectory(this, tr("选择序列图像文件夹"), QDir::homePath(), QFileDialog::ShowDirsOnly);
    
    if (imageFolder != nullptr) {
        while(vfileNames.size()){vfileNames.pop_back();}
        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        std::string GB_imageFolder = code->fromUnicode(imageFolder).data();
        getFileNames(GB_imageFolder, vfileNames);
        if (isSupportedPictureFormats(vfileNames)){
            sequenceSize = vfileNames.size();
            ui->imageIndex->setMaximum(sequenceSize - 1);
            ui->imageIndex->setEnabled(true);
            ui->imageIndex->setValue(0);
            auto *progressDialog = new ProgressDialog(tr("LOADING"), tr("加载序列图像..."));

            removeImage();
            imageStyle->initialize();
            cropImage->importImage(vfileNames[0]);
            cropImage->getImageViewer()->GetRenderer()->ResetCamera();
            drawImage();
            progressDialog->close();
            QCoreApplication::processEvents(); //确保循环过程中，进程对话框正常显示
        }
        else{
            launchWarning(tr("序列图像文件夹有误!").toUtf8().constData());
        }
    }
}

void CropSequenceDialog::importBin(){
    auto * dialog = new BinaryFileDialog(this);
    dialog->exec();
    QString imageFolder = dialog->getBinaryFileFolder();
    
    if (imageFolder != nullptr){
        while(vfileNames.size()){vfileNames.pop_back();}
        QTextCodec *code = QTextCodec::codecForName("GB2312"); //解决中文路径问题
        std::string GB_imageFolder = code->fromUnicode(imageFolder).data();
        getFileNames(GB_imageFolder, vfileNames);
        if (isSupportedBinFormats(vfileNames)){
            sequenceSize = vfileNames.size();
            ui->imageIndex->setMaximum(sequenceSize - 1);
            ui->imageIndex->setEnabled(true);
            ui->imageIndex->setValue(0);
            cv::Mat im(dialog->getWidth(), dialog->getHeight(), CV_32F);
            double totalMax = 0;
            double max = 0;
            auto *progressDialog = new ProgressDialog(tr("Processing"), tr("计算断层全局最大强度值..."));
            progressDialog->setRange();
            progressDialog->show();
            for (int i = 0; i < (int)vfileNames.size(); i++){
                loadBinFloatType(vfileNames[i], &im);
                cv::minMaxLoc(im, NULL, &max);
                if(max > totalMax){
                    totalMax = max;
                }
                progressDialog->updateProgressValue(i * 1.0 / vfileNames.size());
                QCoreApplication::processEvents();
            }
            progressDialog->close();
            // TODO
            std::cout << "max pix value of bin files: " << totalMax << std::endl;
            cropImage->setBinWidthHeightMaxValue(dialog->getWidth(), dialog->getHeight(), totalMax);
            removeImage();
            imageStyle->initialize();
            cropImage->importImage(vfileNames[0]);
            cropImage->getImageViewer()->GetRenderer()->ResetCamera();
            drawImage();
        }
        else{
            launchWarning(tr("序列图像文件夹有误!").toUtf8().constData());
        }
    }
}

void CropSequenceDialog::clearCropWidget(){
    imageStyle->initialize();
    removeImage();
    while(vfileNames.size()){vfileNames.pop_back();}
    ui->imageIndex->setValue(0);
    ui->imageIndex->setDisabled(true);
}

void CropSequenceDialog::resetCropWidget(){
    imageStyle->initialize();
}

void CropSequenceDialog::deliverToVolume(){
    std::vector<std::array<int, 2> > points = imageStyle->getVPickedPoints();
    std::array<int, 2> dims = cropImage->getDimensions();
    cv::Mat mask(dims[0], dims[1], CV_8UC1);
    mask.setTo(0);

    std::vector<cv::Point > contour;
    contour.reserve(points.size() - 1);
    for (int i = 0; i < (int)points.size() - 1; i++){
        contour.push_back(cv::Point(points[i][0], dims[1] - points[i][1]));
    }
    std::vector<std::vector<cv::Point >> contours;
    contours.push_back(contour);
    //填充区域之前，首先采用polylines()函数，可以使填充的区域边缘更光滑
    cv::polylines(mask, contour, true, cv::Scalar(255), 2, cv::LINE_AA);//第2个参数可以采用contour或者contours，均可
    cv::fillPoly(mask, contours, cv::Scalar(255));//fillPoly函数的第二个参数是二维数组！
    
    auto *progressDialog = new ProgressDialog(tr("Sending"), tr("数据传送到体绘制..."));
    progressDialog->setRange();
    progressDialog->show();
    std::string ext = toUpper(getFileExtension(vfileNames[0]));
    mainWindow->slicePlane->show(false);
    mainWindow->disablePlane();
    mainWindow->sculpture->preSetCVMat(dims[0], dims[1], vfileNames.size());
    if (ext == "BIN"){
        cv::Mat im(dims[0], dims[1], CV_32F);
        cv::Mat im8U;
        cv::Mat imageCropped;
        std::string imageFile;
        for (int i = 0; i < (int)vfileNames.size(); i++){
            imageFile = vfileNames[i];
            loadBinFloatType(imageFile, &im);
            floatConvertTo8U(&im, &im8U, cropImage->getBinMaxValue());
            im8U.copyTo(imageCropped, mask); 
            
            mainWindow->sculpture->setCVMat(imageCropped, i);
            progressDialog->updateProgressValue(i * 1.0 / vfileNames.size());
            QCoreApplication::processEvents();
        }

    }
    else{ //图片,非Bin文件
        std::string imageFile;
        cv::Mat imageMat;
        cv::Mat imageCropped;
        for (int i = 0; i < (int)vfileNames.size(); i++){
            imageFile = vfileNames[i];
            imageMat = cv::imread(imageFile, cv::IMREAD_UNCHANGED);
            imageMat.copyTo(imageCropped, mask);
            
            mainWindow->sculpture->setCVMat(imageCropped, i);
            progressDialog->updateProgressValue(i * 1.0 / vfileNames.size());
            QCoreApplication::processEvents();
        }
    }
    mainWindow->sculpture->postSetCVMat();
    mainWindow->slicePlane->setInputData(mainWindow->sculpture->getImageData());
    mainWindow->ui->labelFolder->setText(QString("数据源:").append("数据裁剪"));
    mainWindow->defaultPlanePosition();
    mainWindow->slicePlane->show(true);
    mainWindow->enablePlane();
    mainWindow->slicePlane->getPlane()->GetTexturePlaneProperty()->SetOpacity(0.95);

    mainWindow->drawVolume();
    mainWindow->renderSlice();

    mainWindow->disablePlane(); //解决planeViewer不居中的问题，需先打开再关闭
    progressDialog->close();
}

void CropSequenceDialog::exportSequence(){
    QString imageFolder = QFileDialog::getExistingDirectory(this, tr("选择序列图像文件夹"), QDir::homePath(), QFileDialog::ShowDirsOnly);
    
    if (imageFolder != nullptr) {
        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        std::string GB_imageFolder = code->fromUnicode(imageFolder).data();
        
        std::vector<std::array<int, 2> > points = imageStyle->getVPickedPoints();
        std::array<int, 2> dims = cropImage->getDimensions();
        cv::Mat mask(dims[0], dims[1], CV_8UC1);
        mask.setTo(0);

        std::vector<cv::Point > contour;
        contour.reserve(points.size() - 1);
        for (int i = 0; i < (int)points.size() - 1; i++){
            contour.push_back(cv::Point(points[i][0], dims[1] - points[i][1]));
        }
        std::vector<std::vector<cv::Point >> contours;
        contours.push_back(contour);
        //填充区域之前，首先采用polylines()函数，可以使填充的区域边缘更光滑
        cv::polylines(mask, contour, true, cv::Scalar(255), 2, cv::LINE_AA);//第2个参数可以采用contour或者contours，均可
        cv::fillPoly(mask, contours, cv::Scalar(255));//fillPoly函数的第二个参数是二维数组！
        
        auto *progressDialog = new ProgressDialog(tr("Exporting"), tr("保存裁剪图像..."));
        progressDialog->setRange();
        progressDialog->show();
        std::string ext = toUpper(getFileExtension(vfileNames[0]));
        if (ext == "BIN"){
            cv::Mat im(dims[0], dims[1], CV_32F);
            for (int i = 0; i < (int)vfileNames.size(); i++){
                std::string imageFile = vfileNames[i];
                std::string imageFileName = getFileNameNoExtension(getPathName(imageFile)) + ".BMP";
                std::string exportFile = GB_imageFolder + "/" + imageFileName;
                loadBinFloatType(imageFile, &im);
                cv::Mat im8U;
                floatConvertTo8U(&im, &im8U, cropImage->getBinMaxValue());
                cv::Mat imageCropped;
                im8U.copyTo(imageCropped, mask);
                cv::imwrite(exportFile, imageCropped);
                progressDialog->updateProgressValue(i * 1.0 / vfileNames.size());
                QCoreApplication::processEvents();
            }
        }
        else{
            for (int i = 0; i < (int)vfileNames.size(); i++){
                std::string imageFile = vfileNames[i];
                std::string imageFileName = getPathName(imageFile);
                std::string exportFile = GB_imageFolder + "/" + imageFileName;
                cv::Mat imageMat = cv::imread(imageFile, cv::IMREAD_UNCHANGED);
                cv::Mat imageCropped;
                imageMat.copyTo(imageCropped, mask);
                cv::imwrite(exportFile, imageCropped);
                progressDialog->updateProgressValue(i * 1.0 / vfileNames.size());
                QCoreApplication::processEvents();
            }
        }
        progressDialog->close();
    }
}

void CropSequenceDialog::showImageByIndex(){
    removeImage();
    cropImage->importImage(vfileNames[ui->imageIndex->value()]);
    drawImage();
}

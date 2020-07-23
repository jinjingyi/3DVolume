#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(new QSettings),
    backgrounds(new Backgrounds(QColor::fromRgb(QVariant(settings->value("volume_bg", QVariant(QColor::fromRgbF(0.0, 0.0, 0.0).rgb()))).toUInt()))),
    material(new Material(
        QVariant(settings->value("mat_ambient", 0.5)).toDouble(),
        QVariant(settings->value("mat_diffuse", 0.9)).toDouble(),
        QVariant(settings->value("mat_specular", 0.5)).toDouble(),
        QVariant(settings->value("mat_power", 8.0)).toDouble())),
    sculpture(new Sculpture()),
    slicePlane(new SlicePlane()),
    sphere(new Sphere()),
    sliceViewer(vtkSmartPointer<vtkImageViewer2>::New()),
    volumeRen(vtkSmartPointer<vtkRenderer>::New()),
    volumeStyle(vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New()),
    sliceStyle(vtkSmartPointer<InteractorStylePlaneImage>::New()),
    showPlane(false),
    isMeasuring(new bool),
    
    transf(vtkSmartPointer <vtkTransform>::New()),
    axes(vtkSmartPointer <vtkAxesActor>::New()),
    measureTable(new MeasureTable()),

    cropImage(new CropImage()),
    imageStyle(vtkSmartPointer<InteractorStyleCropImage>::New()),

    cropSequenceDialog(new CropSequenceDialog(this))
{
    ui->setupUi(this);
    
    backgrounds->setVolumeRenderer(volumeRen);
    backgrounds->setVolumeRenderWindow(ui->volumeWidget->GetRenderWindow());

    material->setSculpture(sculpture);
    material->setVolumeRenderWindow(ui->volumeWidget->GetRenderWindow());

    defaultTF();

    colorTFChart = new ColorTFChart(ui->volumeWidget->GetRenderWindow(),
                                    ui->colorTFWidget->GetRenderWindow(),
                                    sculpture->getTransferFunction()->getColorFun(),
                                    tr("grayscale").toUtf8().constData(),
                                    "",
                                    MIN_INTENSITY,
                                    MAX_INTENSITY);
    scalarTFChart = new OpacityTFChart(ui->volumeWidget->GetRenderWindow(),
                                       ui->scalarTFWidget->GetRenderWindow(),
                                       sculpture->getTransferFunction()->getScalarFun(),
                                       tr("grayscale").toUtf8().constData(),
                                       tr("scalar opactity").toUtf8().constData(),
                                       MIN_INTENSITY,
                                       MAX_INTENSITY);
    gradientTFChart = new OpacityTFChart(ui->volumeWidget->GetRenderWindow(),
                                         ui->gradientTFWidget->GetRenderWindow(),
                                         sculpture->getTransferFunction()->getGradientFun(),
                                         tr("grayscale").toUtf8().constData(),
                                         tr("gradient opacity").toUtf8().constData(),
                                         0,
                                         MAX_INTENSITY - MIN_INTENSITY);
    
    sliceViewer->GetWindowLevel()->SetLookupTable(sculpture->getTransferFunction()->getColorFun());
    sliceViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
    sliceViewer->SetColorLevel(127);
    sliceViewer->SetColorWindow(256);

    volumeRen->SetBackground(backgrounds->getVolumeBackground().redF(), backgrounds->getVolumeBackground().greenF(), backgrounds->getVolumeBackground().blueF());
    
    connectComponents();

    renderVolume();
    slicePlane->show(false);
    disablePlane();
    measureTable->setTable(ui->measureResultsTable);
    measureTable->resetTable();
    disableMeasure();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectComponents() {
    ui->volumeWidget->GetRenderWindow()->AddRenderer(volumeRen);
    ui->slicesWidget->GetRenderWindow()->AddRenderer(sliceViewer->GetRenderer());
    volumeRen->SetRenderWindow(ui->volumeWidget->GetRenderWindow());
    sliceViewer->SetRenderWindow(ui->slicesWidget->GetRenderWindow());
    sliceViewer->SetInputData(slicePlane->getPlane()->GetResliceOutput());

    ui->volumeWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(volumeStyle);
    slicePlane->getPlane()->SetInteractor(ui->volumeWidget->GetRenderWindow()->GetInteractor());

    sliceViewer->SetupInteractor(ui->slicesWidget->GetRenderWindow()->GetInteractor());
    sliceStyle->SetSlicePlane(slicePlane);
    sliceStyle->SetDefaultRenderer(sliceViewer->GetRenderer());
    sliceStyle->SetLabel(ui->coordsAndValueLabel);
    sliceStyle->SetRenderWindow(ui->volumeWidget->GetRenderWindow());
    sliceStyle->SetSphere(sphere);;
    ui->slicesWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(sliceStyle);
    slicePlane->setViewer(sliceViewer);
}

void MainWindow::drawVolume() {
    volumeRen->AddViewProp(sculpture->getVolume());
    transf->Translate(0.0, 0.0, 0.0);
    axes->SetUserTransform(transf);
    axes->SetTotalLength(1.0/10 * sculpture->getMaxXBound(), 1.0/10 * sculpture->getMaxXBound(), 1.0/10 * sculpture->getMaxXBound());
    axes->SetXAxisLabelText("");
    axes->SetYAxisLabelText("");
    axes->SetZAxisLabelText("");
    
    volumeRen->AddActor(axes);
    volumeRen->ResetCamera();
    sphere->setRadius(1.0/100 * sculpture->getMaxXBound());
    sliceStyle->resetPick();
    sliceViewer->GetRenderer()->ResetCamera();
    isMeasuring = false;
    measureTable->resetTable();
    renderVolume();
}

void MainWindow::removeVolume() {
    volumeRen->RemoveAllViewProps();
    volumeRen->ResetCamera();
    renderVolume();
}

void MainWindow::renderVolume() {
    ui->volumeWidget->GetRenderWindow()->Render();
}

void MainWindow::renderSlice() {
    sliceViewer->Render();
}

void MainWindow::loadDefaultPreset(QFile *file) {
    if (file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::istringstream ss;
        ss.str(QString(file->readAll()).toStdString()); //Get istringstream from QFile

        sculpture->getTransferFunction()->read(ss);
        // update tags
        ui->tfName->setText(QString::fromUtf8(sculpture->getTransferFunction()->getName().c_str()));
        ui->tfDescription->setText(QString::fromUtf8(sculpture->getTransferFunction()->getDescription().c_str()));
        file->close();
    
    } else {
        cerr << tr("转换函数打开错误").toUtf8().constData() << endl;
        exit(-1);
    }
}

void MainWindow::defaultTF() {
    QFile file(":/presets/defualt-preset.xml");
    loadDefaultPreset(&file);
}

void MainWindow::defaultPlanePosition() {
    if (sculpture->getVolume() != nullptr) {
        // volume dimension
        double xSize = sculpture->getMaxXBound() - sculpture->getMinXBound();
        double ySize = sculpture->getMaxYBound() - sculpture->getMinYBound();
        double zSize = sculpture->getMaxZBound() - sculpture->getMinZBound();
        slicePlane->setOrigin(xSize / 2, ySize / 2, zSize / 2); // plane in the center of the sculpture
        slicePlane->setAxial();
    } else {
        launchWarningNoVolume();
    }
}

void MainWindow::importImageSequence() {
    QString imageFolder = QFileDialog::getExistingDirectory(this, tr("选择序列图像文件夹"), QDir::homePath(), QFileDialog::ShowDirsOnly);

    if (imageFolder != nullptr) {
        std::vector<std::string> vfileNames;
        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        std::string GB_imageFolder = code->fromUnicode(imageFolder).data();
        getFileNames(GB_imageFolder, vfileNames);
        if (isSupportedPictureFormats(vfileNames)){

            removeVolume();

            auto *progressDialog = new ProgressDialog(tr("LOADING"), tr("加载序列图像..."));
            progressDialog->show();

            slicePlane->show(false);
            disablePlane();
            sculpture->setImageFolder(vfileNames);
            slicePlane->setInputData(sculpture->getImageData());
            ui->labelFolder->setText(QString("数据源:").append(imageFolder));
            defaultPlanePosition();
            slicePlane->show(true);
            enablePlane();
            slicePlane->getPlane()->GetTexturePlaneProperty()->SetOpacity(0.95);

            drawVolume();
            renderSlice();

            disablePlane(); //解决planeViewer不居中的问题，需先打开再关闭

            progressDialog->close();
            QCoreApplication::processEvents();
        }
        else{
            launchWarning(tr("序列图像文件夹有误!").toUtf8().constData());
        }
    }
}

void MainWindow::launchWarning(const std::string &message) {
    QPointer<QMessageBox> confirmBox = new QMessageBox();
    confirmBox->setWindowTitle(tr("警告"));
    confirmBox->setWindowIcon(QIcon(":/icons/gooal.ico"));
    confirmBox->setIcon(QMessageBox::Information);
    confirmBox->setText(message.c_str());
    confirmBox->setStandardButtons(QMessageBox::Ok);
    confirmBox->exec();
}

void MainWindow::launchWarningNoVolume() {
    launchWarning(tr("模型为空!").toUtf8().constData());
}

void MainWindow::importVTI() {
    QString vtiFile = QFileDialog::getOpenFileName(this, tr("加载体数据文件"), QDir::homePath(), "VTI (*.vti) ;; XML (*.xml) ;; All files (*.*)");

    if (vtiFile != nullptr) {
        removeVolume();
        auto *progressDialog = new ProgressDialog(tr("LOADING"), tr("加载体数据文件..."));
        progressDialog->show();
        
        slicePlane->show(false);
        disablePlane();
        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        std::string GB_vtiFile = code->fromUnicode(vtiFile).data();
        sculpture->setVTIFile(GB_vtiFile);
        slicePlane->setInputData(sculpture->getImageData());
        ui->labelFolder->setText(QString("数据源:").append(vtiFile));
        defaultPlanePosition();
        slicePlane->show(true);
        enablePlane();
        
        drawVolume();
        renderSlice();

        disablePlane(); //解决planeViewer不居中的问题，需先打开再关闭

        progressDialog->close();
    }
}

void MainWindow::exportVTI() {
    if (sculpture->getLoaded()) {
        QString filename = getExportVTIFilename(tr("volumeData"));
        if (filename != nullptr) {
            auto *progressDialog = new ProgressDialog(tr("EXPORTING"), tr("导出体数据文件..."));
            progressDialog->show();

            vtkSmartPointer<vtkXMLImageDataWriter> writer = vtkSmartPointer<vtkXMLImageDataWriter>::New();
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string GB_filename = code->fromUnicode(filename).data();
            writer->SetFileName(GB_filename.c_str());
            writer->SetInputData(sculpture->getImageData());
            writer->Write();

            progressDialog->close();
        }
    } else {
        launchWarningNoVolume();
    }
}

void MainWindow::importPreset() {
    QString presetFile = QFileDialog::getOpenFileName(this, tr("加载配置文件"), QDir::homePath(), "XML (*.xml) ;; All files (*.*)");

    if (presetFile != nullptr) {
        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        std::string GB_vtiFile = code->fromUnicode(presetFile).data();
        sculpture->getTransferFunction()->read(GB_vtiFile);

        // update tags
        ui->tfName->setText(QString::fromUtf8(sculpture->getTransferFunction()->getName().c_str()));
        ui->tfDescription->setText(QString::fromUtf8(sculpture->getTransferFunction()->getDescription().c_str()));

        colorTFChart->defaultRange();
        scalarTFChart->defaultRange();
        gradientTFChart->defaultRange();
    }
}

void MainWindow::exportPreset(const QString &filename) {
    if (filename != nullptr) {
        // get name and description from GUI
        sculpture->getTransferFunction()->setName(ui->tfName->text().toUtf8().constData());
        sculpture->getTransferFunction()->setDescription(ui->tfDescription->text().toUtf8().constData());

        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        std::string GB_filename = code->fromUnicode(filename).data();
        // Make sure the suffix is named xml
        if (toUpper(getFileExtension(GB_filename)) != "XML"){
            GB_filename.append(".xml");
        }
        sculpture->getTransferFunction()->write(GB_filename);
    }
}

QString MainWindow::getExportPresetFilename(const QString &defaultFilename) {
    return QFileDialog::getSaveFileName(this, tr("保存配置文件"), QDir(QDir::homePath()).filePath(defaultFilename), "XML (*.xml)");
}

QString MainWindow::getExportVTIFilename(const QString &defaultFilename) {
    return QFileDialog::getSaveFileName(this, tr("保存体数据"), QDir(QDir::homePath()).filePath(defaultFilename), "VTI (*.vti) ;; XML (*.xml)");
}


QString MainWindow::getExportImageFilename(const QString &defaultFilename) {
    return QFileDialog::getSaveFileName(this, tr("保存图片"), QDir(QDir::homePath()).filePath(defaultFilename), "PNG (*.png);;JPG (*.jpg)");
}

void MainWindow::exportImageFromRenderWindow(const vtkSmartPointer<vtkRenderWindow> &renWin, const QString &filename) {
    if (filename != nullptr) {
        vtkSmartPointer<vtkWindowToImageFilter> filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
        filter->SetInput(renWin);
        filter->Update();
        vtkSmartPointer<vtkImageWriter> writer;
        if (toUpper(getFileExtension(filename.toUtf8().constData())) == "PNG") {
            writer = vtkSmartPointer<vtkPNGWriter>::New();
        } else {
            writer = vtkSmartPointer<vtkJPEGWriter>::New();
        }
        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        std::string GB_filename = code->fromUnicode(filename).data();
        writer->SetFileName(GB_filename.c_str());
        writer->SetInputConnection(filter->GetOutputPort());
        writer->Write();
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
// GUI Events - Menu
//---------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_actionOpenImageSequence_triggered() {
    importImageSequence();
}

void MainWindow::on_actionPreferences_triggered() {
    auto *dialog = new PreferencesDialog(settings, backgrounds, material);
    dialog->exec();
}

void MainWindow::on_actionExit_triggered() {
    QPointer<QMessageBox> confirmBox = new QMessageBox();
    confirmBox->setWindowTitle(tr("提示"));
    confirmBox->setWindowIcon(QIcon(":/icons/gooal.ico"));
    confirmBox->setIcon(QMessageBox::Information);
    confirmBox->setText(tr("确定退出?"));
    confirmBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmBox->button(QMessageBox::Yes)->setText(tr("是"));
    confirmBox->button(QMessageBox::No)->setText(tr("否"));
    if (confirmBox->exec() == QMessageBox::Yes) {
        exit(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
// GUI Events - Buttons
//---------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_openImageSequence_pressed() {
    importImageSequence();
}

void MainWindow::on_importPreset_pressed() {
    importPreset();
    //update the renderVolume
    renderVolume();
}

void MainWindow::on_axialPlane_pressed() {
    axialPlane();
}

void MainWindow::on_coronalPlane_pressed() {
    coronalPlane();
}

void MainWindow::on_sagitalPlane_pressed() {
    sagitalPlane();
}

void MainWindow::on_exportPreset_pressed() {
    exportPreset(getExportPresetFilename(ui->tfName->text()));
}

void MainWindow::on_openVolume_pressed() {
    importVTI();
}

void MainWindow::on_saveVolume_pressed() {
    exportVTI();
}

void MainWindow::enablePlane() {
    slicePlane->enable(true);
    QIcon icon(":/icons/eye.png");
    ui->enableDisablePlane->setIcon(icon);
    showPlane = true;
}

void MainWindow::disablePlane() {
    slicePlane->enable(false);
    QIcon icon(":/icons/eye-slash.png");
    ui->enableDisablePlane->setIcon(icon);
    showPlane = false;
}

void MainWindow::enableDisablePlane() {
    if (showPlane) {
        disablePlane();
    } else {
        enablePlane();
    }
    renderVolume();
}

void MainWindow::axialPlane() {
    if (sculpture->getLoaded()) {
        slicePlane->setAxial();
        renderVolume();
        renderSlice();
    } else {
        launchWarningNoVolume();
    }
}

void MainWindow::coronalPlane() {
    if (sculpture->getLoaded()) {
        slicePlane->setCoronal();
        renderVolume();
        renderSlice();
    } else {
        launchWarningNoVolume();
    }
}

void MainWindow::sagitalPlane() {
    if (sculpture->getLoaded()) {
        slicePlane->setSagital();
        renderVolume();
        renderSlice();
    } else {
        launchWarningNoVolume();
    }
}

void MainWindow::resetSlice(){
    sliceViewer->GetRenderer()->ResetCamera();
    renderSlice();
}


void MainWindow::on_enableDisablePlane_pressed() {
    enableDisablePlane();
}

void MainWindow::on_exportSliceImage_pressed() {
    exportImageFromRenderWindow(ui->slicesWidget->GetRenderWindow(), getExportImageFilename(QString::fromStdString(std::string("slice_").append(getCurrentDate()))));
}

void MainWindow::on_exportVolumeImage_pressed() {
    exportImageFromRenderWindow(ui->volumeWidget->GetRenderWindow(), getExportImageFilename(QString::fromStdString(std::string("volume_").append(getCurrentDate()))));
}

void MainWindow::on_resetSlice_pressed(){
    resetSlice();
}


//--------------------------------------------------------------------------------
// GUI Events - Buttons Measure
//--------------------------------------------------------------------------------
void MainWindow::on_measure_pressed(){
    if (sculpture->getLoaded()){
        enableDisableMeasure();
    }
    else{
        launchWarningNoVolume();
    }
}

void MainWindow::on_addMeasureItem_pressed(){
    if (sculpture->getLoaded()){
        if (sliceStyle->getPickedPointPair().size() == 2){
            std::array<std::array<double, 3>, 2> pointPair = {sliceStyle->getPickedPointPair()[0], sliceStyle->getPickedPointPair()[1]};
            measureTable->addItem(pointPair);
        }
    }
    else{
        launchWarningNoVolume();
    }
}

void MainWindow::on_deleteMeasureItem_pressed(){
    if (sculpture->getLoaded()){
        measureTable->deleteItem();
    }
    else{
        launchWarningNoVolume();
    }
}

void MainWindow::on_exportMeasureResults_pressed(){
    if (sculpture->getLoaded()){
        exportExecl(ui->measureResultsTable);
    }
    else{
        launchWarningNoVolume();
    }
}

void MainWindow::enableDisableMeasure(){
    if (isMeasuring){
        disableMeasure();
    }
    else{
        enableMeasure();
    }
}

void MainWindow::enableMeasure(){
    sliceStyle->SetShowHoldingPoint(true);
    isMeasuring = true;
}

void MainWindow::disableMeasure(){
    sliceStyle->SetShowHoldingPoint(false);
    isMeasuring = false;
}


void MainWindow::exportExecl(QTableWidget *tableWidget)
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Excle file"),QString("./paper_list.csv"),tr("Excel Files(*.csv)"));    //设置保存的文件名
    if(fileName != nullptr){
        QFile file(fileName);
        bool ret = file.open(QIODevice::Truncate | QIODevice::ReadWrite);
        if(!ret)
        {
            qDebug() << "open failure";
        }
 
        QTextStream stream(&file);
        QString conTents;
        // 写入头
        QHeaderView * header = tableWidget->horizontalHeader() ;
        if (NULL != header)
        {
            for (int i = 0; i < header->count(); i++)
            {
                QTableWidgetItem *item = tableWidget->horizontalHeaderItem(i);
                if (NULL != item)
                {
                    conTents += item->text() + ",";
                }
            }
            conTents += "\n";
        }
 
        // 写内容
        for (int row = 0; row < tableWidget->rowCount(); row++)
        {
            for (int column = 0; column < tableWidget->columnCount(); column++)
            {
                QTableWidgetItem* item = tableWidget->item(row, column);
                if (NULL != item )
                {
                    QString str;
                    str = item->text();
                    str.replace(","," ");
                    conTents += str + ",";
                }
            }
            conTents += "\n";
        }
        stream << conTents;
 
        file.close();
    }
}

void MainWindow::on_cropSequence_pressed(){
    startUpCropSequenceDialog();
}

void MainWindow::startUpCropSequenceDialog(){
    cropSequenceDialog->exec();
}

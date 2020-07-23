#include "BinaryFileDialog.h"
#include "ui_binaryFileDialog.h"

BinaryFileDialog::BinaryFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BinaryFileDialog),
    binfileFolder()
{
    ui->setupUi(this);
    
    QObject::connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

BinaryFileDialog::~BinaryFileDialog() {
    delete ui;
}

void BinaryFileDialog::accept() {
    done(0);
}

void BinaryFileDialog::on_importBinSequence_pressed(){
    std::cout << "import bin" << std::endl;
    QString binfileFolder = QFileDialog::getExistingDirectory(this, tr("选择序列图像文件夹"), QDir::homePath(), QFileDialog::ShowDirsOnly);
    ui->binaryFilePath->setText(binfileFolder);
    this->binfileFolder = binfileFolder;
    
    //TODO 寻找配置文件确定宽度、高度
    configWidthHeight();
}

void BinaryFileDialog::on_width_valueChanged(int value){
    std::cout << "width:" << value << std::endl;
}

void BinaryFileDialog::on_height_valueChanged(int value){
    std::cout << "height:" << value << std::endl;
}

int BinaryFileDialog::getWidth(){
    return ui->width->value();
}

int BinaryFileDialog::getHeight(){
    return ui->height->value();
}

QString BinaryFileDialog::getBinaryFileFolder(){
    return binfileFolder;
}

void BinaryFileDialog::configWidthHeight(){
    std::string currentDirMayExistConfigPath = binfileFolder.toStdString() + "/../config.tsk";
    std::string upperDirMayExistConfigPath = binfileFolder.toStdString() + "/../../config.tsk";
    if (fs::exists(currentDirMayExistConfigPath)){
        std::ifstream infile(currentDirMayExistConfigPath);
        std::string temp;
        if(!infile.is_open()){
            std::cout << "open file failed" << std::endl;
        }
        size_t pos;
        while(getline(infile, temp)){
            if((pos = temp.find(WIDTH_KEY)) != std::string::npos){
                temp.erase(0, WIDTH_KEY.size() + 1);
                int WIDTH_VALUE = atoi(temp.c_str());
                std::cout << WIDTH_VALUE << std::endl;
                ui->width->setValue(WIDTH_VALUE);
                ui->height->setValue(WIDTH_VALUE);
                ui->configBox->setTitle("断层参数配置(已根据config文件设置)");
            }
        }
        infile.close();
    }
    else if(fs::exists(upperDirMayExistConfigPath)){
        std::ifstream infile(upperDirMayExistConfigPath);
        std::string temp;
        if(!infile.is_open()){
            std::cout << "open file failed" << std::endl;
        }
        size_t pos;
        while(getline(infile, temp)){
            if((pos = temp.find(WIDTH_KEY)) != std::string::npos){
                temp.erase(0, WIDTH_KEY.size() + 1);
                int WIDTH_VALUE = atoi(temp.c_str());
                std::cout << WIDTH_VALUE << std::endl;
                ui->width->setValue(WIDTH_VALUE);
                ui->height->setValue(WIDTH_VALUE);
                ui->configBox->setTitle("断层参数配置(已根据config文件设置)");
            }
        }
        infile.close();
    }
    else{
        ui->configBox->setTitle("断层参数配置(未找到config文件，请手动设置)");
    }
}



#ifndef CROPSEQUENCEDIALOG_H
#define CROPSEQUENCEDIALOG_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QPointer>

#include <vtkSmartPointer.h>

#include <ui_mainwindow.h>

#include "Interactor/InteractorStyleCropImage.h"
#include "Core/CropImage.h"
#include "GUI/ProgressDialog.h"
#include "GUI/BinaryFileDialog.h"
#include "GUI/MainWindow.h"

class MainWindow;

namespace Ui{
    class CropSequenceDialog;
}


/**
 * @class   CropSequenceDialog
 * 序列裁剪对话框
 */
class CropSequenceDialog: public QDialog{
    Q_OBJECT
    
public:
    /**
     * 序列裁剪对话框构造函数
     * @param   mainWindow    父窗口为MainWindow对象
     */
    explicit CropSequenceDialog(MainWindow *mainWindow = nullptr);
    
    /**
     * 析构函数
     */
    ~CropSequenceDialog() override;
    
private slots:
    /** 导入图像序列触发信号 */
    void on_importImg_pressed();
    
    /** 导入二进制文件触发信号 */
    void on_importBin_pressed();
    
    /** 清空裁剪窗口触发信号 */
    void on_clearCropWidget_pressed();
    
    /** 重置裁剪触发信号 */
    void on_resetCropWidget_pressed();
    
    /** 裁剪图像传送到三维体绘制触发信号 */
    void on_deliverToVolume_pressed();
    
    /** 导出裁剪图像序列触发信号 */
    void on_exportSequence_pressed();
    
    /** 图像索引滑条触发信号 */
    void on_imageIndex_valueChanged(int value);
    
private:
    Ui::CropSequenceDialog *ui; // UI对象指针
    CropImage *cropImage; // 裁剪对象指针
    vtkSmartPointer<InteractorStyleCropImage> imageStyle; // 裁剪交互器样式
    std::vector<std::string> vfileNames; // 序列文件路径向量
    int sequenceSize; // 序列文件夹容量
    MainWindow *mainWindow; // 声明主窗口对象指针
    
    /***********************功能函数*************************/
    
    /** 可视化管线连接 */
    void connectComponents();
    
    /** 警告框 */
    void launchWarning(const std::string &message);
    
    /** 移除图像 */
    void removeImage();
    
    /** 绘制图像到vtk窗口 */
    void drawImage();
    
    /************************槽函数**************************/
    /** 导入图像(支持bmp, png, tif, jpg格式)序列 */
    void importImg();
    
    /** 导入二进制文件(bin)序列 */
    void importBin();
    
    /** 清空裁剪窗口 */
    void clearCropWidget();
    
    /** 重置裁剪窗口 */
    void resetCropWidget();
    
    /** 将裁剪数据传送到三维体绘制窗口 */
    void deliverToVolume();
    
    /** 导出裁剪序列 */
    void exportSequence();
    
    /** 将滑条value传递给序列索引 */
    void showImageByIndex();
    
};

#endif

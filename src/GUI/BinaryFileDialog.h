#ifndef BINARYFILEDIALOG_H
#define BINARYFILEDIALOG_H

#include <QColor>
#include <QColorDialog>
#include <QDialog>
#include <QSettings>
#include <QString>
#include <QFileDialog>

#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

const std::string WIDTH_KEY = "Prj_ROI_width";

namespace Ui{
    class BinaryFileDialog;
}

class BinaryFileDialog : public QDialog {
    Q_OBJECT
    
public:
        /**
     * Constructor
     */
    explicit BinaryFileDialog(QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~BinaryFileDialog() override;
    
    /**
     * get width
     * @return  WIDTH
     */
    int getWidth();
    
    /**
     * get height
     * @return  HEIGHT
     */
    int getHeight();
    
    /**
     * get binaryFileFolder
     * @return  the folder path of binary files
     */
    QString getBinaryFileFolder();
    
private slots:
    void on_importBinSequence_pressed();
    void on_width_valueChanged(int value);
    void on_height_valueChanged(int value);
    
    /**
     * Ok button selected
     */
    void accept() override;
    
private:
    Ui::BinaryFileDialog *ui; /**< Dialog GUI pointer */
    QString binfileFolder; /**< binary file folder path */
    
    /**
     * find the config.tsk file 
     * if found, config WIDTH and HEIGHT of the image
     * if not found, withhold and fill in manually
     */
    void configWidthHeight();
};


#endif

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QPointer>
#include <QInputDialog>
#include <QSettings>
#include <QMessageBox>
#include <QTextCodec>
#include <QDesktopServices>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QCoreApplication>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkLookupTable.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkDecimatePro.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkPNGWriter.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>

#include "Util/Measures.h"
#include "Util/Utils.h"
#include "Util/Sequence.h"
#include "Util/ReadBin.h"
#include "Core/Sculpture.h"
#include "Core/Sculpture.h"
#include "Core/SlicePlane.h"
#include "Core/Sphere.h"
#include "Core/CropImage.h"
#include "Chart/ColorTFChart.h"
#include "Chart/OpacityTFChart.h"
#include "GUI/ProgressDialog.h"
#include "GUI/PreferencesDialog.h"
#include "GUI/BinaryFileDialog.h"
#include "GUI/CropSequenceDialog.h"
#include "Settings/Material.h"
#include "Settings/Backgrounds.h"
#include "Interactor/InteractorStylePlaneImage.h"
#include "Interactor/InteractorStyleCropImage.h"
#include "Table/MeasureResultTable.h"

#include <opencv2/opencv.hpp>

#include <sstream>
#include <map>

class CropSequenceDialog;

namespace Ui {
    class MainWindow;
}

/**
 * @class MainWindow
 * Main window class
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * Constructor
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~MainWindow() override;

    friend class CropSequenceDialog;
    
private slots:
    // actions slots
    void on_actionOpenImageSequence_triggered();
    void on_actionExit_triggered();
    void on_actionPreferences_triggered();
    
    // button slots of volume rendering widget
    void on_openImageSequence_pressed();
    void on_openVolume_pressed();
    void on_cropSequence_pressed();
    void on_saveVolume_pressed();
    void on_exportVolumeImage_pressed();
    
    // button slots of transfer function widget
    void on_importPreset_pressed();
    void on_exportPreset_pressed();
    
    // button slots of slicePlane widget
    void on_enableDisablePlane_pressed();
    void on_resetSlice_pressed();
    void on_axialPlane_pressed();
    void on_coronalPlane_pressed();
    void on_sagitalPlane_pressed();
    void on_exportSliceImage_pressed();
    
    // button slots of measure widget
    void on_measure_pressed();
    void on_addMeasureItem_pressed();
    void on_deleteMeasureItem_pressed();
    void on_exportMeasureResults_pressed();
    
private:
    Ui::MainWindow *ui; /**< UI pointer */
    QSettings *settings; /**< Settings pointer */
    Backgrounds *backgrounds; /**< Backgrounds pointer */
    Material *material; /**< Material pointer */
    Sculpture *sculpture; /**< Sculpture pointer */
    SlicePlane *slicePlane; /**< Plane pointer */
    Sphere *sphere; /**< Sphere pointer */
    vtkSmartPointer<vtkImageViewer2> sliceViewer; /**< Slice viewer pointer */
    vtkSmartPointer<vtkRenderer> volumeRen; /**< Volume and slice plane renderer pointer */
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> volumeStyle; /**< Volume viewer interaction style */
    vtkSmartPointer<InteractorStylePlaneImage> sliceStyle; /**< Slice viewer interaction style */
    ColorTFChart *colorTFChart; /**< Color transfer function chart pointer */
    OpacityTFChart *scalarTFChart; /**< Scalar transfer function chart pointer */
    OpacityTFChart *gradientTFChart; /**< Gradient transfer function chart pointer */
    vtkSmartPointer <vtkTransform> transf; /**< tansfer function pointer */
    vtkSmartPointer <vtkAxesActor> axes; /**< axes actor pointer */
    MeasureTable *measureTable; /**< MeasureTable pointer */
    bool showPlane; /**< Show or hide plane */
    bool isMeasuring; /**< Measure mode or not */
    
    CropImage *cropImage; /**< CropImage pointer */
    vtkSmartPointer<InteractorStyleCropImage> imageStyle; /**< cropImage interactor style pointer */
    
    CropSequenceDialog *cropSequenceDialog; /**< crop sequence dialog pointer */
    
    /**
     * Set default transfer function
     */
    void defaultTF();
    
    /**
     * Set default plane position
     */
    void defaultPlanePosition();
    
    /**
     * Import transfer function preset
     */
    void importPreset();
    
    /**
     * Export preset of the current transfer function
     * @param	filename	Output filename
     */
    void exportPreset(const QString &filename);
    
    /**
     * Get exported preset filename
     * @param	defaultFilename	Default filename (transfer function name)
     * @return	Output filename
     */
    QString getExportPresetFilename(const QString &defaultFilename);
    
    /**
     * Get exported VTI filename
     * @param	defaultFilename	Default filename
     * @return	Output filename
     */
    QString getExportVTIFilename(const QString &defaultFilename);

    /**
     * Load one of the default presets
     * @param	file	File with the preset that will be used
     */
    void loadDefaultPreset(QFile *file);
    
    /**
     * Import image files
     */
    void importImageSequence();
    
    /**
     * Import VTI file
     */
    void importVTI();

    /**
     * Export VTI file
     */
    void exportVTI();
    
    /**
     * Remove volume from its viewer, reset camera and render void viewer
     */
    void removeVolume();
    
    /**
     * Add volume to its viewer, reset camera and render it
     */
    void drawVolume();
    
    /**
     * Render volume viewer
     */
    void renderVolume();
    
    /**
     * Render slice viewer
     */
    void renderSlice();
    
    /**
     * Connect components of the pipeline
     */
    void connectComponents();
    
    /**
     * Show plane in viewer
     */
    void enablePlane();
    
    /**
     * Hide plane in viewer
     */
    void disablePlane();

    /**
     * Show or hide plane in viewer
     */
    void enableDisablePlane();

    /**
     * Set plane in axial position
     */
    void axialPlane();

    /**
     * Set plane in coronal position
     */
    void coronalPlane();

    /**
     * Set plane in sagital position
     */
    void sagitalPlane();
    
    /**
     * reset slice
     */
    void resetSlice();
    
    /**
     * enable or disable measure
     */
    void enableDisableMeasure();
    
    /**
     * enable measure
     */
    void enableMeasure();
    
    /**
     * diable measure
     */
    void disableMeasure();
    
    /**
     * export CSV file of measure results
     */
    void exportExecl(QTableWidget *tableWidget);
    
    /**
     * Get exported image filename
     * @param	defaultFilename	Default filename (date and hour)
     * @return	Output filename
     */
    QString getExportImageFilename(const QString &defaultFilename);
    
    /**
     * Export image from a render window
     * @param	renWin		RenderWindow from which we will export the image
     * @param	filename	Output filename
     */
    static void exportImageFromRenderWindow(const vtkSmartPointer<vtkRenderWindow> &renWin, const QString &filename);
    
    /**
     * Launch a warning message
     * @param	message	Message that will be shown
     */
    static void launchWarning(const std::string &message);
    
    /**
     * Launch a warning message saying there is no volume loaded
     */
    static void launchWarningNoVolume();
    
    /**
     * start up the crop sequence dialog
     */
    void startUpCropSequenceDialog();
    
};

#endif // MAINWINDOW_H

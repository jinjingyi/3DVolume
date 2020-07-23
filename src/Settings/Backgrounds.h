#ifndef BACKGROUNDS_H
#define BACKGROUNDS_H

#include <QColor>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

/**
 * @class Backgrounds
 * Class with widgets background color
 */
class Backgrounds {
public:
    /**
     * Constructor
     * @params meshBackground               Mesh widget background color
     * @params volumeBackground             Volume widget background color
     * @params volumeDeletingBackground     Volume deleting widget background color
     * @params volumeSegmentingBackground   Volume segmenting widget background color
     */
    explicit Backgrounds( QColor volumeBackground);

    /**
     * Destructor
     */
    ~Backgrounds();

    /**
     * Set volume renderer
     * @param volumeRenderer    Volume renderer
     */
    void setVolumeRenderer(const vtkSmartPointer<vtkRenderer> &volumeRenderer);

    /**
     * Set volume render window
     * @param volumeRenderWindow  Volume render window
     */
    void setVolumeRenderWindow(const vtkSmartPointer<vtkRenderWindow> &volumeRenderWindow);

    /**
     * Set volume widget background color
     * @params background   Background color
     */
    void setVolumeBackground(const QColor &background);

    /**
     * Get volume widget background color
     * @return Volume widget background color
     */
    QColor getVolumeBackground() const;

private:
    QColor volumeBackground; /**< Volume viewer background color */

    vtkSmartPointer<vtkRenderer> volumeRenderer; /**< Volume and slice plane renderer pointer */
    vtkSmartPointer<vtkRenderWindow> volumeRenderWindow; /**< Volume and slice plane render window pointer */
};

#endif // BACKGROUNDS_H

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QColor>
#include <QColorDialog>
#include <QDialog>
#include <QSettings>
#include <QString>

#include "Settings/Backgrounds.h"
#include "Settings/Material.h"

class MainWindow;

namespace Ui {
    class PreferencesDialog;
}

/**
 * @class PreferencesDialog
 * Class of the filter selection dialog
 */
class PreferencesDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * Constructor
     */
    explicit PreferencesDialog(QSettings *settings, Backgrounds *backgrounds, Material *material, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~PreferencesDialog() override;

private slots:
    // GUI events
    void on_volumeBackground_pressed();
    void on_ambientValue_valueChanged(double value);
    void on_diffuseValue_valueChanged(double value);
    void on_specularValue_valueChanged(double value);
    void on_powerValue_valueChanged(double value);

    /**
     * Ok button selected
     */
    void accept() override;

private:
    Ui::PreferencesDialog *ui; /**< Dialog GUI pointer */
    QSettings *settings; /**< Settings pointer */
    Backgrounds *backgrounds; /**< Backgrounds pointer */
    Material *material; /**< Material pointer */

    /**
     * Set backgrounds inputs
     */
    void setupBackgrounds();

    /**
     * Set material inputs
     */
    void setupMaterial();
};

#endif // PREFERENCESDIALOG_H

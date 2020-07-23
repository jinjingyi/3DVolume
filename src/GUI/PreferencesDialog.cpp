#include "PreferencesDialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QSettings *settings, Backgrounds *backgrounds, Material *material, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog),
    settings(settings),
    backgrounds(backgrounds),
    material(material)
{
    ui->setupUi(this);
    setupBackgrounds();
    setupMaterial();

    // Connect button actions
    QObject::connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::setupBackgrounds() {
    ui->volumeBackground->setStyleSheet("background-color: " + backgrounds->getVolumeBackground().name());
}

void PreferencesDialog::setupMaterial() {
    ui->ambientValue->setValue(material->getAmbient());
    ui->diffuseValue->setValue(material->getDiffuse());
    ui->specularValue->setValue(material->getSpecular());
    ui->powerValue->setValue(material->getPower());
}

void PreferencesDialog::accept() {
    done(0);
}

void PreferencesDialog::on_volumeBackground_pressed() {
    QColor color = QColorDialog::getColor(backgrounds->getVolumeBackground());
    if (color.isValid()) {
        backgrounds->setVolumeBackground(color);
        settings->setValue("volume_bg", QVariant(color.rgb()));
        ui->volumeBackground->setStyleSheet("background-color: " + backgrounds->getVolumeBackground().name());
    }
}


void PreferencesDialog::on_ambientValue_valueChanged(double value) {
    material->setAmbient(value);
    settings->setValue("mat_ambient", value);
}

void PreferencesDialog::on_diffuseValue_valueChanged(double value) {
    material->setDiffuse(value);
    settings->setValue("mat_diffuse", value);
}

void PreferencesDialog::on_specularValue_valueChanged(double value) {
    material->setSpecular(value);
    settings->setValue("mat_specular", value);
}

void PreferencesDialog::on_powerValue_valueChanged(double value) {
    material->setPower(value);
    settings->setValue("mat_power", value);
}

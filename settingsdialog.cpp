#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, QSettings &set) :
    QDialog(parent),
    ui(new Ui::SettingsDialog), settings(set)
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(onAccepted()));
    ui->smsNumberEdit->setText(settings.value(SMS_NUMBER_KEY).toString());
    ui->simulateCheckBox->setChecked(settings.value(SIMULATE_KEY).toBool());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onAccepted()
{
    settings.setValue(SMS_NUMBER_KEY, ui->smsNumberEdit->text());
    settings.setValue(SIMULATE_KEY, ui->simulateCheckBox->isChecked());
}

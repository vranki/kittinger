#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsDialog;
}

#define SMS_NUMBER_KEY "sms_number"
#define SIMULATE_KEY "sms_simulate"

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent, QSettings &set);
    ~SettingsDialog();
private slots:
    void onAccepted();
private:
    Ui::SettingsDialog *ui;
    QSettings &settings;
};

#endif // SETTINGSDIALOG_H

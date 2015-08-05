#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include "MDF/MDFLib.h"
#include "mysignal.h"
#include <QMessageBox>

/* for debug */
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    double Cf = 1;
    double Cr = 1;
    double Theta = 1;
    double Lf = 1;
    double Lr = 1;
    double M = 1;
    double ESM = 1;

    QGridLayout *InitMeasurementsLayout();
    QVBoxLayout *InitEstimationSubLayout();
    ~MainWindow();

private slots:
    void on_actionLoad_Identification_Project_triggered();

    void on_actionLoad_vehicle_parameter_triggered();

    void on_actionSave_vehicle_parameter_triggered();

    void on_actionSave_Identification_Project_triggered();

    void on_actionLoad_DAS_Measurement_triggered();


    void on_Optimize_button_clicked();

    void on_Apply_Parameters_clicked();

private:
    Ui::MainWindow *ui;
    QGridLayout *MeasurementLayout;
    void openMDF(QString name);
    MDFlib::MDF *mdf;
    QList<QString> signalNameList;
    QMap<QString, mySignal*>* allSignal;
    QStringList notSelectedSignals;
    QString m_fileNameMDF;
};

#endif // MAINWINDOW_H

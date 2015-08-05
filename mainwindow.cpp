#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfiledialog.h>
#include "QTextEdit"
#include "QGridLayout"
#include <QtQuick/qquickview.h>
#include <QtGui>
#include <QtCore>
#include <qtextbrowser.h>
#include <qcustomplot.h>



int MeasurementCount, MeasurementColumns, MeasurementLines, Number, MeasurementCountOrigi;
int MeasurementCountOld = 0;

QTextBrowser *Measurements[12], *BlankSpot;
QGroupBox *Data[12];
QVBoxLayout *VerticalLayout[12];
QLabel *Euler[12], *Normal[12], *Other[12];

int MeasurementsAreLoaded = 0;
double EulerValue[12], NormalValue[12], OtherValue[12];
double EulerSumValue, NormalSumValue, OtherSumValue;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    allSignal = new QMap<QString, mySignal*>();

    /* Only numbers are accepted (Bottom, top, decimals, parent */
    /* Define the min-max values!!! */

    ui->Cf_value->setValidator( new QDoubleValidator(0, 100, 2, this) );
    ui->Cr_value->setValidator( new QDoubleValidator(0, 100, 2, this) );
    ui->Theta_value->setValidator( new QDoubleValidator(0, 100, 2, this) );
    ui->Lf_value->setValidator( new QDoubleValidator(0, 100, 2, this) );
    ui->Lr_value->setValidator( new QDoubleValidator(0, 100, 2, this) );
    ui->M_value->setValidator( new QDoubleValidator(0, 100, 2, this) );
    ui->ESM_value->setValidator( new QDoubleValidator(0, 100, 2, this) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_vehicle_parameter_triggered()
{
    QStringList LoadVehicleParameter = QFileDialog::getOpenFileNames(this,"","","Vehicle Parameter (*.veh)");
}

void MainWindow::on_actionSave_vehicle_parameter_triggered()
{
    QString SaveVehicleParameter = QFileDialog::getSaveFileName(this,"","","Vehicle Parameter (*.veh)");
}

void MainWindow::on_actionLoad_Identification_Project_triggered()
{
    QStringList LoadIdentificationProject = QFileDialog::getOpenFileNames(this,"","","Identification Project (*.ipt)");
}

void MainWindow::on_actionSave_Identification_Project_triggered()
{
    QString LoadIdentificationProject = QFileDialog::getSaveFileName(this,"","","Identification Project (*.ipt)");
}

void MainWindow::on_actionLoad_DAS_Measurement_triggered()
{
    QGridLayout *MeasurementLayout = this->InitMeasurementsLayout(); //Create a GridLayout which will contain the small icons

    /* Deleting all the widgets from the EstimationSubLayout */
    if ( ui->EstimationSubLayout != NULL )
    {
        QLayoutItem* item;
        while ( ( item = ui->EstimationSubLayout->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }

    /*Deleting all the widgets from the MeasurementLayout */
    if ( ui->MeasurementsLayout != NULL )
    {
        QLayoutItem* item;
        while ( ( item = ui->MeasurementsLayout->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        delete ui->MeasurementsLayout;
    }

    QGridLayout *MeasurementsLayout = new QGridLayout;
    QHBoxLayout *EstimationSubLayout = new QHBoxLayout;



    BlankSpot = new QTextBrowser;
    QLabel *Test = new QLabel;

    QCustomPlot *Customplott = new QCustomPlot;

        for (int i = 0; i < 12; i++)
        {
            Measurements[i] = {new QTextBrowser}; //Create an array for the loaded measurements. This will contain the actual icons respectively.
            Data[i] = new QGroupBox;
            Euler[i] = new QLabel;
            Normal[i] = new QLabel;
            Other[i] = new QLabel;
            VerticalLayout[i] = new QVBoxLayout;
        }

    QString path = "D:\\Diploma\\";

    QStringList LoadDASMeasurement = QFileDialog::getOpenFileNames(this,"",path,"DAS Measurement (*.dl3; *.dl2; *.mdf)"); //Opening measurements in .dl2 and .dl3 format
    MeasurementCount = LoadDASMeasurement.count(); //Saving the number of the loaded measurement
    MeasurementCountOrigi = MeasurementCount;
    qDebug() << "MeasurementCount at the beginning:" << MeasurementCount;

    //qDebug() << ("Measurements: %d",MeasurementCountOld);

    if (LoadDASMeasurement.empty())
    {
        //If no measurement was selected, leave everything as it is
    }
        else if (MeasurementCount > 1)
        {
            int Remain = MeasurementCount % 2; // Checking if it is an even number

            if (Remain == 1)
            {
                MeasurementCount++; //Increasing the number of the loaded measurement, to create enough columns after the division
            }

            MeasurementLines = 2; // 2 lines are used. Can be modified in the future
            MeasurementColumns = MeasurementCount / MeasurementLines;
            Number = 0;

            for (int LineCount = 0; LineCount < MeasurementLines; LineCount++)
            {
                for (int ColumnCount = 0; ColumnCount < MeasurementColumns; ColumnCount++)
                {
                    if ((Remain == 1) && (Number == MeasurementCount - 1))
                    {
                        // If uneven number of measurement is loaded, put a blankspot to the last place.
                        ui->MeasurementsLayout->addWidget(BlankSpot,LineCount,ColumnCount);
                        Number++;
                    }

                    else
                    {
                        openMDF(LoadDASMeasurement[Number]);
                        //qDebug() << LoadDASMeasurement[Number];

                        QList <mySignal*> tmp = allSignal->values();
                        qDebug() << "Csatornák száma:" << tmp.size();
                        Measurements[Number]->setText((QString::number(Number + 1)) + ": " + (LoadDASMeasurement[Number])); //LoadDASMeasurement[Number]);

                        int counter = 0;
                        /*for (int l = 0; l < tmp.size(); ++l)
                            {
                                tmp[l]->loadData();
                                if(*tmp[l]->getName() == "MAIN_CLOCK")
                                {
                                    qDebug() << "data at 0: " << tmp[l]->getData().size();
                                    counter = l;
                                }
                            }*/
                        //int sizeofyawrate = tmp[counter]->getTime().size();

                        //qDebug() << "data at 1: " << tmp[counter]->getData().value(1);

                        //QVector<double> x = tmp[counter]->getTime();
                        //QVector<double> y = tmp[counter]->getData();

                        /* create and configure plottables */
                            /*QCPGraph *mainGraph = Customplott->addGraph();
                            mainGraph->setAdaptiveSampling(true);
                            mainGraph->setData(x, y);
                            mainGraph->setName("YawRate");
                            mainGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QPen(Qt::red), QBrush(Qt::white), 3));
                            mainGraph->setPen(QPen(QColor( 120,  00,  00), 1)); // Magic only happens, when line width == 1*/

                        /* rescale axes according to graph's data */
                            //mainGraph->rescaleAxes();

                        QVector<double> x(101), y(101); // initialize with entries 0..100
                        for (int i=0; i<101; ++i)
                        {
                          x[i] = i/50.0 - 1; // x goes from -1 to 1
                          y[i] = x[i]*x[i]; // let's plot a quadratic function
                        }

                        // create graph and assign data to it:
                        Customplott->addGraph();
                        Customplott->graph(0)->setData(x, y);
                        // give the axes some labels:
                        Customplott->xAxis->setLabel("Time");
                        Customplott->yAxis->setLabel("y");
                        // set axes ranges, so we see all data:
                        Customplott->xAxis->setRange(-1, 1);
                        Customplott->yAxis->setRange(0, 1);
                        Customplott->replot();



                        ui->MeasurementsLayout->addWidget(Customplott,LineCount,ColumnCount);
                        ui->MeasurementsLayout->addWidget(Measurements[Number],LineCount,ColumnCount);
                        ui->EstimationSubLayout->addWidget(Measurements[Number]);

                        /*for(QStringList::iterator it = selectedSignals.begin(); it != selectedSignals.end(); it++)
                            {
                                mySignal* signal = allSignal->value(*it);
                                if(!(signal->isLoaded())) // egĂ©sz pontosan itt!!
                                {
                                    signal->loadData();
                                    graphicWidget->addNewChart(signal->getTime(), signal->getData(), *signal->getName());
                                    qDebug() << "trying to add item to the model";
                                    signalWidget->m_modelManager->addItem(*signal->getName(), signal->getData(), *signal->getUnit(), true);
                                    //signal->setChart(chart);
                                }
                                //signal->setCheckBox(signalWidget->addItem(*it, *signal->getUnit(), signal->getData(), signal->getChart()), signal->isVisible());
                            } */


                        Number++;
                    }
                }
            }
        }

        else if (MeasurementCount = 1)
        {

        openMDF(LoadDASMeasurement[0]);
        //qDebug() << LoadDASMeasurement[0];

        QList <mySignal*> tmp = allSignal->values();
        //qDebug() << "Number of ASAPs:" << tmp.size();
        //Measurements[0]->setText(LoadDASMeasurement[0]);

        int counter = 0;
        for (int l = 0; l < tmp.size(); ++l)
            {
                tmp[l]->loadData();

                if(*tmp[l]->getName() == "RAWYR")
                {
                    //qDebug() << "Size of Main_Clock: " << tmp[l]->getTime().count();
                    counter = l;
                    //qDebug() << "Number of main_clock in the queue:" << counter;
                }
            }
        int sizeofyawrate = tmp[counter]->getTime().size();

       // qDebug() << "data at 1: " << tmp[counter]->getData().value(0);

        QVector<double> x = tmp[counter]->getTime();
        //qDebug() << "Size of x: " << x.size();
        QVector<double> y = tmp[counter]->getData();

        /* create and configure plottables */
            QCPGraph *mainGraph = Customplott->addGraph();
            mainGraph->setAdaptiveSampling(true);
            mainGraph->setData(x, y);
            mainGraph->setName("YawRate");
            mainGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QPen(Qt::red), QBrush(Qt::white), 3));
            mainGraph->setPen(QPen(QColor( 120,  00,  00), 1)); // Magic only happens, when line width == 1

        /* rescale axes according to graph's data */
            mainGraph->rescaleAxes();
            //wideAxisRect->axis(QCPAxis::atLeft, 1)->setRangeLower(0);

        // create graph and assign data to it:
        //Customplott->addGraph();
        //Customplott->graph(0)->setData(x, y);
        // give the axes some labels:
        //Customplott->xAxis->setLabel("Time");
        //Customplott->yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        //Customplott->xAxis->setRange(-1, 1);
        //Customplott->yAxis->setRange(0, 1);
        //Customplott->replot();
        ui->MeasurementsLayout->addWidget(Customplott,0,0);




        }

        else
        {

        }

    for (int k = 0; k < MeasurementCountOrigi; k++)
    {
        //qDebug() << ("MeasurementCount:%d", MeasurementCount);
        ui->EstimationSubLayout->setAlignment(Qt::AlignLeft);
        ui->EstimationSubLayout->addWidget(Data[k]);

        if (k == 0)
        {
            EulerValue[k] = 0;
            NormalValue[k] = 0;
            OtherValue[k] = 0;
        }
        else
        {
            EulerValue[k] = EulerValue[k - 1] + 0.1;
            NormalValue[k] = NormalValue[k - 1] + 0.1;
            OtherValue[k] = OtherValue[k - 1] + 0.1;
        }

        Euler[k]->setNum(EulerValue[k]);
        Normal[k]->setNum(NormalValue[k]);
        Other[k]->setNum(OtherValue[k]);

        Data[k]->setLayout(VerticalLayout[k]);
        Data[k]->setTitle(QString::number(k+1));


        VerticalLayout[k]->addWidget(Normal[k]);
        VerticalLayout[k]->addWidget(Euler[k]);
        VerticalLayout[k]->addWidget(Other[k]);

        EulerSumValue += EulerValue[k];
        NormalSumValue += NormalValue[k];
        OtherSumValue += OtherValue[k];

    }

    qDebug() << "Euler összeg:" << EulerSumValue;
    qDebug() << "Normál összeg:" << NormalSumValue;
    qDebug() << "Other összeg:" << OtherSumValue;
    qDebug() << "MeasurementsCountOrigi" << MeasurementCountOrigi;

    EulerSumValue /= MeasurementCountOrigi;
    NormalSumValue /= MeasurementCountOrigi;
    OtherSumValue /= MeasurementCountOrigi;
    qDebug() << "Euler átlag:" << EulerSumValue;
    qDebug() << "Normál átlag:" << NormalSumValue;
    qDebug() << "Other átlag:" << OtherSumValue;
    //delete ui->EstimationSubLayout;

    ui->EstimationSubLayout->addWidget(Data[MeasurementCount]);

    Euler[MeasurementCount]->setNum(EulerSumValue);
    Normal[MeasurementCount]->setNum(NormalSumValue);
    Other[MeasurementCount]->setNum(OtherSumValue);

    Data[MeasurementCount]->setLayout(VerticalLayout[MeasurementCount]);
    Data[MeasurementCount]->setTitle("Sum");

    VerticalLayout[MeasurementCount]->addWidget(Normal[MeasurementCount]);
    VerticalLayout[MeasurementCount]->addWidget(Euler[MeasurementCount]);
    VerticalLayout[MeasurementCount]->addWidget(Other[MeasurementCount]);

    ui->MeasurementsWidget->setLayout(MeasurementLayout); //Assign the widget to the GridLayout
    ui->EstimationDataGroupBox->setLayout(EstimationSubLayout);
    MeasurementCountOld = MeasurementCount;
    MeasurementsAreLoaded = 1;
}

QGridLayout *MainWindow::InitMeasurementsLayout()
{
    delete ui->MeasurementsLayout; //To be able to open new measurements
    QGridLayout *MeasurementsLayout = new QGridLayout;
    ui->MeasurementsLayout->setHorizontalSpacing(3);
    ui->MeasurementsLayout->setVerticalSpacing(3);
    MeasurementsLayout->setMargin(3);
    return MeasurementsLayout;
}

QVBoxLayout *MainWindow::InitEstimationSubLayout()
{
    delete ui->EstimationSubLayout; //To be able to open new measurements
    QVBoxLayout *EstimationSubLayout = new QVBoxLayout;
    return EstimationSubLayout;
}

///
/// \brief MeasuringWindow::openMDF
/// \param name This is the file name
/// This function will load the MDF file into memory
/// QList<QString> signalNameList;
/// QMap<QString, mySignal*>* allSignal;
/// QStringList notSelectedSignals;
void MainWindow::openMDF(QString name)
{
    QList<MDFlib::MDFDG*> *dgList;
    QList<MDFlib::MDFCG*> *cgList;
    QList<MDFlib::MDFChannel*> *cnList;
    mdf = new MDFlib::MDF();
    signalNameList.clear();
    allSignal->clear();
    notSelectedSignals.clear();
    //signalNameList = new QList<QString>();


    int numberOfSignals = 0;
    bool error = mdf->Open(& name);
    if(error)
    {
        QMessageBox mb(QMessageBox::Critical, tr("Unable to Open"), tr("Unable to open the MDF file!"));
        mb.exec();
        //ui->statusbar->showMessage("ERROR: File not loaded!", 10000);
    }
    else
    {
        //ui->statusbar->showMessage("Loading from file!");
        dgList = mdf->getDataGroups();
        for(QList<MDFlib::MDFDG*>::iterator dgIt = dgList->begin(); dgIt != dgList->end(); dgIt++)
        {
            cgList = (*dgIt)->getChannelGroups();
            for(QList<MDFlib::MDFCG*>::iterator cgIt = cgList->begin(); cgIt != cgList->end(); cgIt++)
            {
                cnList = (*cgIt)->getChannels();
                for(QList<MDFlib::MDFChannel*>::iterator cnIt = cnList->begin(); cnIt != cnList->end(); cnIt++)
                {
                    signalNameList.append(*((*cnIt)->getNameOfSignal()));
                    numberOfSignals++;

                    mySignal* signal = new mySignal(*cnIt);
                    allSignal->insert(*signal->getName(), signal);
                }
            }
        }
        notSelectedSignals.append(signalNameList);
        //ui->statusbar->showMessage("Signals loaded!", 15000);
        //ui->actionMDF_file_info->setEnabled(true);
        //ui->actionSelect_Signals->setEnabled(true);
        m_fileNameMDF = name;
    }
    //ui->statusbar->clearMessage();
}

void MainWindow::on_Optimize_button_clicked()
{

    QMessageBox messageBox;

    /* At least 1 measurement needs to be loaded before we can start the optimization */
    if ( MeasurementCountOrigi == 0)
    {
        messageBox.warning(0,"Error", "Open at least 1 measurement!");
        messageBox.setFixedSize(500,200);
    }
    /* Empty lines are not accepted */
    else if ((ui->Cf_value->text().isEmpty()) || (ui->Cr_value->text().isEmpty()) || (ui->Theta_value->text().isEmpty()))
    {
        messageBox.warning(0,"Error", "Enter values for all of the parameters!");
        messageBox.setFixedSize(500,200);
    }
    /* Zero values are not accepted */
    else if ((QString(ui->Cf_value->text()).toDouble(0) == 0) || (QString(ui->Cr_value->text()).toDouble(0) == 0) || (QString(ui->Theta_value->text()).toDouble(0) == 0))
    {
        messageBox.warning(0,"Error", "Parameter values must not be 0!");
        messageBox.setFixedSize(500,200);
    }
    else
    {
        EulerSumValue = 0;
        NormalSumValue = 0;
        OtherSumValue = 0;
        Cf = QString(ui->Cf_value->text()).toDouble(0);
        Cr = QString(ui->Cr_value->text()).toDouble(0);
        Theta = QString(ui->Theta_value->text()).toDouble(0);
        qDebug() << "Cf értéke: " << Cf;
        qDebug() << "Cr értéke: " << Cr;
        qDebug() << "Theta értéke: " << Theta;

        for (int j = 0; j < MeasurementCountOrigi; j++)
        {
            Euler[j]->setNum(Cr);
            Normal[j]->setNum(Cf);
            Other[j]->setNum(Theta);
            EulerSumValue += Cr;
            NormalSumValue += Cf;
            OtherSumValue += Theta;
            qDebug() << "Euler összeg:" << EulerSumValue;
            qDebug() << "Normál összeg:" << NormalSumValue;
            qDebug() << "Other összeg:" << OtherSumValue;
        }


        qDebug() << "MeasurementsCountOrigi" << MeasurementCountOrigi;

        /*EulerSumValue /= MeasurementCountOrigi;
        NormalSumValue /= MeasurementCountOrigi;
        OtherSumValue /= MeasurementCountOrigi;*/
        qDebug() << "Euler átlag:" << EulerSumValue;
        qDebug() << "Normál átlag:" << NormalSumValue;
        qDebug() << "Other átlag:" << OtherSumValue;

        Euler[MeasurementCountOrigi]->setNum(EulerSumValue);
        Normal[MeasurementCountOrigi]->setNum(NormalSumValue);
        Other[MeasurementCountOrigi]->setNum(OtherSumValue);

        Data[MeasurementCountOrigi]->setLayout(VerticalLayout[MeasurementCountOrigi]);
        Data[MeasurementCountOrigi]->setTitle("Sum");

        VerticalLayout[MeasurementCountOrigi]->addWidget(Normal[MeasurementCountOrigi]);
        VerticalLayout[MeasurementCountOrigi]->addWidget(Euler[MeasurementCountOrigi]);
        VerticalLayout[MeasurementCountOrigi]->addWidget(Other[MeasurementCountOrigi]);
    }
}


void MainWindow::on_Apply_Parameters_clicked()
{
    QMessageBox messageBox;

    /* At least 1 measurement needs to be loaded before we can start the optimization */
    /* Should it be active when no measurement is loaded? */

    /*if ( MeasurementCountOrigi == 0)
    {
        messageBox.warning(0,"Error", "Open at least 1 measurement!");
        messageBox.setFixedSize(500,200);
    }*/

    /* Empty lines are not accepted */
    /*else*/ if ((ui->Lf_value->text().isEmpty()) || (ui->Lr_value->text().isEmpty()) || (ui->M_value->text().isEmpty()) || (ui->ESM_value->text().isEmpty()))
    {
        messageBox.warning(0,"Error", "Enter values for all of the parameters!");
        messageBox.setFixedSize(500,200);
    }
    /* Zero values are not accepted */
    else if ((QString(ui->Lf_value->text()).toDouble(0) == 0) || (QString(ui->Lr_value->text()).toDouble(0) == 0) || (QString(ui->M_value->text()).toDouble(0) == 0) || (QString(ui->ESM_value->text()).toDouble(0) == 0))
    {
        messageBox.warning(0,"Error", "Parameter values must not be 0!");
        messageBox.setFixedSize(500,200);
    }
    else
    {

    }
}

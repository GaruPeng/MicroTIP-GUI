#include "scope.h"
#include <QDateTime>
#include <QPushButton>
#include <QGridLayout>


Scope::Scope(QWidget *parent) : QWidget(parent)
{
    /* Initialize the scope window */
    maxSize = 31;   /* Store data points on scope */
    maxX = 3000;
    maxY = 1000;
    enable = 0;
    splineSeries = new QSplineSeries();
    scatterSeries = new QScatterSeries();
    scatterSeries->setMarkerSize(8);
    chart = new QChart();
    chart->addSeries(splineSeries);
    chart->addSeries(scatterSeries);
    chart->legend()->hide();
    chart->setTitle("Signal");
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, maxX);  /* Axis X */
    chart->axisY()->setRange(-1, 1);    /* Axis Y */
    chart->axisX()->setTitleText("Time (msec)");
    chart->axisY()->setTitleText("Voltage (V)");
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    /* Layout Manager */
    QCheckBox *ckbRun = new QCheckBox("Run / Stop");
    connect(ckbRun,SIGNAL(clicked(bool)),this,SLOT(EnableReceive(bool)));
    QPushButton *btnSave = new QPushButton("Save");
    btnSave->setGeometry(0, 0, 112, 28);
    connect(btnSave,SIGNAL(clicked(bool)),this,SLOT(SaveToFile()));
    QGridLayout *layout = new QGridLayout();
    QGridLayout *layout1 = new QGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    layout1->addWidget(ckbRun);
    layout1->addWidget(btnSave);
    layout->addLayout(layout1,0,1);
    layout->setMargin(10);
    layout->setSpacing(10);
    setLayout(layout);

    /* Random data generator */
    timerId = startTimer(100);
    qsrand(QDateTime::currentDateTime().toTime_t());
}


Scope::~Scope()
{}


void Scope::timerEvent(QTimerEvent *event)
{
    /* Generate continuous random data for simulation */
    /* Should be deleted if there is a real signal */
    if (event->timerId() == timerId) {
        double newData = ((qrand() % (maxY + 1))-(maxY/2)) *2 *0.001;
        dataReceived(newData);
    }
}


void Scope::dataReceived(double value)
{
    if(enable==1)
    {
        /* Data recording */
        plotingdata << value;
        recordingData << value;
        /* Delete the first point when data size exceeds maxSize */
        while (plotingdata.size() > maxSize) {
            plotingdata.removeFirst();
        }

        if (isVisible()) {
            /* Update the scope */
            splineSeries->clear();
            scatterSeries->clear();
            int dx = maxX / (maxSize-1);
            int less = maxSize - plotingdata.size();
            for (int i = 0; i < plotingdata.size(); ++i) {
            splineSeries->append(less*dx+i*dx, plotingdata.at(i));
            scatterSeries->append(less*dx+i*dx, plotingdata.at(i));
            }
        }
    }
}


void Scope::Record(double value)
{
    dataReceived(value);
}


void Scope::EnableReceive(bool chk)
{
    enable = chk;   /* Enable data receving and the scope  */
}


void Scope::SaveToFile()
{
    /* Open savefile dialog */
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Data points"),"",
                                                    tr("Text files (*.txt);;CSV files (*.csv);;All Files (*)"));
    if (fileName.isEmpty())
           return;
    else
    {
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QMessageBox::information(this, tr("Unable to open file"),file.errorString());
            return;
        }

        QListIterator<double> recordingIter(recordingData);
        QTextStream out(&file);
        out.setCodec("UTF-8");
        while(recordingIter.hasNext()){
            out << recordingIter.next() << endl;    /* one point for each row */
        }
        file.close();
    }
}

#ifndef SCOPE_H
#define SCOPE_H

#include <QWidget>
#include <QList>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

class Scope : public QWidget
{
    Q_OBJECT
public:
    explicit Scope(QWidget *parent = nullptr);
    ~Scope();
    QList<double> recordingData; // data list for whole recorded signal

protected:
    //void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE; // only for testing

private:
    void dataReceived(double value);
    int timerId;
    int maxSize;  // the size of datalist
    int maxX;
    int maxY;
    bool enable;
    QList<double> plotingdata; // data list for showing
    QChart *chart;
    QChartView *chartView;
    QSplineSeries *splineSeries;
    QScatterSeries *scatterSeries;


signals:

public slots:
    void Record(double value);
    void SaveToFile();
    void EnableReceive(bool chk);


};


#endif // SCOPE_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QPushButton>
#include <QLCDNumber>
#include <QTimer>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
//#include "digitalclock.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    int counter;
    QLabel* label;
    QLabel* lbl_clk;
    QLabel* lbl_wc_temp;
    QLabel* lbl_wc_min;
    QLabel* lbl_wc_max;
    QLabel* lbl_wf1_temp;
    QLabel* lbl_wf1_min;
    QLabel* lbl_wf1_max;
    QLabel* lbl_wf2_temp;
    QLabel* lbl_wf2_min;
    QLabel* lbl_wf2_max;
    QLabel* lbl_wf3_temp;
    QLabel* lbl_wf3_min;
    QLabel* lbl_wf3_max;
    QLabel* lbl_wf4_temp;
    QLabel* lbl_wf4_min;
    QLabel* lbl_wf4_max;
    QLabel* lbl_wf5_temp;
    QLabel* lbl_wf5_min;
    QLabel* lbl_wf5_max;
    QLabel* lbl_wf6_temp;
    QLabel* lbl_wf6_min;
    QLabel* lbl_wf6_max;
    QLabel* lbl_wf7_temp;
    QLabel* lbl_wf7_min;
    QLabel* lbl_wf7_max;


    QPushButton* bntIncr;
    QPushButton* bntDec;
    QPushButton* bntclk;
    QFont* clkfont;
    virtual ~MainWindow();
    QByteArray downloadedData() const;
    //Current Weather Data
    QString wc_base;
    double wc_clouds_all;
    double wc_cod;
    double wc_coord_lat;
    double wc_coord_lon;
    double wc_dt; //time of data calculation
    double wc_id; //city id
    QString wc_main_feels_like;
    double wc_main_humidity;
    double wc_main_pressure;
    QString wc_main_temp;
    QString wc_main_temp_max;
    QString wc_main_temp_min;
    QString wc_name; //City Name
    QString wc_sys_country;
    double wc_sys_id; // internal parameter
    double wc_sys_sunrise;
    double wc_sys_sunset;
    double wc_sys_type; // internal parameter
    double wc_timezone; // need for unix timsestamp calculation
    double wc_visibility; //in meter
    QString wc_weather_description;
    QString wc_weather_icon;
    double wc_weather_id;
    QString wc_weather_main;
    double wc_wind_deg;
    double wc_wind_speed;
    //Forecast Weather Data
    QString wf_temp_day[8];
    QString wf_temp_eve[8];
    QString wf_temp_max[8];
    QString wf_temp_min[8];
    QString wf_temp_morn[8];
    QString wf_temp_night[8];
    double wf_dt[8];
    QString wf_weather_description[8];
    QString wf_weather_icon[8];
    double wf_weather_id[8];
    QString wf_weather_main[8];

public slots:
  void buttonClicked();
  void button_2Clicked();
  void button_clkClicked();
  void showTime();
  bool urlExists();
  void refreshCurrentWeather();
  void refreshForecastWeather();


private:
  QNetworkAccessManager m_WebCtrl;
  QNetworkAccessManager m_forecase_WebCtrl;
  QByteArray m_DownloadedData;

private slots:
  void createMenus();
  void fileDownloaded(QNetworkReply* pReply);
  void forecastfileDownloaded(QNetworkReply* pReply);
  void loadCurrentWeather();
  void showCurrentWather();
  void loadForecastWeather();
  void setWeatherShow();



signals:
  void downloaded();
  void forecastdownloaded();

};



#endif // MAINWINDOW_H

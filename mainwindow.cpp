#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTime>
#include <QTimer>
#include <QPixmap>
#include <QListView>
#include <QTcpSocket>
#include <QUrl>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#define ZERO_KELVIN 273.15


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //MainWindow::setStyleSheet("background-color:darkblue;");
    MainWindow::setStyleSheet("color: white; background-color: grey");
    QTimer *timer = new QTimer(this);
    QTimer *wc_timer = new QTimer(this);
    QTimer *wf_timer = new QTimer(this);

    timer->start(1000);
    wc_timer->start(600000);
    wf_timer->start(10800000);
    counter = 0;
    lbl_clk = new QLabel("time", this);
    lbl_clk->setGeometry(370, 25, 150, 30);
    QFont fnt_clk( "Verdana", 24, QFont::Bold);
    lbl_clk->setFont(fnt_clk);
    setWeatherShow();
    bntIncr = new QPushButton( "SmartHome", this);
    bntIncr->setGeometry(0, 270, 150, 50);
    bntDec = new QPushButton( "Entertainment", this);
    bntDec->setGeometry(150, 270, 150, 50);
    bntclk = new QPushButton("Statistics", this);
    bntclk->setGeometry(300, 270, 150, 50);
    refreshCurrentWeather();
    MainWindow::refreshForecastWeather();
    QObject::connect(timer, &QTimer::timeout, this, &MainWindow::showTime);
    QObject::connect(bntIncr, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    QObject::connect(bntDec, SIGNAL(clicked()), this, SLOT(button_2Clicked()));
    QObject::connect(bntclk, SIGNAL(clicked()), this, SLOT(button_clkClicked()));
    QObject::connect(wc_timer, &QTimer::timeout, this, &MainWindow::refreshCurrentWeather);
    QObject::connect(wf_timer, &QTimer::timeout, this, &MainWindow::refreshForecastWeather);
    connect(this, SIGNAL (downloaded()), SLOT (loadCurrentWeather()));
    connect(this, SIGNAL (forecastdownloaded()), SLOT (loadForecastWeather()));

}

MainWindow::~MainWindow() {
}

void MainWindow::setWeatherShow(){
    //Global Size Variables
    int temp_height = 40;
    int temp_width = 150;
    int min_height = 25;
    int max_height = 25;
    int min_width = 150;
    int max_width = 150;
    int temp_x_pos = 100;
    int min_x_pos = temp_x_pos+15;
    int max_x_pos = temp_x_pos-5;
    // Current Lables
    lbl_wc_temp = new QLabel("wc_temp", this);
    lbl_wc_temp->setGeometry(0, temp_x_pos, temp_width, temp_height);
    lbl_wc_min = new QLabel("wc_min", this);
    lbl_wc_min->setGeometry(50, min_x_pos, 150, min_height);
    lbl_wc_min->setStyleSheet("color:rgb(51,187,255);");
    lbl_wc_max = new QLabel("wc_max", this);
    lbl_wc_max->setGeometry(50, max_x_pos, 150, max_height);
    lbl_wc_max->setStyleSheet("color:rgb(255,102,153);");
    // Day Plus 1
    lbl_wf1_temp = new QLabel("wf_temp", this);
    lbl_wf1_temp->setGeometry(80, temp_x_pos, temp_width, temp_height);
    lbl_wf1_min = new QLabel("wf_min", this);
    lbl_wf1_min->setGeometry(130, min_x_pos, min_width, min_height);
    lbl_wf1_min->setStyleSheet("color:rgb(51,187,255);");
    lbl_wf1_max = new QLabel("wf_max", this);
    lbl_wf1_max->setGeometry(130, max_x_pos, max_width, max_height);
    lbl_wf1_max->setStyleSheet("color:rgb(255,102,153);");
    // Day Plus 2
    lbl_wf2_temp = new QLabel("wf_temp", this);
    lbl_wf2_temp->setGeometry(160, temp_x_pos, temp_width, temp_height);
    lbl_wf2_min = new QLabel("wf_min", this);
    lbl_wf2_min->setGeometry(210, min_x_pos, min_width, min_height);
    lbl_wf2_min->setStyleSheet("color:rgb(51,187,255);");
    lbl_wf2_max = new QLabel("wf_max", this);
    lbl_wf2_max->setGeometry(210, max_x_pos, max_width, max_height);
    lbl_wf2_max->setStyleSheet("color:rgb(255,102,153);");
    // Day Plus 3
    lbl_wf3_temp = new QLabel("wf_temp", this);
    lbl_wf3_temp->setGeometry(240, temp_x_pos, temp_width, temp_height);
    lbl_wf3_min = new QLabel("wf_min", this);
    lbl_wf3_min->setGeometry(290, min_x_pos, min_width, min_height);
    lbl_wf3_min->setStyleSheet("color:rgb(51,187,255);");
    lbl_wf3_max = new QLabel("wf_max", this);
    lbl_wf3_max->setGeometry(290, max_x_pos, max_width, max_height);
    lbl_wf3_max->setStyleSheet("color:rgb(255,102,153);");
    // Day Plus 4
    lbl_wf4_temp = new QLabel("wf_temp", this);
    lbl_wf4_temp->setGeometry(320, temp_x_pos, temp_width, temp_height);
    lbl_wf4_min = new QLabel("wf_min", this);
    lbl_wf4_min->setGeometry(370, min_x_pos, min_width, min_height);
    lbl_wf4_min->setStyleSheet("color:rgb(51,187,255);");
    lbl_wf4_max = new QLabel("wf_max", this);
    lbl_wf4_max->setGeometry(370, max_x_pos, max_width, max_height);
    lbl_wf4_max->setStyleSheet("color:rgb(255,102,153);");
    // Day Plus 4
    lbl_wf5_temp = new QLabel("wf_temp", this);
    lbl_wf5_temp->setGeometry(400, temp_x_pos, temp_width, temp_height);
    lbl_wf5_min = new QLabel("wf_min", this);
    lbl_wf5_min->setGeometry(450, min_x_pos, min_width, min_height);
    lbl_wf5_min->setStyleSheet("color:rgb(51,187,255);");
    lbl_wf5_max = new QLabel("wf_max", this);
    lbl_wf5_max->setGeometry(450, max_x_pos, max_width, max_height);
    lbl_wf5_max->setStyleSheet("color:rgb(255,102,153);");


    QFont fnt_wc_temp( "Verdana", 32, QFont::Bold);
    lbl_wc_temp->setFont(fnt_wc_temp);
    lbl_wf1_temp->setFont(fnt_wc_temp);
    lbl_wf2_temp->setFont(fnt_wc_temp);
    lbl_wf3_temp->setFont(fnt_wc_temp);
    lbl_wf4_temp->setFont(fnt_wc_temp);
    lbl_wf5_temp->setFont(fnt_wc_temp);



    QFont fnt_wc_min( "Verdana", 12, QFont::Bold);
    lbl_wc_min->setFont(fnt_wc_min);
    lbl_wf1_min->setFont(fnt_wc_min);
    lbl_wf2_min->setFont(fnt_wc_min);
    lbl_wf3_min->setFont(fnt_wc_min);
    lbl_wf4_min->setFont(fnt_wc_min);
    lbl_wf5_min->setFont(fnt_wc_min);

    QFont fnt_wc_max( "Verdana", 12, QFont::Bold);
    lbl_wc_max->setFont(fnt_wc_max);
    lbl_wf1_max->setFont(fnt_wc_max);
    lbl_wf2_max->setFont(fnt_wc_max);
    lbl_wf3_max->setFont(fnt_wc_max);
    lbl_wf4_max->setFont(fnt_wc_max);
    lbl_wf5_max->setFont(fnt_wc_max);

}
void MainWindow::refreshForecastWeather() {
    qInfo() << "refresh Forecast Weather";
    QUrl url("http://api.openweathermap.org/data/2.5/onecall");
    QUrlQuery query;
    query.addQueryItem("lat", "52.52");
    query.addQueryItem("lon", "13.41");
    query.addQueryItem("exclude", "minutely, hourly, alerts");
    query.addQueryItem("mode", "json");
    query.addQueryItem("APPID", "a9a98c5f3e757d9e21d7e0cd9282f518");
    url.setQuery(query);
    connect(&m_forecase_WebCtrl, SIGNAL (finished(QNetworkReply*)),
    SLOT (forecastfileDownloaded(QNetworkReply*)));
    QNetworkRequest request(url);
    m_forecase_WebCtrl.get(request);

}

void MainWindow::refreshCurrentWeather(){
    qInfo() << "refresh Current Weather";
    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;
    query.addQueryItem("q", "Berlin");
    query.addQueryItem("mode", "json");
    query.addQueryItem("APPID", "a9a98c5f3e757d9e21d7e0cd9282f518");
    url.setQuery(query);
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
    SLOT (fileDownloaded(QNetworkReply*)));
    QNetworkRequest request(url);
    m_WebCtrl.get(request);
}

void MainWindow::fileDownloaded(QNetworkReply* pReply)
{
    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();
    emit downloaded();
}

void MainWindow::forecastfileDownloaded(QNetworkReply* pReply)
{
    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();
    emit forecastdownloaded();
}

QByteArray MainWindow::downloadedData() const
{
    return m_DownloadedData;
}

static QString niceTemperatureString(double t)
{
    return QString::number(qRound(t-ZERO_KELVIN)) + QChar(0xB0);
}

void MainWindow::loadCurrentWeather() {
    qInfo() << QDateTime::currentDateTime();
    qInfo() << "Current Weather";
    QJsonDocument document = QJsonDocument::fromJson(downloadedData());
    qInfo() << "Something was downloaded";
    qInfo() << document;

    if (document.isObject()) {
        QJsonObject obj = document.object();
        QJsonObject tempObject;
        QJsonArray tempArray;
        QJsonValue tempString;
        QJsonValue val;

        if (obj.contains(QStringLiteral("main"))) {
            val = obj.value(QStringLiteral("main"));
            tempObject = val.toObject();
            val = tempObject.value(QStringLiteral("temp"));
            //d->now.setTemperature(niceTemperatureString(val.toDouble()));
        }
        if (obj.contains(QStringLiteral("clouds"))) {
            val = obj.value(QStringLiteral("clouds"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("all"));
            wc_clouds_all = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("cod"))) {
            val = obj.value(QStringLiteral("cod"));
            wc_cod = val.toDouble();
        }
        if (obj.contains(QStringLiteral("coord"))) {
            val = obj.value(QStringLiteral("coord"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("lat"));
            wc_coord_lat = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("coord"))) {
            val = obj.value(QStringLiteral("coord"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("lat"));
            wc_coord_lon = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("coord"))) {
            val = obj.value(QStringLiteral("coord"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("lon"));
            wc_coord_lon = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("base"))) {
            val = obj.value(QStringLiteral("base"));
            wc_base = val.toString();
        }
        if (obj.contains(QStringLiteral("dt"))) {
            val = obj.value(QStringLiteral("dt"));
            wc_dt = val.toDouble();
        }
        if (obj.contains(QStringLiteral("id"))) {
            val = obj.value(QStringLiteral("id"));
            wc_id = val.toDouble();
        }
        if (obj.contains(QStringLiteral("main"))) {
            val = obj.value(QStringLiteral("main"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("feels_like"));
            wc_main_feels_like = niceTemperatureString(tempString.toDouble());
        }
        if (obj.contains(QStringLiteral("main"))) {
            val = obj.value(QStringLiteral("main"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("humidity"));
            wc_main_humidity = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("main"))) {
            val = obj.value(QStringLiteral("main"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("pressure"));
            wc_main_pressure = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("main"))) {
            val = obj.value(QStringLiteral("main"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("temp"));
            wc_main_temp = niceTemperatureString(tempString.toDouble());
        }
        if (obj.contains(QStringLiteral("main"))) {
            val = obj.value(QStringLiteral("main"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("temp_max"));
            wc_main_temp_max = niceTemperatureString(tempString.toDouble());
        }
        if (obj.contains(QStringLiteral("main"))) {
            val = obj.value(QStringLiteral("main"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("temp_min"));
            wc_main_temp_min = niceTemperatureString(tempString.toDouble());
        }
        if (obj.contains(QStringLiteral("name"))) {
            val = obj.value(QStringLiteral("name"));
            //tempObject = val.toObject();
            //tempString = tempObject.value(QStringLiteral("name"));
            wc_name = val.toString();
        }
        if (obj.contains(QStringLiteral("sys"))) {
            val = obj.value(QStringLiteral("sys"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("country"));
            wc_sys_country = tempString.toString();
        }
        if (obj.contains(QStringLiteral("sys"))) {
            val = obj.value(QStringLiteral("sys"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("id"));
            wc_sys_id = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("sys"))) {
            val = obj.value(QStringLiteral("sys"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("sunrise"));
            wc_sys_sunrise = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("sys"))) {
            val = obj.value(QStringLiteral("sys"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("sunset"));
            wc_sys_sunset = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("sys"))) {
            val = obj.value(QStringLiteral("sys"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("type"));
            wc_sys_type = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("timezone"))) {
            val = obj.value(QStringLiteral("timezone"));
            wc_timezone = val.toDouble();
        }
        if (obj.contains(QStringLiteral("visibility"))) {
            val = obj.value(QStringLiteral("visibility"));
            wc_visibility = val.toDouble();
        }
        if (obj.contains(QStringLiteral("weather"))) {
            val = obj.value(QStringLiteral("weather"));
            tempArray = val.toArray();
            val = tempArray.at(0);
            tempObject = val.toObject();
            wc_weather_description = tempObject.value(QStringLiteral("description")).toString();
            wc_weather_icon = tempObject.value(QStringLiteral("icon")).toString();
            wc_weather_id = tempObject.value(QStringLiteral("id")).toDouble();
            wc_weather_main = tempObject.value(QStringLiteral("main")).toString();
        }

        if (obj.contains(QStringLiteral("wind"))) {
            val = obj.value(QStringLiteral("wind"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("deg"));
            wc_wind_deg = tempString.toDouble();
        }
        if (obj.contains(QStringLiteral("wind"))) {
            val = obj.value(QStringLiteral("wind"));
            tempObject = val.toObject();
            tempString = tempObject.value(QStringLiteral("speed"));
            wc_wind_speed = tempString.toDouble();
        }
    }
MainWindow::showCurrentWather();
}

void MainWindow::loadForecastWeather(){
    qInfo() << "5 Day Forecast";
    QJsonDocument document = QJsonDocument::fromJson(downloadedData());
    qInfo() << "Something was downloaded";
    qInfo() << document;

    if (document.isObject()) {
        QJsonObject root = document.object();
        QJsonObject jo;
        QJsonArray jv;
        QJsonArray tempArray;
        QJsonValue jj;
        QJsonObject tempObject;
        QJsonValue tempString;
        jj = root.value(QStringLiteral("daily"));
        jv = jj.toArray();
        for(int i =0;i<8;i++){
            jj = jv.at(i);
            jo = jj.toObject();
            if (jo.contains(QStringLiteral("temp"))) {
                jj = jo.value(QStringLiteral("temp"));
                tempObject = jj.toObject();
                tempString = tempObject.value(QStringLiteral("day"));
                 wf_temp_day[i] = niceTemperatureString(tempString.toDouble());
                 tempString = tempObject.value(QStringLiteral("eve"));
                 wf_temp_eve[i] = niceTemperatureString(tempString.toDouble());
                 tempString = tempObject.value(QStringLiteral("max"));
                 wf_temp_max[i] = niceTemperatureString(tempString.toDouble());
                 tempString = tempObject.value(QStringLiteral("min"));
                 wf_temp_min[i] = niceTemperatureString(tempString.toDouble());
                 tempString = tempObject.value(QStringLiteral("morn"));
                 wf_temp_morn[i] = niceTemperatureString(tempString.toDouble());
                 tempString = tempObject.value(QStringLiteral("night"));
                 wf_temp_night[i] = niceTemperatureString(tempString.toDouble());
            }
            if (jo.contains(QStringLiteral("dt"))) {
                jj = jo.value(QStringLiteral("dt"));
                wf_dt[i]= jj.toDouble();
            }
            if (jo.contains(QStringLiteral("weather"))) {
                jj = jo.value(QStringLiteral("weather"));
                tempArray = jj.toArray();
                jj = tempArray.at(0);
                tempObject = jj.toObject();
                wf_weather_description[i] = tempObject.value(QStringLiteral("description")).toString();
                wf_weather_icon[i] = tempObject.value(QStringLiteral("icon")).toString();
                wf_weather_id[i] = tempObject.value(QStringLiteral("id")).toDouble();
                wf_weather_main[i] = tempObject.value(QStringLiteral("main")).toString();
            }
        }

    }
}
void MainWindow::showCurrentWather(){
    lbl_wc_temp->setText(wc_main_temp);
    lbl_wc_max->setText(wf_temp_max[0]);
    lbl_wc_min->setText(wf_temp_min[0]);
    lbl_wf1_temp->setText(wf_temp_day[1]);
    lbl_wf1_max->setText(wf_temp_max[1]);
    lbl_wf1_min->setText(wf_temp_min[1]);
    lbl_wf2_temp->setText(wf_temp_day[2]);
    lbl_wf2_max->setText(wf_temp_max[2]);
    lbl_wf2_min->setText(wf_temp_min[2]);
    lbl_wf3_temp->setText(wf_temp_day[3]);
    lbl_wf3_max->setText(wf_temp_max[3]);
    lbl_wf3_min->setText(wf_temp_min[3]);
    lbl_wf4_temp->setText(wf_temp_day[4]);
    lbl_wf4_max->setText(wf_temp_max[4]);
    lbl_wf4_min->setText(wf_temp_min[4]);
    lbl_wf5_temp->setText(wf_temp_day[5]);
    lbl_wf5_max->setText(wf_temp_max[5]);
    lbl_wf5_min->setText(wf_temp_min[5]);


}

void MainWindow::buttonClicked()
{
}

void MainWindow::button_2Clicked() {
}

void MainWindow::button_clkClicked(){
}

void MainWindow::showTime()

{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    lbl_clk->setText(text);
}

void MainWindow::createMenus() {
  QMenuBar *bar = menuBar();
  QMenu *fileMenu = bar->addMenu(tr("&File"));
  fileMenu->addAction(new QAction("Open", this));
  fileMenu->addAction(new QAction("Close", this));
}

bool MainWindow::urlExists(){
    QUrl theurl("http://api.openweathermap.org/data/2.5/weather?q=London,uk&APPID=a9a98c5f3e757d9e21d7e0cd9282f518");
    QTextStream out(stdout);
    QTcpSocket socket;
    QByteArray buffer;

    socket.connectToHost(theurl.host(), 80);
    if (socket.waitForConnected()) {
        //Standard http request
        socket.write("GET / HTTP/1.1\r\n"
                 "host: " + theurl.host().toUtf8() + "\r\n\r\n");
        if (socket.waitForReadyRead()) {
            while(socket.bytesAvailable()){
                buffer.append(socket.readAll());
                int packetSize=buffer.size();
                while(packetSize>0)
                {
                    //Output server response for debugging
                    qInfo() << "[" << buffer.data() << "]" <<endl;

                    //set Url if 200, 301, or 302 response given assuming that server will redirect
                    if (buffer.contains("200 OK") ||
                        buffer.contains("302 Found") ||
                        buffer.contains("301 Moved")) {
                        return true;
                    }
                    buffer.remove(0,packetSize);
                    //packetSize=getPacketSize(buffer);
                    packetSize=buffer.size();

                } //while packet size >0
            } //while socket.bytesavail

        } //socket wait for ready read
    }//socket write
return false;
}




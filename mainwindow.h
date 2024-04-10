/**
 * @author Jwalant Patel, Ross Cameron, Lance Cheong Youne, Ojas Singh Hunjan, Matthew Morelli
 * @date 2024-03-31
 * @brief This header contains the libraries, variables and methods required for mainwindow.cpp
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cpprest/http_client.h>
#include <cpprest/json.h> // For json::value
#include <QMainWindow>
#include "csvdata.h"
#include "SpotifyAPI.h"
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QDir>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include "json.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <QListWidget>
#include <QTimer>
#include <QLabel>
#include <QIcon>
#include <QScrollArea>
#include <stdlib.h>
#include <QMessageBox>

using namespace std;

class MainWindow : public QMainWindow {
  Q_OBJECT

// initialize public, protected and private variables
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
  void onVolumeChanged(int value);
  void updateCurrentTrack();
  void trackPlayButtonClicked();

private slots:
  void playButtonClicked();
  void pauseButtonClicked();
  void createPlaylistClicked();
  void sharePlaylistClicked();
  void playPlaylistButtonClicked();
    
private: 
  string accessToken;
  string deviceID;
  string trackID;
  string clientID;
  string clientSecret;
  string filename;
  CsvData csvData;
  SpotifyAPI spotifyApi;
  string createdPlaylist;

  QVBoxLayout* mainLayout; 
  QHBoxLayout* currentTrackLayout;

  QVBoxLayout* mergedPlaylistLayout;
  QWidget* containerWidget;
  QScrollArea* scrollArea;

  QHBoxLayout* buttonLayout;

  QString outputPath;
  QLabel* currentTrack;
  QPushButton* trackIcon;
  QTimer* updateTimer;
  
  // Main menu (playlist page):
  void setUpContextMenu();
  QListWidget* songListWidget; // For displaying the playlist
  QMenu* contextMenu; // Context menu for song options

  QPushButton* playButton;
  QPushButton* pauseButton;
  QPushButton* createPlaylist;
  QPushButton* sharePlaylist;
  QPushButton* playPlaylist;
  QSlider* volumeSlider;

protected:
  void closeEvent(QCloseEvent *event) override {
    QDir directory("externals/images");
    QStringList images = directory.entryList(QStringList() << "*.png", QDir::Files);
    foreach(const QString &image, images){
      directory.remove(image);
    }
    event->accept();
  }
};

#endif // MAINWINDOW_H

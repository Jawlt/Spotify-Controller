/**
 * @author Jwalant Patel, Ross Cameron, Lance Cheong Youne, Ojas Singh Hunjan, Matthew Morelli
 * @date 2024-03-31
 * @brief This class contains the UI buttons and windows, which utilizes the SpotifyAPI
*/
#include "mainwindow.h"

using json = nlohmann::json;

/** @brief Constructor for the MainWindow class
 * @param parent pointer to the parent widget
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), csvData("extras/responses.csv") ,spotifyApi(clientID, clientSecret) {

  outputPath = "externals/images";
  clientID = "";
  clientSecret = "";

  createdPlaylist = spotifyApi.createPlaylist(clientID, "3307B Test Playlist");
  accessToken = spotifyApi.getAccessToken();
  deviceID = spotifyApi.getDeviceID();

  QWidget* centralWidget = new QWidget(this);
  mainLayout = new QVBoxLayout(centralWidget); // layout for the song widget

  currentTrackLayout = new QHBoxLayout; // secondary layout for the curent track playing 
  trackIcon = new QPushButton(QIcon(""), "", this);
  trackIcon->setFixedSize(100, 100);
  currentTrack = new QLabel("Current Track: ", centralWidget);
  currentTrackLayout->addWidget(trackIcon);
  currentTrackLayout->addWidget(currentTrack);

  updateTimer = new QTimer(this);
  updateTimer->setInterval(3000); //3000ms (3 seconds)
  connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateCurrentTrack);
  updateTimer->start(); // start the timer

  mergedPlaylistLayout = new QVBoxLayout();

  // initialize the container widget that holds the QVBoxLayout
  containerWidget = new QWidget();
  containerWidget->setLayout(mergedPlaylistLayout);
  
  // initialize the QScrollArea which contains the container widget
  scrollArea = new QScrollArea(this);
  scrollArea->setWidget(containerWidget);
  scrollArea->setWidgetResizable(true);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
 
  // initalize the secondary layout for buttons (play, pause, create playlist, share playlist)
  buttonLayout = new QHBoxLayout; 
  playButton = new QPushButton("Play", centralWidget);
  playButton->installEventFilter(this);
  pauseButton = new QPushButton("Pause", centralWidget);
  pauseButton->installEventFilter(this);
  createPlaylist = new QPushButton("Create Playlist", centralWidget);
  createPlaylist->installEventFilter(this);
  sharePlaylist = new QPushButton("Share Playlist", centralWidget);
  sharePlaylist->installEventFilter(this);
  sharePlaylist->hide();
  playPlaylist = new QPushButton("Play Playlist", centralWidget);
  playPlaylist->installEventFilter(this);
  
  // set up volume slider
  volumeSlider = new QSlider(Qt::Horizontal, centralWidget);
  volumeSlider->setRange(0,100);
  volumeSlider->setValue(spotifyApi.getVolumePercent());

  // connect the buttons to the functions through clicks
  connect(playButton, &QPushButton::clicked, this, &MainWindow::playButtonClicked);
  connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseButtonClicked);
  connect(createPlaylist, &QPushButton::clicked, this, &MainWindow::createPlaylistClicked);
  connect(sharePlaylist, &QPushButton::clicked, this, &MainWindow::sharePlaylistClicked);
  connect(playPlaylist, &QPushButton::clicked, this, &MainWindow::playPlaylistButtonClicked);
  connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);

  // add the buttons to the button layout
  buttonLayout->addWidget(playButton);
  buttonLayout->addWidget(pauseButton);
  buttonLayout->addWidget(createPlaylist);
  buttonLayout->addWidget(sharePlaylist);
  buttonLayout->addWidget(playPlaylist);
  buttonLayout->addWidget(volumeSlider);

  // add the windows/widgets to the main layout
  mainLayout->addLayout(currentTrackLayout);
  mainLayout->addWidget(scrollArea);
  mainLayout->addLayout(buttonLayout);
  mainLayout->addWidget(volumeSlider);

  setCentralWidget(centralWidget);
}

/** @brief void function that resumes the song playback when the play button is clicked
 */ 
void MainWindow::playButtonClicked() {
  spotifyApi.resumePlayback(accessToken);
}

/** @brief void function that pauses the song playback when the play button is clicked
 */ 
void MainWindow::pauseButtonClicked() {
  spotifyApi.pauseTrackOnSpotify(accessToken);
}

/** @brief void function that plays the playlist when the play button is clicked
 */ 
void MainWindow::playPlaylistButtonClicked() {
  spotifyApi.playPlaylistOnSpotify(accessToken,"spotify:playlist:" + createdPlaylist);
}

/** @brief function calls the spotify API to play the track if the play button is clicked
 */
void MainWindow::trackPlayButtonClicked() {
  QPushButton* button = qobject_cast<QPushButton*>(sender());

  if(button){
    QString trackId = button->property("trackId").toString();   
    // Check if the track ID is not empty
    string trackID = trackId.toStdString();
    
    // Call Spotify API to play the track
    spotifyApi.playTrackOnSpotify(accessToken, trackID);
  }
}

/** @brief function displays a message box with a shareable url to the playlist when the share button is clicked
 */
void MainWindow::sharePlaylistClicked() {
  QMessageBox::information(this, "Created Playlist", QString::fromStdString("https://open.spotify.com/playlist/" + createdPlaylist));
  cout << "Created Playlist: https://open.spotify.com/playlist/" + createdPlaylist << endl;
}

/** @brief function calls to merge and create a playlist when the create button is clicked
 */
void MainWindow::createPlaylistClicked() {
  createPlaylist->hide();
  sharePlaylist->show();

  vector<string> trackURLs = csvData.getURLs();

  for(const auto& url : trackURLs){
    string playlistID = spotifyApi.extractPlaylistID(url);
    string playlistDetailsJson = spotifyApi.getPlaylistDetails(accessToken, playlistID);
    vector<string> tracks = spotifyApi.extractTrackIDS(playlistDetailsJson);

    for (const string& id : tracks) {
      // each row has its own QHBoxLayout
      QHBoxLayout *rowLayout = new QHBoxLayout();
      QPushButton *label = new QPushButton();
      QPushButton* button = new QPushButton(QIcon(""), "", this);
      connect(button, &QPushButton::clicked, this, &MainWindow::trackPlayButtonClicked);
      button->setFixedSize(100, 100);
      button->installEventFilter(this);

      // call the spotify API to get the track details (accessToken and id)
      string trackJson = spotifyApi.getTrackDetails(accessToken, id);
      auto trackDetails = json::parse(trackJson);

      string trackName = trackDetails["name"];
      string trackID = trackDetails["id"];
      string artists = "";
      for (auto& artist : trackDetails["artists"]) {
        if (!artists.empty()) artists += ", ";
        artists += artist["name"].get<string>();
      }
      // create the label and button for the current row
      label->setText(QString::fromStdString("Track: " + trackName + " - " + artists));
      button->setProperty("trackID", QString::fromStdString(trackID));
      spotifyApi.downloadTrackImg(trackJson, trackID, outputPath);
      spotifyApi.addTrackToPlaylist(accessToken, createdPlaylist, "spotify:track:" + trackID);
      button->setIcon(QIcon(QString::fromStdString("externals/images/" + trackID + ".png")));
      button->setIconSize(QSize(100,100));

      // add the label and button to the row layout
      rowLayout->addWidget(button);
      rowLayout->addWidget(label);

      // add the current row layout to the rows layout
      mergedPlaylistLayout->addLayout(rowLayout);
    }
  }
}

/** @brief function calls the spotify API to change the volume of the playback on the device connected when detected
 */
void MainWindow::onVolumeChanged(int value) {
  spotifyApi.setVolume(accessToken, value);
}

/** @brief function keeps the current track UI updated with the song currently being played on the device connected
 */
void MainWindow::updateCurrentTrack() {
  string currentlyPlayingJson = spotifyApi.getCurrentTrack(accessToken);

  auto currentlyPlaying = json::parse(currentlyPlayingJson);
  if (!currentlyPlaying.is_null() && currentlyPlaying.contains("item") && currentlyPlaying["item"].contains("name")) {
    string trackName = currentlyPlaying["item"]["name"];
    string trackID = currentlyPlaying["item"]["id"];
    string artists = "";
    for (auto& artist : currentlyPlaying["item"]["artists"]) {
      if (!artists.empty()) artists += ", ";
      artists += artist["name"].get<string>();
    }
    currentTrack->setText(QString::fromStdString("Current Track: " + trackName + " - " + artists));
    string trackJson = spotifyApi.getTrackDetails(accessToken, trackID);
    spotifyApi.downloadTrackImg(trackJson, trackID, outputPath);
    trackIcon->setIcon(QIcon(QString::fromStdString("externals/images/" + trackID + ".png")));
    trackIcon->setIconSize(QSize(100,100));
  }
}

/** @brief event filter for when buttons are pressed
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  return QObject::eventFilter(obj, event);
}

//Destructor 
MainWindow::~MainWindow() {}

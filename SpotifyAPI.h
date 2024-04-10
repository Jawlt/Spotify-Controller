/**
 * @author Jwalant Patel, Ross Cameron, Lance Cheong Youne, Ojas Singh Hunjan, Matthew Morelli
 * @date 2024-03-31
 * @brief This header file contains the variables and libraries needed for the SpotifyAPI class
*/
#ifndef SPOTIFYAPI_H
#define SPOTIFYAPI_H
//include necessary libraries
#include <string>
#include <vector>
#include "json.hpp"
#include <curl/curl.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QEventLoop>
#include <QInputDialog>
#include <QDir>

using namespace std;

class SpotifyAPI {
public:
    //initialize public funtions to be used in SpotifyAPI.h
    SpotifyAPI(const string& clientId, const string& clientSecret); 
    string getTrackDetails(const string& accessToken, const string& trackId);
    string getPlaylistDetails(const string& accessToken, const string& playlistId);
    vector<string> extractTrackIDS(string& playlistJson);
    string extractPlaylistID(const string& url);
    void downloadTrackImg(const string& trackDetailsJson, const string& trackID, const QString& outputPath);
    string getAccessToken();
    int getVolumePercent();
    string createPlaylist(const string& clientId, const string&playlistName);
    string exchangeAuthCodeForAccessCode(const string& code, const string& redirectUri);
    string getUserID();
    string getDeviceID();
    void resumePlayback(const string& accessToken);
    void playTrackOnSpotify(const string& accessToken, const string& trackID);
    void pauseTrackOnSpotify(const string& accessToken); 
    void setVolume(const string& accessToken, int volumePercent);
    string getCurrentTrack(const string& accessToken);
    void addTrackToPlaylist(const string& accessToken, const string& playlistID, const string& trackID);  
    void playPlaylistOnSpotify(const string& accessToken, const string& playlistID);

private:
    string clientId; //initialize variable to contain client ID
    string clientSecret; //initialize variable to contain client secret
    string accessToken; //initialize variable to contain access token
    int volumePercent; //initialize variable to contain the volume level

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* data); //initialize private functions to be used in
    string getSpotifyAccessToken(const string& base64); 
};

#endif // SPOTIFYAPI_H
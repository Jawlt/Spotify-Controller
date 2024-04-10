/**
 * @author Jwalant Patel, Ross Cameron, Lance Cheong Youne, Ojas Singh Hunjan, Matthew Morelli
 * @date 2024-03-31
 * @brief This class contains the method that allows the software to communicate with the Spotify API
*/

#include "SpotifyAPI.h"
#include <iostream>
#include <stdexcept>
using json = nlohmann::json;
// base64 code to be entered (from doing echo ...:... | base64)
string base64Cred = "";

/** @brief Constructor for the SpotifyAPI class
 * @param clientId is the client id obtained from the developer dashboard
 * @param clientSecret is the client secret obtained from the developer dashboard
 */
SpotifyAPI::SpotifyAPI(const string& clientId, const string& clientSecret)
    : clientId(clientId), clientSecret(clientSecret) {
    this->accessToken = getSpotifyAccessToken(base64Cred); //get the access token upon initialization
}
/** @brief Calculates the total size of the data passed in
 * @param contents is a pointer to the data that has been received
 * @param size is the size of each data element
 * @param nmemb is the number of elements
 * @param data is a pointer to a string where the received data will be stored
 * @return Total size of the data received
 */
size_t SpotifyAPI::WriteCallback(void *contents, size_t size, size_t nmemb, string *data) {
    size_t totalSize = size * nmemb;
    //static callback function to write received data to a string
    data->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}
/** @brief private method to authenticate with Spotify and get an access token
 * @param base64 code to be entered (from doing echo ...:... | base64)
 * @return accessToken to be used to gain access to spotify account
 */
string SpotifyAPI::getSpotifyAccessToken(const string& base64) {
    CURL* curl;
    CURLcode res;
    string readBuffer;
    string accessToken;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl){
        string authHeader = "Authorization: Basic " + base64;

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "grant_type=client_credentials");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl);
        if(res == CURLE_OK) {
            auto json = json::parse(readBuffer);
            accessToken = json["access_token"].get<string>();
        }
        else {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    curl_global_cleanup();
    return accessToken;
}

/** @brief Getter method for the access token
 * @return the access token used for authentication (Auth 2.)o
 */
string SpotifyAPI::getAccessToken(){
    return this->accessToken;
}
/** @brief getter method returns current volume percent on spotify instance
 * @return volumePercent
 */
int SpotifyAPI::getVolumePercent(){
    return this->volumePercent;
}

/** @brief public method to fetch track details using the Spotify Web API
 * @param accessToken used for authentication
 * @param trackId used to isolate which song's details are being searched
 * @return readBuffer variable which stores metadata on track
 */

string SpotifyAPI::getTrackDetails(const string& accessToken, const string& trackId) {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        string authHeader = "Authorization: Bearer " + accessToken;

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, authHeader.c_str());

        string url = "https://api.spotify.com/v1/tracks/" + trackId;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if(res == CURLE_OK) {
            //cout << "Track Details: " << readBuffer << endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return readBuffer;
}
/** @brief public method to fetch playlist details using the Spotify Web API
 * @param accessToken used for authentication
 * @param playlistId used to isolate which playlist's details are being searched
 * @return readBuffer variable which stores metadata on playlist
 */
string SpotifyAPI::getPlaylistDetails(const string& accessToken, const string& playlistId) {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        string authHeader = "Authorization: Bearer " + accessToken;

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, authHeader.c_str());

        string url = "https://api.spotify.com/v1/playlists/" + playlistId;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if(res == CURLE_OK) {
            //cout << "Playlist Details: " << readBuffer << endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return readBuffer;
}
/** @brief extracts playlist's ID using the playlist url
 * @param url The Spotify playlist URL from which to extract the playlist ID.
 * @return The extracted playlist ID if found, otherwise an empty string.
 */
string SpotifyAPI::extractPlaylistID(const string& url){
    string prefix = "https://open.spotify.com/playlist/";
    auto starting = url.find(prefix);

    if(starting == string::npos){
        return ""; //prefix substring was not found
    }

    starting += prefix.length(); // moves start to end of the prefix

    auto ending = url.find('?', starting);
    if(ending == string::npos){
        ending = url.length();
    }
    return url.substr(starting, ending-starting);
}

vector<string> SpotifyAPI::extractTrackIDS(string& playlistJson){
    auto playlist = json::parse(playlistJson);
    vector<string> trackIDs;
    
    if (playlist.contains("tracks") && playlist["tracks"].contains("items")) {
        for (const auto& item : playlist["tracks"]["items"]) {
            if (item.contains("track") && item["track"].contains("id") && !item["track"]["id"].is_null()) {
                std::string trackId = item["track"]["id"];
                trackIDs.push_back(trackId);
            }
        }
    }
    return trackIDs;
}
/** @brief downloads the album cover for the desired track
 * @param trackDetailsJson JSON string which contains the track details
 * @param trackId ID of track
 * @param outputPath Path where image is saved
 */
void SpotifyAPI::downloadTrackImg(const string& trackDetailsJson, const string& trackID, const QString& outputPath){

    string imageURL;
    //parse through the JSON string to get the url aray containing the image
    auto trackDetails = json::parse(trackDetailsJson);
    if(trackDetails.contains("album") && trackDetails["album"].contains("images") && !trackDetails["album"]["images"].empty()) {
        auto firstImage = trackDetails["album"]["images"][0];
        if(firstImage.contains("url")) {
            imageURL = firstImage["url"];
        }
    }

    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(QString::fromStdString(imageURL)));
    QNetworkReply* reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();//waits for download to finish

    //checks if no errors occured
    if(reply->error() == QNetworkReply::NoError) {
        //save file as (trackID)img.png
        QFile file(outputPath + "/" + QString::fromStdString(trackID) + ".png");
        if(file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.close();
            //qDebug() << "Downloaded and saved:" << filename;
        } else {
            //qWarning() << "Could not open file for writing:" << filename;
        }
    } else {
        //qWarning() << "Failed to download image:" << reply->errorString();
    }
    reply->deleteLater();
}
/** @brief method used to create playlists
 * @param playlistName string which contains the name of the created playlist
 * @param clientId string containing ID of client
 * @return id string containing id of playlist
 */
string SpotifyAPI::createPlaylist(const string& clientID, const string&playlistName){
    CURL *curl = curl_easy_init();
    string redirectUri = "http://localhost:3000";
    string encodedRedirectUri = curl_easy_escape(curl, redirectUri.c_str(), redirectUri.length());
    string scope = "playlist-modify-private%20playlist-modify-public%20user-read-playback-state%20user-modify-playback-state%20user-read-currently-playing";

    string authUrl = "https://accounts.spotify.com/authorize?client_id=" + clientID +
                     "&response_type=code&redirect_uri=" + encodedRedirectUri + "&scope=" + scope;

    bool ok;
    cout << authUrl << endl;
    
    QString code = QInputDialog::getText(nullptr, "Please visit the following webiste(printed to console) and enter the code to authorize playlist creation." ,
                    "Code:", QLineEdit::Normal, QDir::home().dirName(), &ok);

    //qDebug() << code;
    if (ok && !code.isEmpty()) {
        string exchangedCode = exchangeAuthCodeForAccessCode(code.toStdString(), encodedRedirectUri);
    }

    string id;
    if (curl) {
        //cout << "createplaylist: " + accessToken << endl;
        string url = "https://api.spotify.com/v1/users/" + getUserID() + "/playlists";
        string post = "{\"name\":\"" + playlistName + "\", \"public\":false}";
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());

        string readBuffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        CURLcode res = curl_easy_perform(curl);
        if(res == CURLE_OK) {
            //cout << "Playlist Created: " << readBuffer << endl;
            auto json = json::parse(readBuffer);
            id = json["id"].get<string>();
        }
        else{
        //cout << "Failed to create playlist" << curl_easy_strerror(res) << endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return id;
}  
/** @brief method used to generate access code using Authorization code
 * @param code string containing Authorization code
 * @param redirectUri string containing redirect URI
 * @return readBuffer string containing access code
 */
string SpotifyAPI::exchangeAuthCodeForAccessCode(const string& code, const string& redirectUri){
    CURL* curl = curl_easy_init();
    string readBuffer;
    string authHeader = "Authorization: Basic " + base64Cred;
    string post = "grant_type=authorization_code&code=" + code + "&redirect_uri=" + redirectUri;

    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if(res == CURLE_OK) {
            auto json = json::parse(readBuffer);
            this->accessToken = json["access_token"].get<string>();
        }
        else {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return readBuffer; //contains the access token in JSON
}
/** @brief getter method used to return the user ID
 * @return userID string containing the user ID
 */
string SpotifyAPI::getUserID(){
    CURL* curl = curl_easy_init();
    string readBuffer;
    string userID;
    if (curl) {
    struct curl_slist* headers = nullptr;
    string authHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.spotify.com/v1/me");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);  // WriteCallback function as shown earlier
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
    }

    auto jsonResponse = json::parse(readBuffer);
    userID = jsonResponse["id"];
    //cout << "UserID: " << userID << endl;
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    }
    return userID;
}
/** @brief getter method used to return the Device ID
 * @return id string containing the device ID
 */
string SpotifyAPI::getDeviceID(){
    CURL* curl = curl_easy_init();
    string readBuffer;
    string id;
    if (curl) {
    struct curl_slist* headers = nullptr;
    string authHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.spotify.com/v1/me/player/devices");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);  // WriteCallback function as shown earlier
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
    }
    else {
        //cout << "Available Devices: " << readBuffer << endl;
    }

    auto deviceIdJson = json::parse(readBuffer);
    for(const auto& device : deviceIdJson["devices"]){
        id = device["id"].get<string>();
        this->volumePercent = device["volume_percent"].get<int>();;
    }
    
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    }
    //cout << "Id: " << id << endl;
    return id;
}
/** @brief getter method used to return the track which is currently playing
 * @param accessToken string containg access token
 * @return readBuffer string containing the track which is currently playing on spotify
 */
string SpotifyAPI::getCurrentTrack(const string& accessToken){
    CURL* curl = curl_easy_init();
    string readBuffer;
    if (curl) {
    struct curl_slist* headers = nullptr;
    string authHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.spotify.com/v1/me/player/currently-playing");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);  // WriteCallback function as shown earlier
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    }
    return readBuffer;
}
/** @brief method used to add a track to the desired playlist
 * @param accessToken string containg access token
 * @param playlistID string containing the id of the desired playlist
 * @param trackID string containing the id of the desired track to add to playlist
 */
void SpotifyAPI::addTrackToPlaylist(const string& accessToken, const string& playlistID, const string& trackID) {
    CURL* curl = curl_easy_init();
    string readBuffer; // To store the response

    if (curl) {
        string url = "https://api.spotify.com/v1/playlists/" + playlistID + "/tracks";
        string data = "{\"uris\": [\"" + trackID + "\"]}";

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
        }
        else {
            cout << "Successfully added track to playlist." << endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}
/** @brief method used to play a paused track
 * @param accessToken string containg access token
 */
void SpotifyAPI::resumePlayback(const string& accessToken) {
    CURL* curl = curl_easy_init();
    string readBuffer; // To store the response

    if (curl) {
        // Set the endpoint URL, including the device ID if provided
        string url = "https://api.spotify.com/v1/me/player/play";
  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // Specify the PUT request method
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        // Set the callback function to write the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        // Set the data structure to write the response to
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Prepare headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
        } else {
            cout << "Playback resumed successfully.\n";
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}
/** @brief method used to play a scpeific track on spotify
 * @param accessToken string containg access token
 * @param trackID string containing the id of the desired track to play
 */
void SpotifyAPI::playTrackOnSpotify(const string& accessToken, const string& trackID) {
    CURL* curl = curl_easy_init();
    string readBuffer; // To store the response

    if(curl) {
        // Set the endpoint URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.spotify.com/v1/me/player/play");
        // Specify the PUT request method
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        // Prepare the JSON body
        json bodyData = json::object({{"uris", json::array({"spotify:track:" + trackID})}});
        string jsonData = bodyData.dump();

        // Set the POST fields
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

        // Set the callback function to write the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        // Set the data structure to write the response to
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Prepare headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        else {
            cout << "Playback started successfully." << endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}
/** @brief method used to play desired playlist on spotify
 * @param accessToken string containg access token
 * @param playlistID string containing the id of the desired playlist
 */
void SpotifyAPI::playPlaylistOnSpotify(const string& accessToken, const string& playlistID) {
    CURL* curl = curl_easy_init();
    string readBuffer; // To store the response
    string url = "https://api.spotify.com/v1/me/player/play";
    if(curl) {
        string data = "{\"context_uri\":\"" + playlistID + "\"}"; // Set the playlist to play

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); // Use PUT method
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

        // Prepare headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        else {
            cout << "Playback started successfully." << endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}
/** @brief method used to pause the track that is currently playing
 * @param accessToken string containg access token
 */
void SpotifyAPI::pauseTrackOnSpotify(const string& accessToken) {
    CURL* curl = curl_easy_init();
    string readBuffer; // To store the response

    if(curl) {
        // Set the endpoint URL for pausing playback
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.spotify.com/v1/me/player/pause");
        // Specify the PUT request method
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        // Set the callback function to write the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        // Set the data structure to write the response to
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Prepare headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json"); // Content-Type may be set for consistency
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        else {
            cout << "Playback paused successfully." << endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}
/** @brief method used to set the volume on spotify
 * @param accessToken string containg access token
 * @param volumePercent integer containg the desired level of volume
 */
void SpotifyAPI::setVolume(const string& accessToken, int volumePercent) {
    CURL* curl = curl_easy_init();
    string readBuffer; // To store the response

    if(curl) {
        // Prepare the URL with the volume_percent query parameter
        string url = "https://api.spotify.com/v1/me/player/volume?volume_percent=" + to_string(volumePercent);

        // Set the endpoint URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // Specify the PUT request method
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        // Set the callback function to write the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        // Set the data structure to write the response to
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Prepare headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        else {
            cout << "Volume set successfully." << endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}
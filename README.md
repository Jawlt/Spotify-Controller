## 3307B Project (Application)

## Steps to compile application
qmake app.pro
make
./Application

## Steps to clone bitbucket from WSL
git clone url

## Set user settins in WSL
git config --global user.email "you@example.com"
git config --global user.name "Your Name"

## Steps to Pull Code
https://support.atlassian.com/bitbucket-cloud/docs/pull-code-from-bitbucket/
cd <path_to_local_repo>
git pull

## Steps to Push Code
https://support.atlassian.com/bitbucket-cloud/docs/push-code-to-bitbucket/
cd <path_to_local_repo>
git add --all
git commit -m '<commit message>'
git push

## Requirements to Run Program
External libraries: lcurl (run apt-get install libcurl4-openssl-dev), -lcpprest, -lssl, -lcrypto
Spotify Premium Subscription

## Steps to run program:
--> if you have any questions about the instructions, please reach out via email to rcamer8@uwo.ca
1. Go to https://developer.spotify.com/dashboard and sign into spotify premium account
2. Click on "Create app", enter any name for "App name", enter "http://localhost:3000" for Redirect Url
3. Select "Web Api" and "Web Playback SDK" (if not selected by default)
4. Once the app is created, open the app settings
5. Copy down the Client ID and client secret (You may have to click on View client secret)
6. Go to mainwindow.cpp and paste the Client ID and secret on line 9 and 10
7. Open the terminal and enter: echo -n "*Client ID*:*client secret*" | base64  (Replace *Client ID* and *client secret* with values copied earlier)
8. Copy down the entire base64 code (ends with :)
9. Go to SpotifyAPI.cpp and paste the base64 code on line 6
10. Open an instance of Spotify (open on browser or app)
11. Run ./Application in terminal
12. Copy any text after "code=" (...code=*text to copy*) in web address of site
13. Paste copied text into pop up window and press continue
14. Main menu will pop up

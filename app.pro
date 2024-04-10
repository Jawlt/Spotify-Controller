QT       += core widgets network
TARGET = Application
TEMPLATE = app 
SOURCES += main.cpp mainwindow.cpp csvdata.cpp SpotifyAPI.cpp 
HEADERS += mainwindow.h csvdata.h SpotifyAPI.h 
RESOURCES += resources.qrc
INCLUDEPATH += $$PWD/externals/nlohmann_json
# Adding the SSL and Crypto libraries
LIBS += -lcurl -L/usr/include/cpprest/to/cpprest/lib -lcpprest -lssl -lcrypto
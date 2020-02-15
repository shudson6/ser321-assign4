#include "MusicClient.h"
#include <stdexcept>
#include <iostream>
using namespace std;

MusicClient::MusicClient(const char* _author, const char* _lastfmkey) : MusicClient(_author, _lastfmkey, "music.json") {}


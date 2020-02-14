#include "Track.h"
#include <stdexcept>
using namespace std;

Track::Track(const string& _title, const string& _artist, const string& _album, int _rank, int _len)
	: Track(_title.c_str(), _artist.c_str(), _album.c_str(), _rank, _len) {
}

Track::Track(const char* _title, const char* _artist, const char* _album, int _rank, int _len)
	: MusicDescription(_title, _artist, _len) {
	if (_rank < 1) {
		throw invalid_argument("Track: rank must be a positive integer.");
	}
	rank = _rank;
	album = (_album ? _album : UNKNOWN_ALBUM);
}

const char* Track::getAlbum() const {
	return album.c_str();
}

int Track::getRank() const {
	return rank;
}

bool Track::operator<(Track& that) const {
	return this->rank < that.rank;
}

Json::Value Track::toJson() const {
	Json::Value json(MusicDescription::toJson());
	json["album"] = album;
	json["rank"] = rank;
	return json;
}

Track Track::fromJson(const Json::Value& json) {
	string t = json["title"].asString();
	string a = json["artist"].asString();
	string r = json["album"].asString();
	int o = json["rank"].asInt();
	int s = json["time"].asInt();
	return Track(t, a, r, o, s);
}

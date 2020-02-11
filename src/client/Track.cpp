#include "Track.h"
#include <stdexcept>

Track::Track(const string& _title, const string& _artist, const string& _album, int _rank, int _len)
		: MusicDescription(_title, _artist, _len), album (_album), rank (_rank) {

	if (rank < 1) {
		throw std::invalid_argument("Track: rank must be a positive integer.");
	}
}

string Track::getAlbum() const {
	return album;
}

int Track::getRank() const {
	return rank;
}

bool Track::operator<(Track& that) const {
	return this->rank < that.rank;
}

Json::Value Track::toJSON() const {
	Json::Value json;
	json["title"] = getTitle();
	json["artist"] = getArtist();
	json["album"] = getAlbum();
	json["rank"] = getRank();
	json["time"] = getLength();
	return json;
}

Track Track::fromJSON(Json::Value json) {
	string t = json["title"].asString();
	string a = json["artist"].asString();
	string r = json["album"].asString();
	int o = json["rank"].asInt();
	int s = json["time"].asInt();
	return Track(t, a, r, o, s);
}

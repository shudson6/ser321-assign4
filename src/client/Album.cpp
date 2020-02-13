#include "Album.h"
#include <algorithm>

Album::Album(const string& _title, const string& _artist, std::vector<Track>& _tracks, std::vector<string>& _genres,
		string& _imgURL, string& _summary) : MusicDescription(_title, _artist, calcLen(_tracks)) {
	tracks = _tracks;
	genres = _genres;
	imgURL = _imgURL;
	summary = _summary;
	std::sort(tracks.begin(), tracks.end());
}

std::vector<Track>Album::getTracks() const {
	return tracks;
}

const Track* Album::getTrack(string name) const {
	for (std::vector<Track>::const_iterator iter = tracks.cbegin(); iter != tracks.cend(); iter++) {
		if (iter->getTitle() == name) {
			return &*iter;
		}
	}
	return NULL;
}

std::vector<string> Album::getGenres() const {
	return genres;
}

string Album::getImgURL() const {
	return imgURL;
}

string Album::getSummary() const {
	return summary;
}

int Album::calcLen(std::vector<Track>& tracks) {
	int rt = 0;
	for (std::vector<Track>::iterator iter = tracks.begin(); iter != tracks.cend(); iter++) {
		rt += iter->getLength();
	}
	return rt;
}

int Album::size() const {
	return tracks.size();
}

Json::Value Album::toJSON() const {
	Json::Value json;
	json["artist"] = getArtist();
	json["title"] = getTitle();
	json["image"] = getImgURL();
	json["summary"] = getSummary();
	json["tracks"] = trackListJSON();
	json["genre"] = genresJSON();
	return json;
}

Json::Value Album::trackListJSON() const {
	Json::Value arr(Json::arrayValue);
	for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
		arr.append(iter->toJSON());
	}
	return arr;
}

Json::Value Album::genresJSON() const {
	Json::Value arr(Json::arrayValue);
	for (auto iter = genres.cbegin(); iter != genres.cend(); ++iter) {
		Json::Value str(*iter);
		arr.append(str);
	}
	return arr;
}

Album Album::fromJSON(Json::Value json) {
	string t = json["title"].asString();
	string a = json["artist"].asString();
	string u = json["image"].asString();
	string s = json["summary"].asString();
	auto tv = parseTracks(json["tracks"]);
	auto gv = parseGenres(json["genre"]);
	return Album(t, a, tv, gv, u, s);
}

std::vector<Track> Album::parseTracks(Json::Value json) {
	std::vector<Track> tv;
	for (auto iter = json.begin(); iter != json.end(); ++iter) {
		Track t = Track::fromJSON(*iter);
		tv.push_back(t);
	}
	return tv;
}

std::vector<string> Album::parseGenres(Json::Value json) {
	std::vector<string> gv;
	for (auto iter = json.begin(); iter != json.end(); ++iter) {
		gv.push_back(iter->asString());
	}
	return gv;
}

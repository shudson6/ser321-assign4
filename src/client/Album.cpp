#include "Album.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Album::Album(const std::string& _title, const std::string& _artist, const string& _imgUrl, const string& _summary)
	: Album(_title.c_str(), _artist.c_str(), _imgUrl.c_str(), _summary.c_str()) {
}

Album::Album(const char* _title, const char* _artist, const char* _imgUrl, const char* _summary)
	: MusicDescription(_title, _artist, 0) {
	imgUrl = (_imgUrl ? _imgUrl : NO_IMAGE);
	summary = (_summary ? _summary : NO_SUMMARY);
}

const char* Album::getImgUrl() const {
	return imgUrl.c_str();
}

const char* Album::getSummary() const {
	return summary.c_str();
}

const Track* Album::getTrack(int rank) const {
	for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
		if (iter->getRank() == rank) {
			return &*iter;
		}
	}
	return NULL;
}

const vector<string>& Album::getGenres() const {
	return genres;
}

const vector<Track>& Album::getTracks() const {
	return tracks;
}

int Album::size() const {
	return tracks.size();
}

bool Album::addGenre(const char* toAdd) {
	try {
		if (toAdd) {
			genres.push_back(string(toAdd));
			return true;
		}
	} catch (...) {
		// catch-all just in case of an error on reallocation
		cout << "Failed to add genre for unknown reason." << endl;
	}
	return false;
}

bool Album::removeGenre(const char* toRm) {
	for (auto iter = genres.cbegin(); iter != genres.cend(); ++iter) {
		if (*iter == toRm) {
			genres.erase(iter);
			return true;
		}
	}
	return false;
}

const Track* Album::addTrack(const Track& toAdd) {
	try {
		// insert the element in order
		for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
			if (toAdd.getRank() == iter->getRank()) {
				return &toAdd;
			} else if (toAdd.getRank() < iter->getRank()) {
				return &*tracks.insert(iter, toAdd);
			}
		}
		// if the loop terminates, toAdd belongs at the end
		tracks.push_back(toAdd);
		return &*tracks.crbegin();
	} catch (...) {
		cout << "Failed to add track for unknown reason." << endl;
		return NULL;
	}
}

bool Album::removeTrack(int rank) {
	for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
		if (iter->getRank() == rank) {
			tracks.erase(iter);
			return true;
		}
	}
	return false;
}

// toJson will require some helper functions that i don't feel like putting in the header
Json::Value json_tracklist(vector<Track>&);
Json::Value json_genres(vector<string>&);

Json::Value Album::toJson() {
	// doing it this way will include the length, which the Java version doesn't do,
	// but the extra member won't cause any compatibility issue
	Json::Value json(MusicDescription::toJson());
	json["image"] = imgUrl;
	json["summary"] = summary;
	json["genre"] = json_genres(genres);
	json["tracks"] = json_tracklist(tracks);
	return json;
}

Json::Value json_tracklist(vector<Track>& tracks) {
	Json::Value json(Json::arrayValue);
	for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
		json.append(iter->toJson());
	}
	return json;
}

Json::Value json_genres(vector<string>& genres) {
	Json::Value json(Json::arrayValue);
	for (auto iter = genres.cbegin(); iter != genres.cend(); ++iter) {
		json.append(*iter);
	}
	return json;
}

/**
 * Copyright (c) 2020 Steven Hudson
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Steven Hudson shudson6
 * @version 2020.2
 **/
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

// not the most efficient way, but whatevs
void Album::recalcLen() {
	len = 0;
	for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
		len += iter->getLength();
	}
	lenstr = "";
}

const Track* Album::addTrack(const Track& toAdd) {
	const Track* result;
	try {
		// insert the element in order
		for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
			if (toAdd.getRank() == iter->getRank()) {
				result = &toAdd;
			} else if (toAdd.getRank() < iter->getRank()) {
				result = &*tracks.insert(iter, toAdd);
			}
		}
		// if the loop terminates, toAdd belongs at the end
		tracks.push_back(toAdd);
		result = &*tracks.crbegin();
	} catch (...) {
		cout << "Failed to add track for unknown reason." << endl;
		result = NULL;
	}
	if (result && result != &toAdd) {
		recalcLen();
	}
	return result;
}

bool Album::removeTrack(int rank) {
	for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
		if (iter->getRank() == rank) {
			tracks.erase(iter);
			recalcLen();
			return true;
		}
	}
	return false;
}

const char* Album::timeStr() const {
	if (lenstr.empty()) {
		lenstr = TimeStr(len);
	}
	return lenstr.c_str();
}

// toJson will require some helper functions that i don't feel like putting in the header
Json::Value json_tracklist(const vector<Track>&);
Json::Value json_genres(const vector<string>&);

Json::Value Album::toJson() const {
	// doing it this way will include the length, which the Java version doesn't do,
	// but the extra member won't cause any compatibility issue
	Json::Value json(MusicDescription::toJson());
	json["image"] = imgUrl;
	json["summary"] = summary;
	json["genre"] = json_genres(genres);
	json["tracks"] = json_tracklist(tracks);
	return json;
}

Json::Value json_tracklist(const vector<Track>& tracks) {
	Json::Value json(Json::arrayValue);
	for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
		json.append(iter->toJson());
	}
	return json;
}

Json::Value json_genres(const vector<string>& genres) {
	Json::Value json(Json::arrayValue);
	for (auto iter = genres.cbegin(); iter != genres.cend(); ++iter) {
		json.append(*iter);
	}
	return json;
}

// fromJson will require some helper functions that i don't feel like putting in the header
void parseGenres(const Json::Value&, Album&);
void parseTracks(const Json::Value&, Album&);

Album Album::fromJson(const Json::Value& json) {
	string t, a, u, s;
	t = json["title"].asString();
	a = json["artist"].asString();
	u = json["image"].asString();
	s = json["summary"].asString();
	Album alb(t, a, u, s);
	parseTracks(json["tracks"], alb);
	parseGenres(json["genre"], alb);
	return alb;
}

void parseTracks(const Json::Value& json, Album& alb) {
	for (auto iter = json.begin(); iter != json.end(); ++iter) {
		alb.addTrack(Track::fromJson(*iter));
	}
}

void parseGenres(const Json::Value& json, Album& alb) {
	for (auto iter = json.begin(); iter != json.end(); ++iter) {
		alb.addGenre(iter->asCString());
	}
}

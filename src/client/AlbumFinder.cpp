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
#include "AlbumFinder.h" 
#include <iostream>
#include <sstream>
#include <cstring>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <json/json.h>
using namespace std;

const char* const AlbumFinder::URLFMT = "http://ws.audioscrobbler.com/2.0/?method=album.getinfo&artist=%s&album=%s&format=json&api_key=%s";

AlbumFinder::AlbumFinder(const char* const _lastfmkey) {
	lastfmkey = (_lastfmkey ? _lastfmkey : "");
	cout << "Initialized AlbumFinder with LastFM API key " << lastfmkey << endl;
}

bool AlbumFinder::query(const char* const name, const char* const artist, Album& album) const {
	bool result = false;
	const char* const url = buildUrl(name, artist);
	cout << "AlbumFinder: Executing query: " << url << endl;
	try {
		string str(executeQuery(url));
		Json::Value json = jsonObj(str);
		parseJson(json, album);
		result = true;
	} catch (curlpp::LogicError& ex) {
		errorMessage("curlpp::LogicError", ex);
	} catch (curlpp::RuntimeError& ex) {
		errorMessage("curlpp::RuntimeError", ex);
	} catch (AlbumFinderException& ex) {
		errorMessage("AlbumFinderException", ex);
	}
	delete [] url;
	return result;
}

void AlbumFinder::errorMessage(const char* const type, const exception& ex) const {
	cerr << "AlbumFinder: Query failed due to " << type << endl;
	cerr << "\tmessage: " << ex.what() << endl;
}

Track AlbumFinder::parseTrack(Json::Value& json, const char* const album) const {
	int rank = stoi(json["@attr"]["rank"].asString());
	int dur = stoi(json["duration"].asString());
	Track t(json["name"].asString(), json["artist"]["name"].asString(), album, rank, dur);
	return t;
}

void AlbumFinder::parseTracks(Json::Value& json, Album& album) const {
	Json::Value tracks = json["track"];
	for (auto iter = tracks.begin(); iter != tracks.end(); ++iter) {
		Track t = parseTrack(*iter, album.getTitle());
		album.addTrack(t);
	}
}

void AlbumFinder::parseGenres(Json::Value& tags, Album& album) const {
	Json::Value genreArr = tags["tag"];
	for (auto iter = genreArr.begin(); iter != genreArr.end(); ++iter) {
		album.addGenre((*iter)["name"].asCString());
	}
}

void AlbumFinder::parseJson(Json::Value& json, Album& album) const {
	string title = json["name"].asString();
	string artist = json["artist"].asString();
	string img = parseImg(json["image"]);
	string summ = json["wiki"]["summary"].asString();
	album = Album(title, artist, img, summ);
	parseTracks(json["tracks"], album);
	parseGenres(json["tags"], album);
}

Json::Value AlbumFinder::jsonObj(string& str) const {
	Json::Reader reader;
	Json::Value json;
	if (reader.parse(str, json, false)) {
		return json["album"];
	} else {
		throw AlbumFinderException("Failed to parse JSON input.");
	}
}

string AlbumFinder::executeQuery(const char* const url) const {
	ostringstream os;
	curlpp::Easy request;
	request.setOpt(new curlpp::options::WriteStream(&os));
	request.setOpt(new curlpp::options::Url(url));
	request.perform();
	return os.str();
}

const char* const AlbumFinder::buildUrl(const char* const name, const char* const artist) const {
	// buffer to hold URL format string, name, artist, lastfmkey, -6 for format specifiers, +1 for null-terminator
	int size = strlen(URLFMT) + strlen(name) + strlen(artist) + lastfmkey.size() - 5;
	char* buf = new char[size];
	snprintf(buf, size, URLFMT, artist, name, lastfmkey.c_str());
	return buf;
}

string AlbumFinder::parseImg(Json::Value& imgArr) const {
	// imgArr holds an array of size-url pairs
	for (Json::Value::const_iterator iter = imgArr.begin(); iter != imgArr.end(); ++iter) {
		if ((*iter)["size"] == "extralarge") {
			return (*iter)["#text"].asString();
		}
	}
	return "null";
}


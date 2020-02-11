#include "AlbumFinder.h" 
#include <iostream>
#include <sstream>
#include <cstring>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <json/json.h>

const char* const AlbumFinder::URLFMT = "http://ws.audioscrobbler.com/2.0/?method=album.getinfo&artist=%s&album=%s&format=json&api_key=%s";

AlbumFinder::AlbumFinder(string _lastfmKey) {
	lastfmKey = _lastfmKey;
}

Album AlbumFinder::query(string& title, string& artist) {
	// build search string
	string urlStr = buildUrlStr(title, artist);
	std::cout << "Executing query: " << urlStr << std::endl;
	// now do it
	string json(executeQuery(urlStr));
	Album al(parse(json));
	std::cout << "Retrieved " << al.getTitle() << " by " << al.getArtist() << " (" << al.size() << " tracks, "
			<< al.getLengthStr() << ")" << std::endl;
	return al;
}

string AlbumFinder::buildUrlStr(string& title, string& artist) {
	// get a large enough buffer for the url (could -6 for the %s but nah)
	int len = strlen(URLFMT) + title.length() + artist.length() + lastfmKey.length();
	char* const buf = new char[len];
	// create url string
	snprintf(buf, len, URLFMT, artist.c_str(), title.c_str(), lastfmKey.c_str());
	string url(buf);
	// destroy the buffer and return the string
	delete [] buf;
	return url;
}

string AlbumFinder::executeQuery(string& url) {
	string result;
	try {
		std::ostringstream os;
		curlpp::Easy request;
		request.setOpt(new curlpp::options::WriteStream(&os));
		request.setOpt(new curlpp::options::Url(url.c_str()));
		request.perform();
		result = os.str();
	} catch (curlpp::LogicError& er) {
		std::cerr << er.what() << std::endl;
	} catch (curlpp::RuntimeError& er) {
		std::cerr << er.what() << std::endl;
	}
	return result;
}

Album AlbumFinder::parse(string& jsonStr) {
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(jsonStr, root, false)) {
		return parseAlbum(root["album"]);
	}
	throw AlbumFinderException("Failed to parse JSON input.");
}

Album AlbumFinder::parseAlbum(Json::Value json) {
	string title = json["name"].asString();
	string artist = json["artist"].asString();
	std::vector<Track> tracks(parseTracks(json["tracks"], title));
	std::vector<string> genres(parseGenres(json["tags"]));
	string img = parseImg(json["image"]);
	string summ = json["wiki"]["summary"].asString();
	Album result(title, artist, tracks, genres, img, summ);
	return result;
}

std::vector<Track> AlbumFinder::parseTracks(Json::Value json, string album) {
	std::vector<Track> tracks;
	Json::Value trackArr = json["track"];
	// trackArr holds a JSON array of tracks; parse each one
	for (Json::Value::const_iterator iter = trackArr.begin(); iter != trackArr.end(); ++iter) {
		Track t = parseTrack(*iter, album);
		tracks.push_back(t);
	}
	return tracks;
}

Track AlbumFinder::parseTrack(Json::Value json, string album) {
	int rank = std::stoi(json["@attr"]["rank"].asString());
	int dur = std::stoi(json["duration"].asString());
	Track t(json["name"].asString(), json["artist"]["name"].asString(), album, rank, dur);
	return t;
}

std::vector<string> AlbumFinder::parseGenres(Json::Value tags) {
	std::vector<string> genres;
	Json::Value genreArr = tags["tag"];
	for (Json::Value::const_iterator iter = genreArr.begin(); iter != genreArr.end(); ++iter) {
		genres.push_back((*iter)["name"].asString());
	}
	return genres;
}

string AlbumFinder::parseImg(Json::Value imgArr) {
	// imgArr holds an array of size-url pairs
	for (Json::Value::const_iterator iter = imgArr.begin(); iter != imgArr.end(); ++iter) {
		if ((*iter)["size"] == "extralarge") {
			return (*iter)["#text"].asString();
		}
	}
	return "null";
}

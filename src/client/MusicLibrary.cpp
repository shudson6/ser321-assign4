#include "MusicLibrary.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

MusicLibrary::MusicLibrary() : MusicDescription("Library", "Steven.Hudson", 0) {}

bool MusicLibrary::addAlbum(const Album& toAdd) {
	return albums.insert(pair<string, Album>(toAdd.getTitle(), toAdd)).second;
}

bool MusicLibrary::addTrack(const Track& toAdd, const char* const alb) {
	bool rslt = false;
	Album* album = const_cast<Album*>(getAlbum(alb));
	if (album) {
		rslt = album->addTrack(toAdd);
	}
	return rslt;
}

bool MusicLibrary::removeAlbum(const char* name) {
	return albums.erase(name) > 0;
}

const Album* MusicLibrary::getAlbum(const char* name) const {
	const Album* alb = NULL;
	try {
		alb = &albums.at(string(name));
	} catch (out_of_range& ex) {
		// nothing to do here
	}
	return alb;
}

const string* MusicLibrary::getAlbumNames() const {
	string* names = new string[albums.size()];
	auto iter = albums.begin();
	int i = 0;
	for (; iter != albums.end(); ++i, ++iter) {
		names[i] = iter->second.getTitle();
	}
	return names;
}

int MusicLibrary::size() const {
	return albums.size();
}

int MusicLibrary::getLength() const {
	int length = 0;
	for (auto iter = albums.cbegin(); iter != albums.cend(); ++iter) {
		length += iter->second.getLength();
	}
	return length;
}

const char* MusicLibrary::timeStr() {
	lenstr = TimeStr(getLength());
	return lenstr.c_str();
}

Json::Value MusicLibrary::toJson() const {
	Json::Value json;
	for (auto iter = albums.cbegin(); iter != albums.cend(); ++iter) {
		json[iter->first] = iter->second.toJson();
	}
	Json::Value libjson;
	libjson["library"] = json;
	return libjson;
}

bool MusicLibrary::save(const char* filename) const {
	try {
		if (filename) {
			ofstream out(filename);
			out << toJson().toStyledString();
			out.flush();
			out.close();
			return true;
		}
	} catch (...) {
		cout << "Save failed for unknown reason." << endl;
	}
	return false;
}

// load will need some stuff i don't feel like putting in the header
Json::Value jsonLibrary(string);
string loadFrom(const char*);
#define BUF_SIZE 4096

void MusicLibrary::load(const char* filename) {
	try {
		Json::Value json = jsonLibrary(loadFrom(filename));
		parseJson(json);
		cout << "Successfully loaded library." << endl;
	} catch (logic_error& ex) {
		cout << "Failed to load library: " << ex.what() << endl;
	}
}

void MusicLibrary::parseJson(Json::Value& json) {
	albums.clear();
	Json::Value::Members names = json.getMemberNames();
	for (auto iter = names.begin(); iter != names.end(); ++iter) {
		addAlbum(Album::fromJson(json[*iter]));
	}
}

Json::Value jsonLibrary(string str) {
	Json::Reader reader;
	Json::Value json;
	if (reader.parse(str, json, false)) {
		return json["library"];
	} else {
		throw logic_error("Json::Reader could not parse input.");
	}
}

string loadFrom(const char* fileName) {
	ifstream in(fileName, ifstream::in);
	string str;
	char* buf;

	if (in.good()) {
		cout << "Loading from " << fileName << "...";
		cout.flush();
	} else {
		throw logic_error("Failed to load library from file.");
	}

 	buf = new char[BUF_SIZE];
	do {
		if (in.read(buf, BUF_SIZE).gcount() > 0) {
			str.append(buf, in.gcount());
		}
	} while (!in.eof() && !in.bad());
	delete [] buf;
	cout << " done." << endl;
	return str;
}

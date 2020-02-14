#include "MusicLibrary.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

MusicLibrary::MusicLibrary() {}

bool MusicLibrary::addAlbum(Album& toAdd) {
	// why  not just
	// return library.insert(pair<string, Album>(toAdd.getTitle(), toAdd)) == &toAdd;
	int b4 = library.size();
	if (library.find(toAdd.getTitle()) == library.end()) {
		library.insert(std::pair<string, Album>(toAdd.getTitle(), toAdd));
	}
	return library.size() != b4;
}

const Album* MusicLibrary::getAlbum(string name) const {
	for (auto iter = library.begin(); iter != library.end(); ++iter) {
		if (iter->first == name) {
			return &(iter->second);
		}
	}
	return NULL;
}

string* MusicLibrary::getAlbumNames() const {
	string* names = new string[library.size()];
	auto iter = library.begin();
	int i = 0;
	for (; iter != library.end(); ++i, ++iter) {
		names[i] = iter->second.getTitle();
	}
	return names;
}

bool MusicLibrary::removeAlbum(Album& toRm) {
	return removeAlbum(toRm.getTitle());
}

bool MusicLibrary::removeAlbum(string name) {
	return library.erase(name) > 0;
}

int MusicLibrary::size() const {
	return library.size();
}

Json::Value MusicLibrary::toJSON() {
	Json::Value json;
	for (auto iter = library.cbegin(); iter != library.cend(); ++iter) {
		json[iter->first] = iter->second.toJSON();
	}
	Json::Value libjson;
	libjson["library"] = json;
	return libjson;
}


bool MusicLibrary::save(string& fileName) {
	std::ofstream out(fileName);
	out << toJSON().toStyledString();
	out.flush();
	out.close();
	return true;
}

#define BUF_SIZE 4096
void MusicLibrary::load(string& fileName) {
	try {
		Json::Value json = jsonLibrary(loadFrom(fileName));
		std::cout << "Successfully loaded library." << std::endl;
		std::map<string, Album> lib = parseJSON(json);
		library = lib;
	} catch (std::logic_error& ex) {
		std::cout << "Failed to load library: " << ex.what() << std::endl;
	}
}

void MusicLibrary::load(const char* filename) {
	string foo(filename);
	load(foo);
}

string MusicLibrary::loadFrom(string& fileName) {
	std::ifstream in(fileName, std::ifstream::in);
	string str;
	char* buf;

	if (in.good()) {
		std::cout << "Loading from " << fileName << "...";
		std::cout.flush();
	} else {
		throw std::logic_error("Failed to open " + fileName);
	}

 	buf = new char[BUF_SIZE];
	do {
		if (in.read(buf, BUF_SIZE).gcount() > 0) {
			str.append(buf, in.gcount());
		}
	} while (!in.eof() && !in.bad());
	delete [] buf;
	std::cout << " done." << std::endl;
	return str;
}

Json::Value MusicLibrary::jsonLibrary(string str) {
	Json::Reader reader;
	Json::Value json;
	if (reader.parse(str, json, false)) {
		return json["library"];
	} else {
		throw std::logic_error("Json::Reader could not parse input.");
	}
}

std::map<string, Album> MusicLibrary::parseJSON(Json::Value json) {
	std::map<string, Album> lib;
	Json::Value::Members names = json.getMemberNames();
	for (auto iter = names.begin(); iter != names.end(); ++iter) {
		lib.insert(std::pair<string, Album>(*iter, Album::fromJSON(json[*iter])));
	}
	return lib;
}

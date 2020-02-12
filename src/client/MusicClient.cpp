#include "MusicClient.h"
#include <stdexcept>
#include <iostream>

MusicClient::MusicClient(const char* _author, const char* _lastfmkey) : MusicClient(_author, _lastfmkey, "music.json") {}

MusicClient::MusicClient(const char* _author, const char* _lastfmkey, const char* _filename)
		: MediaClientGui(_author), filename(_filename) {
	if (filename.empty()) {
		throw std::invalid_argument("Filename supplied to MusicClient must not be empty.");
	}
	library = new MusicLibrary();
	library->load(filename);
	finder = new AlbumFinder(_lastfmkey);

	callback(XBtnCallback, (void*) this);
	searchButt->callback(SearchBtnCallback, (void*) this);
	menubar->callback(MenuCallback, (void*) this);
}

MusicClient::~MusicClient() {
	delete [] library;
	delete [] finder;
}

void MusicClient::save() {
	if (library->save(filename)) {
		cout << "Save successful." << endl;
	} else {
		cout << "Save to " << filename << " failed." << endl;
	}
}

void MusicClient::findAlbum() {
	string art(artSrchInput->value());
	string alb(albSrchInput->value());

	std::cout << "Go, go, gadget Search!" << std::endl;
	try {
		finder->query(alb, art);
	} catch (AlbumFinderException ex) {
		std::cout << "Search failed." << std::endl;
	}
}

void MusicClient::menuClicked() {
	char pick[80];
	menubar->item_pathname(pick, sizeof(pick) - 1);
	string path(pick);
	cout << "Selected Menu Path: " << path << endl;
	if (path.compare("File/Exit") == 0) {
		exit(1);
	} else if (path.compare("File/Save") == 0) {
		save();
	} else {
		cout << "Selected path not yet implemented." << endl;
	}
}

void MusicClient::XBtnCallback(Fl_Widget* w, void* obj) {
	std::cout << "Go, go, gadget X button!" << endl;
	exit(1); // why 1? Is it to confirm to the event system that we handled it?
}

void MusicClient::SearchBtnCallback(Fl_Widget* w, void* obj) {
	((MusicClient*) obj)->findAlbum();
}

void MusicClient::MenuCallback(Fl_Widget* w, void* obj) {
	((MusicClient*) obj)->menuClicked();
}

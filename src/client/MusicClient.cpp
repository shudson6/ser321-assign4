#include "MusicClient.h"
#include <stdexcept>
#include <iostream>

MusicClient::MusicClient(const char* _author, const char* _lastfmkey) : MusicClient(_author, _lastfmkey, "music.json") {}

MusicClient::MusicClient(const char* _author, const char* _lastfmkey, const char* _filename)
		: MediaClientGui(_author), filename(_filename) {
	if (filename == 0) {
		throw std::invalid_argument("Filename supplied to MusicClient must not be null.");
	}
	library = new MusicLibrary();
	library->load(filename);
	finder = new AlbumFinder(_lastfmkey);

	callback(XBtnCallback, (void*) this);
	searchButt->callback(SearchBtnCallback, (void*) this);
}

MusicClient::~MusicClient() {
	delete [] library;
	delete [] finder;
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

void MusicClient::XBtnCallback(Fl_Widget* w, void* obj) {
	std::cout << "Go, go, gadget X button!" << endl;
	exit(1); // why 1? Is it to confirm to the event system that we handled it?
}

void MusicClient::SearchBtnCallback(Fl_Widget* w, void* obj) {
	((MusicClient*) obj)->findAlbum();
}

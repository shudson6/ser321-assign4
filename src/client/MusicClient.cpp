#include "MusicClient.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
using namespace std;

string MusicClient::trim(const char* const cstr) {
	string str(cstr);
	// right trim
	while (str.length() > 0 && (str[str.length()-1] == ' '  ||
								str[str.length()-1] == '\t' ||
								str[str.length()-1] == '\n' ||
								str[str.length()-1] == '\r')) {
		str.erase(str.length() - 1, 1);
	}
	// left trim
	while (str.length() > 0 && (str[0] == ' ' || str[0] == '\t')) {
		str.erase(0, 1);
	}
	return str;
}

string& MusicClient::sToPlus(string& str) {
	int pos;
	while ((pos = str.find(' ')) != string::npos) {
		str.replace(pos, 1, 1, '+');
	}
	return str;
}

void requireNonEmpty(const char* const str) {
	if (!str || string(str).empty()) {
		throw invalid_argument("MusicClient: author, lastfmkey, or filename not provided.");
	}
}

MusicClient::MusicClient(const char* const _author, const char* const _lastfmkey, const char* const _filename) 
	: MediaClientGui(_author) {
	requireNonEmpty(_author);
	requireNonEmpty(_lastfmkey);
	requireNonEmpty(_filename);

	finder = new AlbumFinder(_lastfmkey);

	filename = new char[strlen(_filename) + 1];
	strcpy(filename, _filename);
	library.load(filename);

	callback(XBtnCallback, (void*) this);
	menubar->callback(MenuCallback, (void*) this);
	searchButt->callback(SearchBtnCallback, (void*) this);
}

void MusicClient::MenuCallback(Fl_Widget* w, void* obj) {
	((MusicClient*) obj)->menuEvent();
}

void MusicClient::menuEvent() {
	char picked[80];
	menubar->item_pathname(picked, sizeof(picked)-1);
	string menu(picked);
	cout << "Menu Activated: " << menu << endl;
	if (menu.compare("File/Save") == 0) {
		// todo
	} else if (menu.compare("File/Exit") == 0) {
		close();
	} else {
		cout << "Action not implemented." << endl;
	}
}

Fl_Tree_Item* MusicClient::addAlbumNode(Album& alb, int pos) {
	cout << "Adding album node: " << alb.getTitle() << endl;
	Fl_Tree_Item* node = tree->add(alb.getTitle());
	if (node) {
		node->user_data(&alb);
		addTrackNodes(node);
	}
	return node;
}

void MusicClient::addTrackNodes(Fl_Tree_Item* albNode) {
	Album& alb = *((Album*) albNode->user_data());
	if (alb.size() > 0) {
		cout << "Adding track nodes ";
	}
	for (auto iter = alb.getTracks().begin(); iter != alb.getTracks().end(); ++iter) {
		cout << iter->getRank() << " ";
		cout.flush();
		tree->add(albNode, iter->getTitle())->user_data(const_cast<Track*>(&*iter));
	}
	cout << endl;
}
	

void MusicClient::SearchBtnCallback(Fl_Widget* w, void* obj) {
	cout << "Go, go, gadget Search!" << endl;
	((MusicClient*) obj)->findAlbum();
}

void MusicClient::findAlbum() {
	string alb = trim(albSrchInput->value());
	string art = trim(artSrchInput->value());
	cout << "Looking for " << alb << " by " << art << endl;
	// get space for the returned album; we have to provide a reference to the query function
	Album* album = new Album();
	if (finder->query(sToPlus(alb).c_str(), sToPlus(art).c_str(), *album)) {
		if (addAlbumNode(*album, 0)) {
			return; // happy
		}
	}
	// if we get here, not happy. delete album
	delete album;
}

void MusicClient::XBtnCallback(Fl_Widget* w, void* obj) {
	cout << "X button activated!" << endl;
	((MusicClient*) obj)->close();
}

void MusicClient::close() {
	cout << "Shutting down client." << endl;
	exit(1);
}

MusicClient::~MusicClient() {
	delete [] filename;
	delete finder;
}

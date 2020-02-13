#include "MusicClient.h"
#include <stdexcept>
#include <iostream>
using namespace std;

MusicClient::MusicClient(const char* _author, const char* _lastfmkey) : MusicClient(_author, _lastfmkey, "music.json") {}

MusicClient::MusicClient(const char* _author, const char* _lastfmkey, const char* _filename)
		: MediaClientGui(_author), filename(_filename) {
	if (filename.empty()) {
		throw invalid_argument("Filename supplied to MusicClient must not be empty.");
	}
	library = new MusicLibrary();
	library->load(filename);
	finder = new AlbumFinder(_lastfmkey);

	callback(XBtnCallback, (void*) this);
	searchButt->callback(SearchBtnCallback, (void*) this);
	menubar->callback(MenuCallback, (void*) this);
	tree->callback(TreeCallback, (void*) this);

	buildTree();
}

MusicClient::~MusicClient() {
	delete [] library;
	delete [] finder;
}

void MusicClient::buildTree() {
	cout << "Building tree..." << endl;
	tree->clear();
	tree->root_label("Albums");
	string* names = library->getAlbumNames();
	for (int i = 0; i < library->size(); ++i) {
		const Album* alb = library->getAlbum(names[i]);
		cout << "Adding\033[96m Album\033[0m node: " << alb->getTitle() << endl;
		tree->add(alb->getTitle().c_str());
		string path(alb->getTitle());
		path += "/";
		auto tracks = alb->getTracks();
		for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
			cout << "Adding\033[36m Track\033[0m node: " << iter->getTitle() << endl;
			tree->add((path + iter->getTitle()).c_str());
		}
		tree->close(tree->last(), 0);
	}
	cout << "Tree complete." << endl;
}

void MusicClient::save() {
	if (library->save(filename)) {
		cout << "Save successful." << endl;
	} else {
		cout << "Save to " << filename << " failed." << endl;
	}
}

void MusicClient::restore() {
	library->load(filename);
	buildTree();
}

void MusicClient::findAlbum() {
	string art(artSrchInput->value());
	string alb(albSrchInput->value());

	cout << "Go, go, gadget Search!" << endl;
	try {
		finder->query(alb, art);
	} catch (AlbumFinderException ex) {
		cout << "Search failed." << endl;
	}
}

void MusicClient::menuEvent() {
	char pick[80];
	menubar->item_pathname(pick, sizeof(pick) - 1);
	string path(pick);
	cout << "Selected Menu Path: " << path << endl;
	if (path.compare("File/Exit") == 0) {
		exit(1);
	} else if (path.compare("File/Save") == 0) {
		save();
	} else if (path.compare("File/Restore") == 0) {
		restore();
	} else if (path.compare("File/Tree Refresh") == 0) {
		buildTree();
	} else {
		cout << "Selected path not yet implemented." << endl;
	}
}

void MusicClient::treeEvent() {
	Fl_Tree_Item* item = (Fl_Tree_Item*) tree->item_clicked();
	cout << "Tree callback. Item selected: " << (item ? item->label() : "none") << endl;
	cout << "Callback reason: " << treeEventReason() << endl;
	if (tree->callback_reason() == FL_TREE_REASON_SELECTED) {
		// take action based on the depth of the selected item
		// Albums have depth 1 and Tracks depth 2
		if (item->depth() == 1) {
			showInfo(library->getAlbum(item->label()));
		} else if (item->depth() == 2) {
			const Album* alb = library->getAlbum(item->parent()->label());
			showInfo(alb, alb->getTrack(item->label()));
		}
	}
}

void MusicClient::showInfo(const Album* album) {
	cout << "Updating view with: " << album->getTitle() << " - " << album->getArtist() << endl;
	trackInput->value(NULL);
	albumInput->value(album->getTitle().c_str());
	authorInput->value(album->getArtist().c_str());
	rankInput->value(NULL);
	timeInput->value(album->getLengthStr().c_str());
	summaryMLI->value(album->getSummary().c_str());
}

void MusicClient::showInfo(const Album* album, const Track* track) {
	cout << "Updating view with: " << track->getTitle() << " - " << track->getArtist() << " - " << album->getTitle()
			<< endl;
	showInfo(album);
	trackInput->value(track->getTitle().c_str());
	authorInput->value(track->getArtist().c_str());
	rankInput->value(to_string(track->getRank()).c_str());
	timeInput->value(track->getLengthStr().c_str());
}

string MusicClient::treeEventReason() {
	string result;
	switch (tree->callback_reason()) {
		case FL_TREE_REASON_NONE: 
			result = "none";
			break;
		case FL_TREE_REASON_OPENED:
			result = "opened";
			break;
		case FL_TREE_REASON_CLOSED:
			result = "closed";
			break;
		case FL_TREE_REASON_SELECTED:
			result = "selected";
			break;
		case FL_TREE_REASON_DESELECTED:
			result = "deselected";
			break;
		default:
			result = "unknown";
	}
	return result;
}

void MusicClient::XBtnCallback(Fl_Widget* w, void* obj) {
	cout << "Go, go, gadget X button!" << endl;
	exit(1); // why 1? Is it to confirm to the event system that we handled it?
}

void MusicClient::SearchBtnCallback(Fl_Widget* w, void* obj) {
	((MusicClient*) obj)->findAlbum();
}

void MusicClient::MenuCallback(Fl_Widget* w, void* obj) {
	((MusicClient*) obj)->menuEvent();
}

void MusicClient::TreeCallback(Fl_Widget* w, void* obj) {
	((MusicClient*) obj)->treeEvent();
}

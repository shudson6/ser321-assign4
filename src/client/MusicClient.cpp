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

	// guarantee single-select mode
	tree->selectmode(FL_TREE_SELECT_SINGLE);
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
		addAlbumNode(*alb);
	}
	cout << "Tree complete." << endl;
}

Fl_Tree_Item* MusicClient::addAlbumNode(const Album& alb, int pos) {
	Fl_Tree_Item* node;
	if (pos >= 0) {
		node = tree->insert(tree->root(), alb.getTitle().c_str(), pos);
	} else {
		node = tree->add(alb.getTitle().c_str());
	}
	if (!node) {
		return NULL;
	}
	cout << "Adding album node: " << alb.getTitle() << endl;
	node->user_data((void*) &alb);
	if (alb.size() > 0) {
		cout << "\tand Track nodes..." << endl;
		// add tracks to the new node
		auto tracks = alb.getTracks();
		for (auto iter = tracks.cbegin(); iter != tracks.cend(); ++iter) {
			tree->add(node, iter->getTitle().c_str())->user_data((void*) alb.getTrack(iter->getTitle()));
		}
	}
	return node;
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

void MusicClient::addAlbumAction() {
	Album* al = getSelectedAlbum();
	if (al && !library->getAlbum(al->getTitle())) {
		cout << "Adding album " << al->getTitle() << " ... ";
		if (library->addAlbum(*al)) {
			/* sucessfully added:
			 * it's copied into the library but if it wasn't already there, we must be dealing
			 * with a dynamically-allocated album.
			 * change the tree's reference to the version in the library and delete ours
			 */
			tree->find_item(al->getTitle().c_str())->user_data((void*) library->getAlbum(al->getTitle()));
			delete al;
			cout << "done." << endl;
		} else {
			cout << "failed." << endl;
		}
	} else if (!al) {
		cout << "No album selected." << endl;
	} else {
		cout << "Album not added--title already in library." << endl;
	}
}

void MusicClient::removeAlbumAction() {
	Album* al = getSelectedAlbum();
	if (al && library->getAlbum(al->getTitle())) {
		// selected album is in library; just remove it
		cout << "Removing album " << al->getTitle() << " from library ... ";
		if (library->removeAlbum(al->getTitle())) {
			cout << "done." << endl;
		} else {
			cout << "failed." << endl;
		}
	} else if (al) {
		/* selected album not in library; we are dealing with a dynamically-allocated object
		 * remove the tree node and delete the object
		 */
		tree->remove(tree->find_item(al->getTitle().c_str()));
		delete al;
	} else {
		cout << "No album selected." << endl;
	}
}

Album* MusicClient::getSelectedAlbum() {
	// selected album node must match the albumInput text
	string alb = albumInput->value();
	Fl_Tree_Item* item = tree->find_item(trim(alb).c_str());
	if (!item) {
		return NULL;
	} else if (item->depth() == 1) {
		return (Album*) item->user_data();
	} else if (item->depth() == 2) {
		return (Album*) item->parent()->user_data();
	}
	return NULL;
}

void MusicClient::findAlbum() {
	string art(artSrchInput->value());
	string alb(albSrchInput->value());
	art = wsToPlus(trim(art));
	alb = wsToPlus(trim(alb));

	cout << "Go, go, gadget Search!" << endl;
	// get album, add it to tree (unless null)
	Album* found = finder->query(alb, art);
	if (found) {
		tree->select(addAlbumNode(*found, 0));
	} else {
		cout << "Search returned null." << endl;
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
	} else if (path.compare("Album/Add") == 0) {
		addAlbumAction();
	} else {
		cout << "Selected path not yet implemented." << endl;
	}
}

void MusicClient::treeEvent() {
	Fl_Tree_Item* item = (Fl_Tree_Item*) tree->item_clicked();
	cout << "Tree callback. Item selected: " << (item ? item->label() : "none") << endl;
	cout << "Callback reason: " << treeEventReason() << endl;
	if (tree->callback_reason() == FL_TREE_REASON_SELECTED) {
		showItem(item);
	}
}

void MusicClient::showItem(Fl_Tree_Item* item) {
	// type of item is determined by its depth
	if (item->depth() == 1) { 
		showInfo((Album*) item->user_data());
	} else if (item->depth() == 2) {
		showInfo((Album*) item->parent()->user_data(), (Track*) item->user_data());
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
	showInfo(album);
	cout << "Updating view with: " << track->getTitle() << " - " << track->getArtist() << " - " << album->getTitle()
			<< endl;
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

string& MusicClient::trim(string& str) {
	// right trim
	while (str.length() > 0 && isWhiteSpace(str[str.length() - 1])) {
		str.erase(str.length() - 1, 1);
	}
	// left trim
	while (str.length() > 0 && isWhiteSpace(str[0])) {
		str.erase(0, 1);
	}
	return str;
}

bool MusicClient::isWhiteSpace(char& c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

string& MusicClient::wsToPlus(string& str) {
	int pos;
	for (int i = 0; i < str.length(); ++i) {
		if (isWhiteSpace(str[i])) {
			str.replace(i, 1, 1, '+');
		}
	}
	return str;
}

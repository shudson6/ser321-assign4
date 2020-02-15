#include "MusicClient.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
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

	tree->showroot(0);
	buildTree();

	callback(XBtnCallback, (void*) this);
	menubar->callback(MenuCallback, (void*) this);
	searchButt->callback(SearchBtnCallback, (void*) this);
	tree->callback(TreeCallback, (void*) this);
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
		save();
	} else if (menu.compare("File/Restore") == 0) {
		restore();
	} else if (menu.compare("File/Tree Refresh") == 0) {
		treeRefresh();
	} else if (menu.compare("File/Exit") == 0) {
		close();
	} else if (menu.compare("Album/Add") == 0) {
		albumAdd();
	} else if (menu.compare("Album/Remove") == 0) {
		albumRemove();
	} else if (menu.compare("Track/Add") == 0) {
		trackAdd();
	} else if (menu.compare("Track/Remove") == 0) {
		trackRemove();
	} else {
		cout << "Action not implemented." << endl;
	}
}

void MusicClient::save() {
	cout << "Saving library to " << filename << " ... ";
	cout.flush();
	if (library.save(filename)) {
		cout << "complete." << endl;
	} else {
		cout << "failed." << endl;
	}
}

void MusicClient::restore() {
	library.load(filename);
	buildTree();
}

void MusicClient::treeRefresh() {
	cout << "Rebuilding tree from library..." << endl;
	buildTree();
}

void MusicClient::albumAdd() {
	Fl_Tree_Item* node = getSelectedAlbumNode();
	if (node) {
		// attempt to add not-added item to library
		Album* alb = (Album*) node->user_data();
		if (library.addAlbum(*alb)) {
			cout << "Added album " << alb->getTitle() << " to library." << endl;
			// album was copied into library; delete our copy and move node
			tree->remove(node);
			addAlbumNode(*library.getAlbum(alb->getTitle()));
			delete alb;
		} else {
			cout << "Album not added: title already exists." << endl;
		}
	} else {
		cout << "No album selected." << endl;
	}
}

void MusicClient::albumRemove() {
	Fl_Tree_Item* node = getSelectedAlbumNode();
	if (node) {
		// get the album
		Album* alb = (Album*) node->user_data();
		string title(alb->getTitle());
		if (string(node->parent()->label()).compare("Library") == 0) {
			// if it's in the library, simply ask to remove it
			if (library.removeAlbum(alb->getTitle())) {
				tree->remove(node);
				cout << "Removed album " << title << " from library." << endl;
			} else {
				cout << "Failed to remove album: reason unknown." << endl;
			}
		} else {
			// not in the library; remove the node and delete the memory
			cout << "Removing not-added album " << title << endl;
			tree->remove(node);
			delete alb;
		}
	} else {
		cout << "No album selected." << endl;
	}
}

void MusicClient::trackAdd() {
	Fl_Tree_Item* node = getSelectedTrackNode();
	Track* trk = (Track*) node->user_data();
	Album* alb = (Album*) node->parent()->user_data();
	// look for album in library; if there, attempt to add track
	const Album* libalb = library.getAlbum(alb->getTitle());
	if (libalb) {
		if (library.addTrack(*trk, alb->getTitle())) {
			addTrackNode(alb->getTitle(), *trk);
			tree->redraw();
			cout << "Added track " << trk->getTitle() << " to album " << alb->getTitle() << " in library." << endl;
		} else {
			cout << "Failed to add track. Is it already there?" << endl;
		}
	} else {
		// album not in library. we have to add it
		cout << "Album " << alb->getTitle() << " not in library. Adding ... ";
		cout.flush();
		Album toAdd(alb->getTitle(), alb->getArtist(), alb->getImgUrl(), alb->getSummary());
		if (library.addAlbum(toAdd)) {
			cout << "done. Now adding track ... ";
			if (library.addTrack(*trk, toAdd.getTitle())) {
				addAlbumNode(*library.getAlbum(toAdd.getTitle()));
				tree->redraw();
				cout << "done." << endl;
			} else {
				cout << "failed." << endl;
			}
		} else {
			cout << "failed." << endl;
		}
	}
}

void MusicClient::trackRemove() {
	Fl_Tree_Item* node = getSelectedTrackNode();
	if (node) {
		Track* trk = (Track*) node->user_data();
		Album* alb = (Album*) node->parent()->user_data();
		cout << "Remove track " << trk->getTitle() << " from album " << alb->getTitle() << " ... ";
		if (alb->removeTrack(trk->getRank())) {
			tree->remove(node);
			tree->redraw();
			cout << "done." << endl;
		} else {
			cout << "failed." << endl;
		}
	} else {
		cout << "No track selected." << endl;
	}
}

Fl_Tree_Item* MusicClient::getSelectedTrackNode() {
	Fl_Tree_Item* item = tree->first_selected_item();
	if (item->depth() == 3) {
		return item;
	} else {
		return NULL;
	}
}

Fl_Tree_Item* MusicClient::getSelectedAlbumNode() {
	Fl_Tree_Item* item = tree->first_selected_item();
	Fl_Tree_Item* sel = NULL;
	if (item) {
		if (item->depth() == 2) {
			sel = item;
		} else if (item->depth() == 3) {
			sel = item->parent();
		}
	}
	return sel;
}

Fl_Tree_Item* MusicClient::addAlbumNode(const Album& alb, int pos) {
	cout << "Adding album node: " << alb.getTitle() << endl;
	Fl_Tree_Item* node;
	if (pos < 0) {
		node = tree->add(tree->find_item("Library"), alb.getTitle());
	} else {
		node = tree->insert(tree->find_item("Not Added"), alb.getTitle(), 0);
	}
	if (node) {
		node->user_data(const_cast<Album*>(&alb));
		addTrackNodes(node);
		tree->close(node, false);
		tree->redraw();
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
	if (alb.size() > 0) {
		cout << endl;
	}
}

void MusicClient::addTrackNode(const char* alb, const Track& trk) {
	stringstream ss(alb);
	ss << "/" << trk.getTitle();
	Fl_Tree_Item* node = tree->add(ss.str().c_str());
	const Track* tptr = library.getAlbum(alb)->getTrack(trk.getRank());
	node->user_data((void*) tptr);
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

void MusicClient::TreeCallback(Fl_Widget* w, void* obj) {
	((MusicClient*) obj)->treeEvent();
}

void MusicClient::treeEvent() {
	Fl_Tree_Item* src = tree->callback_item();
	cout << "Tree callback: Item selected: " << (src ? src->label() : "none") << endl;
	string rsn;
	switch (tree->callback_reason()) {
		case FL_TREE_REASON_NONE:
			rsn = "s&gs i guess";
			break;
		case FL_TREE_REASON_OPENED:
			rsn = "opened";
			break;
		case FL_TREE_REASON_CLOSED:
			rsn = "closed";
			break;
		case FL_TREE_REASON_SELECTED:
			rsn = "selected";
			break;
		case FL_TREE_REASON_DESELECTED:
			rsn = "deselected";
			break;
		default:
			rsn = "unknown";
	}
	cout << "Reason: " << rsn << endl;
	if (tree->callback_reason() == FL_TREE_REASON_SELECTED) {
		showItem(src);
	}
}

void MusicClient::showItem(Fl_Tree_Item* item) {
	// first, is this an album or a track?
	if (item->depth() == 2) {
		showInfo((Album*) item->user_data());
	} else if (item->depth() == 3) {
		showInfo((Album*) item->parent()->user_data(), (Track*) item->user_data());
	}
}

void MusicClient::showInfo(const Album* alb) {
	trackInput->value(NULL);
	albumInput->value(alb->getTitle());
	authorInput->value(alb->getArtist());
	rankInput->value("n/a");
	timeInput->value(alb->timeStr());
	summaryMLI->value(alb->getSummary());
	updateGenreChoice(alb);
	setImage(alb->getImgUrl());
}

void MusicClient::showInfo(const Album* alb, const Track* trk) {
	trackInput->value(trk->getTitle());
	albumInput->value(alb->getTitle());
	authorInput->value(trk->getArtist());
	rankInput->value(to_string(trk->getRank()).c_str());
	timeInput->value(trk->timeStr());
	summaryMLI->value(alb->getSummary());
	updateGenreChoice(alb);
	setImage(alb->getImgUrl());
}

void MusicClient::updateGenreChoice(const Album* alb) {
	genreChoice->clear();
	auto genres = alb->getGenres();
	for (auto iter = genres.cbegin(); iter != genres.cend(); ++iter) {
		genreChoice->add(iter->c_str());
	}
}

void MusicClient::buildTree() {
	cleanTree();
	tree->add("Library");
	tree->insert(tree->root(), "Not Added", 0);
	const string* names = library.getAlbumNames();
	for (int i = 0; i < library.size(); ++i) {
		addAlbumNode(*library.getAlbum(names[i].c_str()));
	}
	delete [] names;
	cout << "Tree complete." << endl;
}

void MusicClient::cleanTree() {
	// free all the album nodes under "not added"
	Fl_Tree_Item* node = tree->find_item("Not Added");
	if (node) {
		for (int i = 0; i < node->children(); ++i) {
			delete (Album*) node->child(i)->user_data();
		}
	}
	// now it's safe to clear the tree
	tree->clear();
}

void MusicClient::closeTreeNodes() {
	closeTreeNodes(tree->find_item("Library"));
	closeTreeNodes(tree->find_item("Not Added"));
}

void MusicClient::closeTreeNodes(Fl_Tree_Item* node) {
	for (int i = 0; i < node->children(); ++i) {
		tree->close(node->child(i), false);
	}
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

void MusicClient::setImage(const char* const url) {
	if (png) {
		delete png;
		png = NULL;
	}
	if (url) {
		cout << "Fetching image from: " << url << endl;
		try {
			ostringstream os;
			curlpp::Easy request;
			request.setOpt(new curlpp::options::WriteStream(&os));
			request.setOpt(new curlpp::options::Url(url));
			request.perform();
			cout << "Retrieved image data." << endl;
			png = new Fl_PNG_Image(NULL, (const unsigned char*) os.str().c_str(), os.str().size());
			box->image(png);
			box->redraw();
		} catch (...) {
			cout << "crap." << endl;
		}
	}
}

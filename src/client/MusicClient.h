#ifndef __INCLUDE_SHUDSON6_MUSICCLIENT
#define __INCLUDE_SHUDSON6_MUSICCLIENT

#include "MediaClientGui.cpp"
#include "MusicLibrary.h"
#include "AlbumFinder.h"

class MusicClient : public MediaClientGui {
	private:
		char* filename;
		MusicLibrary library;
		AlbumFinder* finder;

	public:
		MusicClient(const char* const _author, const char* const _lastfmkey, const char* const _filename);
		~MusicClient();

		// callbacks for various components, hopefully self-explanatory
		static void XBtnCallback(Fl_Widget*, void*);
		static void SearchBtnCallback(Fl_Widget*, void*);
		static void TreeCallback(Fl_Widget*, void*);
		static void MenuCallback(Fl_Widget*, void*);

		void setImage(const char* const url);

	private:
		void buildTree();
		void cleanTree();
		void closeTreeNodes();
		void closeTreeNodes(Fl_Tree_Item*);
		void treeRefresh();
		Fl_Tree_Item* addAlbumNode(const Album&, int = -1);
		void addTrackNodes(Fl_Tree_Item*);
		void addTrackNode(const char*, const Track&);
		Fl_Tree_Item* getSelectedAlbumNode();
		Fl_Tree_Item* getSelectedTrackNode();
		void showItem(Fl_Tree_Item*);
		void showInfo(const Album*);
		void showInfo(const Album*, const Track*);
		void updateGenreChoice(const Album*);

		// instance methods called by the callbacks
		void close();
		void save();
		void restore();
		void findAlbum();
		void menuEvent();
		void treeEvent();
		string treeEventReason();
		void albumAdd();
		void albumRemove();
		void trackAdd();
		void trackRemove();
		
		static string trim(const char* const);
		static string& sToPlus(string&);
};

#endif // __INCLUDE_SHUDSON6_MUSICCLIENT

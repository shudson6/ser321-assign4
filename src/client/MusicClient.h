#ifndef __INCLUDE_SHUDSON6_MUSICCLIENT
#define __INCLUDE_SHUDSON6_MUSICCLIENT

#include "MediaClientGui.cpp"
#include "MusicLibrary.h"
#include "AlbumFinder.h"

class MusicClient : public MediaClientGui {
	private:
		string filename;
		MusicLibrary* library;
		AlbumFinder* finder;

	public:
		MusicClient(const char* _author, const char* _lastfmkey);
		MusicClient(const char* _author, const char* _lastfmkey, const char* _filename);
		~MusicClient();

		// callbacks for various components, hopefully self-explanatory
		static void XBtnCallback(Fl_Widget*, void*);
		static void SearchBtnCallback(Fl_Widget*, void*);
		static void TreeCallback(Fl_Widget*, void*);
		static void MenuCallback(Fl_Widget*, void*);

	private:
		void buildTree();
		Fl_Tree_Item* addAlbumNode(const Album&, int = -1);
		Album* getSelectedAlbum();
		void showItem(Fl_Tree_Item*);
		void showInfo(const Album*);
		void showInfo(const Album*, const Track*);

		// instance methods called by the callbacks
		void save();
		void restore();
		void findAlbum();
		void menuEvent();
		void treeEvent();
		string treeEventReason();
		void addAlbumAction();
		void removeAlbumAction();
		
		static string& trim(string&);
		static string& wsToPlus(string&);
		static bool isWhiteSpace(char&);
};

#endif // __INCLUDE_SHUDSON6_MUSICCLIENT

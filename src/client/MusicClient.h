#ifndef __INCLUDE_SHUDSON6_MUSICCLIENT
#define __INCLUDE_SHUDSON6_MUSICCLIENT

#include "MediaClientGui.cpp"
#include "MusicLibrary.h"
#include "AlbumFinder.h"

class MusicClient : public MediaClientGui {
	private:
		const char* filename;
		MusicLibrary* library;
		AlbumFinder* finder;

	public:
		MusicClient(const char* _author, const char* _lastfmkey);
		MusicClient(const char* _author, const char* _lastfmkey, const char* _filename);
		~MusicClient();

		void buildTree();
		void findAlbum();
		
		// callbacks for various components, hopefully self-explanatory
		static void XBtnCallback(Fl_Widget*, void*);
		static void SearchBtnCallback(Fl_Widget*, void*);
		static void TreeCallback(Fl_Widget*, void*);
		static void MenuCallback(Fl_Widget*, void*);
};

#endif // __INCLUDE_SHUDSON6_MUSICCLIENT

/**
 * Copyright (c) 2020 Steven Hudson
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Steven Hudson shudson6
 * @version 2020.2
 **/
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

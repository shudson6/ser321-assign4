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
#ifndef __INCLUDE_SHUDSON6_MUSICLIBRARY
#define __INCLUDE_SHUDSON6_MUSICLIBRARY

#include "Album.h"
#include "MusicDescription.h"
#include "Track.h"
#include <map>
#include <string>
#include <json/json.h>

class MusicLibrary : public MusicDescription {
	public:
		MusicLibrary();
		bool addAlbum(const Album&);
		bool addTrack(const Track&, const char* const);
		bool removeAlbum(const char* name);
		void load(const char* filename);
		void parseJson(Json::Value&);
		const char* timeStr();

		const Album* getAlbum(const char* name) const;
		const std::string* getAlbumNames() const;
		int size() const;
		int getLength() const;
		Json::Value toJson() const;
		bool save(const char* filename) const;

	private:
		std::map<std::string, Album> albums;
};

#endif // __INCLUDE_SHUDSON6_MUSICLIBRARY

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
#ifndef __INCLUDE_SHUDSON6_ALBUMFINDER
#define __INCLUDE_SHUDSON6_ALBUMFINDER

#include "Album.h"
#include "Track.h"
#include <vector>
#include <json/json.h>
#include <stdexcept>

class AlbumFinder {
	public:
		AlbumFinder(const char* const _lastfmkey);
		// search lastfm for an album of name by artist; on success, result is store in the referenced album
		bool query(const char* const name, const char* const artist, Album& album) const;

	private:
		std::string lastfmkey;

		const char* const buildUrl(const char* const name, const char* const artist) const;
		void errorMessage(const char* const, const std::exception&) const;
		std::string executeQuery(const char* const) const;
		Json::Value jsonObj(std::string&) const;
		void parseGenres(Json::Value&, Album&) const;
		std::string parseImg(Json::Value&) const;
		void parseJson(Json::Value&, Album&) const;
		Track parseTrack(Json::Value&, const char* const) const;
		void parseTracks(Json::Value&, Album&) const;

		static const char* const URLFMT;
};

class AlbumFinderException : public std::logic_error {
	public:
		AlbumFinderException(const char* what) : logic_error(what) {}
};

#endif // __INCLUDE_SHUDSON6_ALBUMFINDER

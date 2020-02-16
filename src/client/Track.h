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
#ifndef __INCLUDE_SHUDSON6_TRACK
#define __INCLUDE_SHUDSON6_TRACK

#include "MusicDescription.h"

#define UNKNOWN_ALBUM "[unknown album]"

class Track : public MusicDescription {
	public:
		static Track fromJson(const Json::Value& json);

		Track(const std::string& _title, const std::string& _artist, const std::string& _album, int _rank, int _len);
		Track(const char* _title, const char* _artist, const char* _album, int _rank, int _len);

		const char* getAlbum() const;
		int getRank() const;
		Json::Value toJson() const;

		bool operator<(Track&) const;

	private:
		std::string album;
		int rank;
};

#endif // __INCLUDE_SHUDSON6_TRACK

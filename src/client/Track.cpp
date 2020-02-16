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
#include "Track.h"
#include <stdexcept>
using namespace std;

Track::Track(const string& _title, const string& _artist, const string& _album, int _rank, int _len)
	: Track(_title.c_str(), _artist.c_str(), _album.c_str(), _rank, _len) {
}

Track::Track(const char* _title, const char* _artist, const char* _album, int _rank, int _len)
	: MusicDescription(_title, _artist, _len) {
	if (_rank < 1) {
		throw invalid_argument("Track: rank must be a positive integer.");
	}
	rank = _rank;
	album = (_album ? _album : UNKNOWN_ALBUM);
}

const char* Track::getAlbum() const {
	return album.c_str();
}

int Track::getRank() const {
	return rank;
}

bool Track::operator<(Track& that) const {
	return this->rank < that.rank;
}

Json::Value Track::toJson() const {
	Json::Value json(MusicDescription::toJson());
	json["album"] = album;
	json["rank"] = rank;
	return json;
}

Track Track::fromJson(const Json::Value& json) {
	string t = json["title"].asString();
	string a = json["artist"].asString();
	string r = json["album"].asString();
	int o = json["rank"].asInt();
	int s = json["time"].asInt();
	return Track(t, a, r, o, s);
}

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
#include "MusicDescription.h"
#include <cstring>
#include <sstream>
using namespace std;

MusicDescription::MusicDescription(const string& _title, const string& _artist, int _len)
	: MusicDescription(_title.c_str(), _artist.c_str(), _len) {
}

MusicDescription::MusicDescription(const char* _title, const char* _artist, int _len) {
	// gracefully handle the possibility of null inputs
	title = (_title ? _title : NO_TITLE);
	artist = (_artist ? _artist : UNKNOWN_ARTIST);
	len = _len;
	lenstr = TimeStr(len);
}

string MusicDescription::TimeStr(int secs) {
	stringstream result;
	if (secs < 0) {
		result.put('-');
		secs *= -1;
	}
	int hh = secs / 3600;
	int mm = (secs % 3600) / 60;
	int ss = secs % 60;
	if (hh > 0) {
		result << hh << ':';
	}
	char buf[6];
	snprintf(buf, 6, "%02d:%02d", mm, ss);
	result << buf;
	return result.str();
}

const char* MusicDescription::getArtist() const {
	return artist.c_str();
}

int MusicDescription::getLength() const {
	return len;
}

const char* MusicDescription::timeStr() const {
	return lenstr.c_str();
}

const char* MusicDescription::getTitle() const { 
	return title.c_str();
}

Json::Value MusicDescription::toJson() const {
	Json::Value json;
	json["title"] = title;
	json["artist"] = artist;
	json["time"] = len;
	return json;
}

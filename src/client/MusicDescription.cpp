#include "MusicDescription.h"
#include <stdexcept>

MusicDescription::MusicDescription(const string& _title, const string& _artist, int _len) {
	if (_len < 0) {
		throw std::invalid_argument("MusicDescription: _len must not be negative.");
	}
	title = _title;
	artist = _artist;
	len = _len;
	makeLenStr();
}

string MusicDescription::getTitle() const {
	return title;
}

string MusicDescription::getArtist() const {
	return artist;
}

int MusicDescription::getLength() const {
	return len;
}

string MusicDescription::getLengthStr() const {
	return lenstr;
}

void MusicDescription::makeLenStr() {
	char str[16];		// hh:mm:ss format is 8 characters; plenty of room
	int hh = len / 3600;
	int mm = (len % 3600) / 60;
	int ss = len % 60;

	// stuff the values into a string in hh:mm:ss format. there is room for more characters than an int can need.
	// hh part will be left off if zero
	int pos = (hh <= 0) ? 0 : snprintf(str, 8, "%d:", hh);
	snprintf(str + pos, 7, "%02d:%02d", mm, ss);
	lenstr = str;
}

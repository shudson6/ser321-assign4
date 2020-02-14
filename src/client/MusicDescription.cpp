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

#include "../client/MusicClient.h"

const char* const key = "420225dc861196ff11eaa8d583eb7dd4";

int main(int argc, char** argv) {
	MusicClient mc("shudson6", argv[1], "music.json");
	return Fl::run();
}

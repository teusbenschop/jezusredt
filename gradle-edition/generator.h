/*
Copyright (©) 2003-2017 Teus Benschop.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


// Basic C headers.
#include <cstdlib>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>


// C headers.
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/param.h>
#include <unistd.h>
#include <libgen.h>
#include <ifaddrs.h>


// C++ headers.
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <set>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <thread>
#include <cmath>
#include <mutex>
#include <numeric>
#include <random>
#include <limits>
#include <atomic>
#include <unordered_map>
#include <queue>

#include "pugixml.hpp"

using namespace std;

using namespace pugi;


class Html
{
public:
  Html (string title);
  void p ();
  void div (string id);
  void txt (string text);
  string get ();
  string inner ();
  void h (int level, string text);
  void a (string reference, string text);
  void img (string image);
  void buy (string coin);
  void save (string name);
private:
  xml_document htmlDom;
  xml_node currentPDomElement;
  xml_node headDomNode;
  xml_node bodyDomNode;
  bool current_p_node_open = false;
  void newNamedHeading (string style, string text, bool hide = false);
};


void file_put_contents (string filename, string contents);
string file_get_contents (string filename);
string implode (vector <string>& values, string delimiter);
vector <string> explode (string value, char delimiter);
void create_html_page (string file);
void create_html_index (vector <string> files);
string trim (string s);
vector <string> scandir (string folder);
string get_extension (string & url, bool remove);
string str_replace (string search, string replace, string subject);
string str2lower (string value);


template <typename T>
void array_remove (T needle, vector <T> & haystack)
{
  haystack.erase (std::remove (haystack.begin(), haystack.end(), needle), haystack.end());
}


int main (int argc, char *argv[]);


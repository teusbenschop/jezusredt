/*
 Copyright (©) 2017-2019 Teus Benschop.
 
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


#include "generator.h"


// The folder with the input files.
string inputfolder;
// The folder for the web output.
string outputfolder;


Html::Html (string title)
{
  // <html>
  xml_node root_node = htmlDom.append_child ("html");
  
  // <head>
  headDomNode = root_node.append_child ("head");
  
  xml_node title_node = headDomNode.append_child ("title");
  title_node.text ().set (title.c_str());
  
  // <meta http-equiv="content-type" content="text/html; charset=UTF-8" />
  xml_node meta_node = headDomNode.append_child ("meta");
  meta_node.append_attribute ("http-equiv") = "content-type";
  meta_node.append_attribute ("content") = "text/html; charset=UTF-8";
  
  // <meta name="viewport" content="width=device-width, initial-scale=1.0">
  // This tag helps to make the page mobile-friendly.
  // See https://www.google.com/webmasters/tools/mobile-friendly/
  meta_node = headDomNode.append_child ("meta");
  meta_node.append_attribute ("name") = "viewport";
  meta_node.append_attribute ("content") = "width=device-width, initial-scale=1.0";
  
  // <body>
  bodyDomNode = root_node.append_child ("body");
}


void Html::p ()
{
  currentPDomElement = bodyDomNode.append_child ("p");
  current_p_node_open = true;
}


void Html::div (string id)
{
  currentPDomElement = bodyDomNode.append_child ("div");
  currentPDomElement.append_attribute ("id") = id.c_str();
  current_p_node_open = true;
}


void Html::txt (string text)
{
  if (text != "") {
    if (!current_p_node_open) p ();
    xml_node span = currentPDomElement.append_child ("span");
    span.text().set (text.c_str());
  }
}


// This creates a heading
void Html::h (int level, string text)
{
  string style = "h" + to_string (level);
  xml_node textHDomElement = bodyDomNode.append_child (style.c_str());
  textHDomElement.text ().set (text.c_str());
  // Make paragraph null, so that adding subsequent text creates a new paragraph.
  current_p_node_open = false;
}


void Html::a (string reference, string text)
{
  xml_node aDomElement = currentPDomElement.append_child ("a");
  aDomElement.append_attribute ("href") = reference.c_str();
  aDomElement.text ().set (text.c_str());
}


void Html::img (string image)
{
  xml_node aDomElement = currentPDomElement.append_child ("img");
  aDomElement.append_attribute ("src") = image.c_str();
}


// This gets and then returns the html text
string Html::get ()
{
  // Get the html.
  stringstream output;
  htmlDom.print (output, "", format_indent);
  string html = output.str ();
  
  // Add html5 doctype.
  html.insert (0, "<!DOCTYPE html>\n");
  
  return html;
}


// Returns the html text within the <body> tags, that is, without the <head> stuff.
string Html::inner ()
{
  string page = get ();
  size_t pos = page.find ("<body>");
  if (pos != string::npos) {
    page = page.substr (pos + 6);
    pos = page.find ("</body>");
    if (pos != string::npos) {
      page = page.substr (0, pos);
    }
  }
  return page;
}


// This saves the web page to file
// $name: the name of the file to save to.
void Html::save (string name)
{
  string html = get ();
  string path = outputfolder + "/" + name;
  file_put_contents (path, html);
}


void file_put_contents (string filename, string contents)
{
  try {
    ofstream file;
    file.open(filename, ios::binary | ios::trunc);
    file << contents;
    file.close ();
  } catch (...) {
  }
}


string file_get_contents (string filename)
{
  try {
    ifstream ifs(filename.c_str(), ios::in | ios::binary | ios::ate);
    streamoff filesize = ifs.tellg();
    if (filesize == 0) return "";
    ifs.seekg(0, ios::beg);
    vector <char> bytes((int)filesize);
    ifs.read(&bytes[0], (int)filesize);
    return string(&bytes[0], (int)filesize);
  }
  catch (...) {
    return "";
  }
}


string implode (vector <string>& values, string delimiter)
{
  string full;
  for (vector<string>::iterator it = values.begin (); it != values.end (); ++it)
  {
    full += (*it);
    if (it != values.end ()-1) full += delimiter;
  }
  return full;
}


vector <string> explode (string value, char delimiter)
{
  vector <string> result;
  istringstream iss (value);
  for (string token; getline (iss, token, delimiter); )
  {
    result.push_back (move (token));
  }
  return result;
}


void create_html_page (string file)
{
  string title (file);
  string extension = get_extension (title, true);
  Html html (title);
  html.h (2, title);
  html.p ();
  html.a ("index.html", "Index");
  string path (inputfolder);
  path.append ("/" + file);
  string contents = file_get_contents (path);
  contents = trim (contents);
  vector <string> lines = explode (contents, '\n');
  for (auto line : lines) {
    html.p ();
    if (line.find ("http") == 0) {
      html.a (line, "Meer informatie");
    } else {
      html.txt (line);
    }
  }
  html.p ();
  html.a ("index.html", "Index");
  file = title;
  file = str2lower (file);
  file = str_replace (" ", "-", file);
  file = str_replace ("?", "", file);
  file = str_replace ("ë", "e", file);
  html.save (file + ".html");
}


void create_html_index (vector <string> files) // Todo
{
  Html html ("");
  html.h (2, "Jezus Redt");
  for (auto file : files) {
    // E.g. "filename.txt", remove the .txt bit.
    string title (file);
    get_extension (title, true);
    file = title;
    file = str2lower (file);
    file = str_replace (" ", "-", file);
    file = str_replace ("?", "", file);
    html.p ();
    html.a (file + ".html", title);
  }
  html.save ("index.html");
}


string trim (string s)
{
  if (s.length () == 0) return s;
  size_t beg = s.find_first_not_of(" \t\n\r");
  size_t end = s.find_last_not_of(" \t\n\r");
  if (beg == string::npos) return "";
  return string (s, beg, end - beg + 1);
}


vector <string> scandir (string folder)
{
  // Storage.
  vector <string> files;
  // Open folder and iterate over it.
  DIR * dir = opendir (folder.c_str());
  if (dir) {
    struct dirent * direntry;
    while ((direntry = readdir (dir)) != NULL) {
      string name = direntry->d_name;
      // Exclude short-hand directory names.
      if (name == ".") continue;
      if (name == "..") continue;
      // Exclude developer temporal files.
      if (name == ".deps") continue;
      if (name == ".dirstamp") continue;
      // Exclude macOS files.
      if (name == ".DS_Store") continue;
      // Store the name.
      files.push_back (name);
    }
    closedir (dir);
  }
  // Sort the entries.
  sort (files.begin(), files.end());
  // Remove . and ..
  array_remove (string("."), files);
  array_remove (string(".."), files);
  // Done.
  return files;
}


string get_extension (string & url, bool remove)
{
  string extension;
  size_t pos = url.find_last_of (".");
  if (pos != string::npos) {
    extension = url.substr (pos + 1);
    if (remove) url.erase (pos);
  }
  return extension;
}


string str_replace (string search, string replace, string subject)
{
  size_t offposition = subject.find (search);
  while (offposition != string::npos) {
    subject.replace (offposition, search.length (), replace);
    offposition = subject.find (search, offposition + replace.length ());
  }
  return subject;
}


string str2lower (string value)
{
  transform (value.begin(), value.end(), value.begin(), ::tolower);
  return value;
}


int main (int argc, char *argv[])
{
  (void) argc;
  inputfolder = argv[1];
  outputfolder = argv[2];
  vector <string> files = scandir (inputfolder);
  for (auto file : files) {
    create_html_page (file);
  }
  create_html_index (files);
  return EXIT_SUCCESS;
}

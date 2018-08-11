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


#include "generator.h"


// The folder with the input files.
string inputfolder;
// The folder for the web output.
string outputfolder;


Html::Html (string title, string javascript)
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
  
  // Include possible JavaScript.
  if (!javascript.empty ()) {
    
    // <script type="text/javascript" src="jquery.js"></script>
    xml_node script1_node = headDomNode.append_child ("script");
    script1_node.append_attribute ("type") = "text/javascript";
    script1_node.append_attribute ("src") = "jquery.js";
    script1_node.text ().set (" ");
    
    // <script type="text/javascript" src="xxxxx.js"></script>
    xml_node script2_node = headDomNode.append_child ("script");
    script2_node.append_attribute ("type") = "text/javascript";
    javascript.append (".js");
    script2_node.append_attribute ("src") = javascript.c_str();
    script2_node.text ().set (" ");
  }
  
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


// Adds a "Buy" button for the $coin.
void Html::buy (string coin)
{
  xml_node button_node = currentPDomElement.append_child ("button");
  string onclick = "buycoin('" + coin + "')";
  button_node.append_attribute ("onclick") = onclick.c_str();
  button_node.text ().set ("Buy");
}


// This gets and then returns the html text
string Html::get ()
{
  // Get the html.
  stringstream output;
  htmlDom.print (output, "", format_raw);
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


void create_html (string folder, string file)
{
  string path (inputfolder);
  if (!folder.empty ()) path.append ("/" + folder);
  path.append ("/" + file);
  string contents = file_get_contents (path);
  cout << contents << endl;

}


int main (int argc, char *argv[])
{
  (void) argc;
  inputfolder = argv[1];
  outputfolder = argv[2];
  create_html ("", "index.txt");
  return EXIT_SUCCESS;
}

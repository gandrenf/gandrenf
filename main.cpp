#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;

void load_vector(int n_lines, vector<string> *p_v) {
  string line;

  for (int i=0; i<n_lines; i++) {
    getline (cin,line);
    p_v->push_back(line);
  }
}

void print_vector(vector<string> *p_v) {
  for (int i=0; i<p_v->size(); i++) {
      cout << p_v->operator[](i) << "\n";
    }
}

struct attr_t {
  string name;
  string value;
} typedef attr_t;

class TagCode {
  public:
    TagCode *p_tag_father;
    vector<TagCode> tags_sons;
    string name;
    vector<attr_t> attrs;
};

void find_attr (string str_attr, vector<attr_t> attrs, string *p_str_value) {
  *p_str_value = "Not Found!";

  for (int i=0; i<attrs.size(); i++) {
    if ( str_attr.compare(attrs[i].name) == 0 ) {
        *p_str_value = attrs[i].value;
    }
  }
}

void final_process_query (string str_query, TagCode *p_tag, string *p_str_value) {
  string str_tag, str_attr;
  
  *p_str_value = "Not Found!";
  size_t pos = str_query.find("~");
  if ( pos != string::npos ) {
    str_tag  = str_query.substr(0, pos);
    str_attr = str_query.substr(pos+1);
    if ( str_tag.compare(p_tag->name) == 0 ) {
      find_attr( str_attr, p_tag->attrs, p_str_value );
    }
  }
}

/*
string process_query (string str_query, TagCode *p_tag) {
  size_t pos;
  TagCode *p_actual_tag = p_tag;
  string str_tag, str_attr, str_value = "Not Found!";
  int ntags = count(str_query.begin(), str_query.end(), ".") + 1;

  if ( ntags > 1) {
    pos = str_query.find(".");
    if ( pos != string::npos ) {
      str_tag = str_query.substr(0, pos);
      str_query = str_query.substr(pos+1);
      if ( str_tag.compare(p_actual_tag->name) == 0 ) {
        p_actual_tag = &p_actual_tag->tags_sons[0];
        pos = str_query.find(".");
        if ( pos != string::npos ) {
          str_tag = str_query.substr(0, pos);
          if ( str_tag.compare(p_tag->tags_sons[].name) == 0 ) {

          }
        }
      }
    }
  }
  
  
  for (int i=0; i<p_actual_tag->tags_sons.size(); i++) {
    string str_aux;
    if ( find_attr(str_query, &p_actual_tag->tags_sons[i], &str_aux) ) {
      str_value = str_aux;
    }
  }

  return str_value;
}
*/

void decode_tag_line (string line, TagCode *p_tagcode) {
  attr_t attr;
  size_t pos;

  pos = line.find(" ");
  p_tagcode->name = line.substr(1,pos-1);
  line = line.substr(pos);
	line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
  pos = line.find("=");
  while ( pos != string::npos ) {
    attr.name = line.substr(0,pos);
    line = line.substr(pos+2);
    pos = line.find("\"");
    attr.value = line.substr(0,pos);
    p_tagcode->attrs.push_back(attr);
    line = line.substr(pos+1);
    pos = line.find("=");
  }
}

void load_tags(int nlines, TagCode *p_tag) {
    string line, name;
    TagCode *p_actual_tag;

    getline (cin, line);
    p_actual_tag = p_tag;
    decode_tag_line (line, p_actual_tag);
    name = "/" + p_actual_tag->name;
    for (int i=1; i<nlines; i++) {
      getline (cin, line);
      if ( line.find(name) == string::npos ) {
        TagCode *p_subtag = new TagCode();
        decode_tag_line (line, p_subtag);
        name = "/" + p_subtag->name;
        p_subtag->p_tag_father = p_actual_tag;
        p_actual_tag->tags_sons.push_back(*p_subtag);
        p_actual_tag = p_subtag;
      }
      else {
        p_actual_tag = p_actual_tag->p_tag_father;
        name = "/" + p_actual_tag->name;
      }
    }
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */ 
    int l = 0, q = 0;
    string::size_type sz;
    string line;
    vector<string> queries_list;
    TagCode *p_tag = new TagCode();
    p_tag->p_tag_father = p_tag;
  
    getline(cin,line);
    l = stoi (line,&sz);    
    line = line.substr(sz);
    q = stoi (line,&sz);

    load_tags(l, p_tag);
    load_vector(q, &queries_list);
    final_process_query(queries_list[0], p_tag, &line);

    cout << line << "\n";

    return 0;
}

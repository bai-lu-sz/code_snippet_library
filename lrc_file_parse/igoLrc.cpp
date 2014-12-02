#include "igoLrc.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <shlwapi.h>


#pragma comment(lib, "shlwapi.lib")

using std::vector;
using namespace std;

//////////////////////////////////////////////////////////////////////////
// static function
static std::string wstring_2_string(const std::wstring str)
{// wstringתstring
  unsigned len = str.size() * 4;
  setlocale(LC_CTYPE, "");
  char *p = new char[len];
  wcstombs(p,str.c_str(),len);
  std::string str1(p);
  delete[] p;
  return str1;
}


#define STR_WHITESPACE " \n\r"

igoLrc::igoLrc(const wstring lrc_file /* = "" */)
:is_load_file_ok(false)
{
  m_lrc.clear();

  if (!lrc_file.empty())
  {
    is_load_file_ok = set_lrc_file(lrc_file);
  }
}

igoLrc::~igoLrc(void)
{
}

bool igoLrc::set_lrc_file(const wstring& lrc_file)
{
  if (lrc_file.empty() || FALSE == PathFileExistsW(lrc_file.c_str()))
  {
    is_load_file_ok = false;
    return false;
  }

  string ansi_path = wstring_2_string(lrc_file);

  ifstream f(ansi_path.c_str(), ifstream::in | ifstream::binary);
  //f.open("E:\\vs_test\\igolrc\\bin\\abc.lrc", ios::in|ios::out|ios::binary);
  //f.rdbuf()->open(lrc_file.c_str(), ifstream::in | ifstream::binary);
  //string content;
  //f >> content;
  if (NULL == f)
  {
    printf("open failed!\n");
  }
  
  char content[102400] = {0};
  //f >> content;
  f.read(content, 102400);
  f.close();

  return set_lrc_content(content);
}

bool igoLrc::set_lrc_content(const string& lrc_content)
{
  if (lrc_content.empty())
  {
    return false;
  }

  //split
  vector<string> vec_string;
  string split;
  int pos = string::npos;
  int pos_s = 0;
  pos = lrc_content.find('\n', pos_s);
  while (string::npos != pos)
  {
    split =  lrc_content.substr(pos_s, pos - pos_s);
    split = trim(split);

    vec_string.push_back(split);

    pos_s = pos + 1;
    pos = pos_s;
    pos = lrc_content.find('\n', pos_s);
  }

  split =  lrc_content.substr(pos_s);
  split = trim(split);
  vec_string.push_back(split);

  // parse
  vector<string>::iterator it = vec_string.begin();
  for (; it != vec_string.end(); ++it)
  {
    string raw = *it;
    if (0 == raw.find("[al:"))
    {
    }
    else if (0 == raw.find("[ar:"))
    {
    }
    else if (0 == raw.find("[by:"))
    {
    }
    else if (0 == raw.find("[offset:"))
    {
    }
    else if (0 == raw.find("[re:"))
    {
    }
    else if (0 == raw.find("[ti:"))
    {
    }
    else if (0 == raw.find("[ve:"))
    {
    }
    else if (0 == raw.find("["))
    {
      int pos_1 = raw.find(":");
      int pos_2 = raw.find("]");
      if (string::npos == pos_1 || string::npos == pos_2)
      {
        continue;
      }

      int pos_3 = raw.find(":", pos_1 + 1);
      if (string::npos == pos_3)
      {
        pos_3 = raw.find(".", pos_1 + 1);
      }
      
      string temp_time;
      temp_time = raw.substr(1, pos_1 - 1);
      if (temp_time.empty())
      {
        continue;
      }

      unsigned m, s, ms = 0;
      m = atoi(temp_time.c_str());
      if (string::npos != pos_3)
      {
        temp_time = raw.substr(pos_1 + 1, pos_3 - pos_1 - 1);
        if (temp_time.empty())
        {
          continue;
        }
        s = atoi(temp_time.c_str());
        
        temp_time = raw.substr(pos_3 + 1, pos_2 - pos_3 - 1);
        if (temp_time.empty())
        {
          continue;
        }
        ms = atoi(temp_time.c_str());

      }
      else
      {
        temp_time = raw.substr(pos_1 + 1, pos_2 - pos_1 - 1);
        if (temp_time.empty())
        {
          continue;
        }
        s = atoi(temp_time.c_str());
      }

      unsigned t = 1000 * (60 * m + s) + 10 * ms;
      string con = raw.substr(pos_2 + 1);

      m_lrc.insert(make_pair(t, con));
    }
  }

  if (m_lrc.empty())
  {
    return false;
  }

  unsigned temp_key = 0;
  if (m_lrc.end() == m_lrc.find(temp_key))
  {
    m_lrc.insert(make_pair(0, ""));
  }
  
  return true;
}

string igoLrc::trim(const string& str)
{
  string outStr;
  if (!str.empty())
  {
    string::size_type start = str.find_first_not_of(STR_WHITESPACE);

    // If there is only white space we return an empty string
    if (start != string::npos)
    {
      string::size_type end = str.find_last_not_of(STR_WHITESPACE);
      outStr = str.substr(start, end - start + 1);
    }
  }

  return outStr;
}

bool igoLrc::get_lrc(string& lrc_content, int& internal_t, unsigned& start_time)
{
  if (m_lrc.size() <= 1)
  {
    return false;
  }

  map<unsigned, string>::iterator it = m_lrc.begin();
  for (; it != m_lrc.end(); ++it)
  {
    if (it->first > start_time) break;
  }

  if (it != m_lrc.begin()) --it;
  lrc_content = it->second;
  start_time = it->first;

  if (++it != m_lrc.end())
  {
    internal_t = it->first - start_time;
  }
  else
  {
    internal_t = -1;
  }

  return true;
}



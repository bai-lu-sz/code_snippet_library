///////////////////////////////////////////////////////////////////////////
// 
// IGOLRC.H    ÎÄ¼þ×¢ÊÍ
//
// created by bai_lu_
// create time 2014/12/1 15:14
//
// File Description:
// 					        lrc file parse
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <string>

using std::string;
using std::wstring;
using std::map;

typedef map<unsigned, string> lrc_map;

class igoLrc
{
public:
  igoLrc(const wstring lrc_file = L"");
  ~igoLrc(void);

  bool set_lrc_file(const wstring& lrc_file);
  bool set_lrc_content(const string& lrc_content);

  bool get_lrc(string& lrc_content, int& internal_t, unsigned& start_time);

private:
  string trim(const string& str);


private:
  lrc_map   m_lrc;
  bool      is_load_file_ok;
};

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <tchar.h>
#include "igoLrc.h"

int main()
{
  igoLrc lrc;

  wstring lrc_path = L"时代风格.lrc";

  lrc.set_lrc_file(lrc_path);

  string content;
  int internal_t;
  unsigned start_t = 0;

  while (lrc.get_lrc(content, internal_t, start_t))
  {
    printf("[%d]%s\n", start_t, content.c_str());

    if (-1 == internal_t)
    {
      break;
    }

    start_t += internal_t;

    Sleep(internal_t);
  }

  getch();

  return 0;
}
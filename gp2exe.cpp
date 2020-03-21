

#include "stdafx.h"

class GP2EXE
{
  GP2EXE() {}

  void
    FindGP2Exe();
  void
    OpenGP2ForModification(LPCSTR filename);

  FILE *fpexe;
};

void GP2EXE::FindGP2Exe()
{}

void GP2EXE::OpenGP2ForModification(LPCSTR filename)
{}

BOOL isEnglish()
{
  return TRUE;
}

BOOL isGerman()
{
  return FALSE;
}


#include "Jam.h"

#define NON_JAMS 13

class JAMExtraData : public CObject
{
public:
  JAMExtraData(JAM *jam, int listEntry, int subidx, char *filename, int id)
    : jam(jam),
      listEntry(listEntry),
      subidx(subidx),
      filename(filename),
      id(id)
  {}

  JAM *jam;
  int listEntry;
  int subidx;
  char *filename;
  int id;
};

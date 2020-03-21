

typedef struct
{
  char *name;
  int id;
} t_ObjectDict;

static t_ObjectDict ObjectLookUpTable[] = {
  { "bush", IDB_BUSH },
  { "tree", IDB_TREE },
  { "side advert", IDB_SIDE_ADVERT },
  { "advert", IDB_ADVERT },
  { "roof", IDB_STANDROOT },
  { "stand", IDB_STAND },
  { "house", IDB_HOUSE },
  { "building", IDB_BUILDING },
  { "apartment", IDB_BUILDING },
  { "flyover", IDB_OVER_ADVERT },
  { "tower", IDB_BUILDING },
  { "100 metre", IDB_100_SIGN },
  { "200 metre", IDB_200_SIGN },
  { "300 metre", IDB_300_SIGN },
  { "crane", IDB_CRANE },
  { "marshall", IDB_MARSHALL },
  { "post", IDB_MARSHALL },
  { "starting lights", IDB_LIGHTS },
  { "bridge", IDB_BRIDGE },
  { "observation", IDB_OBSERVATION },
  { "shadow", IDB_SHADOW },
  { "tent", IDB_TENT },
  { "yacht", IDB_YACHT },
  { "pillar", IDB_PILLAR },
  { "crowd", IDB_CROWDICON },
  { "score", IDB_SCOREBOARD },
  { "reporter", IDB_REPORTER },
  { "unkown", IDB_OBJECT_UNK },
};

static int SizeOfLookUp = sizeof(ObjectLookUpTable) / sizeof(t_ObjectDict);

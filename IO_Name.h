

class IO_name : public CObject
{
public:
  InternalObject *obj;
  int len;
  char *name;

  IO_name(InternalObject *obj, char *_name) : obj(obj) { name = _strdup(_name); }

  ~IO_name() { free(name); }

  void
    Write(FILE *fp)
  {
    int id = obj->track->InternalObjectOffsets->findElementIndex(obj);
    fwrite(&id, 1, sizeof(int), fp);
    // fwrite(&len,1,sizeof(int),fp);
    fwrite(name, strlen(name) + 1, sizeof(char), fp);
  }

  void
    Read(FILE *fp)
  {}
};

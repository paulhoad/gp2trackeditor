

class ImportDataElement : public CObject
{
public:
  CString str;

  ImportDataElement(LPCSTR buff) { str = CString(buff); }

  CString
    toString()
  {
    return str;
  }

  int toInt()
  {
    int value = atoi((LPSTR)(LPCTSTR)str);
    TRACE("%s = ivalue=%d\n", str, value);
    return value;
  }

  double
    toDouble()
  {
    float value;
    sscanf(str, "%f", &value);
    TRACE("%s = dvalue=%f\n", str, value);
    return (double)value;
  }
};
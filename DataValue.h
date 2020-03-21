
#ifndef _INCLUDED_DATA_VALUE_H_
#define _INCLUDED_DATA_VALUE_H_

class DataValue
{
public:
  DataValue(int value) : value(value) {}

  DataValue(double value) : value(value) {}

  DataValue() { value = 0; }

  void
    SetValue(int val)
  {
    value = val;
  }

  void
    SetValue(double val)
  {
    value = val;
  }

  int GetValue()
  {
    return (int)value;
  }

  double
    GetDoubleValue()
  {
    return value;
  }

  void
    SetDesc(LPCSTR desc)
  {
    descr = CString(desc);
  }

  CString
    GetDesc()
  {
    return descr;
  }

  double value;
  CString descr;
};

#endif
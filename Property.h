

#ifndef _INCLUDE_PROPERTY_H_
#define _INCLUDE_PROPERTY_H_

class PropertyValue
{
    public:

	PropertyValue(CString name,int dataval,CString description):
		  name(name),description(description),dataval(dataval)
	{
	
	}

	CString name;
	int     dataval;
	CString description;
};

class Property
{
	Property()
	{
		properties = new Vector("Property Value",__LINE__);
	}

	virtual ~Property()
	{
	   delete properties;
	}


	void add(PropertyValue *val)
	{
	  properties->addElement(val);
	}

	Vector properties;
};


#endif
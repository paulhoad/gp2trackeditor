
#ifndef _VECTOR_INCLUDED_H_
#define _VECTOR_INCLUDED_H_

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//class Vector;

class VectorIter: public CObject
{
    public:
	VectorIter(CObject *obj,BOOL deleteContents):
    ptr(obj),next(NULL),deleteContents(deleteContents)
	{}

	virtual ~VectorIter()
	{
		if (deleteContents) delete ptr;
	}

	CObject *ptr;
	VectorIter *next;
	VectorIter *prev;
	BOOL   deleteContents;
};



class Vector: public CObject
{
    public:

	Vector(LPCSTR name,int line,BOOL add=TRUE);
	Vector(Vector *vec);

	
	virtual ~Vector();

	void removeAllElements();

	void removeAllData();

    void addElement(CObject *obj);
	void addElementReference(CObject *obj);

	BOOL removeElement(CObject *obj);
	BOOL removeElementRef(CObject *obj);
	
	int size();

	CObject * elementAt(int idx);

	void insertAfterElement(CObject *t,CObject *elm);
	void insertBeforeElement(CObject *t,CObject *elm);

	int findElementIndex(CObject *obj);

	CObject *firstElement();
    CObject *lastElement();
	CObject *previous(CObject *item);
	CObject *next(CObject *item);

	BOOL isFirstElement(CObject *item)
	{
		return (firstElement() == item) ? TRUE : FALSE;
	}

	BOOL isLastElement(CObject *item)
	{
		return (elementAt(size()-1) == item) ? TRUE : FALSE;
	}

	inline VectorIter *Vector::nextIter(VectorIter *selected)
	{
	    return selected->next;
	}

	inline VectorIter * Vector::firstIter()
	{
	return dataPtr;	
	}


	Vector *Copy();

	VectorIter *dataPtr;
	char name[256];
	int line;

	#ifdef _DEBUG
    void  AssertValid() const
    {
	AssertValid();
    }

    void Dump(CDumpContext& dc) const
    {
	TRACE("Dumping %s %d\n",name,line);
	CObject::Dump(dc);
    }
    #endif //_DEBUG

	int ncount;

};

#endif
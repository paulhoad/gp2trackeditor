
#include "stdafx.h"
#include "Vector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define NEW 1

static int VectorsCreated=0;

Vector::Vector(LPCSTR myname,int myline,BOOL add)
{
	   ncount=0;
       dataPtr = NULL;
	   VectorsCreated++;

	   strcpy(name,myname);
	   line = myline;
}

Vector::Vector(Vector *vec)
{
	 ncount=0;
	 VectorsCreated++;
	 int size = vec->size();
	 for(int i=0;i<size;i++)
	 {
		 addElement(vec->elementAt(i));
	 }
}




	
Vector::~Vector()
{
	   ncount = 0;
	   VectorsCreated--;
	   removeAllData();
}



void Vector::removeAllElements()
{
   	   ncount = 0;
       removeAllData();	    
}

void Vector::removeAllData()
{
   	   ncount = 0;
	   if (dataPtr)
	   {
        VectorIter *elm  = dataPtr; 
	    VectorIter *next = elm->next;

	    while (elm!=NULL)
	    {
		  next = elm->next;
		  delete elm;
	      elm = next;
	    }
	    dataPtr = NULL;
	   }
}

void Vector::insertAfterElement(CObject *t,CObject *elmInsert)
{
	    ncount++;
	    if (dataPtr == NULL)
		{
		 VectorIter *elm = new VectorIter(elmInsert,TRUE);
		 dataPtr = elm;
		}
		else
		{
			VectorIter *elm = (VectorIter*)dataPtr; 

			while (elm!=NULL)
			{
				//TRACE("elm=0x%x t=0x%x\n",elm,t);
				if (elm->ptr == t)
				{
					//
					//AfxMessageBox("Found Where to Inset!");
					VectorIter *p = elm->next;
					//VectorIter *q = elm->prev;
					elm->next = new VectorIter(elmInsert,TRUE);
					elm->next->next = p;
					elm->next->prev = elm;
					return;
				}
				if (elm->next == NULL) 
					break;
				elm = elm->next;
			}
			// elm should hold next pointer			
			elm->next = new VectorIter(elmInsert,TRUE);
			elm->next->prev = elm;
		}
}

void Vector::insertBeforeElement(CObject *t,CObject *elmInsert)
{
		ncount++;
	    if (dataPtr == NULL)
		{
		 VectorIter *elm = new VectorIter(elmInsert,TRUE);
		 dataPtr = elm;
		}
		else
		{
			VectorIter *elm = (VectorIter*)dataPtr;
			VectorIter *prev = elm;

			while (elm!=NULL)
			{				
				if (elm->ptr == t)
				{
					VectorIter *p = prev->next;
					prev->next = new VectorIter(elmInsert,TRUE);
					prev->next->next = p;
					return;
				}
				if (elm->next == NULL) 
					break;
				prev = elm;
				elm  = elm->next;
				
			}
			elm->next = new VectorIter(elmInsert,TRUE);
			elm->next->prev = elm;
		}

}

void Vector::addElementReference(CObject *obj)
{
		ncount++;

	    // add only a reference so Vector cannot delete data
	    if (dataPtr == NULL)
		{
		 VectorIter *elm = new VectorIter(obj,FALSE);
		 dataPtr = elm;
		}
		else
		{
			VectorIter *elm = (VectorIter*)dataPtr; 

			while (elm!=NULL)
			{
				if (elm->next == NULL) 
					break;
				elm = elm->next;
			}
			// elm should hold next pointer
			elm->next = new VectorIter(obj,FALSE);
			elm->next->prev = elm;
		}
}

void Vector::addElement(CObject *obj)
{
		ncount++;

	    if (dataPtr == NULL)
		{
		 VectorIter *elm = new VectorIter(obj,TRUE);
		 dataPtr = elm;
		 elm->prev = elm;
		}
		else
		{
			VectorIter *elm = (VectorIter*)dataPtr; 

			while (elm!=NULL)
			{
				if (elm->next == NULL) 
					break;
				elm = elm->next;
			}
			// elm should hold next pointer
			elm->next = new VectorIter(obj,TRUE);
			elm->next->prev = elm;
		}
}

BOOL Vector::removeElement(CObject *obj)
{
	ncount--;
	VectorIter *elm = (VectorIter*)dataPtr;
	VectorIter *next = elm->next;
	VectorIter *last = elm;

	if (elm->ptr == obj)
	{
		dataPtr = next;
		delete elm;
		return TRUE;
	}

	while (elm!=NULL)
	{
	   next = elm->next;

	   if (elm->ptr == obj)
	   {
		  last->next = next;
		  if (next!=NULL) next->prev = last;		  
		  delete elm;
		  return TRUE;
	   }
	   last = elm;
	   elm = next;
	}
	return FALSE;

}

BOOL Vector::removeElementRef(CObject *obj)
{
	ncount--;
    VectorIter *elm = (VectorIter*)dataPtr;
	VectorIter *next = elm->next;
	VectorIter *last = elm;

	if (elm->ptr == obj)
	{
		dataPtr = next;
		//delete elm;
		return TRUE;
	}

	while (elm!=NULL)
	{
	   next = elm->next;

	   if (elm->ptr == obj)
	   {
		  last->next = next;
		  if (next) next->prev = last;		  
		  //delete elm;
		  return TRUE;
	   }
	   last = elm;
	   elm = next;
	}
	return FALSE;
}

int Vector::size()
{
	return ncount;

	/*
	int count = 0;

	if (dataPtr==NULL) 
		return 0;
	else
	{   
	   VectorIter *elm = (VectorIter*)dataPtr;
	   while (elm!=NULL)
	   {
		   count++;
		   if (elm->next == NULL) 
						return count;
		   elm = elm->next;
	   }
	   return count;
	}
	*/
}

CObject * Vector::firstElement()
{
	//return elementAt(0);
	VectorIter *elm = (VectorIter*)dataPtr;
	if (elm)
	 return elm->ptr;
	else
	 return NULL;
}

CObject *Vector::lastElement()
{
	//return elementAt(size()-1);

	VectorIter *elm = (VectorIter*)dataPtr;
	if (elm)
	 return elm->prev->ptr;
	else
	 return NULL;

}

CObject * Vector::elementAt(int idx)
{
	int count=0;
	VectorIter *elm = (VectorIter*)dataPtr;

	while (elm!=NULL)
	{
		   if (count++ == idx) return elm->ptr;
		   //count++;
		   //if ((elm = elm->next) == NULL)  return NULL;
		   elm = elm->next;
	}
	return NULL;
}

int Vector::findElementIndex(CObject *obj)
{
	int count=0;
	VectorIter *elm = (VectorIter*)dataPtr;

	while (elm!=NULL)
	{
		   if (obj == elm->ptr) return count;
		   count++;
		   if ((elm = elm->next) == NULL)  return -1;
	}
	return -1;
}

CObject *Vector::previous(CObject *t)
{
	    VectorIter *prev=NULL;

	    if (dataPtr == NULL)
		{
		  AfxMessageBox("Cannot find previous item Empty List");
		}
		else
		{
			VectorIter *elm = (VectorIter*)dataPtr; 

			while (elm!=NULL)
			{
				if (elm->ptr == t)
				{
					if (prev==NULL) 
						return lastElement();
					return prev->ptr;
				}
				if (elm->next == NULL) 
					break;
				prev = elm;
				elm  = elm->next;

			}
		}
		return NULL;
}

CObject *Vector::next(CObject *t)
{
	    VectorIter *nextitem=NULL;

	    if (dataPtr == NULL)
		{
		  AfxMessageBox("Cannot find next item Empty List");
		}
		else
		{
			VectorIter *elm = (VectorIter*)dataPtr;
			nextitem = elm->next;

			while (elm!=NULL)
			{
				if (elm->ptr == t)
				{
					if (nextitem==NULL) 
						return firstElement();
					return nextitem->ptr;
				}
				//if (elm->next == NULL) 
				//	break;
				elm  = elm->next;
				nextitem = elm->next;
			}
		}
		return NULL;
}


Vector * Vector::Copy()
{
	return new Vector(this);
}


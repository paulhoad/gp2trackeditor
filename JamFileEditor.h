
#ifndef _INCLUDED_JAM_H_
#define _INCLUDED_JAM_H_

#include "Observer.h"
#include "TrackTreeCtrl.h"
#include "Jam.h"


static char lpszJamFilter[] = "F1GP2 Jam Files (*.jam)|*.jam|All Files (*.*)|*.*|";



class JamFileEditor: public Observer, public CObject
{
	  char lpszJamFile[256];
  public:	  
	  //HTREEITEM TreeNode;
	  HTREEITEM SubTree;
	  CTreeCtrl *tree;
	  JAM *JamFileStructure;
	  BOOL Internal;

	  JamFileEditor(GPTrack *track,LPCSTR jam,BOOL Internal=FALSE):
	  Observer(track,t_JamFile,NULL),SubTree(0),Internal(Internal)
	  {
		 JamFileStructure = NULL;
		 Observer::setTreeNode(NULL);
		 setJamFileName((LPSTR)jam);
		 JamFileStructure = new JAM(getFullPathToJamFileName());
		 
	  }

	  virtual ~JamFileEditor()
	  {	
		  delete JamFileStructure;
	  }

	  char * getJamFileName()
	  {
		 return lpszJamFile;
	  }

	  void setJamFileName(char *name)
	  {
		  strcpy(lpszJamFile,name);
		  if (JamFileStructure)
		      JamFileStructure->filename = getFullPathToJamFileName();
	  }

	  char * getFullPathToJamFileName()
	  {
		  static char buffer[512];
		  CString gp2Location = ((CTrackEditorApp*)AfxGetApp())->getGP2Location();

		  wsprintf(buffer,"%s\\%s",gp2Location,getJamFileName());
		  return buffer;
	  }

	  void setTreeNode(CTreeCtrl *listtree,HTREEITEM node)
	  {
		  tree = listtree;
		  Observer::setTreeNode(node);
	  }

	  int Open();

	  void Delete();

	  void Expand(CTrackTree *tracktree,HTREEITEM node)
	  {
		  if (SubTree && tree)
		  {
			  HTREEITEM itemToDelete = SubTree;

			  while(itemToDelete)
			  {
			    HTREEITEM next = tree->GetNextSiblingItem(itemToDelete);
				tree->DeleteItem(itemToDelete);
				itemToDelete = next;
			  }
		  }
		  JamFileStructure->OpenToRead(TRUE);
		  LoadJamTree(tracktree,node);
	  }

	  void LoadJamTree(CTrackTree *tree,HTREEITEM root)
	  {
		  SubTree = JamFileStructure->LoadTree(tree,root,FALSE);
	  }

	  JAM *getJAM()
	  {
		  return JamFileStructure;
	  }

	  BOOL isDefault()
	  {
		  return Internal;
	  }

	  

};

static int numberOfJams(Vector *jams)
{
	int count = 0;
	for(int i=0;i<jams->size();i++)
	{
		JamFileEditor *jam = (JamFileEditor*)jams->elementAt(i);
		if (jam->Internal) continue;
		count++;
	}
	return count;
}

#endif
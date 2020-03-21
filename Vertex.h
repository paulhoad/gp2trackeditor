

#ifndef _VERTEX_INCLUDED_H_
#define _VERTEX_INCLUDED_H_

typedef struct
{
	int pt1;
	int pt2;

} t_SingleVertex;

typedef struct
{
	float r;
	float g;
	float b;
} t_Color;


static t_Color RandomColors[] = {
	{1.0f,0.0f,0.0f},
    {0.0f,1.0f,0.0f},
	{0.0f,0.0f,1.0f},
	{1.0f,1.0f,0.0f},
	{1.0f,0.0f,1.0f},
	{0.0f,1.0f,1.0f},
	{0.5f,0.0f,0.0f},
    {0.0f,0.5f,0.0f},
	{0.0f,0.0f,0.5f},
	{0.5f,0.5f,0.0f},
	{0.5f,0.0f,0.5f},
	{0.0f,0.5f,0.5f},
};

static int numRandomColors = sizeof(RandomColors)/sizeof(RandomColors[0]);




class Vertex: public CObject
{
  public:
	Vertex(int textureid,int u2=-99,int u3=-99,int u4=-99,int u5=-99,int u6=-99):
	textureid(textureid),m_HorizontalRepeat(u2),m_VerticalRepeat(u3),unk4(u4),unk5(u5),unk6(u6)
	{
		points = 0;
		imageLoaded = FALSE;
	}

	BOOL GetImageLoadState()
	{
		return imageLoaded;
	}

	void SetImageLoadState(BOOL state)
	{
		imageLoaded = state;
	}

 private:
	BOOL imageLoaded;

 public:
	

	void addPoint(int p1)
	{
		if (p1 == -32768) return;
		if (points > 9) return;
		pts[points] = p1;
		points ++;
	}

	LPCSTR toString()
	{
		CString msg;

		for(int i=0;i<points;i++)
		{
			CString msgA;
			msgA.Format(" %d",getPoint(i));
			msg += msgA;
		}
		return msg;
	}

	int getPoint(int pt)
	{
		int idx = pts[pt];
		if (idx > 32768) return -(0x10000 - idx);
		return idx;
	}


	int points;
	int textureid;
	int m_HorizontalRepeat;
	int m_VerticalRepeat;
	int unk4;
	int unk5;
	int unk6;

	// center
	double cx,cy,cz;

	// normal
	double nx,ny,nz;

	
	CBitmap bmp;

	private:
	int pts[10];
};


#endif

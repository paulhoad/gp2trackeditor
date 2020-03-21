

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define KERB_PEN 6
#define KERB_LOWER_PEN 7
#define GRASS_BRUSH 8
#define SAND_BRUSH 9
#define ROAD_BRUSH 10
#define RED_BRUSH 11
#define TARMAC_BRUSH 12
#define RULER_PEN 13
#define GRIP_PEN 14
#define GRID_PEN 15
#define GRASS_BITMAP_BRUSH 16
#define SAND_BITMAP_BRUSH 17
#define ROAD_BITMAP_BRUSH 18
#define DRK_ROAD_BITMAP_BRUSH 19
#define DRK_ROAD_BRUSH 20
#define CROWD_BITMAP_BRUSH 21
#define CYAN_BRUSH 22
#define BLUE_PEN 23
#define RED_PEN 24

class Display
{
  CDC *gr;
  CBrush *roadBrush;
  CBrush *drkroadBrush;
  CBrush *grassBrush;
  CBrush *tarmacBrush;
  CBrush *sandBrush;

  CBrush *grassBitmapBrush;
  CBrush *tarmacBitmapBrush;
  CBrush *sandBitmapBrush;
  CBrush *drkroadBitmapBrush;
  CBrush *crowdBitmapBrush;

  CBrush *redBrush;
  CBrush *cyanBrush;
  CPen *ccpen;
  CPen *grassPen;
  CPen *ccselpen;
  CPen *roadselpen;
  CPen *redpen;
  CPen *kerbpen;
  CPen *kerblowerpen;
  CPen *rulerpen;
  // CPen   *gripPen;
  CPen *gridpen;
  CPen *bluePen;
  CPen *redPen;

public:
  Display();

  ~Display()
  {
    delete roadBrush;
    delete tarmacBrush;
    delete grassBrush;
    delete sandBrush;
    delete ccselpen;
    delete ccpen;
    delete roadselpen;
    delete redpen;
    delete kerbpen;
    delete kerblowerpen;
    delete redBrush;
    delete grassPen;
    delete rulerpen;
    // delete gripPen;
    delete gridpen;
    delete drkroadBrush;
    delete grassBitmapBrush;
    delete sandBitmapBrush;
    delete tarmacBitmapBrush;
    delete drkroadBitmapBrush;
    delete crowdBitmapBrush;
    delete cyanBrush;
    delete bluePen;
    delete redPen;
  }

  CDC *
    getGraphics()
  {
    return gr;
  }

  double
    getScale()
  {
    return scale;
  }

  void
    setGraphics(CDC *pDC)
  {
    gr = pDC;
    // gr->SetMapMode(MM_LOENGLISH);
  }

  void
    setColor(int col);

  void
    SelectObject(CPen *pen)
  {
    gr->SelectObject(pen);
  }

  void
    ZoomIn()
  {
    scale = scale * 1.1;
  }

  void
    ZoomOut()
  {
    scale = scale * 0.9;
  }

  void
    ZoomHome()
  {
    scale = 1.0;
  }

  double
    DeviceToLogicalX(double x)
  {
    POINT point;

    point.x = (long)x;
    point.y = (long)x;

    gr->LPtoDP(&point);

    return (double)point.x;
  }

  double
    DeviceToLogicalY(double x)
  {
    POINT point;

    point.x = (long)x;
    point.y = (long)x;

    gr->LPtoDP(&point);

    return (double)point.y;
  }

  long
    getScreenX(double x)
  {
    return (long)((xoffset + (x * scale)));
    // return (int)(DeviceToLogicalY(xoffset+(x*scale)));
  }

  long
    getScreenY(double y)
  {
    return (long)((yoffset + (y * scale)));
    // return (int)(DeviceToLogicalY(yoffset+(y*scale)));
  }

  double
    getObjectX(int sx)
  {
    return ((sx - xoffset) / scale);
  }

  double
    getObjectY(int sx)
  {
    return ((sx - yoffset) / scale);
  }

  void
    drawLine(double x, double y, double x1, double y1)
  {
    gr->MoveTo((int)(xoffset + (x * scale)), (int)(yoffset + (y * scale)));
    gr->LineTo((int)(xoffset + (x1 * scale)), (int)(yoffset + (y1 * scale)));
  }

  void
    drawEllipse(double x, double y, double x1, double y1);
  void
    drawRect(double x, double y, double x1, double y1);
  void
    drawBitmap(int id, double x, double y);

  void
    drawArc(double x, double y, double x1, double y1, double cx, double cy, double angle1, double angle2, double radius);
  void
    drawChord(double x, double y, double x1, double y1, double cx, double cy, double angle1, double angle2, double radius);
  void
    drawArcSpecial(double x, double y, double x1, double y1, double cx, double cy, double angle1, double angle2, double radius1, double radius, BOOL square = FALSE);
  void
    drawPie(double x, double y, double x1, double y1, double cx, double cy, double angle1, double angle2, double radius);

  void
    drawSpiral(double cx, double cy, double angle1, double angle2, double radiusStart, double radiusEnd);

  void
    fillRect(double x, double y, double w, double h)
  {
    CGdiObject *obj = gr->SelectStockObject(BLACK_BRUSH);
    gr->Rectangle((int)x, (int)y, (int)(x + w), (int)(y + h));
    gr->SelectObject(obj);
  }

  void
    drawPolygon(LPPOINT pts, int size)
  {
    // int dc = gr->SaveDC();
    gr->SelectStockObject(NULL_BRUSH);
    gr->SelectStockObject(BLACK_PEN);
    // gr->SelectObject(roadBrush);
    gr->Polygon(pts, size);
    // gr->RestoreDC(dc);
  }

  void
    fillPolygon(LPPOINT pts, int size)
  {
    // gr->SelectObject(roadBrush);
    gr->Polygon(pts, size);
  }

  void
    drawText(double x1, double y1, LPCSTR text)
  {
    int sx = getScreenX(x1);
    int sy = getScreenY(y1);

    // gr->SelectObject(GetStockObject(ANSI_VAR_FONT));
    gr->SelectStockObject(ANSI_VAR_FONT);
    gr->SetBkMode(TRANSPARENT);
    gr->TextOut(sx, sy, text);
  }

  BOOL
    isPrinting()
  {
    return printing;
  }

  void
    beginPrinting()
  {
    printing = TRUE;
  }

  void
    endPrinting()
  {
    printing = FALSE;
  }

  int xoffset;
  int yoffset;
  double scale;

  BOOL printing;
};

#endif

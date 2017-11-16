#include	"arcball.h"
#include	"matrix_3d.h"
#include	"polyptr_3d.h"
#include	"camera_3d.h"

#ifndef VIEW
#define VIEW

#define WIDTH   640
#define HEIGHT  480

class View
{
private:
	// map screen coordinates (dc) to virtual device coordinates (vdc) and back
	point_2d dcToVdc(const POINT& p) const;
	POINT vdcToDc(const point_2d& p) const;

	// methods for dealing with the offscreen buffer
	void createOffscreen();

protected:
	camera cam;
	bool cull;
	Arcball* gui;
	matrix_3d sum;
	matrix_3d transformation;
	matrix_3d inverse;
	matrix_3d viewing;
	real xsize;
	real ysize;
	real aspect;
	point_3d eye;
	POINT pts[128];
	HWND window;
	RECT clientRect;

	HDC offscreenDC;
	HBITMAP offscreenBitmap;
	HBITMAP oldOffscreenBitmap;

public:
	View(HWND window);
	~View(void);
	void moveToPt(const point_2d& pt) const;
	void drawLineToPt(const point_2d& pt, COLORREF strokeColor) const;
	void drawCircle(const point_2d& a, const point_2d& b, COLORREF strokeColor, COLORREF fillColor) const;
	void drawCrossHair(const point_2d& p, COLORREF strokeColor) const;
    void drawPolygon (polyptr poly, COLORREF strokeColor, COLORREF fillColor);
    void drawPolygon (polyptr poly);
    void drawScene(void);
	void handleClick(POINT where);
	void handleDrag(POINT where);

    void eraseOffscreen ();
    void swapOffscreen ();
};

extern View* gView;																													//	the world viewer

#endif // VIEW

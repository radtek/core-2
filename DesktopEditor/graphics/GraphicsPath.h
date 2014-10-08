﻿#ifndef _BUILD_GRAPHICSPATH_H_
#define _BUILD_GRAPHICSPATH_H_

#include "AggPlusEnums.h"
#include "../agg-2.4/include/agg_basics.h"
#include "../agg-2.4/include/agg_path_storage.h"

#include "../agg-2.4/include/agg_rasterizer_scanline_aa.h"
#include "../agg-2.4/include/agg_conv_transform.h"
#include "../agg-2.4/include/agg_conv_stroke.h"
#include "../agg-2.4/include/agg_conv_contour.h"
#include "../agg-2.4/include/agg_conv_unclose_polygon.h"

#include "../agg-2.4/include/agg_bezier_arc.h"
#include "../agg-2.4/include/agg_conv_curve.h"
#include "../agg-2.4/include/agg_renderer_base.h"
#include "../agg-2.4/include/agg_renderer_scanline.h"

#include "IRenderer.h"

#include "Matrix.h"

#include "structures.h"
#include "../fontengine/FontManager.h"

namespace Aggplus
{
class CGraphicsPath : public ISimpleGraphicsPath
{
	friend class CClip;
	friend class CFontManager;
	friend class CGraphics;

public:
	CGraphicsPath();
	~CGraphicsPath();

	CGraphicsPath* Clone();

	Status Reset();
	void SetRuler(bool bEvenOdd);

	Status StartFigure();
	Status CloseFigure();
	bool Is_poly_closed();
	Status MoveTo(double x, double y);
	Status LineTo(double x, double y);
	Status CurveTo(double x1, double y1, double x2, double y2, double x3, double y3);

	Status AddLine(double x1, double y1, double x2, double y2);
	Status AddLines(double* pPoints, int nCount);

	// методы, которые просто будем сводить к трем основным
	Status AddBezier(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	Status AddBeziers(double* pPoints, int nCount);
	Status AddCurve(double* pPoints, int nCount);
	Status AddEllipse(double x, double y, double width, double height);
	Status AddRectangle(double x, double y, double width, double height);
	Status AddPolygon(double* pPoints, int nCount);
	Status AddPath(const CGraphicsPath& oPath);
	Status AddArc(double x, double y, double width, double height, double startAngle, double sweepAngle);
	ULONG GetPointCount() const;
	Status GetPathPoints(PointF* points, int count) const;
	Status GetLastPoint(double& x, double& y);
	Status GetPathPoints(double* points, int count) const;
	void GetBounds(double& left, double& top, double& width, double& height);
	Status Transform(const CMatrix* matrix);
	virtual bool _MoveTo(double x, double y);
	virtual bool _LineTo(double x, double y);
	virtual bool _CurveTo(double x1, double y1, double x2, double y2, double x3, double y3);
	virtual bool _Close();
	Status AddString(const std::wstring& strText, CFontManager* pFont, double x, double y);
	Status AddStringC(const LONG& lText, CFontManager* pFont, double x, double y);
	void z_Stroke(const NSStructures::CPen* Pen);
	void Widen(const NSStructures::CPen* Pen, const CMatrix* matrix, float flatness);
	
	agg::path_storage *z_get_agg_path_storage();

public:
	agg::path_storage m_agg_ps;
	bool m_bEvenOdd;
	bool m_bIsMoveTo;

public:

	int EllipseArc(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection);
	double AngToEllPrm(double fAngle, double fXRad, double fYRad);
	int EllipseArc2(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection);
	int EllipseArc3(double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double *pfXCur, double *pfYCur, INT bClockDirection = FALSE);
	int Ellipse(double fX, double fY, double fXRad, double fYRad);
	Status AddArc2(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
};

class CGraphicsPathSimpleConverter : public ISimpleGraphicsPath
{
	friend class CFontManager;
	
private:
	IRenderer* m_pRenderer;

	bool m_bEvenOdd;
	bool m_bIsMoveTo;
	bool m_bIsClosed;

	agg::path_storage m_agg_ps;

public:
	CGraphicsPathSimpleConverter();
	~CGraphicsPathSimpleConverter();

public:
	void SetRenderer(IRenderer* pRenderer);
	IRenderer* GetRenderer(INT bIsAddref = FALSE);

public:
	bool PathCommandMoveTo(double fX, double fY);
	bool PathCommandLineTo(double fX, double fY);
	bool PathCommandLinesTo(double* pPoints, LONG lCount);
	bool PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3);
	bool PathCommandCurvesTo(double* pData, LONG lCount);
	bool PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
	bool PathCommandClose();
	bool PathCommandEnd();
	bool PathCommandStart();
	bool PathCommandGetCurrentPoint(double* fX, double* fY);
	bool PathCommandText(const std::wstring& bsText, CFontManager* pManager, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset);
	bool PathCommandTextEx(std::wstring& bsText, std::wstring& bsGidText, CFontManager* pManager, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags);
	bool PathCommandGetBounds(double& left, double& top, double& width, double &height);
	
public:

	virtual bool _MoveTo(double x, double y);
	virtual bool _LineTo(double x, double y);
	virtual bool _CurveTo(double x1, double y1, double x2, double y2, double x3, double y3);
	virtual bool _Close();
	bool _Reset();
	bool _Start();

protected:
	bool AddString(const std::wstring& bstrText, CFontManager* pFont, double x, double y);

	int EllipseArc(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection);
	double AngToEllPrm(double fAngle, double fXRad, double fYRad);
	int EllipseArc2(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection);
	int EllipseArc3(double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double *pfXCur, double *pfYCur, INT bClockDirection = FALSE);
	int Ellipse(double fX, double fY, double fXRad, double fYRad);

	bool AddArc(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);

	bool Is_poly_closed();
};
}

#endif // _BUILD_GRAPHICSPATH_H_
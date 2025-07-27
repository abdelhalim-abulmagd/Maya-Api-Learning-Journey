#pragma once

#include <maya/MPxContext.h>
#include <maya/MPointArray.h>

class CostumeSelectContext : public MPxContext
{
public:
	CostumeSelectContext();
	virtual ~CostumeSelectContext() override;

	virtual MStatus	doPress(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;

	virtual MStatus	doDrag(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;

	virtual MStatus	doRelease(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;



private:
	short xStart, yStart;
	bool Mesh, Light;

	void DrawRectangle(short xStart, short yStart, short xEnd, short yEnd, MHWRender::MUIDrawManager& drawMgr);


};
#include <CostumeSelectContext_include/CostumeSelectContext.h>


#include <maya/MUIDrawManager.h>
#include <maya/MPoint.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDagNode.h>


CostumeSelectContext::~CostumeSelectContext()
{ }

CostumeSelectContext::CostumeSelectContext()
{
	setTitleString("CostumeSelectContext");
	setHelpString("<Ctrl to select Meshes Ctrl+Shift to select light>");
}

MStatus	CostumeSelectContext::doPress(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) 
{
	Mesh = false;
	Light = false;

	if (event.isModifierControl())
	{
		if (event.isModifierShift())
		{
			Light = true;
		}
		else
		{
			Mesh = true;
		}

	}

	event.getPosition(xStart, yStart);
	return MS::kSuccess;
}
MStatus	CostumeSelectContext::doDrag(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) 
{
	short xEnd, yEnd;
	event.getPosition(xEnd, yEnd);

	DrawRectangle(xStart, yStart, xEnd, yEnd, drawMgr);

	return MS::kSuccess;

}
MStatus	CostumeSelectContext::doRelease(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
	short xEnd, yEnd;
	event.getPosition(xEnd, yEnd);

	MSelectionList ActiveSelection;
	MGlobal::getActiveSelectionList(ActiveSelection);

	MGlobal::selectFromScreen(xStart, yStart, xEnd, yEnd, MGlobal::kReplaceList);
	
	MSelectionList NewSelection;
	MGlobal::getActiveSelectionList(NewSelection);

	MGlobal::setActiveSelectionList(ActiveSelection);
	

	if (Light || Mesh)
	{
		for (int i = NewSelection.length() - 1; i >= 0; i--)
		{
			MObject CurrentObj;
			NewSelection.getDependNode(i, CurrentObj);

			MFnDagNode DagNodeFn(CurrentObj);
			MObject ChildObj = DagNodeFn.child(0);

			if (!ChildObj.hasFn(MFn::kMesh) && Mesh || !ChildObj.hasFn(MFn::kLight) && Light)
				NewSelection.remove(i);
		}
	}

	MGlobal::selectCommand(NewSelection);


	return MS::kSuccess;
}

void CostumeSelectContext::DrawRectangle(short xStart, short yStart, short xEnd, short yEnd, MHWRender::MUIDrawManager& drawMgr)
{
	drawMgr.beginDrawable();

	drawMgr.line2d(MPoint(xStart, yStart), MPoint(xEnd, yStart));
	drawMgr.line2d(MPoint(xEnd, yStart), MPoint(xEnd, yEnd));
	drawMgr.line2d(MPoint(xEnd, yEnd), MPoint(xStart, yEnd));
	drawMgr.line2d(MPoint(xStart, yEnd), MPoint(xStart, yStart));

	drawMgr.endDrawable();
}
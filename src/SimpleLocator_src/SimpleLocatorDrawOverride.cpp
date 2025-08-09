#include <SimpleLocator_include/SimpleLocatorDrawOverride.h>
#include <SimpleLocator_include/SimpleLocatorUserData.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MHWGeometryUtilities.h>


const MString SimpleLocatorDrawOverride::TypeName = "SimpleLocatorDrawOverride";

SimpleLocatorDrawOverride::~SimpleLocatorDrawOverride()
{
}


SimpleLocatorDrawOverride::SimpleLocatorDrawOverride(const MObject& Obj)
	: MPxDrawOverride(Obj, nullptr, false)
{
	
}

MPxDrawOverride* SimpleLocatorDrawOverride::Creator(const MObject& Obj)
{
	return new SimpleLocatorDrawOverride(Obj);
}

MHWRender::DrawAPI SimpleLocatorDrawOverride::supportedDrawAPIs() const
{
	return MHWRender::kAllDevices;
}


bool SimpleLocatorDrawOverride::hasUIDrawables() const
{
	return true;
}

MUserData* SimpleLocatorDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData)
{
	SimpleLocatorUserData* UserData = dynamic_cast<SimpleLocatorUserData*>(oldData);

	if (!UserData)
		UserData = new SimpleLocatorUserData;

	MObject LocatorObj = objPath.node();
	MFnDependencyNode DependNodeFn(LocatorObj);

	UserData->ShapeIndex = DependNodeFn.findPlug("ShapeIndex", false).asInt();
	
	MHWRender::DisplayStatus DisplayStatus = MGeometryUtilities::displayStatus(objPath);

	if (DisplayStatus == MHWRender::kDormant)
	{
		UserData->Color = MColor(0.0f, 0.1f, 0.0f);
	}
	else
	{
		UserData->Color = MGeometryUtilities::wireframeColor(objPath);
	}


	return UserData;
}

void SimpleLocatorDrawOverride::addUIDrawables(const MDagPath& objPath, MUIDrawManager& drawManager, const MFrameContext& frameContext, const MUserData* data)
{
	const SimpleLocatorUserData* UserData = dynamic_cast<const SimpleLocatorUserData*>(data);

	if (!UserData)
		return;

	drawManager.beginDrawable();

	drawManager.setColor(UserData->Color);

	switch (UserData->ShapeIndex)
	{

	case UserData->Circle:
		drawManager.circle(MPoint(0, 0, 0), MVector(0, 1, 0), 1, false);
		break;

	case UserData->Square:
		drawManager.rect(MPoint(0, 0, 0), MVector(0, 0,1), MVector(0,1, 0), 1,1, false);
		break;

	case UserData->Traingle:
		drawManager.line(MPoint(0, 0, 1), MPoint(-1, 0, -1));
		drawManager.line(MPoint(-1, 0, -1), MPoint(1, 0, -1));
		drawManager.line(MPoint(1, 0, -1), MPoint(0, 0, 1));
		break;
	}

	drawManager.endDrawable();

}



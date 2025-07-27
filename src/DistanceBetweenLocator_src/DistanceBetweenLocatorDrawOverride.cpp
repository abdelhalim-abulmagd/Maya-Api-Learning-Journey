#include <DistanceBetweenLocator_include/DistanceBetweenLocatorDrawOverride.h>
#include <DistanceBetweenLocator_include/DistanceBetweenLocatorUserData.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MRampAttribute.h>

#include <algorithm>

const MString DistanceBetweenLocatorDrawOverride::TypeName = "DistanceBetweenLocatorDrawOverride";

DistanceBetweenLocatorDrawOverride::DistanceBetweenLocatorDrawOverride(const MObject& Obj)
	: MPxDrawOverride(Obj, nullptr, true)
{
}

DistanceBetweenLocatorDrawOverride::~DistanceBetweenLocatorDrawOverride()
{
}


MPxDrawOverride* DistanceBetweenLocatorDrawOverride::Creator(const MObject& Obj)
{
	return new DistanceBetweenLocatorDrawOverride(Obj);
}

MHWRender::DrawAPI DistanceBetweenLocatorDrawOverride::supportedDrawAPIs() const
{
	return MHWRender::kAllDevices;
}
bool DistanceBetweenLocatorDrawOverride::hasUIDrawables() const
{
	return true;
}

bool DistanceBetweenLocatorDrawOverride::refineSelectionPath(const MSelectionInfo& selectInfo, const MRenderItem& hitItem, MDagPath& path, MObject& geomComponents, MSelectionMask& objectMask) 
{
	return false;
}

MUserData* DistanceBetweenLocatorDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData)
{
	DistanceBetweenLocatorUserData* UserData = dynamic_cast<DistanceBetweenLocatorUserData*>(oldData);

	if (!UserData)
		UserData = new DistanceBetweenLocatorUserData;

	MObject LocatorObj = objPath.node();
	MFnDependencyNode DependNodeFn(LocatorObj);

	UserData->Point1 = MPoint(DependNodeFn.findPlug("Point1X", false).asDouble(), DependNodeFn.findPlug("Point1Y", false).asDouble(), DependNodeFn.findPlug("Point1Z", false).asDouble());
	UserData->Point2 = MPoint(DependNodeFn.findPlug("Point2X", false).asDouble(), DependNodeFn.findPlug("Point2Y", false).asDouble(), DependNodeFn.findPlug("Point2Z", false).asDouble());

	UserData->Distance = DependNodeFn.findPlug("Distance", false).asDouble();

	UserData->TextPosition = (UserData->Point1 + UserData->Point2) / 2;

	float MinDistance = DependNodeFn.findPlug("MinDistance", false).asFloat();
	float MaxDistance = DependNodeFn.findPlug("MaxDistance", false).asFloat();

	float Position = 0.0f;

	if (MinDistance < MaxDistance) // this condision i just put it here coz the tut put it - BUT its NOT important and not effecting anything 
	{
		Position = (static_cast<float>(UserData->Distance) - MinDistance) / (MaxDistance - MinDistance);
		Position = std::min(std::max(0.0f, Position), 1.0f);

	}

	MRampAttribute RampAtr(DependNodeFn.findPlug("ColorRamp", false));
	RampAtr.getColorAtPosition(Position, UserData->Color);


	//MRampAttribute CurveAtr(DependNodeFn.findPlug("CurveRamp", false));
	//CurveAtr.getValueAtPosition(Position, Position);
	//UserData->Color = MColor(Position, Position, Position); // coz it will be white or black / 0 or 1

	
	return UserData;

}

void DistanceBetweenLocatorDrawOverride::addUIDrawables(const MDagPath& objPath, MUIDrawManager& drawManager, const MFrameContext& frameContext, const MUserData* data)
{
	const DistanceBetweenLocatorUserData* UserData = dynamic_cast<const DistanceBetweenLocatorUserData*>(data);

	if (!UserData)
		return;

	MString Text = MString() + (static_cast<int>(UserData->Distance * 100) * 0.01); // this math to formate the final string to has 2 dicimal places ex: 5.56 rather 5.564114

	drawManager.beginDrawable();

	drawManager.setFontSize(20);
	drawManager.setLineWidth(2);
	drawManager.setColor(UserData->Color);

	drawManager.text(UserData->TextPosition, Text, MUIDrawManager::kCenter);
	drawManager.line(UserData->Point1, UserData->Point2);

	drawManager.endDrawable();
}



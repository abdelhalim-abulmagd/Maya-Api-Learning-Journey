#include <HelloWorldLocator_include/HelloWorldDrawOverride.h>

const MString HelloWorldDrawOverride::TypeName = "HelloWorldDrawOverride";

HelloWorldDrawOverride::HelloWorldDrawOverride(const MObject& Obj)
	: MPxDrawOverride(Obj, nullptr, false)
{
}

MPxDrawOverride* HelloWorldDrawOverride::Creator(const MObject& Obj)
{
	return new HelloWorldDrawOverride(Obj);
}

MUserData* HelloWorldDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData)
{
	return nullptr;
}

MHWRender::DrawAPI HelloWorldDrawOverride::supportedDrawAPIs() const
{
	return MHWRender::kAllDevices;
}

bool HelloWorldDrawOverride::hasUIDrawables() const
{
	return true;
}
void HelloWorldDrawOverride::addUIDrawables(const MDagPath& objPath, MUIDrawManager& drawManager, const MFrameContext& frameContext, const MUserData* data)
{

	drawManager.beginDrawable();

	drawManager.setFontSize(50);
	drawManager.text2d(MPoint(500, 500), "HelloWorld");

	drawManager.endDrawable();
}
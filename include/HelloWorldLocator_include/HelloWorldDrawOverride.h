#pragma once
 
#include <maya/MPxDrawOverride.h>

class HelloWorldDrawOverride : public MPxDrawOverride
{
public:

	static const MString TypeName;

	static MPxDrawOverride* Creator(const MObject& Obj);

	virtual MHWRender::DrawAPI supportedDrawAPIs() const override;

	virtual MUserData* prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData) override;

	virtual bool hasUIDrawables() const override;
	virtual void addUIDrawables( const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MFrameContext& frameContext, const MUserData* data) override;


private:
	HelloWorldDrawOverride(const MObject& Obj);
};
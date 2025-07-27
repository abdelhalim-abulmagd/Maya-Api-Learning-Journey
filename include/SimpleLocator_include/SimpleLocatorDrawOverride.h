#pragma once

#include <maya/MPxDrawOverride.h>

class SimpleLocatorDrawOverride : public MPxDrawOverride
{
public:

	virtual ~SimpleLocatorDrawOverride() override;

	static const MString TypeName;

	static MPxDrawOverride* Creator(const MObject& Obj);

	virtual MHWRender::DrawAPI supportedDrawAPIs() const override;

	virtual MUserData* prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData)  override ;

	virtual bool hasUIDrawables() const override;
	virtual void addUIDrawables(const MDagPath& objPath, MUIDrawManager& drawManager, const MFrameContext& frameContext, const MUserData* data) override;

private:
	SimpleLocatorDrawOverride(const MObject& Obj);

};
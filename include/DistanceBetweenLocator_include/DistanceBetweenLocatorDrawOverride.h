#pragma once

#include <maya/MPxDrawOverride.h>
class DistanceBetweenLocatorDrawOverride : public MPxDrawOverride
{
public:
	virtual ~DistanceBetweenLocatorDrawOverride() override;

	static const MString TypeName;

	static MPxDrawOverride* Creator(const MObject& Obj);

	virtual MHWRender::DrawAPI supportedDrawAPIs() const override;
	virtual bool hasUIDrawables() const override;

	virtual MUserData* prepareForDraw( const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData)  override;

	virtual void addUIDrawables( const MDagPath& objPath, MUIDrawManager& drawManager, const MFrameContext& frameContext, const MUserData* data) override;

	virtual bool refineSelectionPath(const MSelectionInfo& selectInfo, const MRenderItem& hitItem, MDagPath& path, MObject& geomComponents, MSelectionMask& objectMask) override;





private:
	DistanceBetweenLocatorDrawOverride(const MObject& Obj);


};
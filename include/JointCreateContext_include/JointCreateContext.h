#pragma once

#include <maya/MPxContext.h>
#include <maya/MSelectionList.h>

class JointCreateContext : public MPxContext
{
public:
	virtual ~JointCreateContext() override;

	virtual void toolOnSetup(MEvent& event);

	virtual MStatus doRelease(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;

	virtual void completeAction() override;
	virtual void deleteAction() override;
	virtual void abortAction() override;

private:

	void ChangeHelpText();
	
	MSelectionList Selection;

};
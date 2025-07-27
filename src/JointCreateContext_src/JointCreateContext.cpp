#include <JointCreateContext_include/JointCreateContext.h>

#include <maya/MGlobal.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnTransform.h>
#include <maya/MVector.h>

static const MString HelpTexts[] ={ "Select First Object",
	"Select Second Object",
	"Select Third Object",
	"Press Enter"
};


JointCreateContext::~JointCreateContext()
{ }

void JointCreateContext::toolOnSetup(MEvent& event)
{
	setTitleString("JointCreateContext");
	MGlobal::selectCommand(MSelectionList());

	ChangeHelpText();
	
}

MStatus JointCreateContext::doRelease(MEvent& event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{

	if(Selection.length() == 3)
		return MS::kSuccess;

	short x, y;
	event.getPosition(x, y); 
	MGlobal::selectFromScreen(x, y);
	
	MSelectionList ActiveSelection;
	MGlobal::getActiveSelectionList(ActiveSelection);
	
	Selection.merge(ActiveSelection);

	ChangeHelpText();

	return MS::kSuccess;
}
void JointCreateContext::completeAction()
{
	if (Selection.length() < 3)
		return;

	MGlobal::setActiveSelectionList(MSelectionList());
	MObject Obj;

	for (unsigned int i = 0; i < Selection.length(); i++)
	{
		Selection.getDependNode(i, Obj);
		MVector Translate = MFnTransform(Obj).getTranslation(MSpace::kTransform);

		MGlobal::executeCommand(MString("joint -p ") + Translate.x + " " + Translate.y + " " + Translate[2], true, true);
		MGlobal::executeCommand(MString("delete ") + MFnDependencyNode(Obj).name() , true, true);
	}
	//MGlobal::executeCommand("select -clear", true, true);
	MGlobal::selectCommand(MSelectionList());

	abortAction();
}
void JointCreateContext::deleteAction()
{
	if (Selection.isEmpty())
		return;

	Selection.remove(Selection.length() - 1);

	MGlobal::setActiveSelectionList(Selection);
	ChangeHelpText();

}
void JointCreateContext::abortAction()
{

	MGlobal::setActiveSelectionList(MSelectionList());
	Selection.clear();

	ChangeHelpText();

}

void JointCreateContext::ChangeHelpText()
{
	setHelpString(HelpTexts[Selection.length()]);
}
#include <HelloWorldCmd_include/HelloWorldCmd.h>
#include <maya/MTypeId.h>

#include <maya/MGlobal.h>
#include <maya/MIOStream.h>


const MString HelloWorldCmd::TypeName = "HelloWorldCmd";


void* HelloWorldCmd::Creator()
{
	return new HelloWorldCmd;
}


MStatus HelloWorldCmd::doIt(const MArgList& ArgList)
{
	MGlobal::displayInfo("Hello World!");
	
	// this should be print to maya output window but it doesn't IDK ?
	// #include <maya/MIOStream.h>
	cout << "Hello World!" << endl;


	/*
	
	#include <maya/MItDag.h>
	#include <maya/MSelectionList.h>

	// Travese The Dag
	*
	* 
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MObject obj;
	selection.getDependNode(0, obj);

	MItDag::TraversalType TraversalType = MItDag::kDepthFirst;
	MFn::Type Type = MFn::kTransform;

	MItDag DagItr(TraversalType, Type);

	if(!selection.isEmpty())
		DagItr.reset(obj, TraversalType, Type);

	while (!DagItr.isDone())
	{
		MGlobal::displayInfo(DagItr.partialPathName());

		DagItr.next();
	}
	*/

	/*
	#include <maya/MItDag.h>
	#include <maya/MSelectionList.h>
	#include <maya/MFnDagNode.h>

	// Travese The Dag
	*
	* 
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MObject obj;
	selection.getDependNode(0, obj);


	MFnDagNode DagFn(obj);

	for (size_t i = 0; i < DagFn.childCount(); i++)
	{
		MFnDagNode ChildFn(DagFn.child(i));
		MGlobal::displayInfo(ChildFn.partialPathName());
	}
	for (size_t i = 0; i < DagFn.parentCount(); i++)
	{
		MFnDagNode ParentFn(DagFn.parent(i));
		MGlobal::displayInfo(ParentFn.partialPathName());
	}
	*/


	return MS::kSuccess;
}

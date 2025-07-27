#pragma once

#include <maya/MPxCommand.h>
#include <maya/MObject.h>

class SimpleCmd : public MPxCommand
{
public:
	SimpleCmd();
	virtual ~SimpleCmd() override;

	static const MString CommandName;

	static void* Creator();
	static MSyntax CreateSyntax();

	MStatus doIt(const MArgList& ArgList) override;

	virtual MStatus undoIt() override;
	virtual MStatus redoIt() override;
	virtual bool isUndoable() const override;

	bool IsUndoable;

	bool IsTranslate;
	bool IsVersion;

	bool IsQuery;
	bool IsEdit;

	MObject SelectedObj;

	double NewTranslate[3];
	double OldTranslate[3];
};
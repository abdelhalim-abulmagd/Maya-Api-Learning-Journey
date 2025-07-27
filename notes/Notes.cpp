/*
general notes:
    undo & redo in MPxCommand only

    rotation in maya is in radians

    buil environment:
        windows:
            use any OS like windows 10 or 11
            use specific version of visual studio 2019 or 2022
        mac:
            use any OS & any compiler version

    (multi) word in maya means its array (stored in array)

maya types:
    MObject:
        is type less and can be reperesent any type of object in maya
        for ex: camera, mesh, light, even can be used to represent a plugin, etc..
    
    MFn:: 
        is a function set that is used to manipulate MObject
        for ex: MFnMesh, MFnCamera, MFnLight, etc..
        it is used to create, read, and write data to the MObject
    
    wrapper:
        is a class that provide helper classes
        for ex: itterators, smart pointers, etc..
    
    MPx:
        is a base class for extending maya 
        it is used to create custom nodes, commands, and draw overrides in maya
    
dependency graph aka (DG) directed acyclic graph:
    use connections to traverse
    its composed of : nodes and connections
    nodes are MObject and connections are MPlug
    the attributes of the nodes is simply for storing data
    and the way to access and manipulate the attributes is through MPlug

maya scene graph aka (DAG) Directed Acyclic Graph:
    use parent-child hierarchy to traverse 
    it is composed of : transforms and shapes
    nodes will have only one parent 

MPxCommand:
    doIt():
        function is used for one-time initialization
        the acual logic need to be implemented in the redoIt() function

    CreateSyntax():
        setObjectsType() if you want pass string object as args to the command
        useSelectionAsDefault() is used to use the current selection as the default value for the command if no arguments are passed by the setObjectsType()

        if you specify min and max objects that can pass to the command, check the status for getObjects() function
        no need to check the length of the MSelectionList


MPxNode:
    TypeName is used in maya ASCI and TypeId is used in binary

    any output attribute should be set to keyable that will not make it run properly

    compute():
        there is problem if i reseve from MDataBlock point as MVector
        that mess the calculation IDK why
        so reseve the point as FloatVector 

MPxContext:
    selection in viewport doesn't referesh until the mouse button is released so use doRelease() always

    don't draw text in drawFeadback() use doPtrMove() instead 
MPxLocator:
    is DAG node and this node is Shape node not Transform node and this class use to define (draw) the geometry of this shape
    the object pramter in the constructor is refer to the objcet that the override draw

    to create icon for locator node:
        just rename the icon to:
            for node editor: <TypeName>.png - and the size 40x40
            for outliner: <out_><TypeName>.png and the size 20x20

MDagModifier:
    there is problem if you wanna connect two nodes one of them created by MDagModifier
    that connection will fail
    to make connection work you need to parent the node you created by MDagModifier to transform node first

MGlobal:
    executeCommand():
        if you wanna retun value the command need to be one line for ex: 2+2
        to able to return value from multi-line command use functions like:
            MGlobal::executePythonCommand("import my_module; my_module.my_function()")

MPxDrawOverride
    the nullptr argement in the constructor means that you will not provide a custom draw function
    and you will use the default draw function provided by mpxDrawOverride like addUiDrawables()


Callbacks :
    Order of execution not guaranteed
    Base class for callbacks is MMessage -> this used to remove the callback using (ID)
    to register a callback, use addCallback() in message classes which inherit from MMessage
    MEventMessage for general events. all events name and description in (scriptJob) page at (listEvents(le)) section
    NOTE : any function that you will send to the Callback need to have one prameter that (void* ClintData)
*/






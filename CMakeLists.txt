cmake_minimum_required(VERSION 3.10)

project(MayaApiLearningJourney)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(${PROJECT_NAME} SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PluginMain.cpp

    # ------------------- Z1 -------------------
    # HelloWorldCmd 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/HelloWorldCmd_include/HelloWorldCmd.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/HelloWorldCmd_src/HelloWorldCmd.cpp

    # HelloWorldLocator
        ${CMAKE_CURRENT_SOURCE_DIR}/include/HelloWorldLocator_include/HelloWorldLocator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/HelloWorldLocator_src/HelloWorldLocator.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/HelloWorldLocator_include/HelloWorldDrawOverride.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/HelloWorldLocator_src/HelloWorldDrawOverride.cpp

    # MultiplyNode
        ${CMAKE_CURRENT_SOURCE_DIR}/include/MultiplyNode_include/MultiplyNode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/MultiplyNode_src/MultiplyNode.cpp

    # RollingNode
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RollingNode_include/RollingNode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/RollingNode_src/RollingNode.cpp

    # SimpleCmd
        ${CMAKE_CURRENT_SOURCE_DIR}/include/SimpleCmd_include/SimpleCmd.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SimpleCmd_src/SimpleCmd.cpp

    # ------------------- Z2 -------------------
    # BasicDeformer
        ${CMAKE_CURRENT_SOURCE_DIR}/include/BasicDeformer_include/BasicDeformer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicDeformer_src/BasicDeformer.cpp

    # BlendShapeDeformer
        ${CMAKE_CURRENT_SOURCE_DIR}/include/BlendShapeDeformer_include/BlendShapeDeformer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/BlendShapeDeformer_src/BlendShapeDeformer.cpp

    # AttractDeformer
        ${CMAKE_CURRENT_SOURCE_DIR}/include/AttractDeformer_include/AttractDeformer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AttractDeformer_src/AttractDeformer.cpp

    # CostumeSelectContext
        ${CMAKE_CURRENT_SOURCE_DIR}/include/CostumeSelectContext_include/CostumeSelectContext.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/CostumeSelectContext_src/CostumeSelectContext.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/CostumeSelectContext_include/CostumeSelectContextCommand.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/CostumeSelectContext_src/CostumeSelectContextCommand.cpp

    # JointCreateContext
        ${CMAKE_CURRENT_SOURCE_DIR}/include/JointCreateContext_include/JointCreateContext.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/JointCreateContext_src/JointCreateContext.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/JointCreateContext_include/JointCreateContextCommand.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/JointCreateContext_src/JointCreateContextCommand.cpp
 
    # ------------------- Z3 -------------------
    # SimpleLocator
        ${CMAKE_CURRENT_SOURCE_DIR}/include/SimpleLocator_include/SimpleLocator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SimpleLocator_src/SimpleLocator.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/SimpleLocator_include/SimpleLocatorDrawOverride.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SimpleLocator_src/SimpleLocatorDrawOverride.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/SimpleLocator_include/SimpleLocatorUserData.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SimpleLocator_src/SimpleLocatorUserData.cpp

    # DistanceBetweenLocator
        ${CMAKE_CURRENT_SOURCE_DIR}/include/DistanceBetweenLocator_include/DistanceBetweenLocator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DistanceBetweenLocator_src/DistanceBetweenLocator.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/DistanceBetweenLocator_include/DistanceBetweenLocatorDrawOverride.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DistanceBetweenLocator_src/DistanceBetweenLocatorDrawOverride.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/DistanceBetweenLocator_include/DistanceBetweenLocatorUserData.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DistanceBetweenLocator_src/DistanceBetweenLocatorUserData.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/DistanceBetweenLocator_include/DistanceBetweenLocatorCommand.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DistanceBetweenLocator_src/DistanceBetweenLocatorCommand.cpp

    # ------------------- Z4 -------------------
    # AnimCurveCmd
        ${CMAKE_CURRENT_SOURCE_DIR}/include/AnimCurve_include/AnimCurveCmd.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AnimCurve_src/AnimCurveCmd.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/AnimCurve_include/AnimUtil.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AnimCurve_src/AnimUtil.cpp

    # RetimingTool
        ${CMAKE_CURRENT_SOURCE_DIR}/include/RetimingTool_include/RetimingToolCmd.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/RetimingTool_src/RetimingToolCmd.cpp


)

# additional include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    "C:/Program Files/Autodesk/Maya2024/include"
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# additional library directories
target_link_directories(${PROJECT_NAME} PRIVATE
    "C:/Program Files/Autodesk/Maya2024/lib"
)

# additional dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE
    Foundation.lib
    OpenMaya.lib
    OpenMayaAnim.lib
    OpenMayaUI.lib
    OpenMayaRender.lib
)


target_link_options(${PROJECT_NAME} PRIVATE
    "/export:initializePlugin"
    "/export:uninitializePlugin"
)

# preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    WIN32
    UNICODE
    $<$<CONFIG:Release>:NDEBUG>
    $<$<CONFIG:Debug>:_DEBUG>
)

# set the output suffix for the plugin
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".mll")

# set the generator toolset and system version
set(CMAKE_GENERATOR_TOOLSET v143)

set(CMAKE_SYSTEM_VERSION 10.0)

# enable multi-processor compilation
target_compile_options(${PROJECT_NAME} PRIVATE /MP)


# build events 
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    # Unload the plugin from Maya before building
    # to ensure that the latest version is loaded
    COMMAND python ${CMAKE_SOURCE_DIR}/scripts/BeforeBuild.py 
    COMMENT "Unloading plugin from Maya"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD

    # Copy the plugin to the bin directory
    # and load it into Maya
    COMMAND ${CMAKE_COMMAND} -E copy 
        $<TARGET_FILE:${PROJECT_NAME}> 
        "$ENV{USERPROFILE}/Documents/maya/2024/plug-ins/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
    
    COMMAND python ${CMAKE_SOURCE_DIR}/scripts/AfterBuild.py 
    
    COMMENT "Copying plugin to bin directory And Loading plugin into Maya"
)



cmake_minimum_required(VERSION 3.15)
project(TrackEditor LANGUAGES CXX)

# Require MSVC (MFC is only available with MSVC)
if(NOT MSVC)
    message(FATAL_ERROR "This project requires MSVC to build (MFC dependency).")
endif()

# Use static MFC
set(CMAKE_MFC_FLAG 1)

# Source files (matching the vcxproj)
set(SOURCES
    BankDlg.cpp
    BMP.CPP
    Camera.cpp
    CameraEdit.cpp
    CameraTool.cpp
    CameraView.cpp
    CCCarSetup.cpp
    CCDataLog.cxx
    CCLineSection.cpp
    CCSetupEdit.cpp
    CDIB.CPP
    ChildFrm.cpp
    CLineEditor.cpp
    CmdEditor.cpp
    CmdList.cpp
    CmdRemoval.cpp
    CommandStats.cpp
    Credit.cpp
    CScale.cpp
    DataEditor.cpp
    DataEntry.cpp
    dibkernel.cpp
    DirivingLine.cpp
    Display.cpp
    FileVersion.cpp
    FindCmd.cpp
    GeneralDataEnry.cpp
    gp2ccline.cpp
    gp2exe.cpp
    gp2geom.cpp
    GPPitDraw.cpp
    GPTrack.cpp
    GPTrackDraw.cpp
    HyperLinkButton.cpp
    Info.cpp
    InPlaceEdit.cpp
    InternalObject.cpp
    InternalView.cpp
    Jam.cpp
    JamFileEditor.cpp
    JamList.cpp
    JamView.cpp
    KerbEditor.cpp
    MainFrm.cpp
    MoveTo.cpp
    MyListCtrl.cpp
    MyMdiChildWnd.cpp
    MyTableView.cpp
    ObjectEdit.cpp
    ObjectInfo.cpp
    ObjectName.cpp
    ObjectPos.cpp
    ObjectProperties.cpp
    ObjectView.cpp
    OpenGLView.cpp
    PaletteChooser.cpp
    PaletteCtrl.cpp
    PitEntrance.cpp
    PitInfo.cpp
    PitStopStratergy.cpp
    Point3D.cpp
    RandomObject.cpp
    Register.cpp
    Ruler.cpp
    Sanity.cpp
    ScaleData.cpp
    SceneryCmd.cpp
    SceneryEditor.cpp
    SceneryOnOff.cpp
    SIZECBAR.CPP
    Splash.cpp
    StdAfx.cpp
    TextureChooser.cpp
    TipDlg.cpp
    Tool.cpp
    ToolBarUtils.cpp
    ToolTipWnd.cpp
    TrackCmd.cpp
    TrackEditor.cpp
    TrackEditorDoc.cpp
    TrackEditorScrollView.cpp
    TrackInfo.cpp
    TrackNameInfo.cpp
    TrackObject.cpp
    TrackObjectDefinition.cpp
    TrackOpenGLSides.cpp
    TrackOpenGLView.cpp
    TrackPropertySheet.cpp
    TrackSection.cpp
    TrackSectionSides.cpp
    TrackSelection.cpp
    TrackStats.cpp
    TrackTree.cpp
    TrackTreeCtrl.cpp
    TrackWidth.cpp
    TrChangeDlg.cpp
    Vector.cpp
    VertexEditor.cpp
    ViewSections.cpp
    WorldView.cpp
)

set(HEADERS
    AngleBox.h
    BankDlg.h
    BMP.H
    buildnumber.h
    Camera.h
    CameraEdit.h
    CameraView.h
    CCCarSetup.h
    CCLineSection.h
    CCSetupEdit.h
    CDIB.H
    ChildFrm.h
    CLineEditor.h
    CmdEditor.h
    CmdList.h
    CmdRemoval.h
    CMemDC.h
    CommandStats.h
    Credit.h
    CScale.h
    DataChangeObserver.h
    DataEditor.h
    DataEntry.h
    DataValue.h
    dibkernel.h
    DirivingLine.h
    Display.h
    FileVersion.h
    FindCmd.h
    GeneralDataEnry.h
    GPTrack.h
    htmlhelp.h
    IMAGEDIT.h
    ImportDataElement.h
    Info.h
    InPlaceEdit.h
    InternalObject.h
    InternalObjectCmd.h
    InternalView.h
    IO_Name.h
    Jam.h
    JamFileEditor.h
    JamList.h
    JamPlus.h
    JamView.h
    KerbEditor.h
    MagicNumber.h
    MainFrm.h
    MainToolbar.h
    MoveTo.h
    MyListCtrl.h
    MyMdiChildWnd.h
    MyTableView.h
    ObjectEdit.h
    ObjectInfo.h
    ObjectLookup.h
    ObjectName.h
    ObjectPos.h
    ObjectProperties.h
    ObjectView.h
    Observer.h
    OpenGLView.h
    PaletteChooser.h
    PaletteCtrl.h
    Pallette.h
    PitEntrance.h
    PitInfo.h
    PitStopStratergy.h
    Point3D.h
    Polygon.h
    PolygonOld.h
    Property.h
    RandomObject.h
    Register.h
    resource.h
    RgbPal.h
    Ruler.h
    Sanity.h
    ScaleData.h
    SceneryCmd.h
    SceneryEditor.h
    SceneryOnOff.h
    SingleVarObserver.h
    SIZECBAR.H
    Splash.h
    StdAfx.h
    SubObject.h
    Texture.h
    TextureChooser.h
    TipDlg.h
    Tool.h
    ToolBarUtils.h
    ToolTipWnd.h
    TrackCmd.h
    TrackEditor.h
    TrackEditorDoc.h
    TrackEditorScrollView.h
    TrackEditorView.h
    TrackInfo.h
    TrackNameInfo.h
    TrackObject.h
    TrackObjectDefinition.h
    TrackOpenGLView.h
    TrackPropertySheet.h
    TrackSection.h
    TrackSelection.h
    TrackStats.h
    TrackTableView.h
    TrackTree.h
    tracktreectrl.h
    TrackView.h
    TrackWidth.h
    TrChangeDlg.h
    Vector.h
    Vertex.h
    VertexEditor.h
    ViewSections.h
    vml.h
    WorldView.h
    ZoomFactor.h
)

set(RESOURCES
    TrackEditor.rc
)

add_executable(TrackEditor WIN32 ${SOURCES} ${HEADERS} ${RESOURCES})

# MultiByte character set (not Unicode)
target_compile_definitions(TrackEditor PRIVATE
    WIN32
    _WINDOWS
    USE_OPEN_GL
    _CRT_SECURE_NO_WARNINGS
)

# Debug-specific definitions
target_compile_definitions(TrackEditor PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# Precompiled header
target_precompile_headers(TrackEditor PRIVATE StdAfx.h)

# Include current directory for header resolution
target_include_directories(TrackEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link OpenGL libraries
target_link_libraries(TrackEditor PRIVATE
    opengl32
    glu32
)

# MSVC-specific settings
target_compile_options(TrackEditor PRIVATE
    /W3           # Warning level 3
    /wd4244       # Conversion from larger to smaller type (e.g. INT_PTR to int)
    /wd4267       # Conversion from size_t to smaller type
    /wd4101       # Unreferenced local variable
    /wd4552       # Result of expression not used
    /wd4789       # Buffer overrun warning in legacy code
    /wd5208       # Unnamed class in typedef
    /wd4778       # Unterminated format string
    /wd4474       # Too many arguments for format string
)

# /MP (multi-processor compilation) benefits Visual Studio only;
# Ninja already parallelizes at the file level.
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    target_compile_options(TrackEditor PRIVATE /MP)
endif()

# Runtime library: debug uses MultiThreadedDebug, release uses MultiThreaded
set_target_properties(TrackEditor PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# Set the Windows subsystem
set_target_properties(TrackEditor PROPERTIES
    WIN32_EXECUTABLE TRUE
)

ProjectNameCaps = 'OVR'
ProjectName = 'Ovr'
ProjectLocation = ProjectName .. '/'
workspace(ProjectName)
architecture('x64')
startproject(ProjectName)
OutputDir = '%{cfg.buildcfg}'
Out = 'Bin/$(Configuration)/'
IncludeDir = {}
CppVersion = 'C++20'
MsvcToolset = 'v142'
WindowsSdkVersion = '10.0'
Deps = 'Vendors/'
DepsOut = 'Bin/$(Configuration)/Deps'
function DeclareConfigs()
    configurations({ 'Debug', 'Release' })
end
DeclareConfigs()
function DeclareMSVCOptions()
    filter('system:windows')
    characterset('MBCS')
    staticruntime('Off')
    systemversion(WindowsSdkVersion)
    toolset(MsvcToolset)
    cppdialect(CppVersion)
    defines({ '_CRT_SECURE_NO_WARNINGS', 'NOMINMAX', 'WIN32_LEAN_AND_MEAN', '_WIN32_WINNT=0x601', '_HAS_STD_BYTE=0' })
    disablewarnings({ '4100', '4201', '4307' })
end
function DeclareFlags(main)
    DeclareConfigs()
    DeclareMSVCOptions()
    flags({ 'NoImportLib', 'Maps' })
    filter('configurations:Debug')
        optimize('Off')
    filter('configurations:Release')
        optimize('Speed')
    filter {}
    flags({ 'LinkTimeOptimization', 'MultiProcessorCompile' })
    filter('configurations:Release')
        flags({ 'LinkTimeOptimization', 'NoManifest', 'MultiProcessorCompile' })
    filter {}
end
function DeclareProject(name, loc, lang, out, libType, pch, pchSource, fileList, includeDirs, libDirs, linkedFiles, forcedIncludes)
    project(name)
    location(loc)
    kind(libType)
    language(lang)  
    targetdir(out .. '/')
    objdir(out .. '/Int/')
    if not pch == '' then
        pchheader(pch)
    end
    if not pchSource == '' then
        pchheader(pchSource)
    end
    files(fileList)
    includedirs(includeDirs)
    libdirs(libDirs)
    links(linkedFiles)
    forceincludes(forcedIncludes)
    DeclareFlags(name == ProjectName)
end
SrcFolder = '%{prj.name}/src/'
DepsFolder = Deps .. '%{prj.name}/'
--Vendors (These need to be first, as it goes in order of compliation)
DeclareProject('directx', Deps .. 'directx/', 'C++', DepsOut, 'StaticLib', '', '', { DepsFolder .. '**.h', DepsFolder .. 'shaders/**.inc', DepsFolder .. '**.hpp', DepsFolder .. '**.cpp' }, {}, {}, {})
DeclareProject('imgui', Deps .. 'imgui/', 'C++', DepsOut, 'StaticLib', '', '', { DepsFolder .. '*.h', DepsFolder .. '*.cpp', DepsFolder .. 'backends/imgui_impl_dx11.cpp', DepsFolder .. 'backends/imgui_impl_win32.cpp' }, { DepsFolder }, {}, {})
DeclareProject('minhook', Deps .. 'minhook/', 'C', DepsOut, 'StaticLib', '', '', { DepsFolder .. '**.h', DepsFolder .. '**.c' }, {}, {}, {})
DeclareProject('stackwalker', Deps .. 'stackwalker/', 'C++', DepsOut, 'StaticLib', '', '', { DepsFolder .. 'include/**.h', DepsFolder .. '**.h', DepsFolder .. '**.cpp' }, {}, {}, {})
DeclareProject('zycore', Deps .. 'zycore/', 'C', DepsOut, 'StaticLib', '', '', { DepsFolder .. '**.h', DepsFolder .. '**.h', DepsFolder .. '**.c' }, { Deps, Deps .. 'zycore' }, { DepsOut }, {}, {})
DeclareProject('zydis', Deps .. 'zydis/', 'C', DepsOut, 'StaticLib', '', '', { DepsFolder .. '**.h', DepsFolder .. '**.h', DepsFolder .. '**.c' }, { Deps, Deps .. 'zycore' }, { DepsOut }, { 'zycore.lib' }, {})
--Core
DeclareProject(ProjectName, ProjectLocation, 'C++', Out, 'SharedLib', 'pch.h', 'src/pch/pch.h', { SrcFolder .. '**.h', SrcFolder .. '**.cpp' }, { SrcFolder, Deps, Deps .. 'imgui/backends/', Deps .. 'imgui/' },  { DepsOut }, { 'd3dcompiler.lib', 'd3d11.lib', 'dxgi.lib', 'winmm.lib', 'ws2_32.lib', 'wldap32.lib', 'crypt32.lib', 'imgui.lib', 'libcurl.lib', 'directx.lib', 'minhook.lib', 'stackwalker.lib', 'zycore.lib', 'zydis.lib' }, {})
DeclareProject('ScriptHookV', 'ScriptHookV/', 'C++', Out .. '/ScriptHookV', 'SharedLib', 'pch.h', 'src/pch/pch.h', { SrcFolder .. '**.h', SrcFolder .. '**.cpp' }, { SrcFolder }, {}, {})
DeclareProject('DumpCommandLookup', 'DumpCommandLookup/', 'C++', Out .. '/DumpCommandLookup', 'SharedLib', '', '', { SrcFolder .. '**.h', SrcFolder .. '**.cpp' }, { SrcFolder }, {}, {})
DeclareProject('SEAutoUpdater', 'SEAutoUpdater/', 'C++', Out .. '/SEAutoUpdater', 'ConsoleApp', '', '', { SrcFolder .. '**.h', SrcFolder .. '**.cpp' }, { SrcFolder }, {}, {})
DeclareProject('Loader', 'Loader/', 'C++', Out .. '/Loader', 'ConsoleApp', 'Pch.hpp', 'src/Pch.hpp', { SrcFolder .. '**.hpp', SrcFolder .. '**.cpp' }, { SrcFolder, Deps, Deps .. 'imgui/backends/', Deps .. 'imgui/'  }, { DepsOut }, { 'd3dcompiler.lib', 'd3d11.lib', 'dxgi.lib', 'imgui.lib' }, {})
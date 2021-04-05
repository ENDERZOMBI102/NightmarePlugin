
cmake_minimum_required(VERSION 3.17)
project(NightmarePlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(
    NightmarePlugin
    SHARED
    # main sources
    src/CelesteMoveset.cpp
	src/CelesteMoveset.hpp
	src/Cheats.cpp
	src/Cheats.hpp
	src/Command.cpp
	src/Command.hpp
	src/Game.cpp
	src/Game.hpp
	src/Interface.cpp
	src/Interface.hpp
	src/NightmarePlugin.cpp
	src/NightmarePlugin.hpp
	src/Offsets.cpp
	src/Offsets.hpp
	src/Plugin.cpp
	src/Plugin.hpp
	src/Portal2.cpp
	src/Portal2.hpp
	src/Utils.hpp
	src/Variable.cpp
	src/Variable.hpp
    # Hud
    src/Hud/Hud.cpp
	src/Hud/Hud.hpp
    # Modules
    src/Modules/Client.cpp
	src/Modules/Client.hpp
	src/Modules/Console.cpp
	src/Modules/Console.hpp
	src/Modules/Engine.cpp
	src/Modules/Engine.hpp
	src/Modules/Module.cpp
	src/Modules/Module.hpp
	src/Modules/Server.cpp
	src/Modules/Server.hpp
	src/Modules/Surface.cpp
	src/Modules/Surface.hpp
	src/Modules/Tier1.cpp
	src/Modules/Tier1.hpp
	src/Modules/VGui.cpp
	src/Modules/VGui.hpp
	src/Modules/VScript.cpp
	src/Modules/VScript.hpp
    # Utils
    src/Utils/Math.hpp
	src/Utils/Memory.cpp
	src/Utils/Memory.hpp
	src/Utils/Platform.hpp
	src/Utils/SDK.cpp
	src/Utils/SDK.hpp
)

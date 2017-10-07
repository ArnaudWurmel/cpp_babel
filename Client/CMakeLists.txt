cmake_minimum_required(VERSION 3.5.1)
project(Client)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Definitions
add_definitions(-DQT_PATH=${QT_PATH})
add_definitions(-DOPUS_LIB=${OPUS_LIB})
add_definitions(-DOPUS_DLL=${OPUS_DLL})
add_definitions(-DOPUS_INCLUDE=${OPUS_INCLUDE})
add_definitions(-DPORTAUDIO_LIB=${PORTAUDIO_LIB})
add_definitions(-DPORTAUDIO_DLL=${PORTAUDIO_DLL})
add_definitions(-DPORTAUDIO_INCLUDE=$PORTAUDIO_INCLUDE})

# Sources
set(SOURCE_FILES main.cpp Socket/ISocket.h Socket/ISocket.cpp Socket/endpacked.h Socket/packed.h Socket/Message.h Socket/Message.cpp Socket/QtSocket.cpp Socket/QtSocket.h DataManager/DataManager.cpp DataManager/DataManager.h Exceptions/Exception.cpp Exceptions/Exception.h Logger/Logger.cpp src/Window.h src/Window.cpp src/Channel.cpp src/Channel.h src/User.cpp src/User.h src/SoundManager.cpp src/SoundManager.h Socket/QtUdpServer.cpp Socket/QtUdpServer.h Socket/IServer.h Audio/AudioCodec.cpp Audio/AudioPA.hpp Audio/AudioPA.cpp Audio/AudioPA.hpp Audio/AudioSettings.h Audio/IAudio.hpp Audio/IAudioCodec.hpp Audio/Play.cpp Audio/Play.hpp Audio/Record.cpp Audio/Record.hpp)
add_executable(Client ${SOURCE_FILES})
target_link_libraries(Client Qt5::Widgets Qt5::Network)

# Finding libraries
find_package(Qt5 COMPONENTS Core Widgets Network Gui REQUIRED)
if (WIN32)
	target_link_libraries(Client ${PORTAUDIO_LIB})
	target_link_libraries(Client ${OPUS_LIB})
	set (OPUS_HEADERS
		opus.h
		opus_types.h
		opus_defines.h
	)
	foreach (file_i ${OPUS_HEADERS})
	add_custom_command(
		TARGET Client PRE_BUILD
	        COMMAND ${CMAKE_COMMAND} -E copy
        	        ${OPUS_INCLUDE}/${file_i}
               		${CMAKE_CURRENT_SOURCE_DIR}/opus/${file_i})
	endforeach()
	add_custom_command(
		TARGET Client POST_BUILD
	        COMMAND ${CMAKE_COMMAND} -E remove_directory
               		${CMAKE_CURRENT_SOURCE_DIR}/opus)
	include_directories(${PORTAUDIO_INCLUDE})
else(WIN32)
	target_link_libraries(Client portaudio opus)
endif(WIN32)

if (WIN32)
	target_link_libraries(Client pthreadVC2)
	target_compile_options(
		Client PRIVATE 
		"/EHsc"
	)
	if($<CONFIG:Debug>)
		get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_DEBUG)
		get_target_property(NETDLL Qt5::Network IMPORTED_LOCATION_DEBUG)
		get_target_property(COREDLL Qt5::Core IMPORTED_LOCATION_DEBUG)
		get_target_property(GUIDDLL Qt5::Gui IMPORTED_LOCATION_DEBUG)
	else($<CONFIG:Debug>)
		get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_RELEASE)
		get_target_property(NETDLL Qt5::Network IMPORTED_LOCATION_RELEASE)
		get_target_property(COREDLL Qt5::Core IMPORTED_LOCATION_RELEASE)
		get_target_property(COREDLL Qt5::Gui IMPORTED_LOCATION_RELEASE)
	endif($<CONFIG:Debug>)
	add_custom_command(
		TARGET Client POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:Qt5::Widgets>
			$<TARGET_FILE:Qt5::Network>
			$<TARGET_FILE:Qt5::Core>
			$<TARGET_FILE:Qt5::Gui>
			$<TARGET_FILE_DIR:Client>
	)
else()
	target_link_libraries(Client pthread)
endif(WIN32)
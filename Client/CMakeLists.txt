cmake_minimum_required(VERSION 3.5.1)
project(Client)

if (!WIN32)
	set(CMAKE_PREFIX_PATH "/opt/Qt5.7.0/5.7/gcc_64/")
endif()
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Widgets Network Gui REQUIRED)

set(SOURCE_FILES main.cpp Socket/ISocket.h Socket/ISocket.cpp Socket/endpacked.h Socket/packed.h Socket/Message.h Socket/Message.cpp Socket/QtSocket.cpp Socket/QtSocket.h DataManager/DataManager.cpp DataManager/DataManager.h Exceptions/Exception.cpp Exceptions/Exception.h Logger/Logger.cpp src/Window.h src/Window.cpp src/Channel.cpp src/Channel.h src/User.cpp src/User.h)
add_executable(Client ${SOURCE_FILES})
target_link_libraries(Client Qt5::Widgets Qt5::Network)

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
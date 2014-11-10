cmake_minimum_required(VERSION 2.8.3)
project(EposLib)

find_package(catkin REQUIRED COMPONENTS)

catkin_package(
  INCLUDE_DIRS EposCmd
  LIBRARIES EposCmd
  CATKIN_DEPENDS
  DEPENDS
)


include_directories(CommunicationXLib/Common/source)
include_directories(CommunicationXLib/Common/include)
include_directories(CommunicationXLib/Linux/source)
include_directories(CommonXLib/Common/include)
include_directories(SystemXLib/Common/include)
include_directories(EposCmd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")


file(GLOB_RECURSE CommonXLib_SRC
    "CommonXLib/*.cpp"
)
add_library(CommonXLib ${CommonXLib_SRC})

file(GLOB_RECURSE SystemXLib_SRC
    "SystemXLib/*.cpp"
)
add_library(SystemXLib ${SystemXLib_SRC})
target_link_libraries(SystemXLib
  ftd2xx
)

file(GLOB_RECURSE CommunicationXLib_SRC
    "CommunicationXLib/*.cpp"
)
add_library(CommunicationXLib ${CommunicationXLib_SRC})

file(GLOB_RECURSE EposCmd_SRC
    "EposCmd/*.cpp"
)
add_library(EposCmd ${EposCmd_SRC})
target_link_libraries(EposCmd
  CommonXLib
  SystemXLib
  CommunicationXLib
)

#############
## Install ##
#############

## Mark executables and/or libraries for installation
install(TARGETS EposCmd CommunicationXLib SystemXLib CommonXLib
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY EposCmd/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)

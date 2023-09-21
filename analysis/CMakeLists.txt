project(analysis)

find_package(ROOT REQUIRED)
# find_package(CLHEP)

file(GLOB sources
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

file(GLOB headers
  ${PROJECT_SOURCE_DIR}/include/*.h
)

add_executable(analysis analysis.cpp ${sources} ${headers})

target_link_libraries(
  analysis
  ${ROOT_LIBRARIES}
  podio::podioRootIO
  edm4dr
  edm4dr::edm4drDict
)

target_include_directories(analysis PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(analysis PROPERTIES PUBLIC_HEADER "${headers}")

install(TARGETS analysis EXPORT analysisTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev
)

####### Tower geometry check ##########
add_executable(geoCheck geoCheck.cpp ${sources} ${headers})

target_link_libraries(
  geoCheck
  ${ROOT_LIBRARIES}
  DRsegmentation
)

target_include_directories(geoCheck PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(geoCheck PROPERTIES PUBLIC_HEADER "${headers}")

install(TARGETS geoCheck EXPORT geoCheckTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev
)

####### Tower geometry check using param->SetParamBase(eta num) function ##########
add_executable(geoCheck_usingFunc geoCheck_usingFunc.cpp ${sources} ${headers})

target_link_libraries(
  geoCheck_usingFunc
  ${ROOT_LIBRARIES}
  DRsegmentation
)

target_include_directories(geoCheck_usingFunc PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(geoCheck_usingFunc PROPERTIES PUBLIC_HEADER "${headers}")

install(TARGETS geoCheck_usingFunc EXPORT geoCheck_usingFuncTargets
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev
)

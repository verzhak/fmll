
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/share/qgis/)

find_package(Qt4 REQUIRED)
find_package(QGIS REQUIRED)

set(fmll_plugin_h plugin.h)
set(fmll_plugin_src plugin.cpp)
# TODO SET (COPYRIGHT_UIS pluginguibase.ui)
# TODO SET (COPYRIGHT_MOC_HDRS  plugin.h plugingui.h)
# TODO SET (COPYRIGHT_RCCS  copyright_plugin.qrc)

# TODO qt4_wrap_ui(COPYRIGHT_UIS_H  ${COPYRIGHT_UIS})
qt4_wrap_cpp(fmll_plugin_moc ${fmll_plugin_h})
# TODO QT4_ADD_RESOURCES(COPYRIGHT_RCC_SRCS ${COPYRIGHT_RCCS})

# TODO ADD_LIBRARY (copyrightlabelplugin MODULE ${COPYRIGHT_SRCS} ${COPYRIGHT_MOC_SRCS} ${COPYRIGHT_RCC_SRCS} ${COPYRIGHT_UIS_H})

# include_directories(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIR}/QtCore ${QT_INCLUDE_DIR}/QtGui ${QGIS_INCLUDE_DIR})
set (QGIS_DIR /home/amv/inst/qgis/src/qgis-1.7.2 CACHE STRING "" FORCE) # TODO
include_directories(include ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIR}/QtCore ${QT_INCLUDE_DIR}/QtGui ${QGIS_INCLUDE_DIR} ${QGIS_DIR}/src/ui ${QGIS_DIR}/src/plugins ${QGIS_DIR}/src/gui ${QGIS_DIR}/src/raster ${QGIS_DIR}/src/core)

add_definitions(-DGUI_EXPORT= -DCORE_EXPORT=)
add_library(fmll_plugin MODULE ${fmll_plugin_src} ${fmll_plugin_moc})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pipe -std=gnu++0x" CACHE string "" FORCE)
target_link_libraries(fmll_plugin fmll_gis_fires ${QT_LIBRARIES} qgis_core qgis_gui)

# install(TARGETS fmll_plugin RUNTIME DESTINATION ${QGIS_PLUGIN_DIR} LIBRARY DESTINATION ${QGIS_PLUGIN_DIR})
install(TARGETS fmll_plugin RUNTIME DESTINATION /usr/lib/qgis/plugins/ LIBRARY DESTINATION /usr/lib/qgis/plugins/)


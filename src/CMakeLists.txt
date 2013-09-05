
find_package(PkgConfig REQUIRED)
pkg_check_modules(glib2 REQUIRED glib-2.0>=2.36)
pkg_check_modules(jansson REQUIRED jansson>=2.4)
pkg_check_modules(sde-utils REQUIRED sde-utils-1.0>=0.1)

add_definitions(
    ${glib2_CFLAGS}
    ${jansson_CFLAGS}
    ${sde-utils_CFLAGS}
)


add_library(sde-utils-1.0 MODULE
    utils.c
)

target_link_libraries(sde-utils-jansson-1.0
    ${glib2_LIBRARIES}
    ${jansson_LIBRARIES}
    ${sde-utils_LIBRARIES}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sde-utils-jansson-1.0/sde-utils-jansson)
install(FILES sde-utils-jansson.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sde-utils-jansson-1.0)
install(TARGETS sde-utils-jansson-1.0 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/)


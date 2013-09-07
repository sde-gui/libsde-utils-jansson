set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
if(ENABLE_WERROR)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif()

#if(ENABLE_AS_NEEDED)
#    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
#endif()


find_package(PkgConfig REQUIRED)
pkg_check_modules(gtk2 REQUIRED gtk+-2.0>=2.24)
pkg_check_modules(glib2 REQUIRED glib-2.0>=2.36)
pkg_check_modules(jansson REQUIRED jansson>=2.4)
pkg_check_modules(sde_utils REQUIRED sde-utils-1.0>=0.1)
pkg_check_modules(sde-utils-gtk REQUIRED sde-utils-gtk-1.0>=0.1)

add_definitions(
    ${gtk2_CFLAGS}
    ${sde_utils_gtk_CFLAGS}
    ${glib2_CFLAGS}
    ${jansson_CFLAGS}
    ${sde-utils_CFLAGS}
)

link_directories(
    ${gtk2_LIBRARY_DIRS}
    ${sde_utils_gtk_LIBRARY_DIRS}
    ${glib2_LIBRARY_DIRS}
    ${jansson_LIBRARY_DIRS}
    ${sde_utils_LIBRARY_DIRS}
)

add_library(sde-utils-jansson-1.0 MODULE
    utils.c
)

target_link_libraries(sde-utils-jansson-1.0
    ${sde_utils_gtk_LIBRARIES}
    ${glib2_LIBRARIES}
    ${jansson_LIBRARIES}
    ${sde_utils_LIBRARIES}
    ${gtk2_LIBRARIES}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sde-utils-jansson-1.0/sde-utils-jansson)
install(FILES sde-utils-jansson.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sde-utils-jansson-1.0)
install(TARGETS sde-utils-jansson-1.0 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/)


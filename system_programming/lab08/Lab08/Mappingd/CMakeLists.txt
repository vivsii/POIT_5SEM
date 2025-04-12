add_library(Mappingd SHARED Mappingd.c)
set_target_properties(Mappingd PROPERTIES LINK_FLAGS "-Wl,--output-def,${CMAKE_CURRENT_BINARY_DIR}/Mappingd.def")

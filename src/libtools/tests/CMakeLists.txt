include(LibAddMacros)

add_headers (HDR_FILES)
add_cppheaders (HDR_FILES)
add_toolheaders(HDR_FILES)

include (LibAddTest)

file (GLOB TESTS testtool_*.cpp)
foreach (file ${TESTS})
	get_filename_component (name ${file} NAME_WE)
	add_gtest (${name})
	target_link_elektratools(${name})
endforeach (file ${TESTS})

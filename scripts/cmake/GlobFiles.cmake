## Glob all source files within a particular directory
function(glob_sources OUT_VAR TARGET_DIR)
	if(NOT IS_DIRECTORY ${TARGET_DIR})
		message(FATAL_ERROR "Director '${TARGET_DIR}' does not exist")
	endif()

	file(GLOB_RECURSE SRC_FILES
		CONFIGURE_DEPENDS
		"${TARGET_DIR}/*.cpp"
        "${TARGET_DIR}/*.cc"
        "${TARGET_DIR}/*.cxx"
		"${TARGET_DIR}/*.C"
	)

	if(SRC_FILES)
		list(SORT SRC_FILES)
	endif()

	set(${OUT_VAR} ${SRC_FILES} PARENT_SCOPE)
endfunction()
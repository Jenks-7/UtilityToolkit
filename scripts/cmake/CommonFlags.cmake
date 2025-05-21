## Set compiler warnings, standard flags, etc.
function(set_common_flags)
	if(MSVC)
		add_compile_options(/W4 /permissive- /wd4458)

		# Optional: Treat warnings as errors in Debug builds
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			add_compile_options(/WX)
		endif()
	else()
		# For cross-platform support if building with GCC/Clang on Windows
		add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion)

		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			add_compile_options(-Werror)
		endif()
	endif()
endfunction()
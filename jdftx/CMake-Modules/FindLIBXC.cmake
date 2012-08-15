find_path(LIBXC_INCLUDE_DIR xc.h /usr/include /usr/local/include)
find_library(LIBXC_LIBRARY NAMES xc PATHS /usr/lib /usr/lib64 /usr/local/lib /usr/local/lib64)

if(LIBXC_INCLUDE_DIR AND LIBXC_LIBRARY)
	set(LIBXC_FOUND TRUE)
endif(LIBXC_INCLUDE_DIR AND LIBXC_LIBRARY)


if(LIBXC_FOUND)
	if(NOT LIBXC_FIND_QUIETLY)
		message(STATUS "Found LibXC: ${LIBXC_LIBRARY}")
	endif(NOT LIBXC_FIND_QUIETLY)
else(LIBXC_FOUND)
	if(LIBXC_FIND_REQUIRED)
		if(NOT LIBXC_INCLUDE_DIR)
			message(FATAL_ERROR "Could not find LibXC headers")
		endif(NOT LIBXC_INCLUDE_DIR)
		if(NOT LIBXC_LIBRARY)
			MESSAGE(FATAL_ERROR "Could not find LibXC shared libraries")
		endif(NOT LIBXC_LIBRARY)
	endif(LIBXC_FIND_REQUIRED)
endif(LIBXC_FOUND)
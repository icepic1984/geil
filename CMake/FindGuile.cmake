# Copyright (c) 2008, OpenCog.org (http://opencog.org)
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# - Try to find Guile; Once done this will define
#
# GUILE_FOUND - system has the GUILE library
# GUILE_INCLUDE_DIRS - the GUILE include directory
# GUILE_LIBRARIES - The libraries needed to use GUILE



# Look for the header file
FIND_PATH(GUILE_INCLUDE_DIR libguile.h /usr/include /usr/local/include /usr/include/guile/2.2 /usr/local/include/libguile)

# Look for the library
FIND_LIBRARY(GUILE_LIBRARY NAMES libguile guile-2.2 PATH /usr/lib /usr/lib/guile/2.2 /usr/local/lib)

# Copy the results to the output variables.
IF(GUILE_INCLUDE_DIR AND GUILE_LIBRARY)
	SET(GUILE_FOUND 1)
	SET(GUILE_LIBRARIES ${GUILE_LIBRARY})
	SET(GUILE_INCLUDE_DIRS ${GUILE_INCLUDE_DIR})
ELSE(GUILE_INCLUDE_DIR AND GUILE_LIBRARY)
	SET(GUILE_FOUND 0)
	SET(GUILE_LIBRARIES)
	SET(GUILE_INCLUDE_DIRS)
ENDIF(GUILE_INCLUDE_DIR AND GUILE_LIBRARY)


# check guile's version if we're using cmake >= 2.6
IF(GUILE_INCLUDE_DIR AND NOT CMAKE_MAJOR_VERSION LESS 2 AND NOT CMAKE_MINOR_VERSION LESS 6)
	SET(GUILE_VERSION_MAJOR 0)
	SET(GUILE_VERSION_MINOR 0)
	SET(GUILE_VERSION_PATCH 0)

	# Extract the libguile version from the 'version.h' file
	SET(GUILE_MAJOR_VERSION 0)
	FILE(READ "${GUILE_INCLUDE_DIR}/libguile/version.h" _GUILE_VERSION_H_CONTENTS)

	STRING(REGEX MATCH "#define SCM_MAJOR_VERSION[	 ]+([0-9])" _MATCH "${_GUILE_VERSION_H_CONTENTS}")
	SET(GUILE_VERSION_MAJOR ${CMAKE_MATCH_1})
	STRING(REGEX MATCH "#define SCM_MINOR_VERSION[	 ]+([0-9])" _MATCH "${_GUILE_VERSION_H_CONTENTS}")
	SET(GUILE_VERSION_MINOR ${CMAKE_MATCH_1})
	STRING(REGEX MATCH "#define SCM_MICRO_VERSION[	 ]+([0-9])" _MATCH "${_GUILE_VERSION_H_CONTENTS}")
	SET(GUILE_VERSION_PATCH ${CMAKE_MATCH_1})

	# Check found version against required one
	# Use nested less...equal so that version 2.0.0 is newer than version 1.9.1 is newer than 1.8.6
	IF (Guile_FIND_VERSION_MAJOR AND ${GUILE_VERSION_MAJOR} LESS Guile_FIND_VERSION_MAJOR)
		SET(GUILE_FOUND FALSE)
	ELSE (Guile_FIND_VERSION_MAJOR AND ${GUILE_VERSION_MAJOR} LESS Guile_FIND_VERSION_MAJOR)
		IF (Guile_FIND_VERSION_MAJOR AND ${GUILE_VERSION_MAJOR} EQUAL Guile_FIND_VERSION_MAJOR)
			IF (Guile_FIND_VERSION_MINOR AND ${GUILE_VERSION_MINOR} LESS Guile_FIND_VERSION_MINOR)
				SET(GUILE_FOUND FALSE )
			ELSE (Guile_FIND_VERSION_MINOR AND ${GUILE_VERSION_MINOR} LESS Guile_FIND_VERSION_MINOR)
				IF (Guile_FIND_VERSION_MINOR AND ${GUILE_VERSION_MINOR} EQUAL Guile_FIND_VERSION_MINOR)
					IF (Guile_FIND_VERSION_PATCH AND ${GUILE_VERSION_PATCH} LESS Guile_FIND_VERSION_PATCH)
						SET(GUILE_FOUND FALSE )
					ENDIF (Guile_FIND_VERSION_PATCH AND ${GUILE_VERSION_PATCH} LESS Guile_FIND_VERSION_PATCH)
				ENDIF (Guile_FIND_VERSION_MINOR AND ${GUILE_VERSION_MINOR} EQUAL Guile_FIND_VERSION_MINOR)
			ENDIF (Guile_FIND_VERSION_MINOR AND ${GUILE_VERSION_MINOR} LESS Guile_FIND_VERSION_MINOR)
		ENDIF (Guile_FIND_VERSION_MAJOR AND ${GUILE_VERSION_MAJOR} EQUAL Guile_FIND_VERSION_MAJOR)
	ENDIF (Guile_FIND_VERSION_MAJOR AND ${GUILE_VERSION_MAJOR} LESS Guile_FIND_VERSION_MAJOR)
ENDIF(GUILE_INCLUDE_DIR AND NOT CMAKE_MAJOR_VERSION LESS 2 AND NOT CMAKE_MINOR_VERSION LESS 6)

# Report the results.
IF(NOT GUILE_FOUND)
	IF (Guile_FIND_VERSION_MAJOR)
		SET(_GUILE_VERSION_MESSAGE_STRING " (>=${Guile_FIND_VERSION_MAJOR}.${Guile_FIND_VERSION_MINOR}.${Guile_FIND_VERSION_PATCH})")
	ENDIF (Guile_FIND_VERSION_MAJOR)
	SET(GUILE_DIR_MESSAGE "Guile${_GUILE_VERSION_MESSAGE_STRING} was not found. Make sure GUILE_LIBRARY and GUILE_INCLUDE_DIR are set.")
	IF(NOT GUILE_FIND_QUIETLY)
		MESSAGE(STATUS "${GUILE_DIR_MESSAGE}")
	ELSE(NOT GUILE_FIND_QUIETLY)
		IF(GUILE_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "${GUILE_DIR_MESSAGE}")
		ENDIF(GUILE_FIND_REQUIRED)
	ENDIF(NOT GUILE_FIND_QUIETLY)
ENDIF(NOT GUILE_FOUND)

MARK_AS_ADVANCED(GUILE_INCLUDE_DIR GUILE_LIBRARY)

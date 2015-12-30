# CMakeLists for generating Cairo.
# Primary target is Windows; on other platforms you should be fine.
# Notably for other platforms 

# Project name is cairo
PROJECT(cairo)

# Stop CMake complaining.  This seems low enough; is it?
cmake_minimum_required(VERSION 2.8)

# Used in child files.
SET(PROJECT_PREFIX "CAIRO")

# Stop installation; this would be a *bad* idea
# (It also generates errors for zlib/libpng)
SET(SKIP_INSTALL_ALL ON)

# Configured include output
SET(CONFIGURED_INCLUDES "${PROJECT_BINARY_DIR}/include")
FILE(MAKE_DIRECTORY ${CONFIGURED_INCLUDES})

# Start simple
INCLUDE_DIRECTORIES(
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	${CONFIGURED_INCLUDES}
)

# Options
MACRO(CAIRO_OPTION name default type description)
	IF(NOT DEFINED ${name})
		SET(${name} ${default} CACHE ${type} ${description} FORCE)
	ENDIF()
ENDMACRO()

# Secret-cache
# e.g. CAIRO_SECRET(PNG_SHARED BOOL FALSE)
MACRO(CAIRO_SECRET name type value)
	IF(NOT DEFINED ${name})
		SET(${name} ${value})
	ELSE()
		SET(${name} ${value} CACHE ${type} "" FORCE)
	ENDIF()
	MARK_AS_ADVANCED(FORCE ${name})
ENDMACRO()

CAIRO_OPTION(
	${PROJECT_PREFIX}_USE_ZLIB_STATIC
	FALSE
	BOOL
	"Use Static ZLIB"
)

CAIRO_OPTION(
	${PROJECT_PREFIX}_USE_LIBPNG_STATIC
	FALSE
	BOOL
	"Use Static LIBPNG"
)

CAIRO_OPTION(
	${PROJECT_PREFIX}_USE_CAIRO_STATIC
	FALSE
	BOOL
	"Use Static Cairo"
)

# zlib - both static and dynamic are built; this is a
# side effect of the way it's done by zlib upstream.
SET(DEPENDENCY_COMMON_ZLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zlib")
ADD_SUBDIRECTORY(${DEPENDENCY_COMMON_ZLIB_DIR})

IF(${PROJECT_PREFIX}_USE_ZLIB_STATIC)
SET(${PROJECT_PREFIX}_ZLIB zlibstatic)
ELSE(${PROJECT_PREFIX}_USE_ZLIB_STATIC)
SET(${PROJECT_PREFIX}_ZLIB zlib)
ENDIF(${PROJECT_PREFIX}_USE_ZLIB_STATIC)

# libpng (requires 'zlib')
SET(DEPENDENCY_COMMON_LIBPNG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libpng")

# We *never* want these; the user will have to reset every time.
CAIRO_SECRET(PNG_FRAMEWORK BOOL FALSE)
CAIRO_SECRET(PNG_TESTS BOOL FALSE)

IF(${PROJECT_PREFIX}_USE_LIBPNG_STATIC)
CAIRO_SECRET(PNG_STATIC BOOL TRUE)
CAIRO_SECRET(PNG_SHARED BOOL FALSE)
ELSE(${PROJECT_PREFIX}_USE_LIBPNG_STATIC)
CAIRO_SECRET(PNG_STATIC BOOL FALSE)
CAIRO_SECRET(PNG_SHARED BOOL TRUE)
ENDIF(${PROJECT_PREFIX}_USE_LIBPNG_STATIC)

# libpng's CMake tries to be helpful; it really isn't.
SET(ZLIB_INCLUDE_DIR ${DEPENDENCY_COMMON_ZLIB_DIR})
SET(ZLIB_LIBRARY ${${PROJECT_PREFIX}_ZLIB})

ADD_SUBDIRECTORY(${DEPENDENCY_COMMON_LIBPNG_DIR})

# Users don't need this.
MARK_AS_ADVANCED(FORCE PNG_STATIC PNG_SHARED PNG_FRAMEWORK PNG_TESTS)

IF(${PROJECT_PREFIX}_USE_LIBPNG_STATIC)
GET_DIRECTORY_PROPERTY(
	${PROJECT_PREFIX}_LIBPNG
	DIRECTORY
	${DEPENDENCY_COMMON_LIBPNG_DIR}
	DEFINITION
	PNG_LIB_NAME_STATIC
)
ELSE(${PROJECT_PREFIX}_USE_LIBPNG_STATIC)
GET_DIRECTORY_PROPERTY(
	${PROJECT_PREFIX}_LIBPNG
	DIRECTORY
	${DEPENDENCY_COMMON_LIBPNG_DIR}
	DEFINITION
	PNG_LIB_NAME
)
ENDIF(${PROJECT_PREFIX}_USE_LIBPNG_STATIC)

# libpng dependency on zlib (order!)
ADD_DEPENDENCIES(
	${${PROJECT_PREFIX}_LIBPNG}
	${${PROJECT_PREFIX}_ZLIB}
)

GET_DIRECTORY_PROPERTY(
	DEPENDENCY_COMMON_LIBPNG_BINDIR
	DIRECTORY
	${DEPENDENCY_COMMON_LIBPNG_DIR}
	DEFINITION
	CMAKE_CURRENT_BINARY_DIR
)

INCLUDE_DIRECTORIES(
	${DEPENDENCY_COMMON_LIBPNG_DIR}
	${DEPENDENCY_COMMON_ZLIB_DIR}
	${DEPENDENCY_COMMON_LIBPNG_BINDIR}
)

# This file is NOT from Cairo or Pixman.  It is from SRGE.
# Updates to Cairo and Pixman *may* break it.  You've been warned.

# Call it 'pixman' and make it static.
SET(PIXMAN_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pixman")

FILE(MAKE_DIRECTORY "${CONFIGURED_INCLUDES}/pixman")

INCLUDE_DIRECTORIES(
	${PIXMAN_SRC_DIR}
	"${PIXMAN_SRC_DIR}/pixman"
)

SET(PIXMAN_SRC 
	"${PIXMAN_SRC_DIR}/pixman/pixman.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-access.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-access-accessors.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-bits-image.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-combine32.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-combine-float.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-conical-gradient.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-filter.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-x86.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-mips.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-arm.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-ppc.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-edge.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-edge-accessors.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-fast-path.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-glyph.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-general.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-gradient-walker.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-image.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-implementation.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-linear-gradient.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-matrix.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-noop.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-radial-gradient.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-region16.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-region32.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-solid-fill.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-timer.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-trap.c"
	"${PIXMAN_SRC_DIR}/pixman/pixman-utils.c"
)

ADD_LIBRARY(pixman STATIC ${PIXMAN_SRC})
SET_TARGET_PROPERTIES(pixman PROPERTIES COMPILE_FLAGS -DPACKAGE=pixman -DPACKAGE_VERSION="" -DPACKAGE_BUGREPORT="")

# Port/configure: MMX, SSE2, SSE3.
# MMX_CFLAGS = -DUSE_X86_MMX -w14710 -w14714
# SSE2_CFLAGS = -DUSE_SSE2
# SSSE3_CFLAGS = -DUSE_SSSE3

###############################################################################
# Cairo - This is where Windows devs typically get other-worldly frustrated.
###############################################################################

# Call it 'pixman' and make it static.
SET(CAIRO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cairo")

# Cairo uses the make equivalent of glob; we won't.
SET(CAIRO_SRC
	"${CAIRO_SRC_DIR}/src/cairo-analysis-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-arc.c"
	"${CAIRO_SRC_DIR}/src/cairo-array.c"
	"${CAIRO_SRC_DIR}/src/cairo-atomic.c"
	"${CAIRO_SRC_DIR}/src/cairo-base64-stream.c"
	"${CAIRO_SRC_DIR}/src/cairo-base85-stream.c"
	"${CAIRO_SRC_DIR}/src/cairo-bentley-ottmann-rectangular.c"
	"${CAIRO_SRC_DIR}/src/cairo-bentley-ottmann-rectilinear.c"
	"${CAIRO_SRC_DIR}/src/cairo-bentley-ottmann.c"
	"${CAIRO_SRC_DIR}/src/cairo-botor-scan-converter.c"
	"${CAIRO_SRC_DIR}/src/cairo-boxes-intersect.c"
	"${CAIRO_SRC_DIR}/src/cairo-boxes.c"
	"${CAIRO_SRC_DIR}/src/cairo-cache.c"
	"${CAIRO_SRC_DIR}/src/cairo-cff-subset.c"
	"${CAIRO_SRC_DIR}/src/cairo-clip-boxes.c"
	"${CAIRO_SRC_DIR}/src/cairo-clip-polygon.c"
	"${CAIRO_SRC_DIR}/src/cairo-clip-region.c"
	"${CAIRO_SRC_DIR}/src/cairo-clip-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-clip-tor-scan-converter.c"
	"${CAIRO_SRC_DIR}/src/cairo-clip.c"
	"${CAIRO_SRC_DIR}/src/cairo-color.c"
	"${CAIRO_SRC_DIR}/src/cairo-composite-rectangles.c"
	"${CAIRO_SRC_DIR}/src/cairo-compositor.c"
	"${CAIRO_SRC_DIR}/src/cairo-contour.c"
	"${CAIRO_SRC_DIR}/src/cairo-damage.c"
	"${CAIRO_SRC_DIR}/src/cairo-debug.c"
	"${CAIRO_SRC_DIR}/src/cairo-default-context.c"
	"${CAIRO_SRC_DIR}/src/cairo-deflate-stream.c"
	"${CAIRO_SRC_DIR}/src/cairo-device.c"
	"${CAIRO_SRC_DIR}/src/cairo-error.c"
	"${CAIRO_SRC_DIR}/src/cairo-fallback-compositor.c"
	"${CAIRO_SRC_DIR}/src/cairo-fixed.c"
	"${CAIRO_SRC_DIR}/src/cairo-font-face-twin-data.c"
	"${CAIRO_SRC_DIR}/src/cairo-font-face-twin.c"
	"${CAIRO_SRC_DIR}/src/cairo-font-face.c"
	"${CAIRO_SRC_DIR}/src/cairo-font-options.c"
	"${CAIRO_SRC_DIR}/src/cairo-freed-pool.c"
	"${CAIRO_SRC_DIR}/src/cairo-freelist.c"
	"${CAIRO_SRC_DIR}/src/cairo-gstate.c"
	"${CAIRO_SRC_DIR}/src/cairo-hash.c"
	"${CAIRO_SRC_DIR}/src/cairo-hull.c"
	"${CAIRO_SRC_DIR}/src/cairo-image-compositor.c"
	"${CAIRO_SRC_DIR}/src/cairo-image-info.c"
	"${CAIRO_SRC_DIR}/src/cairo-image-source.c"
	"${CAIRO_SRC_DIR}/src/cairo-image-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-line.c"
	"${CAIRO_SRC_DIR}/src/cairo-lzw.c"
	"${CAIRO_SRC_DIR}/src/cairo-mask-compositor.c"
	"${CAIRO_SRC_DIR}/src/cairo-matrix.c"
	"${CAIRO_SRC_DIR}/src/cairo-mempool.c"
	"${CAIRO_SRC_DIR}/src/cairo-mesh-pattern-rasterizer.c"
	"${CAIRO_SRC_DIR}/src/cairo-misc.c"
	"${CAIRO_SRC_DIR}/src/cairo-mono-scan-converter.c"
	"${CAIRO_SRC_DIR}/src/cairo-mutex.c"
	"${CAIRO_SRC_DIR}/src/cairo-no-compositor.c"
	"${CAIRO_SRC_DIR}/src/cairo-observer.c"
	"${CAIRO_SRC_DIR}/src/cairo-output-stream.c"
	"${CAIRO_SRC_DIR}/src/cairo-paginated-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-path-bounds.c"
	"${CAIRO_SRC_DIR}/src/cairo-path-fill.c"
	"${CAIRO_SRC_DIR}/src/cairo-path-fixed.c"
	"${CAIRO_SRC_DIR}/src/cairo-path-in-fill.c"
	"${CAIRO_SRC_DIR}/src/cairo-path-stroke-boxes.c"
	"${CAIRO_SRC_DIR}/src/cairo-path-stroke-polygon.c"
	"${CAIRO_SRC_DIR}/src/cairo-path-stroke-traps.c"
	"${CAIRO_SRC_DIR}/src/cairo-path-stroke-tristrip.c"
	"${CAIRO_SRC_DIR}/src/cairo-path-stroke.c"
	"${CAIRO_SRC_DIR}/src/cairo-path.c"
	"${CAIRO_SRC_DIR}/src/cairo-pattern.c"
	"${CAIRO_SRC_DIR}/src/cairo-pdf-operators.c"
	"${CAIRO_SRC_DIR}/src/cairo-pdf-shading.c"
	"${CAIRO_SRC_DIR}/src/cairo-pdf-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-pen.c"
	"${CAIRO_SRC_DIR}/src/cairo-png.c"
	"${CAIRO_SRC_DIR}/src/cairo-polygon-intersect.c"
	"${CAIRO_SRC_DIR}/src/cairo-polygon-reduce.c"
	"${CAIRO_SRC_DIR}/src/cairo-polygon.c"
	"${CAIRO_SRC_DIR}/src/cairo-ps-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-raster-source-pattern.c"
	"${CAIRO_SRC_DIR}/src/cairo-recording-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-rectangle.c"
	"${CAIRO_SRC_DIR}/src/cairo-rectangular-scan-converter.c"
	"${CAIRO_SRC_DIR}/src/cairo-region.c"
	"${CAIRO_SRC_DIR}/src/cairo-rtree.c"
	"${CAIRO_SRC_DIR}/src/cairo-scaled-font-subsets.c"
	"${CAIRO_SRC_DIR}/src/cairo-scaled-font.c"
	"${CAIRO_SRC_DIR}/src/cairo-script-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-shape-mask-compositor.c"
	"${CAIRO_SRC_DIR}/src/cairo-slope.c"
	"${CAIRO_SRC_DIR}/src/cairo-spans-compositor.c"
	"${CAIRO_SRC_DIR}/src/cairo-spans.c"
	"${CAIRO_SRC_DIR}/src/cairo-spline.c"
	"${CAIRO_SRC_DIR}/src/cairo-stroke-dash.c"
	"${CAIRO_SRC_DIR}/src/cairo-stroke-style.c"
	"${CAIRO_SRC_DIR}/src/cairo-surface-clipper.c"
	"${CAIRO_SRC_DIR}/src/cairo-surface-fallback.c"
	"${CAIRO_SRC_DIR}/src/cairo-surface-observer.c"
	"${CAIRO_SRC_DIR}/src/cairo-surface-offset.c"
	"${CAIRO_SRC_DIR}/src/cairo-surface-snapshot.c"
	"${CAIRO_SRC_DIR}/src/cairo-surface-subsurface.c"
	"${CAIRO_SRC_DIR}/src/cairo-surface-wrapper.c"
	"${CAIRO_SRC_DIR}/src/cairo-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-svg-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-time.c"
	"${CAIRO_SRC_DIR}/src/cairo-tor-scan-converter.c"
	"${CAIRO_SRC_DIR}/src/cairo-tor22-scan-converter.c"
	"${CAIRO_SRC_DIR}/src/cairo-toy-font-face.c"
	"${CAIRO_SRC_DIR}/src/cairo-traps-compositor.c"
	"${CAIRO_SRC_DIR}/src/cairo-traps.c"
	"${CAIRO_SRC_DIR}/src/cairo-tristrip.c"
	"${CAIRO_SRC_DIR}/src/cairo-truetype-subset.c"
	"${CAIRO_SRC_DIR}/src/cairo-type1-fallback.c"
	"${CAIRO_SRC_DIR}/src/cairo-type1-glyph-names.c"
	"${CAIRO_SRC_DIR}/src/cairo-type1-subset.c"
	"${CAIRO_SRC_DIR}/src/cairo-type3-glyph-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo-unicode.c"
	"${CAIRO_SRC_DIR}/src/cairo-user-font.c"
	"${CAIRO_SRC_DIR}/src/cairo-version.c"
	"${CAIRO_SRC_DIR}/src/cairo-wideint.c"
	"${CAIRO_SRC_DIR}/src/cairo-xml-surface.c"
	"${CAIRO_SRC_DIR}/src/cairo.c"
)

IF(WIN32)
	SET(CAIRO_SRC_PLATFORM
		"${CAIRO_SRC_DIR}/src/win32/cairo-win32-debug.c"
		"${CAIRO_SRC_DIR}/src/win32/cairo-win32-device.c"
		"${CAIRO_SRC_DIR}/src/win32/cairo-win32-display-surface.c"
		"${CAIRO_SRC_DIR}/src/win32/cairo-win32-font.c"
		"${CAIRO_SRC_DIR}/src/win32/cairo-win32-gdi-compositor.c"
		"${CAIRO_SRC_DIR}/src/win32/cairo-win32-printing-surface.c"
		"${CAIRO_SRC_DIR}/src/win32/cairo-win32-surface.c"
		"${CAIRO_SRC_DIR}/src/win32/cairo-win32-system.c"
	)
	INCLUDE_DIRECTORIES("${CAIRO_SRC_DIR}/src/win32")
ENDIF(WIN32)

IF(APPLE)
	SET(CAIRO_SRC_PLATFORM
		"${CAIRO_SRC_DIR}/src/cairo-quartz-font.c"
		"${CAIRO_SRC_DIR}/src/cairo-quartz-image-surface.c"
		"${CAIRO_SRC_DIR}/src/cairo-quartz-surface.c"
	)
ENDIF(APPLE)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET(CAIRO_SRC_PLATFORM
		"${CAIRO_SRC_DIR}/src/cairo-ft-font.c"
		"${CAIRO_SRC_DIR}/src/cairo-xcb-connection-core.c"
		"${CAIRO_SRC_DIR}/src/cairo-xcb-connection-render.c"
		"${CAIRO_SRC_DIR}/src/cairo-xcb-connection-shm.c"
		"${CAIRO_SRC_DIR}/src/cairo-xcb-connection.c"
		"${CAIRO_SRC_DIR}/src/cairo-xcb-resources.c"
		"${CAIRO_SRC_DIR}/src/cairo-xcb-screen.c"
		"${CAIRO_SRC_DIR}/src/cairo-xcb-shm.c"
		"${CAIRO_SRC_DIR}/src/cairo-xcb-surface-core.c"
		"${CAIRO_SRC_DIR}/src/cairo-xcb-surface-render.c"
		"${CAIRO_SRC_DIR}/src/cairo-xcb-surface.c"
	)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Static or Dynamic/Shared?
IF(${PROJECT_PREFIX}_USE_CAIRO_STATIC)
ADD_LIBRARY(cairo STATIC ${CAIRO_SRC} ${CAIRO_SRC_PLATFORM})
ELSE(${PROJECT_PREFIX}_USE_CAIRO_STATIC)
ADD_LIBRARY(cairo SHARED ${CAIRO_SRC} ${CAIRO_SRC_PLATFORM})
ENDIF(${PROJECT_PREFIX}_USE_CAIRO_STATIC)

# Consistency is king.
IF(MSVC)
	SET_TARGET_PROPERTIES(cairo PROPERTIES PREFIX "lib")
	SET_TARGET_PROPERTIES(cairo PROPERTIES IMPORT_PREFIX "lib")
ENDIF(MSVC)

# Set the dependencies!
ADD_DEPENDENCIES(cairo
	pixman
	${${PROJECT_PREFIX}_LIBPNG}
	${${PROJECT_PREFIX}_ZLIB}
)

IF(WIN32)
	TARGET_LINK_LIBRARIES(
		cairo
		gdi32.lib
		msimg32.lib
		user32.lib
		pixman
		${${PROJECT_PREFIX}_LIBPNG}
		${${PROJECT_PREFIX}_ZLIB}
	)
ENDIF(WIN32)

# TODO - get the right libraries
IF(APPLE)
	TARGET_LINK_LIBRARIES(
		cairo
		pixman
		${${PROJECT_PREFIX}_LIBPNG}
		${${PROJECT_PREFIX}_ZLIB}
	)
ENDIF(APPLE)

# TODO - get the right libraries
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	TARGET_LINK_LIBRARIES(
		cairo
		pixman
		${${PROJECT_PREFIX}_LIBPNG}
		${${PROJECT_PREFIX}_ZLIB}
	)

	CAIRO_OPTION(
		${PROJECT_PREFIX}_BUILD_XCB_SURFACE
		TRUE
		BOOL
		"Enable the XCB surface."
	)

	IF(${PROJECT_PREFIX}_BUILD_XCB_SURFACE)
		SET_TARGET_PROPERTIES(cairo PROPERTIES COMPILE_FLAGS -DCAIRO_INCLUDE_XCB)
	ENDIF(${PROJECT_PREFIX}_BUILD_XCB_SURFACE)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# zlib.h rebuild into our config tree
CONFIGURE_FILE(
	"${DEPENDENCY_COMMON_ZLIB_DIR}/zconf.h.cmakein"
	"${CONFIGURED_INCLUDES}/zconf.h"
)

# This needs to be updated whenever pixman updates or it'll stay here.
# This is from 2015-12-29.  (TODO: A better solution)
SET(PIXMAN_VERSION_MAJOR 0)
SET(PIXMAN_VERSION_MINOR 35)
SET(PIXMAN_VERSION_MICRO 1)
CONFIGURE_FILE(
	"${PIXMAN_SRC_DIR}/pixman/pixman-version.h.in"
	"${CONFIGURED_INCLUDES}/pixman-version.h"
)

INCLUDE_DIRECTORIES(
	"${CAIRO_SRC_DIR}/src"
	${CAIRO_SRC_DIR}
)
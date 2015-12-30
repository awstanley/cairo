// Manually written using the cairo manual - http://cairographics.org/manual/
// Update as needed, but be warned this isn't fun to fix if you break it.

// Supports all the non-tricky types.
// For Windows it supports Win32 surface (GDI);
// For Apple/OS X Quartz should work;
// For Linux it supports XCB (stop using Xlib!).
// Common "flat" surfaces are enabled.
//
// Note: certain .c files aren't included.  This is for sanity;
// if you tweak this be prepared to add or remove files...
#ifndef CAIRO_FEATURES_H 
#define CAIRO_FEATURES_H

#if defined _WIN32 || defined _WIN64
#	define CAIRO_HAS_WIN32_SURFACE 1 
#	define CAIRO_HAS_WIN32_FONT 1
#else
#	if defined __APPLE__
#		define CAIRO_HAS_QUARTZ_FONT 1
#	endif//__APPLE__
#	if defined __linux__
#		define CAIRO_HAS_FC_FONT 1
#		define CAIRO_HAS_FT_FONT 1
#		define CAIRO_HAS_GOBJECT_FUNCTIONS 1
#		if defined CAIRO_INCLUDE_XCB
#			define CAIRO_HAS_XCB_SURFACE 1
#			define CAIRO_HAS_XCB_SHM_FUNCTIONS 1
#		endif//CAIRO_INCLUDE_XCB
#	endif//__linux__
#endif 

// Common
#define CAIRO_HAS_USER_FONT 1 
#define CAIRO_HAS_IMAGE_SURFACE 1
#define CAIRO_HAS_MIME_SURFACE 1
#define CAIRO_HAS_OBSERVER_SURFACE 1
#define CAIRO_HAS_RECORDING_SURFACE 1
#define CAIRO_HAS_SCRIPT_SURFACE 1
#define CAIRO_HAS_SVG_SURFACE 1
#define CAIRO_HAS_PS_SURFACE 1
#define CAIRO_HAS_PDF_SURFACE 1
#define CAIRO_HAS_PNG_FUNCTIONS 1 
#define CAIRO_HAS_INTERPRETER 1 
#define CAIRO_HAS_XML_SURFACE 1
// End Common

#endif//CAIRO_FEATURES_H
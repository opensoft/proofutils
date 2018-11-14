find_path(QRencode_INCLUDE_DIR
    NAMES "qrencode.h"
    DOC "QRencode include directory"
)
find_library(QRencode_LIBRARY_RELEASE NAMES qrencode)
find_library(QRencode_LIBRARY_DEBUG   NAMES qrencoded)

include(SelectLibraryConfigurations)
select_library_configurations(QRencode)
mark_as_advanced(QRencode_LIBRARY_RELEASE QRencode_LIBRARY_DEBUG)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QRencode
    REQUIRED_VARS QRencode_INCLUDE_DIR QRencode_LIBRARY
)
mark_as_advanced(QRencode_INCLUDE_DIR QRencode_LIBRARY)

if(NOT QRencode_FOUND)
    return()
endif()

set(QRencode_INCLUDE_DIRS "${QRencode_INCLUDE_DIR}")
set(QRencode_LIBRARIES "${QRencode_LIBRARY}")
if(NOT TARGET QRencode::QRencode)
    add_library(QRencode::QRencode INTERFACE IMPORTED)
endif()
set_property(TARGET QRencode::QRencode PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${QRencode_INCLUDE_DIRS}")
set_property(TARGET QRencode::QRencode PROPERTY INTERFACE_LINK_LIBRARIES "${QRencode_LIBRARIES}")

set(QRencode_INCLUDE_DIRS ${QRencode_INCLUDE_DIR})
if(NOT QRencode_LIBRARIES)
    set(QRencode_LIBRARIES ${QRencode_LIBRARY})
endif()

if(TARGET QRencode::QRencode)
    return()
endif()

add_library(QRencode::QRencode UNKNOWN IMPORTED)
set_target_properties(QRencode::QRencode PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${QRencode_INCLUDE_DIRS}"
    IMPORTED_LOCATION "${QRencode_LIBRARY}")

if(QRencode_LIBRARY_RELEASE)
    set_property(TARGET QRencode::QRencode
        APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
    set_target_properties(QRencode::QRencode
        PROPERTIES IMPORTED_LOCATION_RELEASE "${QRencode_LIBRARY_RELEASE}")
endif()

if(QRencode_LIBRARY_DEBUG)
    set_property(TARGET QRencode::QRencode
        APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(QRencode::QRencode
        PROPERTIES IMPORTED_LOCATION_DEBUG "${QRencode_LIBRARY_DEBUG}")
endif()

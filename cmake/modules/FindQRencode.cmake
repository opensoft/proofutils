find_path(QRencode_INCLUDE_DIRS NAMES qrencode.h
    HINTS $ENV{QRENCODE_DIR}
    PATH_SUFFIXES include/qrencode include
    PATHS ~/Library/Frameworks /Library/Frameworks /usr/local /usr
    DOC "QRencode include directory"
)

find_library(QRencode_LIBRARY_RELEASE NAMES qrencode
    HINTS $ENV{QRENCODE_DIR}
    PATH_SUFFIXES x86_64-linux-gnu
    PATHS /usr /usr/local
)
find_library(QRencode_LIBRARY_DEBUG NAMES qrencoded
    HINTS $ENV{QRENCODE_DIR}
    PATH_SUFFIXES x86_64-linux-gnu
    PATHS /usr /usr/local
)

include(SelectLibraryConfigurations)
select_library_configurations(QRencode)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QRencode
    REQUIRED_VARS QRencode_INCLUDE_DIRS QRencode_LIBRARY
)

if(NOT QRencode_FOUND)
    return()
endif()

if(NOT TARGET QRencode::QRencode)
    add_library(QRencode::QRencode INTERFACE IMPORTED)
endif()
set_property(TARGET QRencode::QRencode PROPERTY INTERFACE_LINK_LIBRARIES "${QRencode_LIBRARIES}")
set_property(TARGET QRencode::QRencode PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${QRencode_INCLUDE_DIRS}")

include(FeatureSummary)
set_package_properties(QRencode PROPERTIES
    URL "https://fukuchi.org/works/qrencode/"
    DESCRIPTION "The QRencode library"
)

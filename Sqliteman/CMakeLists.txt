#
# This is the base config/build file for Sqliteman
# for more info about available options read: README
#
OPTION(WANT_INTERNAL_SQLDRIVER "Use internal sqlite3 driver instead of system one. It enables more features (extensions, r-tree, etc.)" ON)
OPTION(WANT_DEBUG "Set the debug build and possible additional outputs" OFF)
OPTION(WANT_INTERNAL_QSCINTILLA "Use internal/bundled QScintilla2 source" OFF)
OPTION(WANT_BUNDLE "Enable Mac OS X bundle build" OFF)
OPTION(WANT_BUNDLE_STANDALONE "Do not copy required libs and tools into bundle (WANT_BUNDLE)" ON)


CMAKE_MINIMUM_REQUIRED( VERSION 2.6.0 )
# sets for bleeding edge cmake
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

#Pretty colors
SET(CMAKE_COLOR_MAKEFILE ON)
#Dont force verbose
SET(CMAKE_VERBOSE_MAKEFILE OFF)
#Include current dir
# SET(CMAKE_INCLUDE_CURRENT_DIR TRUE)
#Set the custom CMake module directory where our include/lib finders are
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

#PROJECT SETUP
PROJECT(sqliteman)
SET (VERSION_MAJOR "1")
SET (VERSION_MINOR "3")
SET (VERSION_PATCH "0")
SET (VERSION_SUFFIX "svn")
SET (VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${VERSION_SUFFIX})

SET (EXE_NAME "sqliteman")
IF (WANT_BUNDLE)
    SET (EXE_NAME "Sqliteman")
ENDIF (WANT_BUNDLE)

MESSAGE(STATUS "Sqliteman ${VERSION} will be installed into ${CMAKE_INSTALL_PREFIX}")

   IF (NOT LIB_SUFFIX)
       MESSAGE(STATUS "")
       MESSAGE(STATUS "LIB_SUFFIX variable is not defined. It will be autodetected now.")
       MESSAGE(STATUS "You can set it manually with -DLIB_SUFFIX=<value> (64 for example)")
       IF (NOT APPLE)
           # check 64 bit
           if (CMAKE_SIZEOF_VOID_P EQUAL 4)
               SET (LIB_SUFFIX "")
           else (CMAKE_SIZEOF_VOID_P EQUAL 4)
               set (LIB_SUFFIX "64")
               MESSAGE(STATUS "   LIB_SUFFIX is set to '${LIB_SUFFIX}'")
           endif (CMAKE_SIZEOF_VOID_P EQUAL 4)
       ELSE (NOT APPLE)
           SET (LIB_SUFFIX "")
       ENDIF (NOT APPLE)
       MESSAGE(STATUS "")
   ENDIF (NOT LIB_SUFFIX)

# extensions are switched off in some cases later
SET (ENABLE_EXTENSIONS 1)

# set platform dependent stuff - e.g. sqlite3 binary name
# NOTE: it's built with RESOURCES on Windows 
IF (WIN32)
#     SET (SQLITE_BIN "sqlite3.exe")
    SET (WANT_RESOURCES 1)
    #SET (ENABLE_EXTENSIONS 0)
    SET (WANT_INTERNAL_SQLDRIVER 1)
# ELSE (WIN32)
#     SET (SQLITE_BIN "sqlite3")
ENDIF (WIN32)

IF (APPLE)
    #SET (ENABLE_EXTENSIONS 0)
    SET (WANT_INTERNAL_SQLDRIVER 1)
ENDIF (APPLE)

IF (WANT_BUNDLE)
    SET (WANT_RESOURCES 1)
ENDIF (WANT_BUNDLE)

# set paths to install
IF (WANT_RESOURCES)
    SET (TRANSLATION_DIR ":/ts/")
    SET (DOC_DIR ":/doc/")
    SET (ICON_DIR ":/icons/")
ELSE (WANT_RESOURCES)
    SET (TRANSLATION_DIR "${CMAKE_INSTALL_PREFIX}/share/sqliteman/")
    SET (DOC_DIR "${CMAKE_INSTALL_PREFIX}/share/sqliteman/doc/")
    SET (ICON_DIR "${CMAKE_INSTALL_PREFIX}/share/sqliteman/icons")
ENDIF (WANT_RESOURCES)


# defines for compiler based on the previous setup
ADD_DEFINITIONS("-DTRANSLATION_DIR=\\\"${TRANSLATION_DIR}\\\"")
ADD_DEFINITIONS("-DDOC_DIR=\\\"${DOC_DIR}\\\"")
ADD_DEFINITIONS("-DICON_DIR=\\\"${ICON_DIR}\\\"")
# defines for compiler
# ADD_DEFINITIONS("-DSQLITE_BINARY=\\\"${SQLITE_BIN}\\\"")
ADD_DEFINITIONS("-DSQLITEMAN_VERSION=\\\"${VERSION}\\\"")
IF (WANT_RESOURCES)
    ADD_DEFINITIONS("-DWANT_RESOURCES")
ENDIF (WANT_RESOURCES)

IF (WANT_INTERNAL_SQLDRIVER)
    MESSAGE(STATUS "Sqliteman will be built with internal Sqlite driver.")
    ADD_DEFINITIONS("-DINTERNAL_SQLDRIVER")
    SET (ENABLE_EXTENSIONS 1)
ELSE (WANT_INTERNAL_SQLDRIVER)
    MESSAGE(STATUS "Sqliteman will be built with standard Qt4 Sqlite driver.")
ENDIF (WANT_INTERNAL_SQLDRIVER)

IF (DISABLE_SQLITE_EXTENSIONS)
    SET (ENABLE_EXTENSIONS 0)
ENDIF (DISABLE_SQLITE_EXTENSIONS)
IF (FORCE_SQLITE_EXTENSIONS)
    SET (ENABLE_EXTENSIONS 1)
ENDIF (FORCE_SQLITE_EXTENSIONS)
IF (ENABLE_EXTENSIONS)
    MESSAGE(STATUS "Sqliteman will be built with extension support.")
    ADD_DEFINITIONS("-DENABLE_EXTENSIONS")
    MESSAGE(STATUS "Sqliteman will be built with R-Tree support.")
    ADD_DEFINITIONS("-DSQLITE_ENABLE_RTREE=1")
ELSE (ENABLE_EXTENSIONS)
    MESSAGE(STATUS "Sqliteman will be built without extension support.")
ENDIF (ENABLE_EXTENSIONS)


#uninstall
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")


IF (WANT_DEBUG)
    SET (CMAKE_BUILD_TYPE Debug)
    MESSAGE (STATUS " *** Debug build ***")
ELSE(WANT_DEBUG)
    SET(CMAKE_BUILD_TYPE RELEASE)
    MESSAGE(STATUS " *** Release build (use -DWANT_DEBUG=1 for debug one) ***")
ENDIF(WANT_DEBUG)


# Lets disable Wall warnings due the MSVC too much talkative
# output. Wall can slow down compiletion up to hours and
# my boxes can boil eggs too.
# MSVC, MSVC_IDE, MSVC60, MSVC70, MSVC71, MSVC80, CMAKE_COMPILER_2005
IF (MSVC)
    SET (CXX_WARNINGS "")
ELSE (MSVC)
    SET (CXX_WARNINGS "-Wall")
ENDIF (MSVC)


SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 ${CXX_WARNINGS}")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g3 ${CXX_WARNINGS}")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 ${CXX_WARNINGS}")
SET(CMAKE_C_FLAGS_RELEASE "-O2 ${CXX_WARNINGS}")


SET (QT_MT_REQUIRED true)
SET (QT_MIN_VERSION "5.0.0")
SET( QT_USE_QTSQL TRUE )
SET( QT_USE_QTXML TRUE )
FIND_PACKAGE( Qt5 REQUIRED COMPONENTS Widgets Sql )

IF (WANT_INTERNAL_QSCINTILLA)
    MESSAGE(STATUS "QSci: Building with internal copy of QScintilla.")
    MESSAGE(STATUS "QSci: It's recomended to use external library for it")
ELSE (WANT_INTERNAL_QSCINTILLA)

    FIND_PACKAGE(QScintilla REQUIRED)
    IF (QSCINTILLA_FOUND)
        MESSAGE("QScintilla2 Found OK")
        ADD_DEFINITIONS("-DQT -DSCI_LEXER")
    ELSE (QSCINTILLA_FOUND)
        MESSAGE(STATUS "QSci: No QScintilla2 libraries found")
        MESSAGE(STATUS "QSci: You can use -DWANT_INTERNAL_QSCINTILLA=1 to build included copy of it.")
        MESSAGE(FATAL_ERROR "No QScintilla2 found")
    ENDIF (QSCINTILLA_FOUND)

ENDIF (WANT_INTERNAL_QSCINTILLA)


# FIND_PACKAGE (Sqlite)
# IF (SQLITE_FOUND)
# 	MESSAGE("Sqlite environment Found OK")
# 	ADD_DEFINITIONS("-DSQLITE_FOUND")
# ELSE (SQLITE_FOUND)
# 	MESSAGE(STATUS "No Sqlite environment found - support for loadable modules is skipped")
# ENDIF (SQLITE_FOUND)
# MESSAGE(STATUS "SQLITE_INCLUDE_DIR:  ${SQLITE_INCLUDE_DIR}")
# MESSAGE(STATUS "SQLITE_LIBRARIES:  ${SQLITE_LIBRARIES}")
# MESSAGE(STATUS "SQLITE_DEFINITIONS:  ${SQLITE_DEFINITIONS}")


ADD_SUBDIRECTORY( sqliteman )

IF (WIN32)
    MESSAGE(STATUS "Installation directories 'share' etc. aren't created in WIN32")
ELSE (WIN32)
    IF (NOT WANT_BUNDLE)
        # install desktop
        INSTALL(FILES
            sqliteman/icons/sqliteman.png
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor"
        )
        INSTALL(FILES
            sqliteman.desktop
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications"
        )
    ENDIF (NOT WANT_BUNDLE)
ENDIF (WIN32)


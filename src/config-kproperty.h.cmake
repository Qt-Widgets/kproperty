#ifndef KPROPERTY_CONFIG_H
#define KPROPERTY_CONFIG_H

/* config-kproperty.h. Generated by cmake from config-kproperty.h.cmake */

/*! @file config-kproperty.h
    Global KProperty configuration (build time)
*/

//! @def KPROPERTY_GIT_SHA1_STRING
//! @brief Indicates the git sha1 commit which was used for compilation of KProperty
#cmakedefine KPROPERTY_GIT_SHA1_STRING "@KPROPERTY_GIT_SHA1_STRING@"

//! @def BIN_INSTALL_DIR
//! @brief The subdirectory relative to the install prefix for executables.
#define BIN_INSTALL_DIR "${BIN_INSTALL_DIR}"

//! @def KPROPERTY_TESTING_EXPORT
//! @brief Export symbols for testing
#ifdef BUILD_TESTING
#  define KPROPERTY_TESTING_EXPORT KPROPERTY_EXPORT
#else
#  define KPROPERTY_TESTING_EXPORT
#endif

//! @def KPROPERTY_UNFINISHED
//! @brief Defined if unfinished features of KProperty are enabled
#cmakedefine KPROPERTY_UNFINISHED

#endif
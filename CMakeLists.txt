project(KDocumentStore)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${ECM_MODULE_PATH})

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Core Test )
find_package(KF5 REQUIRED COMPONENTS I18n Service Baloo)
find_package(Ejdb REQUIRED)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)
include(ECMAddTests)

set(PROJECT_VERSION "0.1")
ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX KDOCUMENTSTORE
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kdocumentstore_version.h"
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KDocumentStoreConfigVersion.cmake"
)

include_directories(
  ${CMAKE_SOURCE_DIR}/src
  ${EJDB_INCLUDE_DIRS}
)

# Compiler flags
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

# Targets
add_subdirectory(src)

# Config Files
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KDocumentStore")

ecm_configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KDocumentStoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KDocumentStoreConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KDocumentStoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KDocumentStoreConfigVersion.cmake"
    DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    COMPONENT devel
)

install(EXPORT KDocumentStoreTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KDocumentStoreTargets.cmake)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kdocumentstore_version.h"
    DESTINATION ${INCLUDE_INSTALL_DIR}
    COMPONENT Devel
)

feature_summary(WHAT ALL
    INCLUDE_QUIET_PACKAGES
    FATAL_ON_MISSING_REQUIRED_PACKAGES
)

add_subdirectory(autotest)

set(KVARIANTSTORE_LIB_SRCS
    kvariantstore.cpp
    kvariantcollection.cpp
    kvariantquery.cpp
)

add_library(kvariantstore SHARED ${KVARIANTSTORE_LIB_SRCS})
target_link_libraries(kvariantstore
    Qt5::Core
    ${EJDB_LIBRARIES}
)


set_target_properties(kvariantstore PROPERTIES
    VERSION ${KVARIANTSTORE_VERSION_STRING}
    SOVERSION ${KVARIANTSTORE_SOVERSION}
    EXPORT_NAME KVariantStore
)
target_include_directories(kvariantstore INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/KVariantStore>")
generate_export_header(kvariantstore BASE_NAME KVARIANTSTORE EXPORT_FILE_NAME kvariantstore_export.h)

ecm_generate_headers(KVariantStore_CamelCase_HEADERS
    HEADER_NAMES
    KVariantStore
    KVariantCollection
    KVariantQuery

    PREFIX kvariantstore
    REQUIRED_HEADERS KVariantStore_HEADERS
)

install(TARGETS kvariantstore EXPORT KVariantStoreTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kvariantstore_export.h
    ${KVariantStore_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/KVariantStore/kvariantstore
    COMPONENT Devel
)

install(FILES
    ${KVariantStore_CamelCase_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/KVariantStore/KVariantStore
    COMPONENT Devel
)

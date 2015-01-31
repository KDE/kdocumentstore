add_subdirectory(autotest)

set(KDOCUMENTSTORE_LIB_SRCS
    kdocumentstore.cpp
    kdocumentcollection.cpp
    kdocumentquery.cpp
)

add_library(kdocumentstore SHARED ${KDOCUMENTSTORE_LIB_SRCS})
target_link_libraries(kdocumentstore
    Qt5::Core
    ${EJDB_LIBRARIES}
)


set_target_properties(kdocumentstore PROPERTIES
    VERSION ${KDOCUMENTSTORE_VERSION_STRING}
    SOVERSION ${KDOCUMENTSTORE_SOVERSION}
    EXPORT_NAME KDocumentStore
)
target_include_directories(kdocumentstore INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/KDocumentStore>")
generate_export_header(kdocumentstore BASE_NAME KDOCUMENTSTORE EXPORT_FILE_NAME kdocumentstore_export.h)

ecm_generate_headers(KDocumentStore_CamelCase_HEADERS
    HEADER_NAMES
    KDocumentStore
    KDocumentCollection
    KDocumentQuery

    PREFIX kdocumentstore
    REQUIRED_HEADERS KDocumentStore_HEADERS
)

install(TARGETS kdocumentstore EXPORT KDocumentStoreTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kdocumentstore_export.h
    ${KDocumentStore_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/KDocumentStore/kdocumentstore
    COMPONENT Devel
)

install(FILES
    ${KDocumentStore_CamelCase_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/KDocumentStore/KDocumentStore
    COMPONENT Devel
)

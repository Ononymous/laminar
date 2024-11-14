set(HEADER_LIST "${uuid_SOURCE_DIR}/include/uuid/uuid.h" "${uuid_SOURCE_DIR}/include/uuid/uuidd.h" "${uuid_SOURCE_DIR}/include/uuid/uuidP.h")

add_library(uuid
        clear.c
        compare.c
        copy.c
        gen_uuid.c
        isnull.c
        pack.c
        parse.c
        unpack.c
        unparse.c
        uuid_time.c
        ${HEADER_LIST})

target_include_directories(uuid PUBLIC ../include)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
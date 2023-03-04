set(
    APP_DIRS
    bitonic_run
)

foreach(APP_DIR ${APP_DIRS})
    add_subdirectory(${APP_DIR})
endforeach()

list (APPEND APPS bitonic_run)

set(APPS "${APPS}" PARENT_SCOPE)
set(
    LIBS
    bitonic_sort
)

foreach(LIB ${LIBS})
    add_subdirectory(${LIB})
endforeach()


set(LIBS "${LIBS}" PARENT_SCOPE) 

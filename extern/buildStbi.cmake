set(STBI_IMAGE_SOURCE
    extern/stbi/stb_image.h
    extern/stbi/stb_image_write.h
)

add_library(stb_image SHARED
    ${STBI_IMAGE_SOURCE}
)

set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE C)
target_include_directories(stb_image PUBLIC "extern/stbi")

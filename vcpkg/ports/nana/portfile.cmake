vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

if(NOT VCPKG_TARGET_IS_WINDOWS)
    message(WARNING "You will need to install Xorg dependencies to use nana:\napt install libx11-dev libxft-dev libxcursor-dev\n")
endif()

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO cnjinhao/nana
    REF 25913a8319dbcb10d040b05e1e576b17665715e9 # v1.8
    SHA512 ff93cd650f4561a58efb5722dcefd0052b3814838c5ed37fa2572d0e44673144a092280d18c2ed98435842c356862813134ddb3f30efeac8a2263a462d0cac8f
    HEAD_REF develop-1.8
)

file(COPY "${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt" DESTINATION "${SOURCE_PATH}")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/config.cmake.in" DESTINATION "${SOURCE_PATH}")

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DNANA_ENABLE_PNG=ON
        -DNANA_ENABLE_JPEG=ON
    OPTIONS_DEBUG
        -DNANA_INSTALL_HEADERS=OFF)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME unofficial-nana)

vcpkg_copy_pdbs()

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
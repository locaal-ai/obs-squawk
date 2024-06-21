include(ExternalProject)

if(WIN32)
  # get bzip2 precompiled from https://github.com/philr/bzip2-windows use fetchcontent to download and extract the files
  include(FetchContent)
  FetchContent_Declare(
    bzip2 URL "https://github.com/philr/bzip2-windows/releases/download/v1.0.8.0/bzip2-dev-1.0.8.0-win-x64.zip")
  FetchContent_MakeAvailable(bzip2)
  # add the shared library to the IMPORTED library
  add_library(libbzip2 STATIC IMPORTED)
  target_include_directories(libbzip2 INTERFACE ${bzip2_SOURCE_DIR})
  set_property(TARGET libbzip2 PROPERTY IMPORTED_LOCATION ${bzip2_SOURCE_DIR}/libbz2-static.lib)

  set(LIBARCHIVE_VERSION 3.7.4)
  set(LIBARCHIVE_URL
      "https://github.com/libarchive/libarchive/releases/download/v${LIBARCHIVE_VERSION}/libarchive-v${LIBARCHIVE_VERSION}-amd64.zip"
  )

  include(FetchContent)
  FetchContent_Declare(libarchive-build URL ${LIBARCHIVE_URL})
  FetchContent_MakeAvailable(libarchive-build)

  # create an INTERFACE library to link against
  add_library(libarchive INTERFACE)
  add_dependencies(libarchive libarchive-build)
  target_include_directories(libarchive INTERFACE ${libarchive-build_SOURCE_DIR}/include)

  add_library(libarchive::libarchive SHARED IMPORTED)
  set_property(
    TARGET libarchive::libarchive
    PROPERTY IMPORTED_LOCATION
             ${libarchive-build_SOURCE_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}archive${CMAKE_SHARED_LIBRARY_SUFFIX})
  add_dependencies(libarchive::libarchive libarchive-build)
  if(WIN32)
    set_property(
      TARGET libarchive::libarchive
      PROPERTY IMPORTED_IMPLIB
               ${libarchive-build_SOURCE_DIR}/lib/${CMAKE_IMPORT_LIBRARY_PREFIX}archive${CMAKE_IMPORT_LIBRARY_SUFFIX})
    install(FILES ${libarchive-build_SOURCE_DIR}/bin/${CMAKE_SHARED_LIBRARY_PREFIX}archive${CMAKE_SHARED_LIBRARY_SUFFIX}
            DESTINATION ${CMAKE_SOURCE_DIR}/release/$<CONFIG>/obs-plugins/64bit)
  endif()

  target_link_libraries(libarchive INTERFACE libarchive::libarchive libbzip2)
else()
  if(APPLE)
    # Homebrew ships libarchive keg only, include dirs have to be set manually
    execute_process(
      COMMAND brew --prefix libarchive
      OUTPUT_VARIABLE LIBARCHIVE_PREFIX
      OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)
    set(LibArchive_INCLUDE_DIR "${LIBARCHIVE_PREFIX}/include")
    set(LibArchive_LIBRARIES "${LIBARCHIVE_PREFIX}/lib/libarchive.dylib")
  endif()

  find_package(LibArchive REQUIRED)
  find_package(BZip2 REQUIRED)

  add_library(libarchive INTERFACE)
  target_link_libraries(libarchive INTERFACE LibArchive::LibArchive BZip2::BZip2)
endif()

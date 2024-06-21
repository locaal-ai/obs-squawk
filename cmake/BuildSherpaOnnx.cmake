include(FetchContent)

set(SHERPA_ONNX_VERSION "1.10.0")
set(SHERPA_ONNX_BASE_URL "https://github.com/k2-fsa/sherpa-onnx/releases/download/v${SHERPA_ONNX_VERSION}")

if(MSVC)
  set(SHERPA_ONNX_DOWNLOAD_URL "${SHERPA_ONNX_BASE_URL}/sherpa-onnx-v${SHERPA_ONNX_VERSION}-win-x64-shared.tar.bz2")
elseif(APPLE)
  set(SHERPA_ONNX_DOWNLOAD_URL
      "${SHERPA_ONNX_BASE_URL}/sherpa-onnx-v${SHERPA_ONNX_VERSION}-osx-universal2-shared.tar.bz2")
else()
  set(SHERPA_ONNX_DOWNLOAD_URL "${SHERPA_ONNX_BASE_URL}/sherpa-onnx-v${SHERPA_ONNX_VERSION}-linux-x64-shared.tar.bz2")
endif()

FetchContent_Declare(sherpa-onnx-build URL ${SHERPA_ONNX_DOWNLOAD_URL})

FetchContent_MakeAvailable(sherpa-onnx-build)

set(INSTALL_DIR ${sherpa-onnx-build_SOURCE_DIR})

set(SHERPA_LIBS
    espeak-ng
    kaldi-decoder-core
    kaldi-native-fbank-core
    piper_phonemize
    sherpa-onnx-c-api
    sherpa-onnx-core
    sherpa-onnx-fst
    sherpa-onnx-fstfar
    sherpa-onnx-kaldifst-core
    ssentencepiece_core
    ucd)

foreach(lib ${SHERPA_LIBS})
  add_library(${lib} SHARED IMPORTED)
  set_property(
    TARGET ${lib} PROPERTY IMPORTED_LOCATION
                           ${INSTALL_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${lib}${CMAKE_SHARED_LIBRARY_SUFFIX})
  # add implib for windows, except for onnxruntime and onnxruntime_providers_shared
  if(WIN32)
    set_property(
      TARGET ${lib} PROPERTY IMPORTED_IMPLIB
                             ${INSTALL_DIR}/lib/${CMAKE_IMPORT_LIBRARY_PREFIX}${lib}${CMAKE_IMPORT_LIBRARY_SUFFIX})
  endif()
  set_property(TARGET ${lib} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)

  # install the dll to the project release directory: ${CMAKE_BINARY_DIR}/$<CONFIG>/obs-plugins/64bit
  if(WIN32)
    install(FILES ${INSTALL_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${lib}${CMAKE_SHARED_LIBRARY_SUFFIX}
            DESTINATION ${CMAKE_SOURCE_DIR}/release/$<CONFIG>/obs-plugins/64bit)
  endif()
endforeach()

if(WIN32)
  # install the onnxruntime dlls to the project release directory
  install(FILES ${INSTALL_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}onnxruntime${CMAKE_SHARED_LIBRARY_SUFFIX}
          DESTINATION ${CMAKE_SOURCE_DIR}/release/$<CONFIG>/obs-plugins/64bit)
  install(
    FILES ${INSTALL_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}onnxruntime_providers_shared${CMAKE_SHARED_LIBRARY_SUFFIX}
    DESTINATION ${CMAKE_SOURCE_DIR}/release/$<CONFIG>/obs-plugins/64bit)
endif()

# Add the sherpa-onnx target with all the dependencies
add_library(sherpa-onnx INTERFACE)
target_link_libraries(sherpa-onnx INTERFACE ${SHERPA_LIBS})

# Add the include directories
target_include_directories(sherpa-onnx INTERFACE ${INSTALL_DIR}/include)

# Add the dependency
add_dependencies(sherpa-onnx sherpa-onnx-build)

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

if(WIN32)
  set(SHARED_LIBRARY_DESTINATION ${CMAKE_SOURCE_DIR}/release/$<CONFIG>/obs-plugins/64bit)
elseif(APPLE)
  set(SHARED_LIBRARY_DESTINATION ${CMAKE_SOURCE_DIR}/release/$<CONFIG>/${PROJECT_NAME}.plugin/Contents/Frameworks)
else()
  set(SHARED_LIBRARY_DESTINATION ${CMAKE_SOURCE_DIR}/release/$<CONFIG>/${PROJECT_NAME}/64bit)
endif()

foreach(lib ${SHERPA_LIBS})
  add_library(${lib} SHARED IMPORTED)
  set(SHARED_LIBRARY_FILE_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}${lib}${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(SHARED_LIBRARY_FILE_LOCATION ${INSTALL_DIR}/lib/${SHARED_LIBRARY_FILE_NAME})
  set_property(TARGET ${lib} PROPERTY IMPORTED_LOCATION ${SHARED_LIBRARY_FILE_LOCATION})
  # add implib for windows, except for onnxruntime and onnxruntime_providers_shared
  if(WIN32)
    set_property(
      TARGET ${lib} PROPERTY IMPORTED_IMPLIB
                             ${INSTALL_DIR}/lib/${CMAKE_IMPORT_LIBRARY_PREFIX}${lib}${CMAKE_IMPORT_LIBRARY_SUFFIX})
  endif()
  set_property(TARGET ${lib} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)

  if(APPLE)
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE "${SHARED_LIBRARY_FILE_LOCATION}")
    set_property(SOURCE "${SHARED_LIBRARY_FILE_LOCATION}" PROPERTY MACOSX_PACKAGE_LOCATION Frameworks)
    source_group("Frameworks" FILES "${SHARED_LIBRARY_FILE_LOCATION}")
    add_custom_command(
      TARGET "${CMAKE_PROJECT_NAME}"
      POST_BUILD
      COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change "@rpath/${SHARED_LIBRARY_FILE_NAME}"
              "@loader_path/../Frameworks/${SHARED_LIBRARY_FILE_NAME}" $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
  else()
    # install the shared lib to the project release directory
    install(FILES ${SHARED_LIBRARY_FILE_LOCATION} DESTINATION ${SHARED_LIBRARY_DESTINATION})
  endif()
endforeach()

# install the onnxruntime shared libs to the project release directory
install(FILES ${INSTALL_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}onnxruntime${CMAKE_SHARED_LIBRARY_SUFFIX}
        DESTINATION ${SHARED_LIBRARY_DESTINATION})
if(WIN32)
  install(
    FILES ${INSTALL_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}onnxruntime_providers_shared${CMAKE_SHARED_LIBRARY_SUFFIX}
    DESTINATION ${SHARED_LIBRARY_DESTINATION})
endif()
if(APPLE)
  foreach(SHARED_LIBRARY_FILE_NAME libpiper_phonemize.1.dylib libpiper_phonemize.1.2.0.dylib
                                   libonnxruntime.1.17.1.dylib)
    set(SHARED_LIBRARY_FILE_LOCATION ${INSTALL_DIR}/lib/${SHARED_LIBRARY_FILE_NAME})
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE "${SHARED_LIBRARY_FILE_LOCATION}")
    set_property(SOURCE "${SHARED_LIBRARY_FILE_LOCATION}" PROPERTY MACOSX_PACKAGE_LOCATION Frameworks)
    source_group("Frameworks" FILES "${SHARED_LIBRARY_FILE_LOCATION}")
    add_custom_command(
      TARGET "${CMAKE_PROJECT_NAME}"
      POST_BUILD
      COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change "@rpath/${SHARED_LIBRARY_FILE_NAME}"
              "@loader_path/../Frameworks/${SHARED_LIBRARY_FILE_NAME}" $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
  endforeach()
endif()

# Add the sherpa-onnx target with all the dependencies
add_library(sherpa-onnx INTERFACE)
target_link_libraries(sherpa-onnx INTERFACE ${SHERPA_LIBS})

# Add the include directories
target_include_directories(sherpa-onnx INTERFACE ${INSTALL_DIR}/include)

# Add the dependency
add_dependencies(sherpa-onnx sherpa-onnx-build)

cmake_minimum_required(VERSION 3.16)
project(biliardo VERSION 0.1.1)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast")

# abilita asserzioni di debug (in gcc), l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")

# richiedi il componente graphics della libreria SFML, versione 2.5
#   le dipendenze vengono identificate automaticamente
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(ROOT REQUIRED COMPONENTS Hist Gpad)
find_package(TGUI 1.0 REQUIRED)
# execute_process((COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CFLAGS))



add_executable( biliardo main.cpp geometrical_methods.cpp graphic.cpp setup.cpp)

# target_compile_options(bilardo PRIVATE ${ROOT_CFLAGS})
target_link_libraries( biliardo PRIVATE sfml-graphics sfml-window sfml-system tgui ROOT::Hist ROOT::Gpad)

# se il testing e' abilitato...

#   per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_TESTING)

  # aggiungi l'eseguibile all.t
  add_executable(biliardo_test test.cpp geometrical_methods.cpp graphic.cpp setup.cpp)
 target_link_libraries(biliardo_test PRIVATE  sfml-graphics sfml-window sfml-system tgui ROOT::Hist ROOT::Gpad)
  # # aggiungi l'eseguibile all.t alla lista dei test
  add_test(NAME biliardo_test COMMAND biliardo_test)

endif()
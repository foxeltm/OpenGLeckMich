cmake_minimum_required (VERSION 3.8)

# 1. Projekt-Definition
project ("OpenGLeckMich")

# 2. MSVC Hot Reload Policy (Optional, aber gut platziert)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# 3. Globale Pfad-Einstellungen
# Hinweis: Nutze ${...} statt $(...). CMake setzt Build-Files standardmäßig in den "Build"-Ordner.
# Wenn du die .exe unbedingt im /out Ordner haben willst:
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out")

# 4. Sub-Projekte hinzufügen
# WICHTIG: Die Bibliotheken (vendor) zuerst, damit dein Hauptprojekt sie kennt.
add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glew-cmake")
add_subdirectory("OpenGLeckMich")

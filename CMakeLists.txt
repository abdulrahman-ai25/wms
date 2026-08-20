cmake_minimum_required(VERSION 3.16)

project(wms
    VERSION 0.1.0
    LANGUAGES C CXX
    DESCRIPTION "Warehouse Management System"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Global C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Third-party Dependencies (SQLite3 & PicoSHA2)
set(SQLITE3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/sqlite3")
set(PICOSHA2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/picosha2")

if(EXISTS "${SQLITE3_DIR}/sqlite3.c")
    add_library(sqlite3_lib STATIC "${SQLITE3_DIR}/sqlite3.c")
    target_include_directories(sqlite3_lib PUBLIC "${SQLITE3_DIR}")
endif()
if(EXISTS "${PICOSHA2_DIR}/picosha2.h")
    add_library(picosha2_lib INTERFACE)
    target_include_directories(picosha2_lib INTERFACE "${PICOSHA2_DIR}")
endif()

# 2. Main Executable Target
add_executable(${PROJECT_NAME} 
    src/main.cpp
   
    )
    
    # Include paths for all module headers
    target_include_directories(${PROJECT_NAME} PRIVATE 
    src 
    src/database 
    src/models 
    src/repositories 
    src/services
    src/tui
    )

# 3. Add Sources per Module using target_sources


# --- Module: Repositories ---
target_sources(${PROJECT_NAME} PRIVATE
    src/repositories/user_repository.cpp
    src/repositories/product_repository.cpp
    src/repositories/category_repository.cpp
)

# --- Module: Database ---
target_sources(${PROJECT_NAME} PRIVATE
    src/database/database.cpp
    src/database/migration_manager.cpp
    )
    
# --- Module: services
target_sources(${PROJECT_NAME} PRIVATE
src/services/auth_service.cpp
    src/services/session.cpp
    src/services/users_service.cpp
    src/services/category_service.cpp
    src/services/product_service.cpp
    )

# --- Module: TUI ---
target_sources(${PROJECT_NAME} PRIVATE
    src/tui/app.cpp
    src/tui/auth_menu.cpp
    src/tui/users_menu.cpp
    src/tui/category_menu.cpp
    src/tui/product_menu.cpp
)

# 4. Link Libraries
if(TARGET sqlite3_lib)
    target_link_libraries(${PROJECT_NAME} PRIVATE sqlite3_lib)
endif()
if(TARGET picosha2_lib)
    target_link_libraries(${PROJECT_NAME} PRIVATE picosha2_lib)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "WMS")

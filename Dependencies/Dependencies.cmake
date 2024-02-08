#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# Author: Inan Evin
# www.inanevin.com
# 
# Copyright (C) 2022 Inan Evin
# 
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions 
# and limitations under the License.
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

set(LINAGX_FOLDER_BASE LinaEngine/Dependencies/LinaGXProject)
set(LINAVG_FOLDER_BASE LinaEngine/Dependencies/LinaVGProject)
set(LINAGX_ITERATOR_DEBUG_LEVEL ${LINA_ITERATOR_DEBUG_LEVEL})
set(LINAGX_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>)

add_subdirectory(LinaGX)
add_subdirectory(LinaVG)
target_compile_definitions(LinaGX PUBLIC _ITERATOR_DEBUG_LEVEL=${LINA_ITERATOR_DEBUG_LEVEL})
target_compile_definitions(LinaVG PUBLIC _ITERATOR_DEBUG_LEVEL=${LINA_ITERATOR_DEBUG_LEVEL})

add_subdirectory(Dependencies/memory-allocators)
add_subdirectory(Dependencies/lz4-1.9.4)
add_subdirectory(Dependencies/fmt-7.0.3)
add_subdirectory(Dependencies/EASTL)
add_subdirectory(Dependencies/glm-0.9.9.8/glm)

set_property(TARGET lz4 PROPERTY FOLDER LinaEngine/Dependencies)
set_property(TARGET memory-allocators PROPERTY FOLDER LinaEngine/Dependencies)
set_property(TARGET fmt PROPERTY FOLDER LinaEngine/Dependencies)
set_property(TARGET EASTL PROPERTY FOLDER LinaEngine/Dependencies)
set_property(TARGET glm_static PROPERTY FOLDER LinaEngine/Dependencies)
target_compile_definitions(lz4 PUBLIC _ITERATOR_DEBUG_LEVEL=${LINA_ITERATOR_DEBUG_LEVEL})
target_compile_definitions(memory-allocators PUBLIC _ITERATOR_DEBUG_LEVEL=${LINA_ITERATOR_DEBUG_LEVEL})
target_compile_definitions(fmt PUBLIC _ITERATOR_DEBUG_LEVEL=${LINA_ITERATOR_DEBUG_LEVEL})
target_compile_definitions(EASTL PUBLIC _ITERATOR_DEBUG_LEVEL=${LINA_ITERATOR_DEBUG_LEVEL})
target_compile_definitions(glm_static PUBLIC _ITERATOR_DEBUG_LEVEL=${LINA_ITERATOR_DEBUG_LEVEL})

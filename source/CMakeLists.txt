
# game sources
set(STARRULER2_SRC
	game/as/as_jit.cpp
	game/design/design.cpp
	game/design/effect.cpp
	game/design/effector.cpp
	game/design/effector_functions.cpp
	game/design/hull.cpp
	game/design/subsystem.cpp
	game/design/projectiles.cpp
	game/gui/skin.cpp
	game/main/initialization.cpp
	game/main/input_handling.cpp
	game/main/logging.cpp
	game/main/profiler.cpp
	game/main/references.cpp
	game/main/tick.cpp
	game/main/console.cpp
	game/main/save_load.cpp
	game/mods/mod_manager.cpp
	game/network/network_manager.cpp
	game/obj/blueprint.cpp
	game/obj/lock.cpp
	game/obj/object.cpp
	game/obj/obj_group.cpp
	game/obj/universe.cpp
	game/obj/object_saving.cpp
	game/physics/physics_world.cpp
	game/profile/keybinds.cpp
	game/profile/settings.cpp
	game/render/camera.cpp
	game/render/font_fnt.cpp
	game/render/font_ft2.cpp
	game/render/gl_driver.cpp
	game/render/gl_framebuffer.cpp
	game/render/gl_mesh.cpp
	game/render/gl_shader.cpp
	game/render/gl_texture.cpp
	game/render/gl_vertexBuffer.cpp
	game/render/lighting.cpp
	game/render/obj_loader.cpp
	game/render/ogex_loader.cpp
	game/render/render_state.cpp
	game/render/spritesheet.cpp
	game/render/x_loader.cpp
	game/render/bmf_loader.cpp
	game/render/shader_states.cpp
	game/resource/library.cpp
	game/resource/load_font.cpp
	game/resource/load_material.cpp
	game/resource/load_model.cpp
	game/resource/load_shader.cpp
	game/resource/load_skin.cpp
	game/resource/load_sound.cpp
	game/resource/locale.cpp
	game/resource/hot_loading.cpp
	game/scene/animation/anim_linear.cpp
	game/scene/animation/anim_group.cpp
	game/scene/animation/anim_node_sync.cpp
	game/scene/animation/anim_projectile.cpp
	game/scene/particle_system.cpp
	game/scene/billboard_node.cpp
	game/scene/beam_node.cpp
	game/scene/line_trail_node.cpp
	game/scene/frame_line.cpp
	game/scene/mesh_node.cpp
	game/scene/plane_node.cpp
	game/scene/mesh_icon_node.cpp
	game/scene/icon_node.cpp
	game/scene/scripted_node.cpp
	game/scene/culling_node.cpp
	game/scene/node.cpp
	game/scripts/bind_creation.cpp
	game/scripts/bind_data.cpp
	game/scripts/bind_design.cpp
	game/scripts/bind_empire.cpp
	game/scripts/bind_general.cpp
	game/scripts/bind_gui.cpp
	game/scripts/bind_inspection.cpp
	game/scripts/bind_joystick.cpp
	game/scripts/bind_menu.cpp
	game/scripts/bind_object.cpp
	game/scripts/bind_profile.cpp
	game/scripts/bind_render.cpp
	game/scripts/bind_threading.cpp
	game/scripts/bind_events.cpp
	game/scripts/bind_formula.cpp
	game/scripts/bind_sound.cpp
	game/scripts/bind_dynamic.cpp
	game/scripts/bind_network.cpp
	game/scripts/bind_savefile.cpp
	game/scripts/bind_datafile.cpp
	game/scripts/bind_json.cpp
	game/scripts/bind_web.cpp
	game/scripts/bind_irc.cpp
	game/scripts/binds.cpp
	game/scripts/context_cache.cpp
	game/scripts/manager.cpp
	game/scripts/script_bind.cpp
	game/scripts/generic_call.cpp
	game/scripts/script_type.cpp
	game/scripts/script_components.cpp
	game/scripts/script_hooks.cpp
	game/util/format.cpp
	game/util/formula.cpp
	game/util/generic.cpp
	game/util/mesh_generation.cpp
	game/util/random.cpp
	game/util/threaded_loader.cpp
	game/util/elevation_map.cpp
	game/util/name_generator.cpp
	game/util/stat_history.cpp
	game/util/bbcode.cpp
	game/util/save_file.cpp
	game/empire.cpp
	game/empire_stats.cpp
	game/general_states.cpp
	game/processing.cpp
	game/main.cpp
)

if (USE_GLFW)
	set(STARRULER2_GLFW_SRC
		game/os/glfw_driver.cpp
	)

	include_directories(starruler2
		${GLFW_INCLUDE_DIR}
	)
endif (USE_GLFW)

if (USE_SDL2)
	set(STARRULER2_SDL2_SRC
		game/os/sdl2_driver.cpp
	)

	include_directories(starruler2
		${SDL2_INCLUDE_DIR}
	)
endif (USE_SDL2)

include_directories(starruler2
    os/include
    game
    util/include
    network/include
    sound/include
    as_addons/include
    rapidjson/include
    libircclient/include
    ${FREETYPE_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

if(STARRULER2_BUILD_ANGELSCRIPT)
    include_directories(starruler2 angelscript/include)
endif(STARRULER2_BUILD_ANGELSCRIPT)

add_executable(starruler2 ${STARRULER2_SRC} ${STARRULER2_GLFW_SRC} ${STARRULER2_SDL2_SRC})

target_link_libraries(starruler2
    ${FREETYPE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${CURL_LIBRARIES}
    ircclient
    starruler2_as_addons
    starruler2_network
    starruler2_sound
)

if (USE_GLFW)
	target_link_libraries(starruler2
		${GLFW_LIBRARIES}
	)
endif (USE_GLFW)

if (USE_SDL2)
	target_link_libraries(starruler2
		${SDL2_LIBRARY}
	)
endif (USE_SDL2)

install(
    TARGETS starruler2
    RUNTIME DESTINATION bin
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
subdir('animation')
subdir('paint')
subdir('path')

source_file = [
       'actor.cpp',
       'actor_artboard.cpp',
       'actor_axis_constraint.cpp',
       'actor_bone.cpp',
       'actor_bone_base.cpp',
       'actor_collider.cpp',
       'actor_component.cpp',
       'actor_constraint.cpp',
       'actor_distance_constraint.cpp',
       'actor_drawable.cpp',
       'actor_drawable_proxy.cpp',
       'actor_event.cpp',
       'actor_ik_constraint.cpp',
       'actor_image.cpp',
       'actor_instance.cpp',
       'actor_jelly_bone.cpp',
       'actor_node.cpp',
       'actor_node_solo.cpp',
       'actor_procedural_path.cpp',
       'actor_root_bone.cpp',
       'actor_rotation_constraint.cpp',
       'actor_scale_constraint.cpp',
       'actor_shape.cpp',
       'actor_skin.cpp',
       'actor_skinnable.cpp',
       'actor_static_mesh.cpp',
       'actor_targeted_constraint.cpp',
       'actor_transform_constraint.cpp',
       'actor_translation_constraint.cpp',
       'binary_reader.cpp',
       'block_reader.cpp',
       'cubic_path_point.cpp',
       'custom_property.cpp',
       'dependency_sorter.cpp',
       'jelly_component.cpp',
       'nested_actor_asset.cpp',
       'nested_actor_node.cpp'
]

flare_dep = declare_dependency(
   include_directories : headers,
   dependencies : [animation_dep, paint_dep, path_dep],
   sources : source_file
)

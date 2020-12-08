subdir('interpolators')
subdir('keyframes')

animation_src = [
   'actor_animation.cpp',
   'actor_animation_instance.cpp',
   'component_animation.cpp',
   'property_animation.cpp'
]

animation_dep = declare_dependency(
   include_directories : headers,
   dependencies : [interpolation_dep, keyframe_dep],
   sources : animation_src
)


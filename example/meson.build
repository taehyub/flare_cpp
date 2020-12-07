source_file = [
   'layout.hpp',
   'main.cpp'
]

#flare_dep = dependency('flare', required : true)
thorvg_dep = dependency('thorvg', required : true)
elementary_dep = dependency('elementary', required : true)

executable('flare',
           source_file,
           dependencies : [elementary_dep, thorvg_dep, flare_lib_dep],
           install : true)

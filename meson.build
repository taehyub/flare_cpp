project('flare',
        'cpp',
        version : '0.1.0',
        license : 'MIT')

subdir('lib')

thorvg_dep = dependency('thorvg', required : true)

flare_lib = library(
	'flare',
	include_directories : [headers, math_headers, thorvg_headers],
	version             : meson.project_version(),
	dependencies        : [math_dep, flare_dep, flare_render_dep, thorvg_dep],
	install             : true,
        gnu_symbol_visibility : 'hidden',
)

flare_lib_dep = declare_dependency(
	include_directories : [headers, math_headers, thorvg_headers],
        link_with : flare_lib
)

pkg_mod = import('pkgconfig')

pkg_mod.generate(
        libraries    : flare_lib,
        version      : meson.project_version(),
        name         : 'libflare',
        filebase     : 'flare',
        description  : 'A Flare Animation library'
)

subdir('example')
subdir('assets')


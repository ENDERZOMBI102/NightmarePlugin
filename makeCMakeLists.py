from pathlib import Path

sources = {
    'Hud': [],
    'Main': [],
    'Modules': [],
    'Utils': []
}

for file in Path('./src').glob('*'):
    if file.is_dir():
        for file2 in file.glob('*'):
            sources[file.name].append(str(file2))
    else:
        sources['Main'].append( str( file ) )

base = '\n\t'
CMakeLists = f'''
cmake_minimum_required(VERSION 3.17)
project(NightmarePlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(
    NightmarePlugin
    SHARED
    # main sources
    {base.join( sources["Main"] )}
    # Hud
    {base.join( sources["Hud"] )}
    # Modules
    {base.join( sources["Modules"] )}
    # Utils
    {base.join( sources["Utils"] )}
'''

CMakeLists = f'{CMakeLists})\n'.replace('\\', '/')

Path('./CMakeLists.txt').write_text(CMakeLists)

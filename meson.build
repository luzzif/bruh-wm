project('bruh-wm', 'c')

sources = [
    'src/includes/events.h',
    'src/implementations/events.c',
    'src/includes/utils.h',
    'src/implementations/utils.c',
    'src/includes/clients.h',
    'src/implementations/clients.c',
    'src/includes/cursor.h',
    'src/implementations/cursor.c',

    'src/implementations/main.c'
]
executable(
    'bruh-wm',
    sources: sources,
    dependencies: [dependency('x11'), dependency('cairo')]
)
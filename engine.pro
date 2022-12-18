QT += widgets
requires(qtConfig(filedialog))

HEADERS        = mainwindow.h     \
                 componentsList.h \
                 enginewidget.h
RESOURCES      = engine.qrc
SOURCES        = main.cpp           \
                 window.cpp         \
                 componentslist.cpp \
                 enginewidget.cpp

QMAKE_PROJECT_NAME = vessel

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widget/draganddrop/engine
INSTALLS += target

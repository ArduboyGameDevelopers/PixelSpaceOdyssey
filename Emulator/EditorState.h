#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "Level.h"

typedef struct _EditorState {
    Level* level;
    int tileIndex;
} EditorState;

extern EditorState editorState;

#endif // EDITORSTATE_H

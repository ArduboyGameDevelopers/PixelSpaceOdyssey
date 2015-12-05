//
//  EditorState.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef EditorState_h
#define EditorState_h

#include "Level.h"

typedef struct _EditorState {
    Level* level;
    int tileIndex;
} EditorState;

extern EditorState editorState;

#endif /* EditorState_h */

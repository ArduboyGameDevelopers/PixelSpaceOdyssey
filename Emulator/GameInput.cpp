#include "GameInput.h"

#include <QKeyEvent>

#include "Arduboy.h"

int getButtonFromKey(int key)
{
    switch (key)
    {
        case Qt::Key_W:
        case Qt::Key_Up:
            return UP_BUTTON;

        case Qt::Key_A:
        case Qt::Key_Left:
            return LEFT_BUTTON;

        case Qt::Key_S:
        case Qt::Key_Down:
            return DOWN_BUTTON;

        case Qt::Key_D:
        case Qt::Key_Right:
            return RIGHT_BUTTON;

        case Qt::Key_N:
        case Qt::Key_Alt:
            return A_BUTTON;

        case Qt::Key_M:
        case Qt::Key_Space:
            return B_BUTTON;
    }

    return 0;
}

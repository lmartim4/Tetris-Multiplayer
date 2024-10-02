#ifndef TETRIS_ACTIONS_HPP
#define TETRIS_ACTIONS_HPP

enum class TetrisAction {
    START,
    RESTART,
    PAUSE,
    RESUME,
    LEFT,
    RIGHT,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    DROP_FASTER,
    DROP_INSTANT
};

inline const char* TetrisActionToString(TetrisAction action) {
    switch (action) {
    case TetrisAction::START: return "START";
    case TetrisAction::RESTART: return "RESTART";
    case TetrisAction::PAUSE: return "PAUSE";
    case TetrisAction::RESUME: return "RESUME";
    case TetrisAction::LEFT: return "LEFT";
    case TetrisAction::RIGHT: return "RIGHT";
    case TetrisAction::ROTATE_LEFT: return "ROTATE_LEFT";
    case TetrisAction::ROTATE_RIGHT: return "ROTATE_RIGHT";
    case TetrisAction::DROP_FASTER: return "DROP_FASTER";
    case TetrisAction::DROP_INSTANT: return "DROP_INSTANT";
    default: return "UNKNOWN_ACTION";
    }
}

#endif // TETRIS_ACTIONS_HPP

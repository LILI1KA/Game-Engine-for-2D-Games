#ifndef END_STATE_H
#define END_STATE_H

enum class EndStateType { Win, Lose, Continue };

struct EndState {
    EndStateType type;
    EndState() { type = EndStateType::Continue; }
    EndState( EndStateType type ): type{type} {}
};


#endif

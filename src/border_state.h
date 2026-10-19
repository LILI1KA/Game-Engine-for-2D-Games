enum class BorderStateType { Solid, View };

struct BorderState {
    BorderStateType type;
    BorderState() { type = BorderStateType::View; }
    BorderState( BorderStateType type ): type{type} {}
};

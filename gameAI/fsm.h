#pragma once
#include <stdio.h>
#include <memory>
#include "common.h"

struct State
{
    State(void (*on_enter)(), void (*on_state)(), void (*on_exit)());
    void (*on_enter)();
    void (*on_state)();
    void (*on_exit)();
};

class Fsm
{
public:
    Fsm(State* init_state);
    ~Fsm();

    void add_transition(State* state_from, State* state_to, int event, void (*on_transition)());
    void add_timed_transition(State* state_from, State* state_to, unsigned long interval, void (*on_transition)());
    void check_timed_transition();
    void trigger(int event);
    void run_machine();
private:
    struct Transition
    {
        State* state_from;
        State* state_to;
        int event;
        void(*on_transition)();
    };

    struct TimedTransition
    {
        Transition transition;
        int64_t start;
        int64_t interval;
    };

    static Transition create_transition(State* state_from, State* state_to, int event, void(*on_transition)());
    void make_transition(Transition* transition);

private:
    State* m_current_state = nullptr;
    Transition* m_transitions = nullptr;
    int64_t m_num_transitions = 0;

    TimedTransition* m_timed_transitions = nullptr;
    int64_t m_num_timed_transitions = 0;
    bool m_initialized = false;
};

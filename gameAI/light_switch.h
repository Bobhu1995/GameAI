#pragma once
#include <windows.h>
#include <stdio.h>
#include "fsm.h"

#define FLIP_LIGHT_ON 1
#define FLIP_LIGHT_CLOSE 2

void on_light_on_enter()
{
    simple_log("Entering LIGHT_ON");
}

void on_light_on_exit()
{
    simple_log("Exiting LIGHT_ON");
}

void on_light_off_enter()
{
    simple_log("Entering LIGHT_OFF");
}

void on_light_off_exit()
{
    simple_log("Exiting LIGHT_OFF");
}

void on_trans_light_on_light_off()
{
    simple_log("Transitioning from LIGHT_ON to LIGHT_OFF");
}

void on_trans_light_off_light_on()
{
    simple_log("Transitioning from LIGHT_OFF to LIGHT_ON");
}

State state_light_on(on_light_on_enter, nullptr, &on_light_on_exit);
State state_light_off(on_light_on_enter, nullptr, &on_light_off_exit);
Fsm fsm(&state_light_on);

void init()
{
    simple_log("begin");
    fsm.add_transition(&state_light_on, &state_light_off, FLIP_LIGHT_CLOSE, &on_trans_light_on_light_off);
    fsm.add_transition(&state_light_off, &state_light_on, FLIP_LIGHT_ON, &on_trans_light_off_light_on);
    fsm.add_timed_transition(&state_light_on, &state_light_off, 2000, &on_trans_light_on_light_off);
    fsm.add_timed_transition(&state_light_off, &state_light_on, 3000, &on_trans_light_off_light_on);
    fsm.run_machine();
}

void loop()
{
    // 定时轮询
    fsm.check_timed_transition();

    // 主动触发
    // fsm.trigger(FLIP_LIGHT_CLOSE);
    // fsm.trigger(FLIP_LIGHT_ON);
}
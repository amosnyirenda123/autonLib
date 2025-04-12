#include "./include/automaton.h"
#include "./include/auton_memory.h"
#include <string.h>

int main(void){

    a_automaton_t* automaton = auton_init_automaton(2, 2);

    automaton->states[0] = auton_init_state("q0", 1, 1);
    automaton->states[1] = auton_init_state("q1", 0, 0);

    automaton->alphabet->members[0] = strdup("a");
    automaton->alphabet->members[1] = strdup("b");

    auton_add_transition(automaton->states[0], automaton->states[1], "b");
    auton_add_transition(automaton->states[0], automaton->states[0], "c");
    auton_add_transition(automaton->states[1], automaton->states[0], "b");

    auton_generate_dotfile(automaton, "example.txt");
    return 0;
}
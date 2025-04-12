#include "./include/automaton.h"
#include "./include/auton_memory.h"
#include <string.h>

int main(void){

    a_automaton_t* automaton = auton_init_automaton(4, 3); 
    // Initialize states
    automaton->states[0] = auton_init_state("q0", 1, 0); 
    automaton->states[1] = auton_init_state("q1", 1, 1); 
    automaton->states[2] = auton_init_state("q2", 0, 1); 
    automaton->states[3] = auton_init_state("q3", 0, 0);

    // Define alphabet
    automaton->alphabet->members[0] = strdup("a");
    automaton->alphabet->members[1] = strdup("b");
    automaton->alphabet->members[2] = strdup("c");

    // Transitions
    auton_add_transition(automaton->states[0], automaton->states[1], "a");
    auton_add_transition(automaton->states[0], automaton->states[2], "a"); 
    auton_add_transition(automaton->states[1], automaton->states[3], "b");
    auton_add_transition(automaton->states[1], automaton->states[0], "a");
    auton_add_transition(automaton->states[2], automaton->states[2], "b"); 
    auton_add_transition(automaton->states[3], automaton->states[2], "c");
    auton_add_transition(automaton->states[3], automaton->states[0], "a");

    // Optional weird case: unreachable state
    auton_add_transition(automaton->states[3], automaton->states[3], "c"); 

    // Generate file
    auton_generate_jsonfile(automaton, "complex_example.json");
    return 0;
}
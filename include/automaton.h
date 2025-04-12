#ifndef AUTOMATON_H
#define AUTOMATON_H
#include <stdio.h>
#define STATE_REPR_LENGTH 10

typedef struct a_state_t a_state_t;
typedef struct a_t_node a_t_node;

typedef struct {
    char** members;
    int num_members;
}a_alphabet_t;


typedef struct a_t_node {
    char* input_symbol;
    a_state_t* from;
    a_state_t* to;
    a_t_node* next;

}a_t_node;

typedef struct a_state_t{
    char repr[STATE_REPR_LENGTH];
    int is_starting_state;
    int is_ending_state;
    a_t_node* transitions;
    int out_transitions;
}a_state_t;



typedef struct {
    int num_states;
    a_alphabet_t* alphabet;
    a_state_t** states;
}a_automaton_t;

a_alphabet_t* auton_init_alphabet(size_t num_members);
a_state_t* auton_init_state(const char* repr, int is_starting_state, int is_ending_state);
a_automaton_t* auton_init_automaton(const int states_count, const int alphabet_size);
void auton_add_transition(a_state_t* origin, a_state_t* destination, const char* symbol);
void auton_generate_dotfile(const a_automaton_t* automaton, const char* file_name);
void auton_generate_jsonfile(const a_automaton_t* automaton, const char* file_name);
void auton_print_automaton(a_automaton_t* automaton);



#endif //AUTOMATION_H
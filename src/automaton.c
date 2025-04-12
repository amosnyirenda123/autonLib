#include "../include/automaton.h"
#include "../include/auton_memory.h"
#include <string.h>
#include <stdlib.h>

#define ASSERT_NOT_ALL(arg)\
    do{\
        if((arg) == NULL){\
            fprintf(stderr, "[ASSERT] %s is NULL | %s:%d\n", #arg, __FILE__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)


a_alphabet_t* auton_init_alphabet(size_t num_members){
    a_alphabet_t* alphabet = (a_alphabet_t*)auton_malloc(sizeof(a_alphabet_t));
    alphabet->num_members = num_members;
    alphabet->members = auton_calloc(sizeof(char*), num_members);

    return alphabet;
}

a_state_t* auton_init_state(const char* repr, int is_starting_state, int is_ending_state){
    if(!repr){
        fprintf(stderr, "[auton_init_state] State repr can not be null");
        exit(EXIT_FAILURE);
    }
    a_state_t* state = (a_state_t*)auton_malloc(sizeof(a_state_t));

    strcpy(state->repr, repr);
    state->is_starting_state = is_starting_state;
    state->is_ending_state = is_ending_state;
    state->transitions = NULL;
    state->out_transitions = 0;

    return state;
}


a_automaton_t* auton_init_automaton(const int states_count, const int alphabet_size){
    a_automaton_t* automaton = (a_automaton_t*)auton_malloc(sizeof(a_automaton_t));

    automaton->alphabet = auton_init_alphabet(alphabet_size);
    automaton->states = (a_state_t**)auton_calloc(sizeof(a_state_t*), states_count);

    automaton->num_states = states_count;

    return automaton;
}

void auton_free_automaton(a_automaton_t* automaton){
    //free states
    for(int i = 0; i < automaton->num_states; i++){
        a_t_node* current_transition = automaton->states[i]->transitions;

        while (current_transition)
        {
            a_t_node* next_transition = current_transition->next;
            auton_free(current_transition);
            current_transition = next_transition;
        }

        auton_free(automaton->states[i]);
        
    }

    auton_free(automaton->states);

    //free alphabet
    for(int i = 0; i < automaton->alphabet->num_members; i++){
        auton_free(automaton->alphabet->members[i]);
    }
    auton_free(automaton->alphabet->members);
    auton_free(automaton->alphabet);



    auton_free(automaton);
}


void auton_print_automaton(a_automaton_t* automaton){
    for(int i = 0; i < automaton->num_states; i++){
        printf("State: %s\n", automaton->states[i]);
    }

    for(int i = 0; i < automaton->alphabet->num_members; i++){
        printf("Symbol: %s\n", automaton->alphabet->members[i]);
    }

    for(int i = 0; i < automaton->num_states; i++){
        a_state_t* current_state = automaton->states[i];
        a_t_node* current_transition = current_state->transitions;
        while (current_transition)
        {
            printf("%s --[%s]--> %s \n", current_transition->from->repr, current_transition->input_symbol, current_transition->to->repr);

            current_transition = current_transition->next;
        }
        
        printf("%s has %d outgoing transitions\n", current_state->repr, current_state->out_transitions);
    }
}

void auton_add_transition(a_state_t* origin, a_state_t* destination, const char* symbol){
    ASSERT_NOT_ALL(origin);
    ASSERT_NOT_ALL(destination);
    ASSERT_NOT_ALL(symbol);

    a_t_node* new_transition = (a_t_node*)auton_malloc(sizeof(a_t_node));

    new_transition->input_symbol = strdup(symbol);
    new_transition->from = origin;
    new_transition->to = destination;


    new_transition->next = origin->transitions;
    origin->transitions = new_transition;
    origin->out_transitions++;
}

void auton_generate_dotfile(const a_automaton_t* automaton, const char* file_name){
    ASSERT_NOT_ALL(automaton);
    ASSERT_NOT_ALL(file_name);

    //TODO: Configure paths

    FILE* dot_file = fopen(file_name, "w");
    ASSERT_NOT_ALL(dot_file);


    fprintf(dot_file, "digraph Automaton {\n");
    //Write 
    fprintf(dot_file, "  num_states = \"%d\"\n", automaton->num_states);
    fprintf(dot_file, "  num_symbols = \"%d\"\n", automaton->alphabet->num_members);
    fprintf(dot_file, "  Alphabet = \"");
        for(size_t i = 0; i < automaton->alphabet->num_members; i++){
            if( i == (automaton->alphabet->num_members - 1))
                fprintf(dot_file, "%s", automaton->alphabet->members[i]);
            else
                fprintf(dot_file, "%s,", automaton->alphabet->members[i]);
        }
    fprintf(dot_file, "\"\n");

    for(size_t i = 0; i < automaton->num_states; i++){
        a_state_t* current_state = automaton->states[i];
        a_t_node* current_transition = current_state->transitions;

        if(current_state->is_starting_state == 1){
            fprintf(dot_file, "  %s [shape=circle, style=filled, fillcolor=lightyellow];\n", current_state->repr);
        }


        if(current_state->is_ending_state == 1){
            fprintf(dot_file, "  %s [shape=circle, peripheries=2];\n", current_state->repr);
        }

        while(current_transition){
            fprintf(dot_file, "  %s -> %s [label=\"%s\"] \n", current_transition->from->repr, current_transition->to->repr, current_transition->input_symbol);
            current_transition = current_transition->next;
        }
    }

    fprintf(dot_file, "}\n");

    
    fclose(dot_file);
}


void auton_generate_jsonfile(const a_automaton_t* automaton, const char* file_name){
    ASSERT_NOT_ALL(automaton);
    ASSERT_NOT_ALL(file_name);

    FILE* json_file = fopen(file_name, "w");
    ASSERT_NOT_ALL(json_file);



    fprintf(json_file, "{\n");
        fprintf(json_file, "\t\"num_states\": %d,\n", automaton->num_states);
        fprintf(json_file, "\t\"num_symbols\": %d,\n", automaton->alphabet->num_members);
        //Alphabet
        fprintf(json_file, "\t\"alphabet\": ");
            fprintf(json_file, "[");
                for(size_t i = 0; i < automaton->alphabet->num_members; i++){
                    if(i == (automaton->alphabet->num_members - 1)){
                        fprintf(json_file, "\"%s\"", automaton->alphabet->members[i]);
                    }else{
                        fprintf(json_file, "\"%s\", ", automaton->alphabet->members[i]);
                    }
                    
                }
            fprintf(json_file, "],\n");
        //States
        fprintf(json_file, "\t\"states\": ");
            fprintf(json_file, "[");
                for(size_t i = 0; i < automaton->num_states; i++){
                
                    if(i == (automaton->num_states - 1)){
                        fprintf(json_file, "\"%s\"", automaton->states[i]->repr);
                    }else{
                        fprintf(json_file, "\"%s\", ", automaton->states[i]->repr);
                    }
                    
                }
            fprintf(json_file, "],\n");

        //Initial States
        int start_written = 0;
        fprintf(json_file, "\t\"initial_states\": ");
            fprintf(json_file, "[");
                for(size_t i = 0; i < automaton->num_states; i++){
                    if(automaton->states[i]->is_starting_state == 1){
                        if (start_written++) fprintf(json_file, ", ");
                        fprintf(json_file, "\"%s\"", automaton->states[i]->repr);
                        
                    }
                }
            fprintf(json_file, "],\n");

            //Final States
            int end_written = 0;
            fprintf(json_file, "\t\"final_states\": ");
            fprintf(json_file, "[");
                for(size_t i = 0; i < automaton->num_states; i++){
                    if(automaton->states[i]->is_ending_state) {
                        if (end_written++) fprintf(json_file, ", ");
                        fprintf(json_file, "\"%s\"", automaton->states[i]->repr);
                    }
                }
            fprintf(json_file, "],\n");

            //Transitions
            fprintf(json_file, "\t\"transitions\":{\n");
                for(size_t i = 0; i < automaton->num_states; i++){
                    const a_state_t* current_state = automaton->states[i];
                    fprintf(json_file, "\t\t\"%s\": {\n", current_state->repr);

    
                    int sym_written = 0;
                    for (size_t j = 0; j < automaton->alphabet->num_members; j++) {
                        const char* symbol = automaton->alphabet->members[j];
                        int found = 0;

        
                        const a_t_node* tmp = current_state->transitions;
                        while (tmp) {
                            if (strcmp(tmp->input_symbol, symbol) == 0) {
                                found = 1;
                                break;
                            }
                            tmp = tmp->next;
                        }

                        if (found) {
                            if (sym_written++) fprintf(json_file, ",\n");
                            fprintf(json_file, "\t\t\t\"%s\": [", symbol);

                
                            int dst_written = 0;
                            tmp = current_state->transitions;
                            while (tmp) {
                                if (strcmp(tmp->input_symbol, symbol) == 0) {
                                    if (dst_written++) fprintf(json_file, ", ");
                                    fprintf(json_file, "\"%s\"", tmp->to->repr);
                                }
                                tmp = tmp->next;
                            }

                            fprintf(json_file, "]");
                        }
                    }

                    fprintf(json_file, "\n\t\t}%s\n", (i == automaton->num_states - 1) ? "" : ",");
                    
                }
            fprintf(json_file, "\t}\n");

    fprintf(json_file, "}\n");

    fclose(json_file);
}
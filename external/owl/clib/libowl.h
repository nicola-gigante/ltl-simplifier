#ifndef __LIBOWL_H
#define __LIBOWL_H

#include <graal_isolate.h>


#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Read a (deterministic) automaton from a char* serialised in the HOA format.
 * Decodes a 0 terminated C char* to a Java string using the platform's default charset.
 * This function returns a void pointer to an opaque Java object handle. The object is not collected by the garbage collected unless 'destroy_object_handle' is called on the pointer.
 */
void * automaton_parse(graal_isolatethread_t*, char*);

/*
 * Translate the given formula to deterministic parity automaton. For greater performance it 
 * is recommended to use the decomposed DPA construction and reassemble the DPA later.
 * This function returns a void pointer to an opaque Java object handle. The object is not collected by the garbage collected unless 'destroy_object_handle' is called on the pointer.
 */
void * automaton_of(graal_isolatethread_t*, void *);

acceptance_t automaton_acceptance_condition(graal_isolatethread_t*, void *);

int automaton_acceptance_condition_sets(graal_isolatethread_t*, void *);

int automaton_atomic_propositions(graal_isolatethread_t*, void *);

/*
 * Atomic propositions of the range [0, s[ are uncontrollable and [s, l[ are controllable, where s is the value returned by this method. -1 is the default return value, when this value cannot be determined.
 */
int automaton_atomic_propositions_uncontrollable_size(graal_isolatethread_t*, void *);

size_t automaton_atomic_propositions_label(graal_isolatethread_t*, void *, int, char*, size_t);

/*
 * Return true if edge masking could speed up computation.
 */
int automaton_edge_tree_masking(graal_isolatethread_t*, void *);

/*
 * Serialise the edges leaving the given state into a tree buffer, edge buffer, and an 
 * optional score buffer. If the scores are not required, the pointer may be set to NULL.
 * The pointer returned via the sized_{int,double}_array_t structures must be freed using
 * the method `free_unmanaged_memory`.
 */
void automaton_edge_tree(graal_isolatethread_t*, void *, int, sized_int_array_t*, sized_int_array_t*, sized_double_array_t*);

/*
 * Returns true if the automaton only has one state, the initial state.
 */
int automaton_is_singleton(graal_isolatethread_t*, void *);

/*
 * Translate the given formula to a decomposed DPA.
 * This function returns a void pointer to an opaque Java object handle. The object is not collected by the garbage collected unless 'destroy_object_handle' is called on the pointer.
 */
void * decomposed_dpa_of(graal_isolatethread_t*, void *);

/*
 * Retrieve the number of subautomata.
 */
int decomposed_dpa_automata_size(graal_isolatethread_t*, void *);

/*
 * Retrieve the i-th subautomaton.
 * This function returns a void pointer to an opaque Java object handle. The object is not collected by the garbage collected unless 'destroy_object_handle' is called on the pointer.
 */
void * decomposed_dpa_automata_get(graal_isolatethread_t*, void *, int);

int decomposed_dpa_declare_realizability_status(graal_isolatethread_t*, void *, realizability_status_t, int*, int);

realizability_status_t decomposed_dpa_query_realizability_status(graal_isolatethread_t*, void *, int*, int);

/*
 * This function returns a void pointer to an opaque Java object handle. The object is not collected by the garbage collected unless 'destroy_object_handle' is called on the pointer.
 */
void * decomposed_dpa_structure_get(graal_isolatethread_t*, void *);

node_type_t decomposed_dpa_structure_node_type(graal_isolatethread_t*, void *);

int decomposed_dpa_structure_children(graal_isolatethread_t*, void *);

/*
 * This function returns a void pointer to an opaque Java object handle. The object is not collected by the garbage collected unless 'destroy_object_handle' is called on the pointer.
 */
void * decomposed_dpa_structure_get_child(graal_isolatethread_t*, void *, int);

int decomposed_dpa_structure_referenced_automaton(graal_isolatethread_t*, void *);

/*
 * This function returns a void pointer to an opaque Java object handle. The object is not collected by the garbage collected unless 'destroy_object_handle' is called on the pointer.
 */
void * decomposed_dpa_structure_referenced_formula(graal_isolatethread_t*, void *);

int decomposed_dpa_structure_referenced_alphabet_mapping(graal_isolatethread_t*, void *, int);

void destroy_object_handle(graal_isolatethread_t*, void *);

void free_unmanaged_memory(graal_isolatethread_t*, void *);

size_t print_object_handle(graal_isolatethread_t*, void *, char*, size_t);

size_t owl_version(graal_isolatethread_t*, char*, size_t);

/*
 * Parse the given string with the given atomic propositions and return an LTL formula.
 * Decodes a 0 terminated C char* to a Java string using the platform's default charset.
 * This function returns a void pointer to an opaque Java object handle. The object is not collected by the garbage collected unless 'destroy_object_handle' is called on the pointer.
 */
void * ltl_formula_parse(graal_isolatethread_t*, char*, char**, int);

/*
 * Parse the given string with the given atomic propositions and return an LTL formula with finite semantics.
 * Decodes a 0 terminated C char* to a Java string using the platform's default charset.
 * This function returns a void pointer to an opaque Java object handle. The object is not collected by the garbage collected unless 'destroy_object_handle' is called on the pointer.
 */
void * ltl_formula_parse_with_finite_semantics(graal_isolatethread_t*, char*, char**, int);

/*
 * Simplify the given LTL formula assuming a Game-semantics where atomic propositions less 
 * than `firstOutputAtomicProposition` is controlled by the environment trying to dissatisfy 
 * the formula and atomic proposition greater or equal are controlled by the system. The 
 * status of atomic proposition is written to the passed int pointer using the encoding 
 * provided by `atomic_proposition_status_t`
 * This function returns a void pointer to an opaque Java object handle. The object is not collected by the garbage collected unless 'destroy_object_handle' is called on the pointer.
 */
void * ltl_formula_simplify(graal_isolatethread_t*, void *, int, int*, int);

int run_main(int paramArgc, char** paramArgv);

void vmLocatorSymbol(graal_isolatethread_t* thread);

#if defined(__cplusplus)
}
#endif
#endif

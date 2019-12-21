"""
Algorithms for Computational Linguistics - HW 03 and HW 04
Tel Aviv Univerity, course no. 0627.2235, Spring 2018.

An implementation of finite state machines (FSM).

@author: <Noah Harel>
"""


#===================================================================================================
# Classes
#===================================================================================================

class DFA:
    def __init__(self, states, transition_function, initial_state, accepting_states):
        """
        @param states: The states of the DFA.
        @type states: A sequence of str objects.
        @param transition_function: The DFA's transition function, that maps
            (current state, current char) -> next state
        @type transition_function: A mapping that maps a sequence of 2 str objects to a str.
        @param initial_state: The state of the DFA before reading the first char in an input
            string. 
        @type initial_state: str
        @param accepting_states: The accepting states of the DFA.
        @type accepting_states: A sequence of str objects.
        """
        #raise NotImplementedError()
        self.states = states
        self.transition_function = transition_function
        self.alphabet = [key[1] for key in self.transition_function]
        self.initial_state = initial_state
        self.accepting_states = accepting_states
        

    def decide(self, string):
        """
        @param string: A string that may or may not belong to the language of this DFA.
        @type string: str
        @return: Whether the string belongs to the DFA's language.
        @rtype: bool
        """
        #raise NotImplementedError()
        current_state = self.initial_state
        for char in string:
            if char not in self.alphabet:
                return False
            current_state = self.transition_function[(current_state, char)]
        last_state = current_state
        return last_state in self.accepting_states

    def get_complement(self):
        """
        @return: A new DFA instance that decides the complement language of the language decided by
            this DFA.  
        @rtype: DFA
        """
        #raise NotImplementedError()
        reverse_acc_states = []
        for state in self.states:
            if state not in self.accepting_states:
                reverse_acc_states.append(state)
        return DFA(self.states, self.transition_function, self.initial_state, reverse_acc_states)

    def get_union(self, other):
        """
        @param other: The DFA with which the union should be performed.
        @type other: DFA
        @return: A new DFA instance that decides the union of the languages of this DFA and the
            input DFA.
        @rtype: DFA
        """
        #raise NotImplementedError()
        import itertools
        union_alphabet = self.alphabet
        cross_states = list(itertools.product(self.states,other.states))
        cross_acc = [pair for pair in cross_states if \
                     ((pair[0] in self.accepting_states) or (pair[1] in other.accepting_states))]
        cross_initial = (self.initial_state, other.initial_state)
        cross_function = {}
        for pair in cross_states:
            for char in union_alphabet:
                cross_function[pair,char] = (self.transition_function[(pair[0],char)],\
                                             other.transition_function[(pair[1],char)])
        return DFA (cross_states, cross_function, cross_initial, cross_acc)

    def get_intersection(self, other):
        """
        @param other: The DFA with which the intersection should be performed.
        @type other: DFA
        @return: A new DFA instance that decides the intersection of the languages of this DFA and
            the input DFA.
        @rtype: DFA
        """
        #raise NotImplementedError()
        comp1 = self.get_complement()
        comp2 = other.get_complement()
        comp_union = comp1.get_union(comp2)
        return comp_union.get_complement()

    def get_concatenation(self, other):
        """
        @param other: The DFA with which the concatenation should be performed.
        @type other: DFA
        @return: A new DFA instance that decides the concatenation of the languages of this DFA
            followed by the language of the input DFA.
        @rtype: DFA
        """
        raise NotImplementedError()

    def get_kleene_star(self):
        """
        @return: A new DFA instance that decides the Kleene closure of the language of this DFA.
        @rtype: DFA
        """
        raise NotImplementedError()

    def get_nfa(self):
        """
        @return: A new NFA instance that decides the same language as this DFA.
        @rtype: NFA
        """
        raise NotImplementedError()

class NFA:
    def __init__(self, states, transition_function, initial_state, accepting_states):
        """
        @param states: The states of the NFA.
        @type states: A sequence of str objects.
        @param transition_function: The NFA's transition function, that maps
            (current state, current char) -> possible next states
        @type transition_function: A mapping that maps a sequence of 2 str objects to a sequence
            of str objects.
        @param initial_state: The state of the NFA before reading the first char in an input
            string. 
        @type initial_state: str
        @param accepting_states: The accepting states of the NFA.
        @type accepting_states: A sequence of str objects.
        """
        raise NotImplementedError()

    def decide(self, string):
        """
        @param string: A string that may or may not belong to the language of this NFA.
        @type string: str
        @return: Whether the string belongs to the NFA's language.
        @rtype: bool
        """
        raise NotImplementedError()

    def get_complement(self):
        """
        @return: A new NFA instance that decides the complement language of the language decided by
            this NFA.  
        @rtype: NFA
        """
        raise NotImplementedError()

    def get_union(self, other):
        """
        @param other: The NFA with which the union should be performed.
        @type other: NFA
        @return: A new NFA instance that decides the union of the languages of this NFA and the
            input NFA.
        @rtype: NFA
        """
        raise NotImplementedError()

    def get_intersection(self, other):
        """
        @param other: The NFA with which the intersection should be performed.
        @type other: NFA
        @return: A new NFA instance that decides the intersection of the languages of this NFA and
            the input NFA.
        @rtype: NFA
        """
        raise NotImplementedError()

    def get_concatenation(self, other):
        """
        @param other: The NFA with which the concatenation should be performed.
        @type other: NFA
        @return: A new NFA instance that decides the concatenation of the languages of this NFA
            followed by the language of the input NFA.
        @rtype: NFA
        """
        raise NotImplementedError()

    def get_kleene_star(self):
        """
        @return: A new NFA instance that decides the Kleene closure of the language of this NFA.
        @rtype: NFA
        """
        raise NotImplementedError()

    def get_dfa(self):
        """
        @return: A new DFA instance that decides the same language as this NFA.
        @rtype: DFA
        """
        raise NotImplementedError()



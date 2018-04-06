/**
 * File: DSQ.h
 * Organization: RIT Space Exploration
 * Author: Dylan Wagner (drw6528@rit.edu)
 * Description:
 *	    Dynamic Scheduling Queue(DSQ) is a library used to provide dynamic execution
 *	    of tasks or other routines within a microprocessor.
*/

#ifndef DSQ_H
#define DSQ_H

#include "queue"

#define PRIORITY_BASE 4

// WARNING! Platform Specific
// Change these values, platform specific
// 4294967295 for 4 byte long 18446744073709551615 for 8 byte long
#define MAX_SIZE_LONG 4294967295 // Platform Specific

// struct Routine -- holds information regarding routines
struct Routine{
	int type;	// type as int, can also be used with a enum
	unsigned long int priority_value; // determines the placement within the DEQ
	void (*routine_addr)(); // function pointer to static routine
};

// create alis between struct Routine and Routine
typedef struct Routine Routine;

// Comparator class used to to compare Routine objects
class Comparator{
public:
    bool operator() (const Routine& lhs, const Routine& rhs);
};

class DSQ{
private:

	unsigned long int priority_cnt; // main priority counter

	// min-heap
	priority_queue<Routine, Comparator> sch_queue;

    /**
     * creates a Routine struct on the stack, fills in the values
	 * then returns the created struct.
     * @param type
     *      type as int, can also be used with a enum
     * @param priority_mult
     *      ranging from (0+)
     * @param routine_addr
     *      Function pointer to a routine function
     * @return
     *      Newly constructed Routine struct
     */
	Routine create_routine(int type, int priority_mult, void (*routine_addr)());

    /**
     * Inserts the provided routine into the sch_queue
     * @param routine
     *      The routine to be inserted
     */
	void insert_routine(const Routine &routine);

    /**
     * Used to calculate the priority value stored in the Routine struct
     * @param priority_mult
     *      Used to specify execution interval
     * @return
     *      Value containing the priority value
     */
	unsigned long int calculate_priority(int priority_mult);

    /**
     * resets all priority values within DSQ entires, used when a priority value reaches sizeof long
     */
	void priority_reset();
	
	
public:

    explicit DSQ(unsigned int capacity);

	/**
     * Main method for submitting routines to the DEA
     * @param type
     *      type as int, can also be used with a enum
     * @param priority_mult
     *      ranging from (0+)
     * @param routine_addr
     *      Address to execute
     */
	void add_routine(int type, int priority_mult, void (*routine_addr)());

    /**
     * called in a loop, main function for executing routines placed in the DEQ.
     */
	void execute();

    /**
     * Getter function
     * @return
     *     The size of the priority_queue/DSQ
     */
	int get_size();

    /**
     * Sets a default route within the queue, the default route is executed when
     * the priority queue is empty.
     * @param type
     *      Type of routine
     * @param routine_addr
     *      Code to execute
     */
    void set_default(int type, void (*routine_addr)());

	/**
	 * Remove all elements from the DSQ
	 */
    void clear();

};

#endif /* end of include guard: DSQ_H */
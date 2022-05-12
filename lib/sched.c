#include <env.h>
#include <pmap.h>
#include <printf.h>
/*
#define PRI(X) (((X)->env_pri) & 0xff)
#define FUNC_1(X) ((((X)->env_pri) >> 8) & 0xff)
#define FUNC_2(X) ((((X)->env_pri) >> 16) & 0xff)
#define FUNC_3(X) ((((X)->env_pri) >> 24) & 0xff)
*/

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.15 ***/
void sched_yield(void)
{
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
    static struct Env* cur = NULL;
	//struct Env* tempe;
	//int maxpri = 0;

    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`, 
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */
// ****** self annotations ******
	// in env.c/env_alloc():  set e->env_status = ENV_RUNNABLE;
	// in env.c/env_create_priority(): LIST_INSERT_HEAD(env_sched_list, e, env_sched_link);
	// in env.c/env_free(): LIST_REMOVE(e, env_sched_link);
	
	if (count <= 0) {
		do {
			if (LIST_EMPTY(&env_sched_list[point])) {
				point = 1 - point;
			}	
			cur = LIST_FIRST(&env_sched_list[point]);
			if (cur != NULL) {
				LIST_REMOVE(cur, env_sched_link);
				LIST_INSERT_TAIL(&env_sched_list[1 - point], cur, env_sched_link);
				count = cur->env_pri;
			}
		} while (cur == NULL || cur->env_status != ENV_RUNNABLE);
	}
	--count;
	env_run(cur);

	/*
	while (!LIST_EMPTY(&env_sched_list[1])) { // abandon list[1]
	 	tempe = LIST_FIRST(&env_sched_list[1]);
	 	LIST_REMOVE(tempe, env_sched_link);
	 	LIST_INSERT_HEAD(&env_sched_list[0], tempe, env_sched_link);
	 	
	 }
	 if (cur != NULL) {
		 if (FUNC_1(cur) > 0) {
			 if (PRI(cur) >= FUNC_1(cur)) {
				 cur->env_pri -= FUNC_1(cur);
			 } else {
				 cur->env_pri = 0;
			 }
		 }
	 }
	 maxpri = 0;
	 LIST_FOREACH(tempe, &env_sched_list[0],env_sched_link) {
	 	if (PRI(tempe) > maxpri && tempe->env_status == ENV_RUNNABLE) {
	 		cur = tempe;
	 		maxpri = PRI(tempe);
	 	}
	 }
	
	env_run(cur);
	*/
}

#include "mmu.h"
#include "pmap.h"
#include "printf.h"
#include "env.h"
#include "error.h"
#define BIT_MAP_SIZE 512

/* These variables are set by mips_detect_memory() */
u_long maxpa;            /* Maximum physical address */
u_long npage;            /* Amount of memory(in pages) */
u_long basemem;          /* Amount of base memory(in bytes) */
u_long extmem;           /* Amount of extended memory(in bytes) */

Pde *boot_pgdir;

struct Page *pages;
static u_long freemem;

static struct Page_list page_free_list;	/* Free list of physical pages */

u_long high_start = 0x00400000 + (1 << 25);
u_long high_end = 0x4000000 - 1;
static struct Buddy_list buddy_list;
/* Exercise 2.1 */
/* Overview:
   Initialize basemem and npage.
   Set basemem to be 64MB, and calculate corresponding npage value.*/
void mips_detect_memory()
{
	/* Step 1: Initialize basemem.
	 * (When use real computer, CMOS tells us how many kilobytes there are). */
	// 64MB = 2 ^ (6 + 20) B
	maxpa = 0x4000000;  // 1 << 26
	basemem = 0x4000000;  // 1 << 26
	// Step 2: Calculate corresponding npage value.
	extmem = 0;
	npage = basemem >> PGSHIFT; // PGSHIFT defined in include/mmu.h
	// napge = 1 << 26 >> 12 = 1 << 14 = 16k

	printf("Physical memory: %dK available, ", (int)(maxpa / 1024));
	printf("base = %dK, extended = %dK\n", (int)(basemem / 1024),
			(int)(extmem / 1024));
}

/* Overview:
   Allocate `n` bytes physical memory with alignment `align`, if `clear` is set, clear the
   allocated memory.
   This allocator is used only while setting up virtual memory system.

   Post-Condition:
   If we're out of memory, should panic, else return this address of memory we have allocated.*/
static void *alloc(u_int n, u_int align, int clear)
{
	extern char end[];  // defined in tools/scse0_3.ld
	// vaddr = 0x80400000, paddr = 0x00400000
	// paddr = [0x00400000, 0x03ffffff] \ vaddr = [0x80400000, 0x83ffffff]
	u_long alloced_mem;

	/* Initialize `freemem` if this is the first time. The first virtual address that the
	 * linker did *not* assign to any kernel code or global variables. */
	if (freemem == 0) {
		freemem = (u_long)end;
	}

	/* Step 1: Round up `freemem` up to be aligned properly */
	freemem = ROUND(freemem, align);  // ROUND defined in include/types.h

	/* Step 2: Save current value of `freemem` as allocated chunk. */
	alloced_mem = freemem;

	/* Step 3: Increase `freemem` to record allocation. */
	freemem = freemem + n;

	/* Check if we're out of memory. If we are, PANIC !! */
	if (PADDR(freemem) >= maxpa) {  // PADDR defined in include/mmu.h
		panic("out of memory\n");
		return (void *)-E_NO_MEM;
	}

	/* Step 4: Clear allocated chunk if parameter `clear` is set. */
	if (clear) {
		bzero((void *)alloced_mem, n);  // bzero defined in init/init.c
	}

	/* Step 5: return allocated chunk. */
	return (void *)alloced_mem;
}

/* Exercise 2.6 */
/* Overview:
   Get the page table entry for virtual address `va` in the given
   page directory `pgdir`.
   If the page table is not exist and the parameter `create` is set to 1,
   then create it.*/
static Pte *boot_pgdir_walk(Pde *pgdir, u_long va, int create)
{

	Pde *pgdir_entryp;
	Pte *pgtable, *pgtable_entry;

	/* Step 1: Get the corresponding page directory entry and page table. */
	/* Hint: Use KADDR and PTE_ADDR to get the page table from page directory
	 * entry value. */


	/* Step 2: If the corresponding page table is not exist and parameter `create`
	 * is set, create one. And set the correct permission bits for this new page
	 * table. */


	/* Step 3: Get the page table entry for `va`, and return it. */


}

/* Exercise 2.7 */
/*Overview:
  Map [va, va+size) of virtual address space to physical [pa, pa+size) in the page
  table rooted at pgdir.
  Use permission bits `perm | PTE_V` for the entries.

  Pre-Condition:
  Size is a multiple of BY2PG.*/
void boot_map_segment(Pde *pgdir, u_long va, u_long size, u_long pa, int perm)
{
	int i, va_temp;
	Pte *pgtable_entry;

	/* Step 1: Check if `size` is a multiple of BY2PG. */


	/* Step 2: Map virtual address space to physical address. */
	/* Hint: Use `boot_pgdir_walk` to get the page table entry of virtual address `va`. */


}

/* Overview:
   Set up two-level page table.

Hint:
You can get more details about `UPAGES` and `UENVS` in include/mmu.h. */
void mips_vm_init()
{
	extern char end[];
	extern int mCONTEXT;
	extern struct Env *envs;

	Pde *pgdir;
	u_int n;

	/* Step 1: Allocate a page for page directory(first level page table). */
	pgdir = alloc(BY2PG, BY2PG, 1);
	printf("to memory %x for struct page directory.\n", freemem);
	mCONTEXT = (int)pgdir;

	boot_pgdir = pgdir;  // original virtual address of page directory

	/* Step 2: Allocate proper size of physical memory for global array `pages`,
	 * for physical memory management. Then, map virtual address `UPAGES` to
	 * physical address `pages` allocated before. In consideration of alignment,
	 * you should round up the memory size before map. */
	pages = (struct Page *)alloc(npage * sizeof(struct Page), BY2PG, 1);
	printf("to memory %x for struct Pages.\n", freemem);
	n = ROUND(npage * sizeof(struct Page), BY2PG);
	boot_map_segment(pgdir, UPAGES, n, PADDR(pages), PTE_R);  // UPAGES: original virtual address of page tables

	/* Step 3, Allocate proper size of physical memory for global array `envs`,
	 * for process management. Then map the physical address to `UENVS`. */
	envs = (struct Env *)alloc(NENV * sizeof(struct Env), BY2PG, 1);
	n = ROUND(NENV * sizeof(struct Env), BY2PG);
	boot_map_segment(pgdir, UENVS, n, PADDR(envs), PTE_R);  // UENVS: original virtual address of page tables for process management

	printf("pmap.c:\t mips vm init success\n");
}
/*
	struct Page *pages;
	static struct Page_list page_free_list;
*/

/* Exercise 2.3 */
/*Overview:
  Initialize page structure and memory free list.
  The `pages` array has one `struct Page` entry per physical page. Pages
  are reference counted, and free pages are kept on a linked list.
Hint:
Use `LIST_INSERT_HEAD` to insert something to list.*/
void page_init(void)
{
	/* Step 1: Initialize page_free_list. */
	/* Hint: Use macro `LIST_INIT` defined in include/queue.h. */
	LIST_INIT((&page_free_list));

	/* Step 2: Align `freemem` up to multiple of BY2PG. */
	freemem = ROUND(freemem, BY2PG);

	/* Step 3: Mark all memory blow `freemem` as used(set `pp_ref`
	 * filed to 1) */
	int used_pages_num = PPN(PADDR(freemem));
	int i;
	for (i = 0; i < used_pages_num; i++)  // PPN = VPN, defined in include/mmu.h 
		pages[i].pp_ref = 1;
	/* Step 4: Mark the other memory as free. */
	for (; i < npage; i++) {
		pages[i].pp_ref = 0;
		LIST_INSERT_HEAD(&page_free_list, (pages + i), pp_link);
	}
}

//u_long high_start;
//u_long high_end;
//static struct Buddy_list buddy_free_list;
void buddy_init(void) {
	LIST_INIT((&buddy_list));
	u_long high_base = high_start;
	struct Buddy* buddy = (struct Buddy*) alloc(sizeof(struct Buddy), sizeof(struct Buddy), 1);
	int i = 0;
	for (; i < 8; i++) {
		buddy->base = high_base;
		buddy->size = (1 << 22);
		buddy->free = 0;
		buddy->id = (u_short) i;
		LIST_INSERT_TAIL(&buddy_list, buddy, bb_link);
		high_base += (1 << 22);
		buddy = (struct Buddy*) alloc(sizeof(struct Buddy), sizeof(struct Buddy), 1);
	}
}

/*
unsigned int page_bitmap[BIT_MAP_SIZE];
void page_init(void) {
	freemem = ROUND(freemem, BY2PG);

	int used_page = PPN(PADDR(freemem));
	int i, j, im, jm;
	for (i = 0; i < MAPSIZE; i++) {
		page_bitmap[i] = 0;
	}
	im = used_page >> 5;
	jm = used_page % 32;
	for (i = 0; i < im; i++) {
		page_bitmap[i] = ~0;
	}
	for (j = 0; j < jm; j++) {
		page_bitmap[im] |= (1 << j);
	}
	printf("page bitmap size is %x\n", BIT_MAP_SIZE);
}
*/











/* Exercise 2.4 */
/*Overview:
  Allocates a physical page from free memory, and clear this page.

  Post-Condition:
  If failing to allocate a new page(out of memory(there's no free page)),
  return -E_NO_MEM.
  Else, set the address of allocated page to *pp, and return 0.

Note:
DO NOT increment the reference count of the page - the caller must do
these if necessary (either explicitly or via page_insert).

Hint:
Use LIST_FIRST and LIST_REMOVE defined in include/queue.h .*/
int page_alloc(struct Page **pp)
{
	struct Page *ppage_temp;
	/* Step 1: Get a page from free memory. If fail, return the error code.*/
	/* Step 2: Initialize this page.
	 * Hint: use `bzero`. */
	if (LIST_EMPTY(&page_free_list))  return -E_NO_MEM;
	ppage_temp = LIST_FIRST(&page_free_list);
	bzero((void*) page2kva(ppage_temp), BY2PG);
	*pp = ppage_temp;
	LIST_REMOVE(ppage_temp, pp_link);
	return 0;
}

int buddy_alloc(u_int size, u_int *pa, u_char *pi) {
	struct Buddy *buddy;
	u_short find = 0;
	LIST_FOREACH(buddy, &buddy_list, bb_link) {
        if (buddy->size >= size && buddy->free == 0) {
            buddy->free = 1;
			find = 1;
            break;
        }
    }
	if (!find) return -1;
	struct Buddy* temp = (struct Buddy*) alloc(sizeof(struct Buddy), sizeof(struct Buddy), 1);
	while ((buddy->size >> 1) > size) {
		buddy->size >>= 1;
		temp->base = buddy->base + buddy->size;
		temp->size = buddy->size;
		temp->id = buddy->id;
		temp->free = 0;
		LIST_INSERT_AFTER(buddy, temp, bb_link);
	}
	pa = buddy->base;
	u_long temp_size = buddy->size >> 12;
	u_char j = 0;
	while (temp_size > 1) {
		j++;
		temp_size >>= 1;
	}
	*pi = j;
	return 0;
}

/*
int page_alloc2(struct Page **pp) {
    struct Page *ppage_temp;
    if (LIST_EMPTY(&page_free_list))  return -E_NO_MEM;
    ppage_temp = LIST_FIRST(&page_free_list);
    bzero((void*) page2kva(ppage_temp), BY2PG);
    *pp = ppage_temp;
    LIST_REMOVE(ppage_temp, pp_link);
    
    printf("page number is %x, start from pa %x\n",
           page2ppn(ppage_temp), page2pa(ppage_temp));
    return 0;
}
*/


/*
int page_alloc(struct Page **pp) {
	int ppn;
	int byte, bit;
	unsigned int bits;
	for (byte = 0; byte < BIT_MAP_SIZE; byte++) {
		bits = page_bitmap[byte];
		if (bits != ~0) {
			for (bit = 0; bit < 32; j++) {
				if ((bits & (1 << bit)) == 0) {
					bits |= (1 << bit);
					page_bitmap[byte] = bits;
					ppn = byte << 5 + bit;
					*pp = pages + ppn;
					return 0;
				}
			}
		}
	}

	return -E_NO_MEM;
}
*/




/*
static int exam_times = 0;
void get_page_status(int pa) {
    struct Page *pp = pa2page(pa);
    struct Page *page;  // used for for-iteration
    
    int is_free = 0;
    int status;
    
    LIST_FOREACH(page, &page_free_list, pp_link) {
        if (page == pp) {
            is_free = 1;
            break;
        }
    }
    exam_times++;
    if (is_free) status = 3; 
    else {
        if (pp->pp_ref == 0)  status = 2;
        else status = 1;
    }
    printf("times:%d, page status:%d\n", exam_times, status);
    
}
*/




/* Exercise 2.5 */
/*Overview:
  Release a page, mark it as free if it's `pp_ref` reaches 0.
Hint:
When you free a page, just insert it to the page_free_list.*/
void page_free(struct Page *pp)
{
	/* Step 1: If there's still virtual address referring to this page, do nothing. */
	/* Step 2: If the `pp_ref` reaches 0, mark this page as free and return. */
	if (pp->pp_ref == 0) {
		LIST_INSERT_HEAD(&page_free_list, pp, pp_link);
		return;
	} else if (pp->pp_ref > 0) return;

	/* If the value of `pp_ref` is less than 0, some error must occurr before,
	 * so PANIC !!! */
	panic("cgh:pp->pp_ref is less than zero\n");
}

void buddy_free(u_int pa) {
	struct Buddy* buddy;
	struct Buddy* former;
	LIST_FOREACH(buddy, &buddy_list, bb_link) {
        if (buddy->base == pa) {
			buddy->free = 0;
            break;
        }
    }
	u_short former_none = 1;	
	LIST_FOREACH(former, &buddy_list, bb_link) {
		if (LIST_NEXT(former, bb_link) == NULL) break;
        if (LIST_NEXT(former, bb_link)->base == pa) 
			former_none = 0;
            break;
        }
    }

	if (!former_none) {
	if (former->size == buddy->size && former->id == buddy->id && buddy->size <= (1 <<22)) {
		buddy->base = former->base;
		buddy->size <<= 1;
		buddy->free = 0;
		LIST_REMOVE(former, bb_link);
	}
	}
	if (LIST_NEXT(buddy, bb_link) == NULL) return;
	while (buddy->size == LIST_NEXT(buddy, bb_link)->size && buddy->id == LIST_NEXT(buddy, bb_link)->id) {
		buddy->size <<= 1;
		LIST_REMOVE(LIST_NEXT(buddy, bb_link), bb_link);
		if (LIST_NEXT(buddy, bb_link) == NULL) return;
	}
}


/*
void page_free(struct Page *pp)
{
	if(pp->pp_ref > 0) {
		return;
	}

	int ppn = page2ppn(pp);

	if(pp->pp_ref == 0) {
		page_bitmap[ppn >> 5] &= ~(1 << (ppn % 32));
		return;
	}


    panic("cgh:pp->pp_ref is less than zero\n");
}
*/






/* Exercise 2.8 */
/*Overview:
  Given `pgdir`, a pointer to a page directory, pgdir_walk returns a pointer
  to the page table entry (with permission PTE_R|PTE_V) for virtual address 'va'.

  Pre-Condition:
  The `pgdir` should be two-level page table structure.

  Post-Condition:
  If we're out of memory, return -E_NO_MEM.
  Else, we get the page table entry successfully, store the value of page table
  entry to *ppte, and return 0, indicating success.

Hint:
We use a two-level pointer to store page table entry and return a state code to indicate
whether this function execute successfully or not.
This function has something in common with function `boot_pgdir_walk`.*/
int pgdir_walk(Pde *pgdir, u_long va, int create, Pte **ppte)
{
	Pde *pgdir_entryp;
	Pte *pgtable;
	struct Page *ppage;

	/* Step 1: Get the corresponding page directory entry and page table. */


	/* Step 2: If the corresponding page table is not exist(valid) and parameter `create`
	 * is set, create one. And set the correct permission bits for this new page table.
	 * When creating new page table, maybe out of memory. */


	/* Step 3: Set the page table entry to `*ppte` as return value. */


	return 0;
}

/* Exercise 2.9 */
/*Overview:
  Map the physical page 'pp' at virtual address 'va'.
  The permissions (the low 12 bits) of the page table entry should be set to 'perm|PTE_V'.

  Post-Condition:
  Return 0 on success
  Return -E_NO_MEM, if page table couldn't be allocated

Hint:
If there is already a page mapped at `va`, call page_remove() to release this mapping.
The `pp_ref` should be incremented if the insertion succeeds.*/
int page_insert(Pde *pgdir, struct Page *pp, u_long va, u_int perm)
{
	u_int PERM;
	Pte *pgtable_entry;
	PERM = perm | PTE_V;

	/* Step 1: Get corresponding page table entry. */
	pgdir_walk(pgdir, va, 0, &pgtable_entry);

	if (pgtable_entry != 0 && (*pgtable_entry & PTE_V) != 0) {
		if (pa2page(*pgtable_entry) != pp) {
			page_remove(pgdir, va);
		} else	{
			tlb_invalidate(pgdir, va);
			*pgtable_entry = (page2pa(pp) | PERM);
			return 0;
		}
	}

	/* Step 2: Update TLB. */

	/* hint: use tlb_invalidate function */


	/* Step 3: Do check, re-get page table entry to validate the insertion. */

	/* Step 3.1 Check if the page can be insert, if can’t return -E_NO_MEM */

	/* Step 3.2 Insert page and increment the pp_ref */

	return 0;
}

/*Overview:
  Look up the Page that virtual address `va` map to.

  Post-Condition:
  Return a pointer to corresponding Page, and store it's page table entry to *ppte.
  If `va` doesn't mapped to any Page, return NULL.*/
struct Page *page_lookup(Pde *pgdir, u_long va, Pte **ppte)
{
	struct Page *ppage;
	Pte *pte;

	/* Step 1: Get the page table entry. */
	pgdir_walk(pgdir, va, 0, &pte);

	/* Hint: Check if the page table entry doesn't exist or is not valid. */
	if (pte == 0) {
		return 0;
	}
	if ((*pte & PTE_V) == 0) {
		return 0;    //the page is not in memory.
	}

	/* Step 2: Get the corresponding Page struct. */

	/* Hint: Use function `pa2page`, defined in include/pmap.h . */
	ppage = pa2page(*pte);
	if (ppte) {
		*ppte = pte;
	}

	return ppage;
}

// Overview:
// 	Decrease the `pp_ref` value of Page `*pp`, if `pp_ref` reaches to 0, free this page.
void page_decref(struct Page *pp) {
	if(--pp->pp_ref == 0) {
		page_free(pp);
	}
}

// Overview:
// 	Unmaps the physical page at virtual address `va`.
void page_remove(Pde *pgdir, u_long va)
{
	Pte *pagetable_entry;
	struct Page *ppage;

	/* Step 1: Get the page table entry, and check if the page table entry is valid. */

	ppage = page_lookup(pgdir, va, &pagetable_entry);

	if (ppage == 0) {
		return;
	}

	/* Step 2: Decrease `pp_ref` and decide if it's necessary to free this page. */

	/* Hint: When there's no virtual address mapped to this page, release it. */
	ppage->pp_ref--;
	if (ppage->pp_ref == 0) {
		page_free(ppage);
	}

	/* Step 3: Update TLB. */
	*pagetable_entry = 0;
	tlb_invalidate(pgdir, va);
	return;
}

// Overview:
// 	Update TLB.
void tlb_invalidate(Pde *pgdir, u_long va)
{
	if (curenv) {
		tlb_out(PTE_ADDR(va) | GET_ENV_ASID(curenv->env_id));
	} else {
		tlb_out(PTE_ADDR(va));
	}
}

void physical_memory_manage_check(void)
{
	struct Page *pp, *pp0, *pp1, *pp2;
	struct Page_list fl;
	int *temp;

	// should be able to allocate three pages
	pp0 = pp1 = pp2 = 0;
	assert(page_alloc(&pp0) == 0);
	assert(page_alloc(&pp1) == 0);
	assert(page_alloc(&pp2) == 0);

	assert(pp0);
	assert(pp1 && pp1 != pp0);
	assert(pp2 && pp2 != pp1 && pp2 != pp0);



	// temporarily steal the rest of the free pages
	fl = page_free_list;
	// now this page_free list must be empty!!!!
	LIST_INIT(&page_free_list);
	// should be no free memory
	assert(page_alloc(&pp) == -E_NO_MEM);

	temp = (int*)page2kva(pp0);
	//write 1000 to pp0
	*temp = 1000;
	// free pp0
	page_free(pp0);
	printf("The number in address temp is %d\n",*temp);

	// alloc again
	assert(page_alloc(&pp0) == 0);
	assert(pp0);

	// pp0 should not change
	assert(temp == (int*)page2kva(pp0));
	// pp0 should be zero
	assert(*temp == 0);

	page_free_list = fl;
	page_free(pp0);
	page_free(pp1);
	page_free(pp2);
	struct Page_list test_free;
	struct Page *test_pages;
	test_pages= (struct Page *)alloc(10 * sizeof(struct Page), BY2PG, 1);
	LIST_INIT(&test_free);
	//LIST_FIRST(&test_free) = &test_pages[0];
	int i,j=0;
	struct Page *p, *q;
	//test inert tail
	for(i=0;i<10;i++) {
		test_pages[i].pp_ref=i;
		//test_pages[i].pp_link=NULL;
		//printf("0x%x  0x%x\n",&test_pages[i], test_pages[i].pp_link.le_next);
		LIST_INSERT_TAIL(&test_free,&test_pages[i],pp_link);
		//printf("0x%x  0x%x\n",&test_pages[i], test_pages[i].pp_link.le_next);

	}
	p = LIST_FIRST(&test_free);
	int answer1[]={0,1,2,3,4,5,6,7,8,9};
	assert(p!=NULL);
	while(p!=NULL)
	{
		//printf("%d %d\n",p->pp_ref,answer1[j]);
		assert(p->pp_ref==answer1[j++]);
		//printf("ptr: 0x%x v: %d\n",(p->pp_link).le_next,((p->pp_link).le_next)->pp_ref);
		p=LIST_NEXT(p,pp_link);

	}
	// insert_after test
	int answer2[]={0,1,2,3,4,20,5,6,7,8,9};
	q=(struct Page *)alloc(sizeof(struct Page), BY2PG, 1);
	q->pp_ref = 20;

	//printf("---%d\n",test_pages[4].pp_ref);
	LIST_INSERT_AFTER(&test_pages[4], q, pp_link);
	//printf("---%d\n",LIST_NEXT(&test_pages[4],pp_link)->pp_ref);
	p = LIST_FIRST(&test_free);
	j=0;
	//printf("into test\n");
	while(p!=NULL){
		//      printf("%d %d\n",p->pp_ref,answer2[j]);
		assert(p->pp_ref==answer2[j++]);
		p=LIST_NEXT(p,pp_link);
	}



	printf("physical_memory_manage_check() succeeded\n");
}


void page_check(void)
{
	struct Page *pp, *pp0, *pp1, *pp2;
	struct Page_list fl;

	// should be able to allocate three pages
	pp0 = pp1 = pp2 = 0;
	assert(page_alloc(&pp0) == 0);
	assert(page_alloc(&pp1) == 0);
	assert(page_alloc(&pp2) == 0);

	assert(pp0);
	assert(pp1 && pp1 != pp0);
	assert(pp2 && pp2 != pp1 && pp2 != pp0);

	// temporarily steal the rest of the free pages
	fl = page_free_list;
	// now this page_free list must be empty!!!!
	LIST_INIT(&page_free_list);

	// should be no free memory
	assert(page_alloc(&pp) == -E_NO_MEM);

	// there is no free memory, so we can't allocate a page table
	assert(page_insert(boot_pgdir, pp1, 0x0, 0) < 0);

	// free pp0 and try again: pp0 should be used for page table
	page_free(pp0);
	assert(page_insert(boot_pgdir, pp1, 0x0, 0) == 0);
	assert(PTE_ADDR(boot_pgdir[0]) == page2pa(pp0));

	printf("va2pa(boot_pgdir, 0x0) is %x\n",va2pa(boot_pgdir, 0x0));
	printf("page2pa(pp1) is %x\n",page2pa(pp1));

	assert(va2pa(boot_pgdir, 0x0) == page2pa(pp1));
	assert(pp1->pp_ref == 1);

	// should be able to map pp2 at BY2PG because pp0 is already allocated for page table
	assert(page_insert(boot_pgdir, pp2, BY2PG, 0) == 0);
	assert(va2pa(boot_pgdir, BY2PG) == page2pa(pp2));
	assert(pp2->pp_ref == 1);

	// should be no free memory
	assert(page_alloc(&pp) == -E_NO_MEM);

	printf("start page_insert\n");
	// should be able to map pp2 at BY2PG because it's already there
	assert(page_insert(boot_pgdir, pp2, BY2PG, 0) == 0);
	assert(va2pa(boot_pgdir, BY2PG) == page2pa(pp2));
	assert(pp2->pp_ref == 1);

	// pp2 should NOT be on the free list
	// could happen in ref counts are handled sloppily in page_insert
	assert(page_alloc(&pp) == -E_NO_MEM);

	// should not be able to map at PDMAP because need free page for page table
	assert(page_insert(boot_pgdir, pp0, PDMAP, 0) < 0);

	// insert pp1 at BY2PG (replacing pp2)
	assert(page_insert(boot_pgdir, pp1, BY2PG, 0) == 0);

	// should have pp1 at both 0 and BY2PG, pp2 nowhere, ...
	assert(va2pa(boot_pgdir, 0x0) == page2pa(pp1));
	assert(va2pa(boot_pgdir, BY2PG) == page2pa(pp1));
	// ... and ref counts should reflect this
	assert(pp1->pp_ref == 2);
	printf("pp2->pp_ref %d\n",pp2->pp_ref);
	assert(pp2->pp_ref == 0);
	printf("end page_insert\n");

	// pp2 should be returned by page_alloc
	assert(page_alloc(&pp) == 0 && pp == pp2);

	// unmapping pp1 at 0 should keep pp1 at BY2PG
	page_remove(boot_pgdir, 0x0);
	assert(va2pa(boot_pgdir, 0x0) == ~0);
	assert(va2pa(boot_pgdir, BY2PG) == page2pa(pp1));
	assert(pp1->pp_ref == 1);
	assert(pp2->pp_ref == 0);

	// unmapping pp1 at BY2PG should free it
	page_remove(boot_pgdir, BY2PG);
	assert(va2pa(boot_pgdir, 0x0) == ~0);
	assert(va2pa(boot_pgdir, BY2PG) == ~0);
	assert(pp1->pp_ref == 0);
	assert(pp2->pp_ref == 0);

	// so it should be returned by page_alloc
	assert(page_alloc(&pp) == 0 && pp == pp1);

	// should be no free memory
	assert(page_alloc(&pp) == -E_NO_MEM);

	// forcibly take pp0 back
	assert(PTE_ADDR(boot_pgdir[0]) == page2pa(pp0));
	boot_pgdir[0] = 0;
	assert(pp0->pp_ref == 1);
	pp0->pp_ref = 0;

	// give free list back
	page_free_list = fl;

	// free the pages we took
	page_free(pp0);
	page_free(pp1);
	page_free(pp2);

	printf("page_check() succeeded!\n");
}

void pageout(int va, int context)
{
	u_long r;
	struct Page *p = NULL;

	if (context < 0x80000000) {
		panic("tlb refill and alloc error!");
	}

	if ((va > 0x7f400000) && (va < 0x7f800000)) {
		panic(">>>>>>>>>>>>>>>>>>>>>>it's env's zone");
	}

	if (va < 0x10000) {
		panic("^^^^^^TOO LOW^^^^^^^^^");
	}

	if ((r = page_alloc(&p)) < 0) {
		panic ("page alloc error!");
	}

	p->pp_ref++;

	page_insert((Pde *)context, p, VA2PFN(va), PTE_R);
	printf("pageout:\t@@@___0x%x___@@@  ins a page \n", va);
}


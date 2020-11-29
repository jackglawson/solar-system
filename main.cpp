#include "sim.h"
#include "collision.h"

// All this for detecting memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif


int main() {
	run_sim();
	_CrtDumpMemoryLeaks();
	return 0;
}
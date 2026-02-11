/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003

  Helper Functions Stuff

  I kept all the memory stuff and string stuff as seperate functions, so you can
  change the function calls if you're using a memory management or string library.
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/


#include "CaLua_helper.h"


#define TURN_CRTDBG_ON(x,f) x |= f
#define TURN_CRTDBG_OFF(x,f) x &= ~f

static FILE* err = NULL;		//Global file stream used for error reporting

void CLU_InitError()
{
	int tmpFlag;
	
#ifdef _DEBUG
	tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG);

	TURN_CRTDBG_ON(tmpFlag,		_CRTDBG_ALLOC_MEM_DF);			// Check the allocated memory blocks
	TURN_CRTDBG_OFF(tmpFlag,	_CRTDBG_DELAY_FREE_MEM_DF);		// Don't delay the freeing of memory blocks
	TURN_CRTDBG_ON(tmpFlag,		_CRTDBG_CHECK_ALWAYS_DF);		// Check heap every allocation op
	TURN_CRTDBG_OFF(tmpFlag,	_CRTDBG_CHECK_CRT_DF);			// Don't check CRT memory
	TURN_CRTDBG_ON(tmpFlag,		_CRTDBG_LEAK_CHECK_DF);			// Check for memory leaks upon program exit

	_CrtSetDbgFlag( tmpFlag);
#endif 

	// Open the file up, and damned to whatever the hell is in there!
	err = fopen(CLU_ERR_FILENAME,"wt");
	if(!err)
	{
		//shit shitty shit shit.. just try calling for help
		fprintf(stderr, "%s Unable to start up reporting system (very bad!)\n",CLU_ERR_PREFIX);
	}
}


void CLU_ShutdownError()
{
	fflush(err);
	fclose(err);
}


void CLU_Error(char* d, ...)
{
	va_list arg;
	
	va_start(arg, d);
#ifdef CLU_NOISY_ERROR
	printf("%s",CLU_ERR_PREFIX);
	vprintf(d,arg);
	printf("\n");
#endif
	fprintf(err,"%s",CLU_ERR_PREFIX);
	vfprintf(err,d,arg);
	fprintf(err,"\n");
	va_end(arg);
	fflush(err);
}


void* CLU_Calloc(int size)
{
	return(calloc(size,1));
}


void* CLU_Malloc(int size)
{
	return malloc(size);
}


void CLU_Free(void* memptr) {
	free(memptr);
}


void CLU_ExpandBuffer(void** in, int type, int data, int amount)
{
	void* tmp;

	if(!amount) return;
	if (*in == NULL) {
		*in = (void*)malloc(amount*type);
	} else if ((unsigned int)(type*(amount+data)) > _msize(*in)) {
		tmp = (void*)malloc(_msize(*in) + (amount*type));
		if (_msize(*in) > 0) {
			memcpy(tmp, *in, _msize(*in));
		}
		free(*in);
		*in = tmp;
	}
}


void CLU_InsertBuffer(void** in, int type, int sz, void* data, int where, int len)
{
	char* tmp;

	sz *= type;
	where *= type;
	len *= type;

	tmp = CLU_Calloc(sz + len);

	// Copy first half of data over
	memcpy( tmp, *in, where);

	// Insert data in spot
	memcpy( (void*)((int)tmp + where), data, len);

	// Copy the rest of the data over
	memcpy( (void*)((int)tmp + where + len), (void*)((int)*in + where), (sz - where));

	SAFE_FREE(*in);
	*in = tmp;
}


void CLU_SnipBuffer(void** in, int type, int sz, int loc, int len)
{
	char* tmp;

	sz *= type;
	loc *= type;
	len *= type;

	tmp = CLU_Calloc(sz - len);

    //Copy over part before ommission
	memcpy( (void*)tmp, *in, loc);

	//Copy over part after ommission
	memcpy( (void*)((int)tmp + loc), (void*)((int)(*in) + loc + len), sz - len - loc);

	SAFE_FREE(*in);
	*in = tmp;
}


char * CLU_String_Copy(char * string) {
	char * ret;
	int i = 0;
	if (!string) return NULL;
	do {
		i++;
	} while (string[i-1]);
	ret = CLU_Malloc(i*sizeof(char));
	while (i) {
		ret[i-1] = string[i-1];
		i--;
	}
	return ret;
}


int CLU_String_Concat(char** dst, char* src)
{
	if (!src) {
		return 1;
	} else if (!*dst) {
		*dst = CLU_String_Copy(src);
	} else {
		CLU_ExpandBuffer(dst, sizeof(char), strlen(*dst)+1, strlen(src));
		strcat(*dst, src);
	}
	return(0);
}
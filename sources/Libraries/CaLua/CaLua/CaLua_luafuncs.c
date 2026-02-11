/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003/2004

  Exposed Lua API functions

  All the functions exposed to Lua can be found up in heere.
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/


#include "CaLua_guts.h"


char* CLUArraySignature = "ptr:data, short:size, int:type, int:element_size, ptr:notouch";


// Handler function which gets called by Lua to take care of Lua->C function calls
// Prototype :
// CLU_Call(string, arg, arg, arg, ...);
// 
// string : Name of registered C function to call
// args : as many arguments as the C function is expecting
// 
// returns : Whatever the C function is registered to return
int Handler(lua_State* vm)
{
	char*			fName;
	int				fIndex;		//functions index
	CStack*			stack;
	
	//Variables used for the assembly loop
	int				stackSize;
	int				dataLocation;
	int				funcLoc;

	//registers used to pass arguments back
	int				a;			//EAX register
	int				d;			//EDX register
	double			f;			//Floating Stack pop

	// First argument off the stack is the functions registered name
	fName = (char*)lua_tostring(vm, 1);

	if((fIndex = GetFunctionIndex(fName)) == -1)
	{
		CLU_Error("Unknown function called from Lua script");
		return(0);
	}
	
	PushVM(vm);		// Set vm as our working virtual machine

	stack = PrepareStack(fIndex);
	if(!stack)
	{
		CLU_Error("Error converting stacks, aborting call...");
		return(0);
	}

	// More the stack pointer ourself, then move our stack stream onto the stack
	// one byte at a time.
	
	dataLocation = (int)&stack->data[0];
	stackSize = stack->dataLength;
	funcLoc = (int)funcs[fIndex]->fPtr;

	__asm
	{
		cmp stackSize,0;
		je Caller;
		sub esp, stackSize;
		mov edx, 0;
		mov ecx, dataLocation;
	Pusher:
		mov al, byte ptr[ecx+edx];
		mov byte ptr[esp+edx], al;
		inc edx;
		cmp edx, stackSize;
		jne Pusher;
	Caller:
		call funcLoc;
		mov a, eax;
		mov d, edx;
		fstp f;
	}

	if(funcs[fIndex]->callType == CLU_CDECL)
		__asm add esp, stackSize;

	if(funcs[fIndex]->numRetArgs)
		ParseReturnArgument( funcs[fIndex]->retType, a, d, f);

	FreeCStack(stack);

	PopVM();	// Go back to our original virtual machine
	
return(funcs[fIndex]->numRetArgs);
}


static int GetArraySize(void* a, int size)
{
	return(_msize(a)/size);
}


static void ThrowLuaError(lua_State* vm)
{
	lua_pushstring(vm,"Error calling CaLua function. See CaLua error log\n");
	lua_error(vm);
}


static int TypeCheck(lua_State* vm, char* func, int pos, int type)
{
	if(!(lua_type(vm, pos) == type))
	{
		CLU_Error("Call to %s : Expected %s, got %s", func, lua_typename(vm, type), lua_typename(vm, lua_type(vm,pos)));
		ThrowLuaError(vm);
		return(0);
	}
return(1);
}


static int ValidateArray(CLUArray* a)
{
	void* t = NULL;

	if(a->valid)
		t = (void*)a->valid;
	
	a->valid = CLU_Calloc(sizeof(char));
	*a->valid = 1;

	if(t)
	{
		*(char*)t = 0;
		SAFE_FREE(t);
	}

return(1);
}


static int VerfiyArrayIntegrity(CLUArray* a)
{
	int s;
	
	if(*a->valid != 1)					//This COULD fuck up if a compiler sets freed data to all 1's.. it won't ever be entirely perfect, but this is good enough
	{
		CLU_Error("Array Integrity Problem: Array is not valid (don't forget to do the '=' thing after array operations!!)");
		return(0);
	}

	if(!a->data)
	{
		CLU_Error("Array Integrity Problem: Data pointer of passed array is NULL.");
		return(0);
	}

	s = _msize(a->data);
	
	if( s / a->elementSize != a->size)
	{
		CLU_Error("Array Integrity Problem: Array size mismatch.");
		return(0);
	}

	if( GetDataSize( a->type) != a->elementSize)
	{
		CLU_Error("Array Integrity Problem: Array type size does not match element size.");
		return(0);
	}

return(1);
}


// Blindly frees a chunk of memory
// Prototype :
// CLU_DllFree(LUD);
// 
// LUD : LUD of the data to free
//
// returns : Nothing
int	LuaFree(lua_State* vm)
{
	void* p = NULL;

	TypeCheck(vm,"CLU_DllFree",1,LUA_TLIGHTUSERDATA);

	//Everything seems to be ok
	p = lua_touserdata(vm,1);
	SAFE_FREE(p);
	
return(0);
}


// Destroys a CaLua array
// Prototype :
// CLU_Array_Create(CLUTable);
// 
// CLUTable	:	CaLua array table that contains all the info of the table
//
// returns : Empty CLUTable
int Array_Destroy(lua_State* vm)
{
	CLUArray* a;

	TypeCheck(vm,"CLU_Array_Destroy",1,LUA_TTABLE);

	lua_pushvalue(vm, 1);
	
	a = (CLUArray*)PopRegTable(CLUARRAY_REG);

	SAFE_FREE(a->data);
	a->elementSize = 0;
	a->size = 0;
	a->type = CLU_UNKNOWN;
	*a->valid = 0;			//invalidate this array
	SAFE_FREE(a->valid);

	PushStructure(CLUARRAY_REG, a);

	SAFE_FREE(a);

return(0);
}


// From an array on the top of the stack, it pops off 'n'
// things of 'type'
void* PopInitArrayTable(int type, int* n)
{
	void* ret = NULL;
	void* tmp = NULL;
	int convertedSize;
	int dataSize;
	int i;

	dataSize = GetDataSize(type);

	i = 0;
	while(1)
	{
		lua_rawgeti(virtualMachine, -1, i+1);
		if(lua_type(virtualMachine, -1) == LUA_TNIL)
			break;
		else
			i++;
		
		CLU_ExpandBuffer(&ret, dataSize , (i-1), 1);
		
		tmp = ConvertLuaStackData( -1, type, &convertedSize);

		lua_pop(virtualMachine,1);

		if(convertedSize != dataSize)
		{
			CLU_Error("CaLua library fuckup: Call to PopInitArrayTable, converted size does not match array size");
			ThrowLuaError(virtualMachine);
		}

		memcpy( (void*)((int)ret + ((i-1)*dataSize)), tmp, dataSize);

		SAFE_FREE(tmp);
	}

	*n = i;

return(ret);
}


// Creates an array of a given size of a given type and returns LUD to it
// Prototype :
// CLU_Array_Create(string,num,[array]);
// 
// string	:	C data type of the array
// num		:	number of array elements to start off with
// [array]	:	A Lua array with initialization data (for my own laziness)
//
// returns : CLUTable
int	Array_Create(lua_State* vm)
{
	void* a;
	int dsize;
	int dtype;
	int	n;
	CLUArray ret;

	TypeCheck(vm,"CLU_Array_Create",1,LUA_TSTRING);
	TypeCheck(vm,"CLU_Array_Create",2,LUA_TNUMBER);

	dsize = GetDataSize((dtype=GetDataType((char*)lua_tostring(vm,1))));
	n = (int)lua_tonumber(vm,2);

	if(dsize==CLU_UNKNOWN)
	{
		CLU_Error("Call to CLU_Array_Create : Unknown passed type '%s'",dtype);
		ThrowLuaError(vm);
		return(0);
	}

	memset(&ret,0,sizeof(CLUArray));
	
	a = NULL;
	if(lua_gettop(vm) >= 3)
	{
		int numPopped;
        
		if(lua_type(vm,3) == LUA_TSTRING)
		{
			a = (char*)lua_tostring(vm,3);
            ret.data = CLU_Calloc((n = strlen(a)+1));
			memcpy(ret.data, a, n);
		}
		else
		{
	    	TypeCheck(vm,"CLU_Array_Create",3,LUA_TTABLE);
			PushVM(vm);
				a = PopInitArrayTable(dtype, &numPopped);
			PopVM();

			if(numPopped >= n)
			{
				n = numPopped;
				ret.data = a;
			}
			else
			{
				CLU_ExpandBuffer(&ret.data, dsize, numPopped, n);
				memcpy(ret.data, a, numPopped*dsize);
				SAFE_FREE(a);
			}
		}
	}
	else
	{
		ret.data = CLU_Calloc( n * dsize);
		
	}

	ret.elementSize = dsize;
	ret.size = n;
	ret.type = dtype;
	ValidateArray(&ret);

	PushStructure( CLUARRAY_REG, &ret);

return(1);
}


// Treat array as a C array and index into it as a C data type
// Prototype :
// CLU_Array_Get(CLUTable,num);
// 
// CLUTable	:	Array
// num		:	index to retrieve from array
//
// returns : Converted data type (either table, string, or number
int	Array_Get(lua_State* vm)
{
	CLUArray* a;
	int i;

	TypeCheck(vm,"CLU_Array_Get",1,LUA_TTABLE);
	TypeCheck(vm,"CLU_Array_Get",2,LUA_TNUMBER);

	lua_pushvalue(vm, 1);
	PushVM(vm);
		a = (CLUArray*)PopRegTable(CLUARRAY_REG);
	PopVM();
	
	if(!VerfiyArrayIntegrity(a))
	{
		CLU_Error("Call to CLU_Array_Get: Array integrity compromised. See error log for more info");
		ThrowLuaError(vm);
	}

	i = (int)lua_tonumber(vm,2);

	if( i > (a->size-1))
	{
		CLU_Error("Call to CLU_Array_Get : Attempting to index beyond array bounds");
		ThrowLuaError(vm);
	}

	PushVM(vm);
		CToLuaData(a->type, (void*)((int)a->data + (a->elementSize*i)) );
	PopVM();

	SAFE_FREE(a);

return(1);
}

// Inserts a piece of data into an array
// Prototype :
// CLU_Array_Insert(CLUTable,data,num);
// 
// CLUTable	:	Array to insert into
// data		:	some Lua data type to insert
// num		:	place in array to insert (0 puts it at the start of the array)
//
// returns : CLUTable that points to the new array
int	Array_Insert(lua_State* vm)
{
	CLUArray* a;
	void* dat;
	int pos;
	int sz;
	
	TypeCheck(vm,"CLU_Array_Insert",1,LUA_TTABLE);
	TypeCheck(vm,"CLU_Array_Insert",3,LUA_TNUMBER);
	
	PushVM(vm);
		lua_pushvalue(vm, 1);
			a = (CLUArray*)PopRegTable(CLUARRAY_REG);
		lua_pop(vm,1);
	PopVM();

		if(!VerfiyArrayIntegrity(a))
		{
			CLU_Error("Call to CLU_Array_Insert: Array integrity compromised. See error log for more info");
			ThrowLuaError(vm);
		}

		pos = (int)lua_tonumber(vm,3);

	PushVM(vm);
		dat = ConvertLuaStackData(2, a->type, &sz);
	PopVM();

	if( sz != a->elementSize)
	{
		CLU_Error("Call to CLU_Array_Insert: Data to be inserted does not mach data type in array");
		ThrowLuaError(vm);
	}

    if(!dat)
	{
		CLU_Error("Call to CLU_Array_Insert: Error converting passed type");
		ThrowLuaError(vm);
	}

	if(pos > a->size)
	{
		CLU_Error("Call to CLU_Array_Insert: Tried to insert past end of Array");
		ThrowLuaError(vm);
	}

	if( lua_type(vm,2) == LUA_TSTRING)				// Might look a bit dirty, but fuck it, we'll make a special case for strings
	{
		int len;
		SAFE_FREE(dat);
		dat = (char*)lua_tostring(vm,2);
		len = strlen((char*)dat);
		CLU_InsertBuffer(&a->data, a->elementSize, a->size, (char*)dat, pos, len);
		a->size += len;
	}
	else
	{
		CLU_InsertBuffer(&a->data, a->elementSize, a->size, dat, pos, 1);
		a->size += 1;
		SAFE_FREE(dat);
	}

	ValidateArray(a);
	PushStructure(CLUARRAY_REG, a);
	
	SAFE_FREE(a);

return(1);
}

// Slaps a piece of data onto the end of an array
// Prototype :
// CLU_Array_Append(CLUTable, data);
// 
// LUD		:	LUD of the C array
// data		:	some Lua data type to insert
//
// returns : CLUTable that points to the new array
int Array_Append(lua_State* vm)
{
	CLUArray* a;

	TypeCheck(vm,"CLU_Array_Append",1,LUA_TTABLE);
	TypeCheck(vm,"CLU_Array_Append",2,LUA_TSTRING);

	PushVM(vm);
		lua_pushvalue(vm,1);
		a = PopRegTable(CLUARRAY_REG);
		lua_pop(vm,1);
	PopVM();

	lua_pushnumber(vm, a->size);

	SAFE_FREE(a);

	Array_Insert(vm);

return(1);
}

// Turns an array of chars into a string
// Prototype :
// CLU_Array_CharToString(CLUTable);
// 
// LUD		:	LUD of the C array
//
// returns : Lua string representation of chars
int	Array_CharToString(lua_State* vm)
{
	CLUArray* a;

	TypeCheck(vm,"CLU_Array_CharToString",1,LUA_TTABLE);

	PushVM(vm);
		a = PopRegTable(CLUARRAY_REG);
	PopVM();

	lua_pushstring(vm, a->data);

	SAFE_FREE(a);

return(1);
}

// Deletes an array index
// Prototype :
// CLU_Array_Delete(CLUTable,number,number);
// 
// LUD		:	LUD of the C array
// number	:	array location to delete (zero based)
// number	:	amount of spots to take out
//
// returns  :	CLUTable to new array
int Array_Delete(lua_State* vm)
{
	CLUArray* a;
	int where;
	int howLong;

	TypeCheck(vm,"CLU_Array_Delete",1,LUA_TTABLE);
	TypeCheck(vm,"CLU_Array_Delete",2,LUA_TNUMBER);
	TypeCheck(vm,"CLU_Array_Delete",3,LUA_TNUMBER);

	PushVM(vm);
		lua_pushvalue(vm,1);
		a = PopRegTable(CLUARRAY_REG);
	PopVM();

	where = (unsigned int)lua_tonumber(vm, 2);
	howLong = (unsigned int)lua_tonumber(vm, 3);

	if(where + howLong > a->size)
		howLong = a->size - where + 1;

	if( where + howLong <= a->size)
	{
		CLU_SnipBuffer(&a->data, a->elementSize, a->size, where, howLong);
		a->size -= howLong;
	}
    
	ValidateArray(a);
	PushStructure(CLUARRAY_REG, a);

	SAFE_FREE(a);

return(1);
}


// Inserts the contents of one array into another, assuming they're both of the same sort
// Prototype :
// CLU_Array_InsertArray(CLUTable,CLUTable,number);
// 
// CLUTable		:	Array to be penetrated
// CLUTable 	:	Array to be penetrator
// number		:	Where you want it, girly?
//
// returns  :	CLUTable to new array
int	Array_InsertArray(lua_State* vm)
{
	CLUArray* dst;
	CLUArray* src;
	int where;

	TypeCheck(vm, "CLU_Array_InsertArray", 1, LUA_TTABLE);
	TypeCheck(vm, "CLU_Array_InsertArray", 2, LUA_TTABLE);
	TypeCheck(vm, "CLU_Array_InsertArray", 3, LUA_TNUMBER);

	PushVM(vm);
		lua_pushvalue(vm,1);
		dst = PopRegTable(CLUARRAY_REG);
		lua_pop(vm,1);

		lua_pushvalue(vm,2);
		src = PopRegTable(CLUARRAY_REG);
		lua_pop(vm,1);
	PopVM();

	where = (unsigned int)lua_tonumber(vm,3);

	if(dst->type != src->type)
	{
		CLU_Error("Call to CLU_Array_InsertArray: Source type does not match dest type");
		ThrowLuaError(vm);
	}

	if(where > dst->size)
	{
		CLU_Error("Call to CLU_Array_InsertArray: Attempting to insert past end of destination array");
		ThrowLuaError(vm);
	}

	CLU_InsertBuffer(&dst->data, dst->elementSize, dst->size, src->data, where, src->size);
	
	dst->size += src->size;

	ValidateArray(dst);
	PushStructure(CLUARRAY_REG, dst);

	SAFE_FREE(dst);
	SAFE_FREE(src);

return(1);
}


// Dereferences a pointer and converts it to number
// Prototype :
// CLU_LUD2Number(LUD,string);
// 
// LUD		:	Pointer to be dereferenced
// string 	:	A C numerical type
//
// returns  :	A number
int	LUD_To_Number(lua_State* vm)
{
	void* lud;
	int dtype;

	TypeCheck(vm,"CLU_LUD2Number",1,LUA_TLIGHTUSERDATA);
	TypeCheck(vm,"CLU_LUD2Number",2,LUA_TSTRING);
	
	dtype = GetDataType( (char*)lua_tostring(vm,2));
	lud = lua_touserdata(vm,1);

	if(!lud)
	{
		CLU_Error("Call to CLU_LUD2Number: NULL pointer passed (WTF?)");
		ThrowLuaError(vm);
	}

	switch(CLU_C_BASE_TYPE(dtype))
	{
	case CLU_CHAR :
		{
			if(CLU_IS_UNSIGNED(dtype))
				lua_pushnumber(vm, (double)(*(unsigned char*)lud));
			else
				lua_pushnumber(vm, (double)(*(signed char*)lud));
		}break;
	case CLU_SHORT :
		{
			if(CLU_IS_UNSIGNED(dtype))
				lua_pushnumber(vm, (double)(*(unsigned short*)lud));
			else
				lua_pushnumber(vm, (double)(*(signed short*)lud));
		}break;
	case CLU_INT :
		{
			if(CLU_IS_UNSIGNED(dtype))
				lua_pushnumber(vm, (double)(*(unsigned int*)lud));
			else
				lua_pushnumber(vm, (double)(*(signed int*)lud));
		}break;
	case CLU_FLOAT :
		{
			lua_pushnumber(vm, (double)(*(float*)lud));
		}break;
	case CLU_DOUBLE :
		{
			lua_pushnumber(vm, (double)(*(double*)lud));
		}break;
	default :
		{
			CLU_Error("Call to CLU_LUD2Number: Unknown or non-numeric type passed");
			ThrowLuaError(vm);
		}break;
	}

return(1);
}


// Turns a pointer into a Lua string
// Prototype :
// CLU_LUD2String(LUD);
// 
// LUD		:	Pointer to be dereferenced
//
// returns  :	A Lua string
int	LUD_To_String(lua_State* vm)
{
	TypeCheck(vm, "CLU_LUD2String", 1, LUA_TLIGHTUSERDATA);

	lua_pushstring(vm, (char*)lua_touserdata(vm, 1));
    
return(1);
}


// Dereferences a structure pointer and convert it to a table
// Prototype :
// CLU_LUD2Table(LUD,string);
// 
// LUD		:	Pointer to be dereferenced
// string 	:	String of the registered C type to convert it to
//
// returns  :	A table
int	LUD_To_Table(lua_State* vm)
{
	TypeCheck(vm, "CLU_LUD2Table", 1, LUA_TLIGHTUSERDATA);
	TypeCheck(vm, "CLU_LUD2Table", 2, LUA_TSTRING);

	PushStructure( CLU_TYPE_TO_INDEX( GetDataType( (char*)lua_tostring(vm, 2))), lua_touserdata(vm,1));

return(1);
}


// Converts a Lua table into a structure and returns a pointer (LUD) to that structure
// Which must later be freed with a call to CLU_DllFree passing the LUD
// Prototype :
// CLU_Table2LUD(table,string);
// 
// table	:	Registered table to be converted
// string 	:	Type of table to be converted
//
// returns  :	LUD, which must later be freed (CLU_DllFree)
int	Table_To_LUD(lua_State* vm)
{
	void* ret;
	int sz;

	TypeCheck(vm, "CLU_Table2LUD", 1, LUA_TTABLE);
	TypeCheck(vm, "CLU_Table2LUD", 2, LUA_TSTRING);

	PushVM(vm);
		ret = ConvertLuaStackData( 1, GetDataType( (char*)lua_tostring(vm,2)), &sz);
	PopVM();

	lua_pushlightuserdata(vm, ret);
      
return(1);
}


// Converts a Lua string to a C string (char*), which is LUD in Lua
// Prototype :
// CLU_String2LUD(string);
// 
// string	:	Lua string
//
// returns  :	LUD, which must later be freed (CLU_DllFree)
int	String_To_LUD(lua_State* vm)
{
	TypeCheck(vm, "CLU_String2LUD", 1, LUA_TSTRING);

	lua_pushlightuserdata(vm,(void*)CLU_String_Copy((char*)lua_tostring(vm, 1)));
    
return(1);
}


// Converts a Lua number into a pointer which points to some C numerical type
// CaLua will do it's best to convert the number into an appropriate form
// Prototype :
// CLU_Number2LUD(number,string);
// 
// number	:	Lua number
// string 	:	String of the C numerical pointer type
//
// returns  :	LUD, which must later be freed (CLU_DllFree)
int	Number_To_LUD(lua_State* vm)
{
	void* ret;
	int dtype;

	TypeCheck(vm, "CLU_Number2LUD", 1, LUA_TNUMBER);
	TypeCheck(vm, "CLU_Number2LUD", 2, LUA_TSTRING);

    dtype = GetDataType((char*)lua_tostring(vm, 2));

	switch(CLU_C_BASE_TYPE(dtype))
	{
	case CLU_CHAR :
		{
			ret = CLU_Calloc(GetDataSize(dtype));
			if(CLU_IS_UNSIGNED(dtype))
				*(unsigned char*)ret = (unsigned char)lua_tonumber(vm,1);
			else
				*(signed char*)ret = (signed char)lua_tonumber(vm,1);
		}break;
	case CLU_SHORT :
		{
			ret = CLU_Calloc(GetDataSize(dtype));
			if(CLU_IS_UNSIGNED(dtype))
				*(unsigned short*)ret = (unsigned short)lua_tonumber(vm,1);
			else
				*(signed short*)ret = (signed short)lua_tonumber(vm,1);
		}break;
	case CLU_INT :
		{
			ret = CLU_Calloc(GetDataSize(dtype));
			if(CLU_IS_UNSIGNED(dtype))
				*(unsigned int*)ret = (unsigned int)lua_tonumber(vm,1);
			else
				*(signed int*)ret = (signed int)lua_tonumber(vm,1);
		}break;
	case CLU_FLOAT :
		{
			ret = CLU_Calloc(GetDataSize(dtype));
			*(float*)ret = (float)lua_tonumber(vm,1);
		}break;
	case CLU_DOUBLE :
		{
			ret = CLU_Calloc(GetDataSize(dtype));
			*(double*)ret = (double)lua_tonumber(vm,1);
		}break;
	default :
		{
			CLU_Error("Call to CLU_Number2LUD: Unknown or non-numeric type requested");
			ThrowLuaError(vm);
		}break;
	}
	
	lua_pushlightuserdata(vm, ret);
    
return(1);
}


// Takes LUD of a known type, and makes it look like Lua data (will convert as best as possible)
// Prototype :
// CLU_UpdateLUD(LUD,string,data);
// 
// LUD		:	Pointer to be updated
// string 	:	String of the C type of the pointer
// data		:	Some Lua data that will be converted and update the pointer
//
// returns  :	Nothing!!
int	UpdateLUD(lua_State* vm)
{
	int sz = 0;
	void* lud;
	void* dat;

	TypeCheck(vm, "CLU_UpdateLUD", 1, LUA_TLIGHTUSERDATA);
	TypeCheck(vm, "CLU_UpdateLUD", 2, LUA_TSTRING);

	lud = lua_touserdata(vm, 1);
	dat = ConvertLuaStackData( 3, GetDataType((char*) lua_tostring( vm, 2)), &sz);
	
	memcpy( lud, dat, sz);

	SAFE_FREE(dat);

return(0);
}
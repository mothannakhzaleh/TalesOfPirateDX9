/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003/2004

  Function routines(?)

  Anything dealing with Lua->C function calling can be found in here.
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/

#include "CaLua_guts.h"



//\\ Takes offset 's' and moves it to the next boundary point divisible by 4
int To4ByteBoundary(int s)
{
	while(s%4)
		s++;
return(s);
}


//\\ Free's 'in'
int FreeStackData(StackData* in)
{
	int i = 0;

	while(i < in->numArgs)
	{
		SAFE_FREE(in->data[i]);
		i++;
	}

	SAFE_FREE(in->dataSize)
	SAFE_FREE(in->data);
	SAFE_FREE(in);

return(0);
}


//\\ Destroys our beloved C stack
int FreeCStack(CStack* in)
{
	if(in->data)
		SAFE_FREE(in->data);

	SAFE_FREE(in);
return(0);
}


//\\ Prepare's a CStack by popping 'args' off the stack 'vm'
//\\ args can take on the form of any C data type, or a registered structure
CStack* PrepareStack(int rFunc)
{
	StackData*	stackData;
	CStack*		ret;

	int i,j;					//counters
	int lastArg;				//Stack index of the highest argument
	int boundSize;				//Size of the stack element, bound to the nearest 4 byte offset
	
	//First we gotta get our data from the Lua stack, and convert it to appropriate C values
	
	stackData = CLU_Calloc(sizeof(StackData));

	lastArg = funcs[rFunc]->numPassArgs + 1;
	i = 2;						// Start at argument 2, since the first argument was popped in Handler()
	while( i <= lastArg)
	{
		CLU_ExpandBuffer(&(void*)stackData->data, sizeof(void*), stackData->numArgs,1);
		CLU_ExpandBuffer(&stackData->dataSize, sizeof(int*), stackData->numArgs,1);

		stackData->data[stackData->numArgs] = ConvertLuaStackData( i, funcs[rFunc]->passType[i-2], &stackData->dataSize[stackData->numArgs]);

		if(!stackData->data[stackData->numArgs])
		{
			stackData->numArgs++;
			FreeStackData(stackData);
			return(NULL);
		}
		
		stackData->numArgs++;
		i++;
	}

	
	// Next we gotta prepare the C stack. Look through the converted Lua data, and make sure the arguments are aligned on 4 bytes
	ret = CLU_Calloc(sizeof(CStack));

	// ARGH!! Another difficulty we have to deal with is returning structures. If we're returning a structure, we have to
	// Create a space for it, (has to be on the stack??)....
	if(CLU_IS_STRUCTURE(funcs[rFunc]->retType))
	{
		CLU_ExpandBuffer( &ret->data, sizeof(void*), ret->dataLength, sizeof(void*));
		((void*)((int*)ret->data)[0]) = CLU_Calloc(GetDataSize(funcs[rFunc]->retType));
		ret->dataLength += 4;
	}
	for(i = 0; i < stackData->numArgs; i++)
	{
		boundSize = To4ByteBoundary(stackData->dataSize[i]);

		CLU_ExpandBuffer( &ret->data, sizeof(char), ret->dataLength, boundSize);
	
		memcpy(&ret->data[ret->dataLength], stackData->data[i], stackData->dataSize[i]);

		ret->dataLength += stackData->dataSize[i];
		
		//Pad the rest with 0's
		for(j = 0; j < (boundSize - stackData->dataSize[i]); j++)
			ret->data[ ret->dataLength + j] = 0;

		ret->dataLength += (boundSize - stackData->dataSize[i]);
	}

	FreeStackData(stackData);

return(ret);
}


//\\ Look at what the C function returned and try to pass it back to Lua as best as possible
//\\ ea : state of EAX after function call
//\\ ed : state of EDX after function call
//\\ fs : top of the floating point stack after function call	
int ParseReturnArgument(int cType, int ea, int ed, double fs)
{
	switch(CLU_C_BASE_TYPE(cType))
	{
	case CLU_CHAR :
		{
			lua_pushnumber(virtualMachine, (0xFF&ea));
		}break;
	case CLU_SHORT :
		{
			lua_pushnumber(virtualMachine, 0xFFFF&ea);
		}break;
	case CLU_INT :
		{
			lua_pushnumber(virtualMachine, ea);
		}break;
	case CLU_FLOAT :										// Floats, while being 32-bit, are returned in the floating point register
		{
			lua_pushnumber(virtualMachine,fs);
		}break;
	case CLU_DOUBLE :
		{
			lua_pushnumber(virtualMachine,fs);			
		}break;
	case CLU_POINTER :
		{
			lua_pushlightuserdata(virtualMachine, (void*)(ea));
		}break;
	case CLU_UNKNOWN :
		{
			if(CLU_IS_POINTER(cType))
			{
				lua_pushlightuserdata(virtualMachine, (void*)(ea));			// They're expecting a pointer to SOMETHING, in any case, it would be stored in eax
			}
			else
			{
				CLU_Error("Unable to return unknown C type to Lua\n");
			}
		}break;
	default :
		{
			if(CLU_IS_POINTER(cType))
			{
				lua_pushlightuserdata(virtualMachine, (void*)(ea));
			}
			else
			{
				PushStructure(CLU_TYPE_TO_INDEX(cType), (void*)(ea));
				CLU_Free((void*)ea);						//A literal structure has to have it's pointer free'd
			}
		}break;
	}	
	
return(0);
}
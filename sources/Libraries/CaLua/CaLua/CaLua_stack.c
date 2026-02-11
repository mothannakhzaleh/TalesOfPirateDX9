/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003/2004

  Lua stack interaction routines

  If it touches the stack in any way, chances are it will be in here.
  If you're looking to hack around with CaLua, this is definitly a file you'll want to be familiar
  with.
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/

#include "CaLua_guts.h"


//\\ Pops values off the Lua stack, from a table, and returns a void pointer
//\\ to a structure that looks like the data structure of the registered function
void* PopRegTable(int regStruct)
{
	int i = 0;

	void* ret;

	void**	dPointers = NULL;		//D.A of pointers of the data types themselves
	int*	dTypes = NULL;			//D.A of the types of data at the pointers
	int		nMembers = 0;		//Number of datat members that have come off the stack

	// We can assume that the table is at the top of the stack

	nMembers = structs[regStruct]->numMembers;

	for(i = 0; i < structs[regStruct]->numMembers; i++)
	{
		lua_pushstring(virtualMachine, structs[regStruct]->memberName[i]);
		lua_rawget(virtualMachine, -2);

		CLU_ExpandBuffer(&(void*)dPointers,sizeof(void*), i, 1);
		CLU_ExpandBuffer(&(void*)dTypes,sizeof(int*), i, 1);

		dTypes[i] = structs[regStruct]->memberType[i];
		
		// Data member should be at the top of the stack now...

		switch(CLU_C_BASE_TYPE(structs[regStruct]->memberType[i]))
		{
		case CLU_CHAR :
			{
				dPointers[i] = CLU_Calloc(sizeof(char));
				*(char*)dPointers[i] = (char)lua_tonumber(virtualMachine, -1);
			}break;
		case CLU_SHORT :
			{
				dPointers[i] = CLU_Calloc(sizeof(short));
				*(short*)dPointers[i] = (short)lua_tonumber(virtualMachine, -1);
			}break;
		case CLU_INT :
			{
				dPointers[i] = CLU_Calloc(sizeof(int));
				*(int*)dPointers[i] = (int)lua_tonumber(virtualMachine, -1);
			}break;
		case CLU_FLOAT :
			{
				dPointers[i] = CLU_Calloc(sizeof(float));
				*(float*)dPointers[i] = (float)lua_tonumber(virtualMachine, -1);
			}break;
		case CLU_DOUBLE :
			{				
				dPointers[i] = CLU_Calloc(sizeof(double));
				*(double*)dPointers[i] = (double)lua_tonumber(virtualMachine, -1);
			}break;
		case CLU_POINTER :
			{
				dPointers[i] = CLU_Calloc(sizeof(void*));
				*(void**)dPointers[i] = lua_touserdata(virtualMachine, -1);
			}break;
		case CLU_STRING :
			{
				dPointers[i] = CLU_Calloc(sizeof(char*));
				*(char**)dPointers[i] = (char*)lua_tostring(virtualMachine, -1);
			}break;
		case CLU_UNKNOWN :
			{
				CLU_Error("Unknown type passed to PopTable : %X", regStruct);
			}
		default :
			{
				if(CLU_IS_POINTER(structs[regStruct]->memberType[i]))
				{
					dPointers[i] = CLU_Calloc(sizeof(void*));
					*(void**)dPointers[i] = PopRegTable( CLU_TYPE_TO_INDEX(structs[regStruct]->memberType[i]) );
				}
				else
				{
					dPointers[i] = PopRegTable( CLU_TYPE_TO_INDEX(structs[regStruct]->memberType[i]) );
				}
			}break;
		}

		lua_pop(virtualMachine, 1);
	}

	ret = Pack(nMembers, dPointers, dTypes);

	// Free our D.A's
	for(i=0; i < nMembers;i++)
		SAFE_FREE(dPointers[i]);

	SAFE_FREE(dTypes);
	SAFE_FREE(dPointers);

	
return(ret);
}


//\\ Pops values off the Lua stack, and returns a void pointer
//\\ to a structure that looks like 'desc'
void* PopValuesBySig(char* desc)
{
	void* ret = NULL;
	int nToken;
	int totalTokens;
	char* tok = NULL;
	int dataType;
	int i;

	void**	dPointers = NULL;		//D.A of pointers of the data types themselves
	int*	dTypes = NULL;			//D.A of the types of data at the pointers
	int		nMembers;		//Number of datat members that have come off the stack
	

	// First, we have to simply get the data off the Lua stack..
	// We'll store it in a D.A of pointers, and a D.A of data types, which we'll
	// Put together with proper alignment bellow

	// Tokens are passed on the Lua stack left to right, therefore, the arguments on top of the stack
	// are the rightmost arguments....

	totalTokens = GetNumberOfTokens(desc);
	nToken = totalTokens;
	nMembers = 0;
	while(nToken > 0)
	{
		tok = GetToken(desc, nToken);
		dataType = GetDataType(tok);

		if(dataType == CLU_UNKNOWN)
		{
			CLU_Error("Unknown data type : %s", tok);
			// Be super safe!
			SAFE_FREE(tok);
			SAFE_FREE(dPointers);
			SAFE_FREE(dTypes);
			return(NULL);
		}

		CLU_ExpandBuffer((void*)&dPointers, sizeof(void*), nMembers, 1);
		CLU_ExpandBuffer((void*)&dTypes, sizeof(int), nMembers, 1);
		nMembers++;

		dTypes[nMembers-1] = dataType;

		switch(CLU_C_BASE_TYPE(dataType))
		{
		case CLU_CHAR :
			{
				dPointers[nMembers-1] = CLU_Calloc(sizeof(char));
				*(char*)dPointers[nMembers-1] = (char)lua_tonumber(virtualMachine, -1);
			}break;
		case CLU_SHORT :
			{
				dPointers[nMembers-1] = CLU_Calloc(sizeof(short));
				*(short*)dPointers[nMembers-1] = (short)lua_tonumber(virtualMachine, -1);
			}break;
		case CLU_INT :
			{
				dPointers[nMembers-1] = CLU_Calloc(sizeof(int));
				*(int*)dPointers[nMembers-1] = (int)lua_tonumber(virtualMachine, -1);
			}break;
		case CLU_FLOAT :
			{
				dPointers[nMembers-1] = CLU_Calloc(sizeof(float));
				*(float*)dPointers[nMembers-1] = (float)lua_tonumber(virtualMachine, -1);
			}break;
		case CLU_DOUBLE :
			{
				dPointers[nMembers-1] = CLU_Calloc(sizeof(double));
				*(double*)dPointers[nMembers-1] = (double)lua_tonumber(virtualMachine, -1);
			}break;
		case CLU_STRING :
			{
				dPointers[nMembers-1] = CLU_Calloc(sizeof(char*));
				*(char**)dPointers[nMembers-1] = (char*)lua_tostring(virtualMachine, -1);
			}break;
		case CLU_POINTER :
			{
				dPointers[nMembers-1] = CLU_Calloc(sizeof(void*));
				*(void**)dPointers[nMembers-1] = lua_touserdata(virtualMachine, -1);
			}break;
		default :
			{
				if(CLU_IS_POINTER(dataType))
				{
					dPointers[nMembers-1] = CLU_Calloc(sizeof(void*));
					*(void**)dPointers[nMembers-1] = PopRegTable( CLU_TYPE_TO_INDEX(dataType) );
				}				
				else
				{
					dPointers[nMembers-1] = PopRegTable( CLU_TYPE_TO_INDEX(dataType) );
				}
			}
		}

		lua_pop(virtualMachine, 1);

		SAFE_FREE(tok);
		nToken--;
	}

	// Because the arguments came in the reverse order, we should flip them around....
	for(i=0; i < (int)(totalTokens/2);i++)
	{
		void* t;		// Temporary variables used for flipping
		int t2;

		// Flip the pointers
		t = dPointers[i];
		dPointers[i] = dPointers[totalTokens-i-1];
		dPointers[totalTokens-i-1] = t;

		// Then the data types
		t2 = dTypes[i];
		dTypes[i] = dTypes[totalTokens-i-1];
		dTypes[totalTokens-i-1] = t2;
	}

	ret = Pack(totalTokens, dPointers, dTypes);

	// Free our D.A's

	for(i=0; i<totalTokens; i++)
		SAFE_FREE(dPointers[i]);
	
	SAFE_FREE(dTypes);
	SAFE_FREE(dPointers);

return(ret);
}


//\\ Pushes data from 'data' onto the stack according to how
//\\ registered structure 'regStruct' is layed out
int PushStructure(int regStruct, void* data)
{
	StructForm* st;			//cleans up the code quite a bit
	void* marker;				//temporary variable for moving data
	int i;
	
	int tableLocation;		//Table's stack location
	
	st = structs[regStruct];
	marker = data;


	//Push a new table onto the stack
	lua_checkstack(virtualMachine, lua_gettop(virtualMachine) + 1);
	lua_newtable(virtualMachine);
	tableLocation = lua_gettop(virtualMachine);

	//Go through all the structures data and push it
	//We'll push two data elements onto the stack at a time, but the setttable at the end, will pop the values off the stack
	for(i = 0; i < st->numMembers; i++)
	{
		lua_pushstring(virtualMachine, st->memberName[i]);
		
		switch(CLU_C_BASE_TYPE(st->memberType[i]))
		{
		case CLU_CHAR :
			{
				lua_pushnumber(virtualMachine, (double)(*(char*)marker));
			}break;
		case CLU_SHORT :
			{
				lua_pushnumber(virtualMachine, (double)(*(short*)marker));
			}break;
		case CLU_INT :
			{
				lua_pushnumber(virtualMachine, (double)(*(int*)marker));
			}break;
		case CLU_FLOAT:
			{
				lua_pushnumber(virtualMachine, (double)(*(float*)marker));
			}break;
		case CLU_DOUBLE :
			{
				lua_pushnumber(virtualMachine, (double)(*(double*)marker));
			}break;
		case CLU_POINTER :
			{
				lua_pushlightuserdata(virtualMachine, *(void**)marker);
			}break;
		case CLU_STRING :
			{
				lua_pushstring(virtualMachine, *(char**)marker);
			}break;
		case CLU_UNKNOWN :
			{
				printf("Attempting to push unknown data type : %X", st->memberType[i]);
				return(1);
			}break;
		default :					//Must be a registered structure
			{
				if(CLU_IS_POINTER(st->memberType[i]))
					PushStructure( CLU_TYPE_TO_INDEX(st->memberType[i]), *(void**)marker);
				else
					PushStructure( CLU_TYPE_TO_INDEX(st->memberType[i]), marker);
			}break;
		}
	
		lua_settable(virtualMachine, tableLocation);
		((char*)marker) += st->NextMemberOffset[i];
	}

return(0);
}


//\\ A real mess that I wanted to rip out of PrepareStack...
//\\ Basically a chunk of code that looks at every possible case
//\\ that could come up when converting Lua data types to C data types
//\\ NOTE :
//\\ All pointers have deep copies made of them
void* ConvertLuaStackData(int pos, int cType, int* size)
{
	void* ret = NULL;
	
	switch(CLU_C_BASE_TYPE(cType))
	{
	case CLU_CHAR :
		{
			ret = CLU_Calloc(sizeof(char));
			*size = sizeof(char);
			switch( lua_type(virtualMachine,pos))
			{
				case LUA_TNUMBER :						//Most likely a number between 0 and 256
				{
					*(char*)ret = (char)lua_tonumber(virtualMachine, pos);
				}
				break;
				case LUA_TBOOLEAN :						//Just a 1 or a zero
				{
					*(char*)ret = (char)lua_toboolean(virtualMachine, pos);
				}
				break;
				case LUA_TSTRING :
					{
						*(char*)ret = *(char*)lua_tostring(virtualMachine, pos);
					}break;
				case LUA_TNIL :							//Just pass 0
				{
					*(char*)ret = 0;
				}
				break;
				default :								//Nothing else makes sense
				{
					CLU_Error("Unable to convert from Lua type %s to C type 'char'", lua_typename(virtualMachine,lua_type(virtualMachine,pos)) );
					SAFE_FREE(ret);
					return(NULL);
				}
				break;
			}
		}break;
	case CLU_SHORT :
		{
			ret = CLU_Calloc(sizeof(short));
			*size = sizeof(short);
			switch( lua_type(virtualMachine,pos))
			{
				case LUA_TNUMBER :						//Makes perfect sense
				{
					*(short*)ret = (short)lua_tonumber(virtualMachine, pos);
				}
				break;
				case LUA_TBOOLEAN :						//Just a 1 or a zero
				{
					*(short*)ret = (short)lua_toboolean(virtualMachine, pos);
				}
				break;
				case LUA_TNIL :							//Just pass it as zero
				{
					*(short*)ret = 0;
				}
				break;
				default :								//Nothing else makes sense
				{
					CLU_Error("Unable to convert from Lua type %s to C type 'short'", lua_typename(virtualMachine,lua_type(virtualMachine,pos)) );
					SAFE_FREE(ret);
					return(NULL);
				}
				break;
			}
		}break;
	case CLU_INT :
		{
			ret = CLU_Calloc(sizeof(int));
			*size = sizeof(int);
			switch( lua_type(virtualMachine,pos))
			{
				case LUA_TNUMBER :						//Makes perfect sense
				{
					*(int*)ret = (int)lua_tonumber(virtualMachine, pos);
				}
				break;
				case LUA_TBOOLEAN :						//Just a 1 or a zero
				{
					*(int*)ret = (int)lua_toboolean(virtualMachine, pos);
				}
				break;
				case LUA_TNIL :							//Just pass it as zero
				{
					*(int*)ret = 0;
				}
				break;
				default :								//Nothing else makes sense
				{
					CLU_Error("Unable to convert from Lua type %s to C type 'int'", lua_typename(virtualMachine,lua_type(virtualMachine,pos)) );
					SAFE_FREE(ret);
					return(NULL);
				}
				break;
			}
		}break;
	case CLU_FLOAT :
		{
			ret = CLU_Calloc(sizeof(float));
			*size = sizeof(float);
			switch( lua_type(virtualMachine,pos))
			{
				case LUA_TNUMBER :						//Makes perfect sense
				{
					*(float*)ret = (float)lua_tonumber(virtualMachine, pos);
				}
				break;
				case LUA_TBOOLEAN :						//Just a 1 or a zero
				{
					*(float*)ret = (float)lua_toboolean(virtualMachine, pos);
				}
				break;
				case LUA_TNIL :							//Just pass it as zero
				{
					*(float*)ret = 0;
				}
				break;
				default :								//Nothing else makes sense
				{
					CLU_Error("Unable to convert from Lua type %s to C type 'float'", lua_typename(virtualMachine,lua_type(virtualMachine,pos)) );
					SAFE_FREE(ret);
					return(NULL);
				}
				break;
			}
		}break;
	case CLU_DOUBLE :
		{
			ret = CLU_Calloc(sizeof(double));
			*size = sizeof(double);
			switch( lua_type(virtualMachine,pos))
			{
				case LUA_TNUMBER :						//Makes perfect sense
				{
					*(double*)ret = lua_tonumber(virtualMachine, pos);
				}
				break;
				case LUA_TBOOLEAN :						//Just a 1 or a zero
				{
					*(double*)ret = lua_toboolean(virtualMachine, pos);
				}
				break;
				case LUA_TNIL :							//Just pass it as zero
				{
					*(double*)ret = 0;
				}
				break;
				default :								//Nothing else makes sense
				{
					CLU_Error("Unable to convert from Lua type %s to C type 'double'", lua_typename(virtualMachine,lua_type(virtualMachine,pos)) );
					SAFE_FREE(ret);
					return(NULL);
				}
				break;
			}
		}break;

	case CLU_STRING :
		{
			ret = CLU_Calloc(sizeof(char*));
			*size = sizeof(char*);
			switch(lua_type(virtualMachine,pos))
			{
				case LUA_TSTRING :
					{
						*(char**)ret = (char*)lua_tostring(virtualMachine, pos);
					}break;
				case LUA_TUSERDATA :
				case LUA_TLIGHTUSERDATA :
				{
					*(void**)ret = lua_touserdata(virtualMachine, pos);
				}break;
				default :
				{
					CLU_Error("Unable to convert from Lua type %s to C type 'string(char*)'", lua_typename(virtualMachine,lua_type(virtualMachine,pos)) );
					SAFE_FREE(ret);
					return(NULL);
				}break;
			}
		}break;
	case CLU_POINTER :
		{
			ret = CLU_Calloc(sizeof(void*));
			*size = sizeof(void*);
			switch(lua_type(virtualMachine,pos))
			{
				case LUA_TUSERDATA :
				case LUA_TLIGHTUSERDATA :
				{
					*(void**)ret = lua_touserdata(virtualMachine, pos);
				}break;
				default :
				{
					CLU_Error("Unable to convert from Lua type %s to C type 'raw pointer'", lua_typename(virtualMachine,lua_type(virtualMachine,pos)) );
					SAFE_FREE(ret);
					return(NULL);
				}break;
			}
		}break;
	case CLU_UNKNOWN :										//In some cases, it is possible to guess what the user is expecting
		{
			if(CLU_IS_POINTER(cType))										
			{
				switch( lua_type(virtualMachine,pos))
				{
					case LUA_TUSERDATA :
					case LUA_TLIGHTUSERDATA :				//These types can be passed safely as ambiguous pointers
					{
						ret = CLU_Calloc(sizeof(void*));
						*size = sizeof(void*);

						*(void**)ret = lua_touserdata(virtualMachine,pos);
					}
					break;
					default :								//Anything else should signal an error
					{
						CLU_Error("Unable to convert from Lua type %s to C unregistered structure pointer", lua_typename(virtualMachine,lua_type(virtualMachine,pos)));
						return(NULL);
					}
					break;
				}
			}
			else											//There's no safe way to pass it as an ambiguous SOMETHING.. so.. error
			{
				CLU_Error("Unable to convert from Lua type %s to C unknown type", lua_typename(virtualMachine,lua_type(virtualMachine,pos)));
			}
		}break;
	default :												//It must be a registered structure
		{
			if( CLU_IS_POINTER(cType))
			{
				ret = CLU_Calloc(sizeof(void*));
				*size = sizeof(void*);
				switch( lua_type(virtualMachine,pos))
				{
					case LUA_TUSERDATA :
					case LUA_TLIGHTUSERDATA :				//Just make a copy of the pointer.. fairly obvious
					{
						*(void**)ret = lua_touserdata(virtualMachine, pos);
					}
					break;
					default :								//Nothing else makes sense
					{
						CLU_Error("Unable to convert from Lua type %s to C registered structure '%s' pointer", lua_typename(virtualMachine,lua_type(virtualMachine,pos)), structs[CLU_TYPE_TO_INDEX(cType)]->name );
						SAFE_FREE(ret);
						return(NULL);
					}
					break;
				}
			}
			else
			{
				*size = GetDataSize(cType);
				switch( lua_type(virtualMachine,pos))
				{
				case LUA_TTABLE :						//Pop the table, pack it, then make the 'data' point to it....						
					{
						lua_pushvalue(virtualMachine, pos);		//Push the table to the top of the stack
						ret = PopRegTable(CLU_TYPE_TO_INDEX(cType));	//Now let PopTable deal with it
						lua_pop(virtualMachine,1);				//Don't forget to cleanup the stack when we're done
					}
					break;
				default :								//Nothing else makes sense
					{
						CLU_Error("Unable to convert from Lua type %s to C registered structure '%s'",lua_typename(virtualMachine,lua_type(virtualMachine,pos)), structs[CLU_TYPE_TO_INDEX(cType)]->name);
						return(NULL);
					}
					break;
				}
			}
		}break;
	}

return(ret);
}


int	CToLuaData(int type, void* data)
{
	switch(type)
	{
	case CLU_CHAR :		{lua_pushnumber(virtualMachine, (double)(*(char*)data));}break;
	case CLU_SHORT :	{lua_pushnumber(virtualMachine, (double)(*(short*)data));}break;
	case CLU_INT :		{lua_pushnumber(virtualMachine, (double)(*(int*)data));}break;
	case CLU_FLOAT :	{lua_pushnumber(virtualMachine, (double)(*(float*)data));}break;
	case CLU_DOUBLE :	{lua_pushnumber(virtualMachine, (double)(*(double*)data));}break;
	case CLU_STRING :	{lua_pushstring(virtualMachine, *(const char**)data);}break;
	case CLU_POINTER :	{lua_pushlightuserdata(virtualMachine, data);}break;
	case CLU_UNKNOWN :	{CLU_Error("Error pushing type %X",type);return(1);}break;				//Don't know how the hell we'd ever get here
	default :
		{
			if(CLU_IS_POINTER(type))
			{
				PushStructure(CLU_TYPE_TO_INDEX(type),*(void**)data);
			}
			else
			{
                PushStructure(CLU_TYPE_TO_INDEX(type),data);
			}
		}break;
	}
 
return(0);
}
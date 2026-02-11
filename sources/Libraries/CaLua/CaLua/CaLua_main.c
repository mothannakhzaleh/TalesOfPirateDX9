/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003/2004

  Exposed C API routines

  Functions that are exposed as dll exports can be found in here...
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/

#include "CaLua_guts.h"

#define NUMTYPES	9

int	typeList[] = {CLU_CHAR, CLU_CHAR|CLU_UNSIGNED_BIT, 
					CLU_SHORT, CLU_SHORT|CLU_UNSIGNED_BIT,
					CLU_INT, CLU_INT|CLU_UNSIGNED_BIT,
					CLU_POINTER,
					CLU_FLOAT,
					CLU_DOUBLE};

char* typeNameList[] = {"char","uchar",
						"short","ushort",
						"int","uint",
						"ptr",
						"float",
						"double"};


CLU_AVL*	baseTypeLU = NULL;			//A Quick way to look up base types in GetDataType

StructForm** structs = NULL;			//List of registered structures
int			numStructs = 0;
CLU_AVL*	structsLU = NULL;			//Quick Name Lookup Table

FuncForm**	funcs = NULL;				//List of registered functions
int			numFuncs = 0;
CLU_AVL*	funcsLU = NULL;				//Quick Name Lookup Table

lua_State*	virtualMachine = NULL;		//Main virtual machine

lua_State*	vmStack[CLU_VM_STACK_SIZE]; // virtualMachine stack
int			vmStackPos = 0;					// How deep we are into the stack..

VMSlate		stateStack[CLU_STATE_STACK_SIZE];		//The stack that will be used to push whole CaLua states
int			clearStateStackFlag = 0;				//Needed to clean out the state stack
int			currentState = 0;						//The current state that CaLua is using

//\\ Starts up the lua virtual machine, and loads all the standard Lua libraries
//\\ Also registers our Lua->C handler function
int CLU_Init()
{
	int i;
    CLU_LUNode* node;

	CLU_InitError();

	UpdateState(currentState);
	
	//Reset global variables
	structs = NULL;			
	numStructs = 0;
	structsLU = NULL;
	funcs = NULL;
	numFuncs = 0;
	funcsLU = NULL;
	virtualMachine = NULL;

	if(!clearStateStackFlag)
	{
		memset(stateStack, 0, sizeof(VMSlate) * CLU_STATE_STACK_SIZE);
		clearStateStackFlag++;
	}

	virtualMachine = lua_open();
	if(!virtualMachine)
	{
		CLU_Error("Error creating virtual machine");
		return(-1);
	}

	//Open up our libraries
	//Hmmm... base returns 0 if it completes fine, the others return 1... interesting...
	if(lua_baselibopen(virtualMachine)){		CLU_Error("Error opening Lua base library");		return(-1);}
	if(!lua_tablibopen(virtualMachine)){		CLU_Error("Error opening Lua table library");	return(-1);}
	if(!lua_iolibopen(virtualMachine)){			CLU_Error("Error opening Lua io library");		return(-1);}
	if(!lua_strlibopen(virtualMachine)){		CLU_Error("Error opening Lua string library");	return(-1);}
	if(!lua_mathlibopen(virtualMachine)){		CLU_Error("Error opening Lua math library");		return(-1);}
	if(!lua_dblibopen(virtualMachine)){			CLU_Error("Error opening Lua debug library");	return(-1);}
	
	//register our handler functions, too bad there's no return error code :(
	lua_register(virtualMachine,"CLU_Call",Handler);
	lua_register(virtualMachine,"CLU_DllFree",LuaFree);
	lua_register(virtualMachine,"CLU_Array_Create",Array_Create);
	lua_register(virtualMachine,"CLU_Array_Destroy",Array_Destroy);
	lua_register(virtualMachine,"CLU_Array_Get",Array_Get);
	lua_register(virtualMachine,"CLU_Array_Insert",Array_Insert);
	lua_register(virtualMachine,"CLU_Array_Append",Array_Append);
	lua_register(virtualMachine,"CLU_Array_CharToString",Array_CharToString);
	lua_register(virtualMachine,"CLU_Array_Delete",Array_Delete);
	lua_register(virtualMachine,"CLU_Array_InsertArray",Array_InsertArray);
	lua_register(virtualMachine,"CLU_LUD2Number",LUD_To_Number);
	lua_register(virtualMachine,"CLU_LUD2String",LUD_To_String);
	lua_register(virtualMachine,"CLU_LUD2Table",LUD_To_Table);
	lua_register(virtualMachine,"CLU_Number2LUD",Number_To_LUD);
	lua_register(virtualMachine,"CLU_String2LUD",String_To_LUD);
	lua_register(virtualMachine,"CLU_Table2LUD",Table_To_LUD);
	lua_register(virtualMachine,"CLU_UpdateLUD",UpdateLUD);
	
	//Init LU tables
	funcsLU = CLU_AVL_Create(CLU_LUNode_Destroy, CLU_Hash_Key_Destroy, CLU_Hash_Key_Cmp);
	structsLU = CLU_AVL_Create(CLU_LUNode_Destroy, CLU_Hash_Key_Destroy, CLU_Hash_Key_Cmp);
	baseTypeLU = CLU_AVL_Create(CLU_LUNode_Destroy, CLU_Hash_Key_Destroy, CLU_Hash_Key_Cmp);

	for(i = 0; i < NUMTYPES; i++)
	{
        node = CLU_LUNode_Create();
		node->index = typeList[i];
		CLU_AVL_Insert(baseTypeLU, node, CLU_Hash_Key_Create(typeNameList[i]));
	}
		
	//Register any Lua API table types
	if(CLU_RegisterStructure("CLUArray", CLUArraySignature))
		CLU_Error("Error registering CLUArray.... something has gone terribly wrong!!");

return(CLU_LoadState(SaveNewState()));
}


// Shuts down a particular CaLua State
int	CLU_ShutdownState(int state)
{
	int i;
	int j;

	if(CLU_LoadState(state) == -1)
		return(-1);
	
	lua_close(virtualMachine);

	//Free up memory from all our tables and shit...
	
	for(i = 0; i < numStructs; i++)
	{
		for(j = 0; j < structs[i]->numMembers; j++)
		{
			SAFE_FREE(structs[i]->memberName[j]);
		}

		SAFE_FREE(structs[i]->memberName);
		SAFE_FREE(structs[i]->memberType);
		SAFE_FREE(structs[i]->memberSize);
		SAFE_FREE(structs[i]->NextMemberOffset);
		
		SAFE_FREE(structs[i]->name);
		SAFE_FREE(structs[i]->signature);
		SAFE_FREE(structs[i]);
	}

	for(i = 0; i < numFuncs; i++)
	{
		SAFE_FREE(funcs[i]->name);
		SAFE_FREE(funcs[i]->passArgs);
		SAFE_FREE(funcs[i]->retArgs);
		SAFE_FREE(funcs[i]->passType);
		SAFE_FREE(funcs[i]);
	}

	SAFE_FREE(funcs);
	SAFE_FREE(structs);
	CLU_AVL_Destroy(funcsLU);
	CLU_AVL_Destroy(structsLU);
	CLU_AVL_Destroy(baseTypeLU);

	ClearState(state);

return(0);
}


//\\ Shuts down Lua
int CLU_Shutdown()
{
	int k;

	CLU_ShutdownError();

	// Loop through all our saved states and free them all
	for(k = 0; k < CLU_STATE_STACK_SIZE; k++)
		if(stateStack[k]._used)
			CLU_ShutdownState(k);
		else
			continue;
	
return(0);
}


//\\ Loads up a Lua script, compiles it, then runs the main function of the script
//\\ a scripts main function is any code that's not within a function definition
int CLU_LoadScript(const char* filename, int isObfuscate)
{
	if(isObfuscate == 0){
	
	int status;
	FILE* fp;
	unsigned char* aux;
	unsigned long dwOldLoc;
	unsigned long dwSize;
	int i;
	//Load in binary mode, unscramble and compile our LUA code
	fp = fopen(filename, "rb");
	if(fp == NULL) return 0;

	// Getting the size of file
	dwOldLoc = ftell(fp);
    fseek(fp , 0 , SEEK_END);
   	dwSize = ftell(fp);
    fseek(fp , dwOldLoc , SEEK_SET);

	// Allocate and clean memory before copying new stuff into it
	aux = (unsigned char*) malloc(dwSize);
	memset(aux, 0, dwSize);
	// Reading bytes into aux
	fread(aux, dwSize, 1, fp);
	//Unscrambling...
	for(i = 0; i < dwSize; i++)
	{
		aux[i] -= 23;
	}
	
	status  = luaL_loadbuffer(virtualMachine, (char*)(aux), dwSize, "name");
	// Disabled direct file reading
	//status = luaL_loadfile(virtualMachine, filename);
	if(status)
	{
		CLU_Error("Error compiling %s : \n %s\n",filename,lua_tostring(virtualMachine,-1));
		return(1);
	}

	//Run the 'main' function
	status = lua_pcall(virtualMachine, 0, 0, 0);
	if(status)
	{
		CLU_Error("Error running script %s : \n %s\n", filename, lua_tostring(virtualMachine, -1));
		return(1);
	}

		return(0);
	}

	else{
	// Just obfuscate ("compile") the files in here
	// Open original file in binary read mode
	FILE* fp = fopen(filename, "rb");
	FILE* fp_bin;
	unsigned long dwOldLoc;
	unsigned long dwSize;
	unsigned char* aux;
	char new_filename[250];
	char* p;
	int i;
	if(fp==NULL) return 0;
	// Get file size in bytes (taken from Util_GetFileSize @ util2.cpp)
    dwOldLoc = ftell(fp);
  	fseek(fp , 0 , SEEK_END);
  	dwSize = ftell(fp);
  	fseek(fp, dwOldLoc , SEEK_SET);

	aux = (unsigned char*) malloc(dwSize);
	memset(aux, 0, dwSize);

	// Read data into auxiliary pointer
	fread(aux, dwSize, 1, fp);
	
	// Scramble whole auxiliary pointer
	for(i = 0; i < dwSize; i++)
	{
		aux[i] += 23;
	}
	
	// Open and write new file
	memset(new_filename, 0, sizeof(new_filename));
	strncpy(new_filename, filename, strlen(filename) - 4);
	strcat(new_filename, ".bin");
	fp_bin = fopen(new_filename, "wb");
	if(fp_bin==NULL) return 0;
	fwrite(aux, dwSize, 1, fp_bin);
	
	}
	
}



//\\ Since this dll creates memory for the structures that it returns, we need to export
//\\ a function that will clean up memory that we allocate and return to the user
//\\ We won't send back and errors, since people often like to free pointers like
//\\ ptr = FreeFunction(ptr);
void CLU_DllFree(void* memptr)
{
	SAFE_FREE(memptr);
}

//\\ Simply returns the current CaLua states virtual machine
lua_State*  CLU_GetVirtualMachine()
{
	return(virtualMachine);
}

//\\ Just returns the current CaLua state
int	CLU_GetCurrentState()
{
	return(currentState);
}

//\\ Calls the script function 'funcName' with elipses arguments that look like 'args'
//\\ and returns a CLU_Return structure that looks like 'retArgs'
void* CLU_CallScriptFunction(const char* funcName, const char* retArgs, const char* args, ...)
{
	va_list par;
	int numArgs;
	int status;
	int nToken;
	int nReturns;
	int dataType;
	void* ret;

	char* tok;							//Current token we're processing

	//Parse args string and push arguments onto the stack accordingly, if there are any
	va_start(par,args);

	//Push the function to call on the stack
	lua_pushstring(virtualMachine, funcName);
	lua_gettable(virtualMachine, LUA_GLOBALSINDEX);
	if(!lua_isfunction(virtualMachine,-1))
	{
		CLU_Error("Function %s does not exist\n",funcName);
		va_end(par);
		return(NULL);
	}
		
	numArgs = 0;

	if(args)
	{		
		nToken = 1;
		while((tok=GetToken(args,nToken)))
		{			
			dataType = GetDataType(tok);
			nToken++;

			switch(CLU_C_BASE_TYPE(dataType))
			{
			case CLU_CHAR :								// C89 says that chars are promoted to integers
				{					
					lua_pushnumber(virtualMachine,(double)va_arg(par, int));
				}break;
			case CLU_SHORT :					// C89 says that shorts are promoted to integers
				{
					lua_pushnumber(virtualMachine,(double)va_arg(par, int));
				}break;
			case CLU_INT :
				{	
					lua_pushnumber(virtualMachine,(double)va_arg(par, int));
				}break;
			case CLU_FLOAT :					// C89 states that floats are promoted as doubles
				{
					lua_pushnumber(virtualMachine,(double)va_arg(par, double));
				}break;
			case CLU_DOUBLE :
				{
					lua_pushnumber(virtualMachine,(double)va_arg(par, double));
				}break;
			case CLU_POINTER :
			{
				lua_pushlightuserdata(virtualMachine,va_arg(par, void*));
			}break;
			case CLU_STRING :
			{
				lua_pushstring(virtualMachine,va_arg(par, char*));
			}break;
			case CLU_UNKNOWN :
			{									// We could check if it's a pointer and pass it as light user data
												// but, this is safer...
				CLU_Error("Unknown type : %s",tok);
				va_end(par);
				return(NULL);
			}break;
			default :				//Must be a registered structure
				{
					if(CLU_IS_POINTER(dataType))
					{
						lua_pushlightuserdata(virtualMachine, va_arg(par, void*));
					}
					else
					{
						PushStructure( CLU_TYPE_TO_INDEX(dataType), (void*)par);
						par += structs[CLU_TYPE_TO_INDEX(dataType)]->size;
					}
				}break;
			}
			numArgs++;

			SAFE_FREE(tok);
		}
	}

	va_end(par);

	nReturns = GetNumberOfTokens(retArgs);
	status = lua_pcall(virtualMachine, numArgs, nReturns, 0);
	if(status)
	{
		CLU_Error("Error calling function %s : \n %s\n", funcName, lua_tostring(virtualMachine, -1));
		return(NULL);
	}

	ret = PopValuesBySig(retArgs);
		
return(ret);
}


//\\ Register structure with desired 'name', with 'desc'
//\\ 'desc' can be a string that looks like.. well.. you know..
int CLU_RegisterStructure(char* name, char* desc)
{
	char* tok;
	int dataType;
	char* strType;
	char* expName;
	int nToken;
	CLU_LUNode * node;
	CLU_ExpandBuffer((void**)&structs, sizeof(StructForm*),numStructs,1);
	
	structs[numStructs] = CLU_Calloc(sizeof(StructForm));
	
	structs[numStructs]->name = CLU_String_Copy(name);
	structs[numStructs]->regNum = numStructs + CLU_NUM_C_TYPES + 1;
	structs[numStructs]->signature = CLU_String_Copy(desc);

	//Parse supplied args into the arrays....
	if(desc)
	{
		tok = GetToken(desc, 1);
		nToken = 1;
		while(tok)
		{
			strType=ParseDataType(tok);

			if(!strType)
			{
				CLU_Error("Unable to parse data type: %s \n %s",tok,desc);
				return(1);
			}

			dataType = GetDataType(strType);

			if(dataType == CLU_UNKNOWN)
			{
				CLU_Error("Unknown data type : %s \n %s",tok,desc);
				return(1);
			}

			expName = ParseDataName(tok);

			if(expName == CLU_UNKNOWN)
			{
				CLU_Error("Unknown to parse data name: %s \n %s",tok,desc);
				return(1);
			}
			
			CLU_ExpandBuffer( (void**)&structs[numStructs]->memberSize, sizeof(int), structs[numStructs]->numMembers, 1);
			CLU_ExpandBuffer( (void**)&structs[numStructs]->memberName, sizeof(char*), structs[numStructs]->numMembers, 1);
			CLU_ExpandBuffer( (void**)&structs[numStructs]->memberType, sizeof(int), structs[numStructs]->numMembers, 1);

			structs[numStructs]->memberName[structs[numStructs]->numMembers] = CLU_String_Copy(expName);
			structs[numStructs]->memberSize[structs[numStructs]->numMembers] = GetDataSize(dataType);	
			structs[numStructs]->memberType[structs[numStructs]->numMembers] = dataType; 
			
			structs[numStructs]->numMembers++;

			SAFE_FREE(tok);
			SAFE_FREE(strType);
			SAFE_FREE(expName);
			
			nToken++;
			
			tok = GetToken(desc, nToken);
		}
	}
	else
	{	//I doubt this would ever happen, but...	
		CLU_Error("No description passed to CLU_RegisterStructure");
		return(1);
	}

	ComputeStructureAlignment(structs[numStructs]);
	structs[numStructs]->size = ComputeStructureSize(structs[numStructs]);
	
	node = CLU_LUNode_Create();
	node->index = numStructs;
	CLU_AVL_Insert(structsLU, node, CLU_Hash_Key_Create(name));
	//Instead of littering the code with a bunch of -1's, just increment it down here.... dangerous I suppose
	numStructs++;

return(0);
}


//\\ Register a function 'f' with 'name', returns 'ret', is passed data that looks like 'args', and
//\\ is called using 'conventions' calling convention
int CLU_RegisterFunction(const char* name, const char* ret, const char* args, int convention, void (__cdecl *f)(void))
{
	char* tok;
	int i;
	CLU_LUNode * node;

	CLU_ExpandBuffer( (void**)&funcs, sizeof(FuncForm*), numFuncs, 1);

	funcs[numFuncs] = CLU_Calloc(sizeof(FuncForm));
	
	funcs[numFuncs]->name = CLU_String_Copy(name);
	funcs[numFuncs]->fPtr = f;
	funcs[numFuncs]->callType = convention;
	funcs[numFuncs]->passArgs = CLU_String_Copy(args);
	funcs[numFuncs]->retArgs = CLU_String_Copy(ret);
	
	if((funcs[numFuncs]->numRetArgs = GetNumberOfTokens(ret)) > 1)
	{
		CLU_Error("More than one return argument specified with %s",name);
		return(1);
	}

	funcs[numFuncs]->numPassArgs = GetNumberOfTokens(args);
	
	if(ret)
	{
		if((funcs[numFuncs]->retType = GetDataType(ret)) == CLU_UNKNOWN)
		{
			CLU_Error("Unknown return type for %s", name);
			return(1);
		}
	}

	CLU_ExpandBuffer(&funcs[numFuncs]->passType, sizeof(int*), 0, funcs[numFuncs]->numPassArgs);

	for(i=1; i <= funcs[numFuncs]->numPassArgs; i++)
	{
		if((funcs[numFuncs]->passType[i-1] = GetDataType((tok=GetToken(args,i)))) == CLU_UNKNOWN)
		{
			CLU_Error("Unknown argument type %s in %s", tok, name);
			SAFE_FREE(tok);
			return(1);
		}
		SAFE_FREE(tok);
	}

	node = CLU_LUNode_Create();
	node->index = numFuncs;
	CLU_AVL_Insert(funcsLU, node, CLU_Hash_Key_Create(name));

	numFuncs++;

return(0);
}

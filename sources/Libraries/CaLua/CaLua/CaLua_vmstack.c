/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003/2004

  Lua virtual machine stack stuff

  Didn't know where else to put this, so I gave it it's own .c file...
  Basically all this does is allows CaLua to work with multiple CaLua stacks
  I may expose these functions later on to API programmers if people request it
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/

#include "CaLua_guts.h"


// Pushes the current virtual machine, and sets 'vm' as the new, main
// virtual machine that CaLua will work with
void PushVM(lua_State* vm)
{
	if(vmStackPos < CLU_VM_STACK_SIZE)
		vmStack[vmStackPos] = virtualMachine;

	virtualMachine = vm;
	vmStackPos++;
}


// Pops a virtual machine from the stack, and sets is as the main virtual machine
// This returns the old virtual machine
lua_State* PopVM(void)
{
	lua_State* ret;

	ret = virtualMachine;

	if(vmStackPos > 0)
	{
		vmStackPos--;
		virtualMachine = vmStack[vmStackPos];
		vmStack[vmStackPos] = NULL;
	}
return(ret);
}


static int FindNextAvailPos()
{
	int i;

	for(i=0; i< CLU_STATE_STACK_SIZE; i++)
		if(stateStack[i]._used == 0)
			return(i);

return(-1);
}
	

int SaveNewState()
{
	if((currentState = FindNextAvailPos()) == -1)
	{
		CLU_Error("No state stack positions available. Max is %d", CLU_STATE_STACK_SIZE);
		return(-1);
	}

	//Save the current CaLua state
	stateStack[currentState]._virtualMachine = virtualMachine;
	stateStack[currentState]._baseTypeLU = baseTypeLU;
	stateStack[currentState]._structs = structs;
	stateStack[currentState]._structsLU = structsLU;
	stateStack[currentState]._numStructs = numStructs;
	stateStack[currentState]._funcs = funcs;
	stateStack[currentState]._funcsLU = funcsLU;
	stateStack[currentState]._numFuncs = numFuncs;
	stateStack[currentState]._used = 1;
	
return(currentState);
}


void UpdateState(int state)
{
	//Save the current CaLua state
	stateStack[state]._virtualMachine = virtualMachine;
	stateStack[state]._baseTypeLU = baseTypeLU;
	stateStack[state]._structs = structs;
	stateStack[state]._structsLU = structsLU;
	stateStack[state]._numStructs = numStructs;
	stateStack[state]._funcs = funcs;
	stateStack[state]._funcsLU = funcsLU;
	stateStack[state]._numFuncs = numFuncs;
}


int CLU_LoadState(int state)
{
	if((state < 0)||(state > CLU_STATE_STACK_SIZE)||(!stateStack[state]._used))
	{
		CLU_Error("Trying to load unknown state");
		return(-1);
	}

	//Save the state one last time before we switch...
	UpdateState(currentState);

	virtualMachine = stateStack[state]._virtualMachine;
	baseTypeLU = stateStack[state]._baseTypeLU;
	structs = stateStack[state]._structs;
	numStructs = stateStack[state]._numStructs;
	structsLU = stateStack[state]._structsLU;
	funcs = stateStack[state]._funcs;
	numFuncs = stateStack[state]._numFuncs;
	funcsLU = stateStack[state]._funcsLU;

	currentState = state;

return(state);
}


int ClearState(int state)
{
	memset(&stateStack[state], 0, sizeof(VMSlate));
return(0);
}
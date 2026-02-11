/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003/2004

  Parsing routines

  Anything that involves taking a character string and turning it into something else
  can be found in here...
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/

#include "CaLua_guts.h"


//\\ Checks to get the index into global variable 'funcs'.
//\\ If it can't find it, it just returns -1
int GetFunctionIndex(char* func)
{
	CLU_LUNode * node = NULL;
	node = CLU_AVL_Access(funcsLU, func);
	if (node)
		return node->index;
	/*
	int curFunc;
	
	curFunc = 0;

	while(curFunc < numFuncs )
	{
		if( strnicmp(func, funcs[curFunc]->name, strlen(funcs[curFunc]->name)) == 0 )
			return(curFunc);

		curFunc++;
	}
	*/

return(-1);
}


//\\ Looks through the list of registered tables, and returns the index
//\\ of table 'tab'. -1 if nothing is found.
int GetTableIndex(char* tab)
{
	CLU_LUNode * node = NULL;
	node = CLU_AVL_Access(structsLU, tab);
	if (node)
		return node->index;
	/*
	int curStruct;
	
	curStruct = 0;

	while(curStruct < numStructs )
	{
		if( strnicmp(tab, structs[curStruct]->name, strlen(structs[curStruct]->name)) == 0 )
			return(curStruct);

		curStruct++;
	}
	*/
return(-1);
}


//\\ Gets the 'n'th token from 'str', which are strings seperated
//\\ by spaces and comma's
//\\ Maybe there's a cleaner way to do this, I have no idea, I'm so god damn tired...
char* GetToken(char* str, int n)
{
	char* ret;
	char* tRet;
	int i;
	char del[] = "\t\n ,";
	char* tStr;

	if(n < 1)
		return(NULL);

	tStr = CLU_String_Copy(str);

	tRet = strtok(tStr, del);
	i = 1;
	while( i++ < n)
		tRet = strtok(NULL, del);

	ret = CLU_String_Copy(tRet);
	SAFE_FREE(tStr);

return(ret);
}


//\\ Basically this just counts all the commas in the
//\\ 'in' string
int GetNumberOfTokens(char* in)
{
	int i = 1;

	if((in==NULL)||(*in==0))
		return(0);

	while(*in)
		if((*in++) == ',')
			i++;

	return(i);
}


int IsPointer(char* in)
{
	if(in[strlen(in)-1] == '*')
		return(1);
	
return(0);
}


// Turns a textual C data type into a number
int GetDataType(char* in)
{
	int ret;
	int tableIndex;
	int len;
	CLU_LUNode* n;
	
	ret = 0;

	if(!in)
		return(CLU_UNKNOWN);
	
	len = strlen(in);

	if(IsPointer(in))
	{
		len -= 1;
		in[len] = '\0';
		ret |= CLU_POINTER_BIT;
	}

	n = (CLU_LUNode*)CLU_AVL_Access(baseTypeLU, in);
	
	if(!n)
		if( (tableIndex = GetTableIndex(in)) >= 0 )
		{
			ret |= structs[tableIndex]->regNum;
		}
		else
			return(CLU_UNKNOWN);
	else
		switch(CLU_C_BASE_TYPE(n->index))
		{
		case CLU_CHAR :
			{
				if(CLU_IS_POINTER(ret))
					ret |= CLU_STRING;
				else
					ret |= n->index;
			}break;
		default :
			{
				if(CLU_IS_POINTER(n->index))
					ret |= CLU_POINTER;
				else
					ret |= n->index;
			}break;
		}
	
return(ret);
}


// Data Type appears before the colon
char* ParseDataType(char* in)
{
	char* ret;
	int len;
	
	len = (strchr(in,':') - in) + 1;

	if(len == (((char*)(0)-in)+1))
		return(NULL);

	ret = CLU_Calloc(len);
	memcpy(ret,in,len-1);

	ret[len-1] = '\0';

return(ret);
}


//Data Name appears AFTER the colon
char* ParseDataName(char* in)
{
	char* ret;

	if(!in || !strchr(in,':'))
		return(NULL);
	else
		ret = CLU_String_Copy( strchr(in,':') + 1);

return(ret);
}


int GetDataSize(int t)
{
	if(CLU_IS_POINTER(t))
		return(sizeof(void*));
	else
	{
		switch(CLU_C_BASE_TYPE(t))
		{
		case CLU_CHAR : return(sizeof(char));break;
		case CLU_SHORT : return(sizeof(short));break;
		case CLU_INT : return(sizeof(int));break;
		case CLU_FLOAT : return(sizeof(float));break;
		case CLU_DOUBLE : return(sizeof(double));break;
		case CLU_STRING : return(sizeof(char*));break;
		case CLU_POINTER : return(sizeof(void*));break;
		case CLU_UNKNOWN : return(0);
		default : return( structs[CLU_TYPE_TO_INDEX(t)]->size);break;
		}
	}
return(0);
}
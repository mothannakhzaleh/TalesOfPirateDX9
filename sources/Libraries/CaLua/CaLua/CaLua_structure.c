/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003/2004

  Structure Alignment/Registration shit...

  Structure alignments and packing and all that shit is a major pain in the ass, so,
  all of that pain will reside in this file...
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/

#include "CaLua_guts.h"


int NextAlignment(int offset, int type)
{
	int typeSize;

	if( CLU_IS_STRUCTURE(type) )		// If it's a structure, recurse to get the size of the first element of the structure
		typeSize = NextAlignment(offset, structs[CLU_TYPE_TO_INDEX(type)]->memberType[0]);
	else
		typeSize = GetDataSize(type);

	while( (offset%typeSize) )
		offset++;

return(offset);
}


// data is a D.A of pointers to the data, and type is a D.A of
// data types for the data D.A
void* Pack(int nElements, void** data, int* type)
{
	char* ret = NULL;
	int buffSize;
	int i;
	int offset;
	int dataSize;
			
	for(i=0, offset=0, buffSize=0; i < nElements; i++)
	{
		offset += (dataSize = GetDataSize(type[i]));
		
		if( i < nElements-1)
			offset = NextAlignment(offset, type[i+1]);
		
		CLU_ExpandBuffer( &ret, sizeof(char), buffSize, (offset - buffSize));
		memcpy((void*)(ret+buffSize), data[i], dataSize);
		
		buffSize += (offset - buffSize);
	}

return(ret);
}

//\\ Returns the next alignment position from 'offset', that is divisible by 'align' 
static int GetNextAlignmentPosition(int offset, int align)
{
	int ret;

	//Get the lowest or align, or the compilers structure member alignment
	if(align > __STRUCTURE_MEMBER_ALIGNMENT) align = __STRUCTURE_MEMBER_ALIGNMENT;

	ret = offset;

	while( (ret % align) )
		ret++;

return(ret);
}


//\\ Gets the size of the data member that will next appear in memory...
//\\ For most cases it's trivial, but for structures, it's a bit of a bitch!
static int GetMemberAlignmentSize(StructForm* s, int mem)
{
	int ret;

	if(mem == (s->numMembers))
		return(0);

	if( CLU_IS_STRUCTURE(s->memberType[mem]))
	{
		//If it's a structure, recurse down into that structure and get the size of it's first data element 
		ret = GetMemberAlignmentSize( structs[CLU_TYPE_TO_INDEX(s->memberType[mem])], 0);		
	}
	else
		ret = s->memberSize[mem];

return(ret);
}


//\\ As a post process, we have to set the offsets according to structure alignment boundaries
//\\ taking into account compiler and natural alignment settings.
int ComputeStructureAlignment(StructForm* s)
{
	int curMem;			// Current member
	int nextAvail;		// Next available address to possibly store data
	int lastPos;		// Position in absolute offset space, of last data member...
	int offset;			// Current position, in absolute offset space

	curMem = 0;
	nextAvail = 0;
	lastPos = 0;
	offset = 0;

	//Go through every member except the last
	while(curMem < (s->numMembers-1))
	{
		CLU_ExpandBuffer( (void**)&s->NextMemberOffset, sizeof(int*), curMem, 1);
		
		nextAvail = lastPos + s->memberSize[curMem]; //Skip over the size of this data element...
		
		offset = GetNextAlignmentPosition(nextAvail, GetMemberAlignmentSize(s,curMem+1));
		
		s->NextMemberOffset[curMem] = (offset - lastPos);

		lastPos = offset;

		curMem++;
	}

	CLU_ExpandBuffer( (void**)&s->NextMemberOffset, sizeof(int*), curMem, 1);
	
	s->NextMemberOffset[curMem] = 0; //Last member has no offset.... eee... it's just assumed it won't be used in any stupid calculation

return(0);
}


//\\ Gets the whole value of a structure, taking into account alignment offsets
int ComputeStructureSize(StructForm* s)
{
	int i;
	int sz;

	sz = 0;
	i = 0;
	while(i < s->numMembers)
		sz += s->NextMemberOffset[i++];

	sz += s->memberSize[i-1]; 

return(sz);
}


// Free's a registered structure...
/*void FreeStructure(void* s, int rStruct)
{
	void* sTmp;
	int i;
	
	sTmp = s;
	i = 0;

	while(i < structs[rStruct]->numMembers)
	{
		if( CLU_IS_POINTER(structs[rStruct]->memberType[i]) )
		{
			if( ! (CLU_C_BASE_TYPE(structs[rStruct]->memberType[i]) & CLU_RAW_POINTER))
			{


		sTmp += structs[rStruct]->NextMemberOffset[i];
		i++;
	}
	
	SAFE_FREE(s);
}*/

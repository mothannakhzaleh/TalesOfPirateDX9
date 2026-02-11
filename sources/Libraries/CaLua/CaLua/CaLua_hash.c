/*
	Serpent Engine
	Dan Carter, 2003
*/

#include "CaLua_hash.h"

CLU_Hash_Key *CLU_Hash_Key_Create(char *a) {
	return CLU_HASH_STRING_COPY(a);
}

CLU_Hash_Key *CLU_Hash_Key_Destroy(CLU_Hash_Key *a) {
	if (!a) return NULL;
	CLU_AVL_FREE(a);
	return NULL;
}

char CLU_Hash_Key_Cmp(CLU_Hash_Key *a, CLU_Hash_Key *b) {
	return stricmp(a, b);
}

CLU_LUNode * CLU_LUNode_Create() {
	CLU_LUNode * n;
	n = CLU_AVL_MALLOC(sizeof(CLU_LUNode));
	n->index = -1;
	return n;
}

CLU_LUNode * CLU_LUNode_Destroy(CLU_LUNode *n) {
	if (!n) return NULL;
	CLU_AVL_FREE(n);
	return NULL;
}
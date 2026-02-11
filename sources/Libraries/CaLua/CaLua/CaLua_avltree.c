/*
	Serpent Engine
	Dan Carter, 2003
*/

#include "CaLua_avltree.h"

CLU_AVL_Node * CLU_AVL_Node_Create(CLU_AVL *avl) {
	CLU_AVL_Node * n;
	n = CLU_AVL_MALLOC(sizeof(CLU_AVL_Node));
	n->d = n->key = n->left = n->right = NULL;
	n->skew = 0;
	avl->num++;
	return n;
}

CLU_AVL_Node * CLU_AVL_Node_Destroy(CLU_AVL *avl, CLU_AVL_Node *n) {
	if (!avl || !n) return NULL;
	if (avl->destroyData)
		avl->destroyData(n->d);
	if (avl->destroyKey)
		avl->destroyKey(n->key);
	CLU_AVL_FREE(n);
	avl->num--;
	return NULL;
}

/*
 *  avlrotleft: perform counterclockwise rotation
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node
 */
void avlrotleft(CLU_AVL_Node **n) {
	struct CLU_AVL_Node *tmp = *n;

	*n = (*n)->right;
	tmp->right = (*n)->left;
	(*n)->left = tmp;
}

/*
 *  avlrotright: perform clockwise rotation
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node
 */
void avlrotright(CLU_AVL_Node **n) {
	struct CLU_AVL_Node *tmp = *n;

	*n = (*n)->left;
	tmp->left = (*n)->right;
	(*n)->right = tmp;
}

/*
 *  avlleftgrown: helper function for avlinsert
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node. This node's left 
 *                subtree has just grown due to item insertion; its 
 *                "skew" flag needs adjustment, and the local tree 
 *                (the subtree of which this node is the root node) may 
 *                have become unbalanced.
 *
 *  Return values:
 *
 *    1          The local tree could be rebalanced or was balanced 
 *                from the start. The parent activations of the avlinsert 
 *                activation that called this function may assume the 
 *                entire tree is valid.
 *
 *    2     The local tree was balanced, but has grown in height.
 *                Do not assume the entire tree is valid.
 */
int avlleftgrown(CLU_AVL_Node **n) {
	switch ((*n)->skew) {
	case 1:
		if ((*n)->left->skew == 1) {
			(*n)->skew = (*n)->left->skew = 0;
			avlrotright(n);
		}	
		else {
			switch ((*n)->left->right->skew) {
			case 1:
				(*n)->skew = 2;
				(*n)->left->skew = 0;
				break;

			case 2:
				(*n)->skew = 0;
				(*n)->left->skew = 1;
				break;

			default:
				(*n)->skew = 0;
				(*n)->left->skew = 0;
			}
			(*n)->left->right->skew = 0;
			avlrotleft(& (*n)->left);
			avlrotright(n);
		}
		return 1;

	case 2:
		(*n)->skew = 0;
		return 1;
	
	default:
		(*n)->skew = 1;
		return 2;
	}
}

/*
 *  avlrightgrown: helper function for avlinsert
 *
 *  See avlleftgrown for details.
 */
int avlrightgrown(CLU_AVL_Node **n) {
	switch ((*n)->skew) {
	case 1:					
		(*n)->skew = 0;
		return 1;

	case 2:
		if ((*n)->right->skew == 2) {	
			(*n)->skew = (*n)->right->skew = 0;
			avlrotleft(n);
		}
		else {
			switch ((*n)->right->left->skew) {
			case 2:
				(*n)->skew = 1;
				(*n)->right->skew = 0;
				break;

			case 1:
				(*n)->skew = 0;
				(*n)->right->skew = 2;
				break;

			default:
				(*n)->skew = 0;
				(*n)->right->skew = 0;
			}
			(*n)->right->left->skew = 0;
			avlrotright(& (*n)->right);
			avlrotleft(n);
		}
		return 1;

	default:
		(*n)->skew = 2;
		return 2;
	}
}

/*
 *  avlleftshrunk: helper function for avlremove and avlfindlowest
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node. The node's left
 *                subtree has just shrunk due to item removal; its
 *                "skew" flag needs adjustment, and the local tree
 *                (the subtree of which this node is the root node) may
 *                have become unbalanced.
 *
 *   Return values:
 *
 *    1          The parent activation of the avlremove activation
 *                that called this function may assume the entire
 *                tree is valid.
 *
 *    2     Do not assume the entire tree is valid.
 */                
int avlleftshrunk(CLU_AVL_Node **n) {
	switch ((*n)->skew) {
	case 1:
		(*n)->skew = 0;
		return 2;

	case 2:
		if ((*n)->right->skew == 2) {
			(*n)->skew = (*n)->right->skew = 0;
			avlrotleft(n);
			return 2;
		}
		else if ((*n)->right->skew == 0) {
			(*n)->skew = 2;
			(*n)->right->skew = 1;
			avlrotleft(n);
			return 1;
		}
		else {
			switch ((*n)->right->left->skew) {
			case 1:
				(*n)->skew = 0;
				(*n)->right->skew = 2;
				break;

			case 2:
				(*n)->skew = 1;
				(*n)->right->skew = 0;
				break;

			default:
				(*n)->skew = 0;
				(*n)->right->skew = 0;
			}
			(*n)->right->left->skew = 0;
			avlrotright(& (*n)->right);
			avlrotleft(n);
			return 2;
		}

	default:
		(*n)->skew = 2;
		return 1;
	}
}

/*
 *  avlrightshrunk: helper function for avlremove and avlfindhighest
 *
 *  See avlleftshrunk for details.
 */
int avlrightshrunk(CLU_AVL_Node **n) {
	switch ((*n)->skew) {
	case 2:
		(*n)->skew = 0;
		return 2;

	case 1:
		if ((*n)->left->skew == 1) {
			(*n)->skew = (*n)->left->skew = 0;
			avlrotright(n);
			return 2;
		}
		else if ((*n)->left->skew == 0) {
			(*n)->skew = 1;
			(*n)->left->skew = 2;
			avlrotright(n);
			return 1;
		}
		else {
			switch ((*n)->left->right->skew) {
			case 1:
				(*n)->skew = 2;
				(*n)->left->skew = 0;
				break;

			case 2:
				(*n)->skew = 0;
				(*n)->left->skew = 1;	
				break;
			
			default:
				(*n)->skew = 0;
				(*n)->left->skew = 0;
			}
			(*n)->left->right->skew = 0;
			avlrotleft(& (*n)->left);
			avlrotright(n);
			return 2;
		}

	default:
		(*n)->skew = 1;
		return 1;
	}
}

/*
 *  avlfindhighest: replace a node with a subtree's highest-ranking item.
 *
 *  Parameters:
 *
 *    target      Pointer to node to be replaced.
 *
 *    n           Address of pointer to subtree.
 *
 *    res         Pointer to variable used to tell the caller whether
 *                further checks are necessary; analog to the return
 *                values of avlleftgrown and avlleftshrunk (see there). 
 *
 *  Return values:
 *
 *    1           A node was found; the target node has been replaced.
 *
 *    0           The target node could not be replaced because
 *                the subtree provided was empty.
 *
 */
int avlfindhighest(CLU_AVL * avl, CLU_AVL_Node *target, CLU_AVL_Node **n, char *res) {
	CLU_AVL_Node *tmp;

	*res = 2;
	if (!(*n)) {
		return 0;
	}
	if ((*n)->right) {
		if (!avlfindhighest(avl, target, &(*n)->right, res)) {
			return 0;
		}
		if (*res == 2) {
			*res = avlrightshrunk(n);
		}
		return 1;
	}
	
	if (avl->destroyData)
		avl->destroyData(target->d);
	if (avl->destroyKey)
		avl->destroyKey(target->key);
	target->d = (*n)->d;
	target->key = (*n)->key;

	tmp = *n;
	*n = (*n)->left;
	CLU_AVL_FREE(tmp);
	avl->num--;
	return 1;
}

/*
 *  avlfindlowest: replace node with a subtree's lowest-ranking item.
 *
 *  See avlfindhighest for the details.
 */
int avlfindlowest(CLU_AVL * avl, CLU_AVL_Node *target, CLU_AVL_Node **n, char *res) {
	struct CLU_AVL_Node *tmp;

	*res = 2;
	if (!(*n)) {
		return 0;
	}
	if ((*n)->left) {
		if (!avlfindlowest(avl, target, &(*n)->left, res)) {
			return 0;
		}
		if (*res == 2) {
			*res =  avlleftshrunk(n);
		}
		return 1;
	}
	if (avl->destroyData)
		avl->destroyData(target->d);
	if (avl->destroyKey)
		avl->destroyKey(target->key);
	target->d = (*n)->d;
	target->key = (*n)->key;

	tmp = *n;
	*n = (*n)->right;
	CLU_AVL_FREE(tmp);
	avl->num--;
	return 1;
}

CLU_AVL * CLU_AVL_Create(void * destroyData, void * destroyKey, void * cmpKeys) {
	CLU_AVL *avl;
	avl = CLU_AVL_MALLOC(sizeof(CLU_AVL));
	avl->root = NULL;
	avl->cmpKeys = cmpKeys;
	avl->destroyData = destroyData;
	avl->destroyKey = destroyKey;
	avl->num = 0;
	return avl;
}

int CLU_AVL_Destroy_Rec(CLU_AVL * avl, CLU_AVL_Node *n, CLU_AVL_Node **list, int num) {
	if (!n) return num;
	if (n->left)
		num = CLU_AVL_Destroy_Rec(avl, n->left, list, num);
	list[num++] = n;
	if (n->right)
		num = CLU_AVL_Destroy_Rec(avl, n->right, list, num);
	return num;
}

CLU_AVL * CLU_AVL_Destroy(CLU_AVL * avl) {
	int i, cnt;
	CLU_AVL_Node ** list;
	if (!avl) return NULL;
	
	cnt = avl->num;
	list = CLU_AVL_MALLOC(sizeof(CLU_AVL_Node*)*cnt);
	CLU_AVL_Destroy_Rec(avl, avl->root, list, 0);
	for (i = 0; i < cnt; i++) {
		CLU_AVL_Node_Destroy(avl, list[i]);
	}
	CLU_AVL_FREE(list);

	CLU_AVL_FREE(avl);
	return NULL;
}

int CLU_AVL_Insert_Rec(CLU_AVL * avl, CLU_AVL_Node ** np, void *d, void * key) {
	char tmp, cmp;
	CLU_AVL_Node * n;
	n = *np;

	if (!n) {
		n = CLU_AVL_Node_Create(avl);
		n->d = d;
		n->key = key;
		*np = n;
		return 2;
	}
	
	cmp = avl->cmpKeys(n->key,key);

	if (cmp > 0) {
		if ((tmp = CLU_AVL_Insert_Rec(avl, &n->left, d, key)) == 2) {
			return avlleftgrown(np);
		}
		return tmp;
	}
	if (cmp < 0) {
		if ((tmp = CLU_AVL_Insert_Rec(avl, &n->right, d, key)) == 2) {
			return avlrightgrown(np);
		}
		return tmp;
	}
	return 0;
}

int CLU_AVL_Insert(CLU_AVL * avl, void *d, void *key) {
	if (!avl) return 0;
	return CLU_AVL_Insert_Rec(avl, &avl->root, d, key);
}


int CLU_AVL_Remove_Rec(CLU_AVL * avl, CLU_AVL_Node **np, void *key) {
	char tmp = 2, cmp;
	CLU_AVL_Node * n;
	n = *np;

	if (!n)
		return 0;

	cmp = avl->cmpKeys(n->key,key);
	if (cmp > 0) {
		if ((tmp = CLU_AVL_Remove_Rec(avl, &n->left, key)) == 2) {
			return avlleftshrunk(np);
		}
		return tmp;
	}
	if (cmp < 0) {
		if ((tmp = CLU_AVL_Remove_Rec(avl, &n->right, key)) == 2) {
			return avlrightshrunk(np);
		}
		return tmp;
	}
	if (n->left) {
		if (avlfindhighest(avl, n, &n->left, &tmp)) {
			if (tmp == 2) {
				tmp = avlleftshrunk(np);
			}
			return tmp;
		}
	}
	if (n->right) {
		if (avlfindlowest(avl, n, &n->right, &tmp)) {
			if (tmp == 2) {
				tmp = avlrightshrunk(np);
			}
			return tmp;
		}
	}
	CLU_AVL_Node_Destroy(avl, n);
 	*np = NULL;
	return 2;
}

int CLU_AVL_Remove(CLU_AVL * avl, void *key) {
	if (!avl) return 0;
	return CLU_AVL_Remove_Rec(avl, &avl->root, key);
}


void * CLU_AVL_Access_Rec(CLU_AVL * avl, CLU_AVL_Node **np, void *key) {
	char cmp;
	CLU_AVL_Node * n;
	n = *np;
    if (!n) {
        return NULL;
    }
	cmp = avl->cmpKeys(n->key,key);
    if (cmp > 0) {
        return CLU_AVL_Access_Rec(avl, &n->left, key);
    }
    if (cmp < 0) {
        return CLU_AVL_Access_Rec(avl, &n->right, key);
    }
    return n->d;
}

void * CLU_AVL_Access(CLU_AVL * avl, void *key) {
	if (!avl) return NULL;
	return CLU_AVL_Access_Rec(avl, &avl->root, key);
}

int CLU_AVL_MinList_Rec(CLU_AVL * avl, CLU_AVL_Node *n, void **list, int num) {
	if (!n) return num;
	if (n->left)
		num = CLU_AVL_MinList_Rec(avl, n->left, list, num);
	list[num++] = n->d;
	if (n->right)
		num = CLU_AVL_MinList_Rec(avl, n->right, list, num);
	return num;
}

void ** CLU_AVL_MinList(CLU_AVL * avl) {
	void ** list;
	if (!avl) return NULL;
	list = CLU_AVL_MALLOC(sizeof(void*)*avl->num);
	CLU_AVL_MinList_Rec(avl, avl->root, list, 0);
	return list;
}

int CLU_AVL_MaxList_Rec(CLU_AVL * avl, CLU_AVL_Node *n, void **list, int num) {
	if (!n) return num;
	if (n->right)
		num = CLU_AVL_MaxList_Rec(avl, n->right, list, num);
	list[num++] = n->d;
	if (n->left)
		num = CLU_AVL_MaxList_Rec(avl, n->left, list, num);
	return num;
}

void ** CLU_AVL_MaxList(CLU_AVL * avl) {
	void ** list;
	if (!avl) return NULL;
	list = CLU_AVL_MALLOC(sizeof(void*)*avl->num);
	CLU_AVL_MaxList_Rec(avl, avl->root, list, 0);
	return list;
}

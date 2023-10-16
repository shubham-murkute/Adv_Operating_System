#ifndef _utility_h_
#define _utility_h_

struct node_s {
	struct node_s *next;
	struct node_s *prev;
	void* data;
};

typedef struct node_s node;
struct linked_list_s {
	node * head;
	node * tail;
	int size;
};

typedef struct linked_list_s linked_list;

node* Create_Node(void* data);
linked_list * Create_Linked_List();
void AddNode(linked_list* ll, void* data);
void RemoveData(linked_list* ll, void* data);
void RemoveNode(linked_list* ll, node* n);
void AddAfter(linked_list* ll, node *after_node, void* data);
void Sort(linked_list *ll, int (*cmp)(void *data1, void *data2));
void SwapNodes(node *a, node *b);




// Queue Implementatin //

typedef struct linked_list_s queue;

queue * CreateQueue();
void enqueue(queue * q, void * data);
void * dequeue(queue * q);

#endif

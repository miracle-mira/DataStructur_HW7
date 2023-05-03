/*
 *  doubly-linked-list.c
 *
 *  Doubly Linked List
 *
 *  Data Structures
 *  School of Computer Science 
 *  at Chungbuk National University
 *
 */


#include<stdio.h>
#include<stdlib.h>


/*노드 구조체*/
typedef struct Node {
	int key;
	struct Node* llink;
	struct Node* rlink;
} listNode;


/*헤드 노드 구조체: 첫 노드의 주소만 담기만 하면됨*/
typedef struct Head {
	struct Node* first;
}headNode;


int initialize(headNode** h);            //doubly-link 초기화 함수

int freeList(headNode* h);               //동적 메모리 할당 해제

int insertNode(headNode* h, int key);    //노드 삽입
int insertLast(headNode* h, int key);    //마지막에 노드 삽입
int insertFirst(headNode* h, int key);   //처음에 노드 삽입

int deleteNode(headNode* h, int key);    //노드 삭제
int deleteLast(headNode* h);             //마지막 노드 삭제
int deleteFirst(headNode* h);            //첫 노드 삭제

int invertList(headNode* h);             //doubly-link 전치

void printList(headNode* h);             //doubly-link 출력


int main()
{

	printf("[----- [Park Mira] [2020069006] -----]");

	char command;
	int key;
	headNode* headnode = NULL;

	do{
		printf("----------------------------------------------------------------\n");
		printf("                     Doubly Linked  List                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize    = z           Print         = p \n");
		printf(" Insert Node   = i           Delete Node   = d \n");
		printf(" Insert Last   = n           Delete Last   = e\n");
		printf(" Insert First  = f           Delete First  = t\n");
		printf(" Invert List   = r           Quit          = q\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&headnode);
			break;
		case 'p': case 'P':
			printList(headnode);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insertNode(headnode, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(headnode, key);
			break;
		case 'n': case 'N':
			printf("Your Key = ");
			scanf("%d", &key);
			insertLast(headnode, key);
			break;
		case 'e': case 'E':
			deleteLast(headnode);
			break;
		case 'f': case 'F':
			printf("Your Key = ");
			scanf("%d", &key);
			insertFirst(headnode, key);
			break;
		case 't': case 'T':
			deleteFirst(headnode);
			break;
		case 'r': case 'R':
			invertList(headnode);
			break;
		case 'q': case 'Q':
			freeList(headnode);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

/*
[초기화 함수]
1. 'call by reference' 형태로 주소를 인자로 넘기기 때문에 포인터 형태로 파라미터가 설정되어야한다.
그런데 이때 인자로 넘겨주는 인자가 주소를 담는 포인터이기 때문에 파라미터는 특별히 이중 포인터를 설정한다.
그래야 변수 값을 조작할 수 있기 때문이다.
*/
int initialize(headNode** h) {	

	//doubly-link가 채워있을 경우 동적할당해제를 진행함. 파라미터에 맞게 인자를 설정함.
	if(*h != NULL){
		free(*h);
	}

	//헤드노드 구조체 크기의 동적 할당을 받고, 그 구조체가 가르키는 첫 번째 노드를 NULL 값으로 설정한다. 
	*h = (headNode *)malloc(sizeof(headNode));
	(*h)->first = NULL;

	return 1;
}

/*
[동적 메모리 할당 해제 함수]
1. 각 노드가 동적 메모리 할당을 받고 있기 때문에 우선 각 노드의 동적 메모리 할당을 해제해준다.
2. 마지막으로 헤드노드의 동적 메모리 할당을 해제해준다. 
*/
int freeList(headNode* h){

	/*
	[노드를 두 개 사용하는 이유]
	하나만 사용할 경우 해제를 하면 다음 노드로 진행할 수 없기 때문에 현재 노드 값을 복사하고, 다음 노드를 가리킨 다음, 현재 노드를 메모리 할당 해제를 해야한다.
	*/
	listNode * p = h->first; //현재 노드 -> 다음 노드
	listNode * prev = NULL;  //현재 노드의 위치를 저장

	while(p != NULL){

		prev = p;      //현재 노드 위치 저장
		p = p->rlink;  //다음 노드 가리킴
 
		free(prev);    //현재 노드 동적 메모리 해제
	}

	free(h); //헤드 노드 동적 메모리 해제

	return 0;
}

/*
[출력함수]
*/
void printList(headNode* h) {
	int i = 0;
	listNode* p;

	printf("\n---PRINT\n");

	if(h == NULL) {
		printf("Nothing to print....\n");
		return;
	}

	p = h->first;

	while(p != NULL) {
		printf("[ [%d]=%d ] ", i, p->key);
		p = p->rlink;
		i++;
	}

	printf("  items = %d\n", i);
}


/*
[마지막에 노드 삽입 함수]
*/
int insertLast(headNode* h, int key) {

	listNode * newnode = (listNode *)malloc(sizeof(listNode)); //새로운 노드 생성
	newnode->key = key;                                        //새로운 노드에 주어진 키 값 넣어줌

	/*노드가 비어있는데 마지막 노드를 삽입할려고 했을 때*/
	if(h->first == NULL){
		h->first = newnode;     //헤드노드가 새로운 노드 주소를 가리킴.
		//새로운 노드의 왼쪽,오른쪽 링크 값을 NULL로 함: 유일한 노드이므로 어느 하나 가리킬 것이 없음
		newnode->llink = NULL;  
		newnode->rlink = NULL;

		return 0;
	}

	listNode * p = h->first; //현재 노드->다음 노드
	listNode * prev = NULL;  //현재 노드 위치 저장

	/*while문이 끝났을 때 prev의 값: 마지막 노드의 주소값*/
	while(p != NULL){
		prev = p;      //현재 노드 값 저장
		p = p->rlink;  //다음 노드 가리킴
	}

	prev->rlink = newnode;     //현재 마지막 노드의 다음 노드 값을 새로운 노드의 주소값으로 삽입
	newnode->rlink = NULL;     //새로운 노드의 다음 노드 값을 NULL 값으로 초기화
	newnode->llink = prev;     //새로운 노드의 이전 노드 값을 현재 마지막 노드의 주소값으로 삽입

	return 0;
}


/*
[마지막 노드 삭제 함수]
*/
int deleteLast(headNode* h) {

	listNode * p = h->first; //현재 노드->다음 노드
	listNode * prev = NULL;  //현재 노드 위치 저장

	/*만약에 첫 노드가 마지막 노드일 경우 마지막 노드를 삭제*/
	if(p->rlink == NULL){
		h->first = NULL; //유일한 노드가 사라졌으므로 헤드노드 또한 가리킬 것이 없음.
		free(p);         //현재 노드를 동적 메모리 할당 해제하여 노드를 삭제함

		return 0;
	}

	/*while문을 빠져나왔을 때, p: 마지막 노드, prev: 마지막 전 노드*/
	while(p->rlink != NULL){

		prev = p;      //현재노드 값 저장
		p = p->rlink;  //다음 노드 가리킴
	}

	prev->rlink = NULL; //마지막 노드 전의 오른쪽 링크값을 NULL로 하여 마지막 노드로 설정해줌.
	free(p);            //마지막 노드를 동적 메모리 할당하여 노드를 삭제함

	return 0;
}

/*
[첫 노드 삽입]
*/
int insertFirst(headNode* h, int key) {

	listNode * newnode = (listNode *)malloc(sizeof(listNode)); //새로운 노드 생성
	newnode->key = key; //새로운 노드의 key값을 주어진 key값으로 설정함

	/*doubly-linked가 비어있을 경우*/
	if(h->first == NULL){
		h->first = newnode;     //새로운 노드가 처음이므로 헤드노드는 새로운 노드를 가리키면 됨.
		//새로운 노드가 유일한 노드이므로 왼쪽,오른쪽 링크값을 NULL로 설정함.
		newnode->llink = NULL;  
		newnode->rlink = NULL;

		return 0;
	}

	/*doubly-linked가 비어있지 않을 경우*/
	listNode * p = h->first;   //현재 첫 번째 노드를 저장함.

	newnode->rlink = h->first; //새로운 노드의 오른쪽 노드를 원래 첫번째 노드의 주소를 저장함
	newnode->llink = NULL;     //새로운 노드가 첫 노드이므로 왼쪽 링크값은 NULL로 설정해 첫번째 노드임을 밝혀줌.

	p->llink = newnode; //원래 첫번째 노드의 왼쪽 링크값을 새로운 노드의 주소로 넣어 이어줌.
	h->first = newnode; //헤드노드는 새로운 노드를 첫 번째 노드로 가리킴.


	return 0;
}

/*
[첫 노드 삭제]
*/
int deleteFirst(headNode* h) {

	listNode * p = h->first; //첫 노드
	listNode * n = p->rlink; //두 번째 노드

	/*doubly-linked에 하나의 노드만 있을 경우*/
	if(p->rlink == NULL){
		h->first = NULL; //유일한 노드가 삭제되었으므로 헤드노드는 가리킬 것이 없음.

		free(p); //첫 노드 동적 메모리 할당 해제
	}

	/*doubly-linked가 두 개 이상의 노드가 있을 경우*/
	else{
		h->first = p->rlink; //헤드노드는 두 번째 노드의 주소값을 가지고 있는 첫 노드의 오른쪽 링크 값으로 가리킴.
		n->llink = NULL;     //두번째노드가 첫노드가 되었으므로 왼쪽 링크는 NULL로 설정

		free(p); //첫 노드 동적 메모리 할당 해제
	}

	return 0;
}

/*
[전치함수]
*/
int invertList(headNode* h) {

	listNode * p = h->first;  //현재 노드 -> 다음 노드
	listNode * n = NULL;      //현재 노드  
	listNode * c = NULL;      //노드 값 일시 저장
	 
	while(p != NULL){
		n = p;        //현재 노드 
		p = p->rlink; //다음 노드 위치 

		c = n->rlink;         //노드의 오른쪽 링크 값 일시 저장
		n->rlink = n->llink;  //노드의 오른쪽 링크 값에 노드의 왼쪽 값 저장
		n->llink = c;         //노드의 왼쪽 링크 값에 노드의 오른쪽 링크 값이 저장된 변수 값을 저장
		
	}

	h->first = n; //헤드포인터를 마지막 노드의 위치 값으로 바꿈

	return 0;
}

/*
[노드 삽입 함수]
*/
int insertNode(headNode* h, int key) {

	listNode * newnode = (listNode *)malloc(sizeof(listNode)); //새로운 노드 생성
	newnode->key = key; //새로운 노드의 key 값을 주어진 key 값으로 설정


	/*doubly-linked가 비어있을 경우: 첫 노드 생성과 동일*/
	if(h->first == NULL){
		h->first = newnode;
		newnode->llink = NULL;
		newnode->rlink = NULL;

		return 0;
	}

	/*새로운 노드가 doubly-linked의 첫 번째 노드의 값보다 작을 경우: 첫 번째 노드에 삽입과 동일*/
	listNode * p = h->first;

	if(key <= p->key){
		newnode->rlink = p;
		newnode->llink = NULL;
		p->llink = newnode;
		h->first = newnode;

		return 0;
	}

	/*doubly-linked가 비어있지 않을 경우: 중간 삽입*/
	listNode * prev = p; //이전 노드의 주소값 넣음.
	p = p->rlink;        //이미 위에서 새 노드가 기존 첫번째노드의 key 값보다 큼을 확인함.

	while(p != NULL){
		if(key <= p->key){
			//새로운 노드의 오른쪽, 왼쪽값을 설정한 후에 기존 노드의 값을 변경
			newnode->rlink = prev->rlink; 
			newnode->llink = p->llink;

			prev->rlink = newnode;
			p->llink = newnode;

			return 0;
		}

		prev = p;
		p = p->rlink; //다음 노드로 옮김
	}

	/*새 노드가 현재 리스트에 있는 모든 key값보다 크므로 마지막에 삽입: 마지막 노드 삽입과 동일*/
	prev->rlink = newnode;
	newnode->llink = prev;
	newnode->rlink = NULL;

	return 0;
}

/*
[노드 삭제 함수]
*/
int deleteNode(headNode* h, int key) {

	listNode * p = h->first; //현재 노드 위치

	/*doubly-linked에 노드가 하나만 있는 경우: 첫 노드 삭제와 동일*/
	if(p->rlink == NULL){
		h->first = NULL;
		free(p);

		return 0;
	}

	/*doubly-linked에 노드가 두 개 이상 남아있는 경우: 중간 삭제*/
	listNode * prev = NULL; //현재 노드 기준 그 전 노드 위치
	listNode * n = NULL;    //현재 노드 기준 그 후 노드 위치

	while(p->key != key){
		prev = p;
		p = p->rlink;
		n = p->rlink;
	}

	//삭제하려는 노드의 전,후 노드를 서로 연결 시켜주는 작업
	prev->rlink = p->rlink;
	n->llink = p->llink;

	free(p); //삭제하려는 노드를 동적 메모리 할당 해제함으로써 노드 삭제

	return 1;
}


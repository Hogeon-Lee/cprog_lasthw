#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
	int year;
	int month;
	int day;
}DATE;

typedef struct
{
	char name[100];
	DATE date;
	int price;
	char w_time[100];
	char type[100];
}Cloth;

typedef struct _node
{
	Cloth cloth;
	struct _node* next;
}Node;


void Clothinfo(const Cloth* cloth)
{
	printf("옷 이름 : %s\n", cloth->name);
	printf("옷 구매일 : %d년 %d월 %d일\n", cloth->date.year, cloth->date.month, cloth->date.day);
	printf("옷 가격 : %d\n", cloth->price);
	printf("착용 시기 : %s\n", cloth->w_time);
	printf("옷 종류 : %s\n", cloth->type);
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
}

void Print_List(Node* head, void(*print)(const Cloth*)) 
{
	Node* cur = head->next;

	printf("\n------옷 목록------\n");
	while (cur != NULL) 
	{
		print(&cur->cloth);
		cur = cur->next;
	}
}

void clearInputBuffer()
{
	while (getchar() != '\n');
}

void addNode(Node* head_ptr, Cloth cloth) {
	Node* newNode = (Node*)malloc(sizeof(Node));

	Node* last = head_ptr;
	while (last->next != NULL)
		last = last->next;

	last->next = newNode;

	newNode->cloth = cloth;
	newNode->next = NULL;
}

void addCloth(Node* head_ptr, void(*add)(Node* head, Cloth cloth)) {
	Cloth cloth = {0, };
	printf("지시에 따라 정보를 입력해 주세요\n");

	printf("옷 이름 : ");
	scanf("%[^\n]s", &cloth.name);
	clearInputBuffer();

	printf("옷 구매일 (Year Month Day) : ");
	scanf("%d %d %d", &cloth.date.year, &cloth.date.month, &cloth.date.day);
	clearInputBuffer();

	printf("옷 가격 : ");
	scanf("%d", &cloth.price);
	clearInputBuffer();

	printf("착용 시기 : ");
	scanf("%s", &cloth.w_time);
	clearInputBuffer();

	printf("옷 종류 : ");
	scanf("%s", &cloth.type);
	clearInputBuffer();

	add(head_ptr, cloth);
}

void deleteNode(Node* head_ptr, char* name) {
	Node* target_ptr = head_ptr->next;
	Node* prev = head_ptr;

	while (target_ptr != NULL) {
		if (strcmp(target_ptr->cloth.name, name) == 0) break;
		else {
			prev = target_ptr;
			target_ptr = target_ptr->next;
		}
	}

	if (target_ptr == NULL) return;
	prev->next = target_ptr->next;
	free(target_ptr);
}

void del_Cloth(Node* head_ptr, void(*del)(Node* head, char* name)) {
	char name[100];
	printf("옷의 이름을 입력하시오 : ");
	scanf("%[^\n]s", &name);
	del(head_ptr, name);
}

void find_Cloth(Node* head, int (*shouldprint)(const Cloth*, char* search), void(*print)(const Cloth*)) {
	Node* cur = head->next;
	char search[100];
	scanf("%[^\n]s", search);

	printf("\n------검색 결과------\n");

	while (cur != NULL) {
		if (shouldprint(&cur->cloth, search) == 1)
			print(&cur->cloth);
		cur = cur->next;
	}
}

int searchname(const Cloth* cloth, char* name) {
	if (strcmp(cloth->name, name) == 0) return 1;
	else return 0;
}

void readCloth(Node* head_ptr, void(*add)(Node* head, Cloth cloth)) {
	FILE* fp = NULL;

	fp = fopen("cloth.dat", "r");
	if (fp == NULL) {
		printf("Cannot open file\n");
		return;
	}

	while (!feof(fp)) {
		Cloth cloth = {0, };

		fscanf(fp, "%[^\n]s", &cloth.name);
		fscanf(fp, "%d %d %d %d %s %s\n", &cloth.date.year, &cloth.date.month, &cloth.date.day, &cloth.price, &cloth.w_time, &cloth.type);

		add(head_ptr, cloth);
	}
	fclose(fp);
}

void saveCloth(Node* head) {
	FILE* fp = NULL;

	fp = fopen("movie.dat", "w");

	Node* cur = head->next;
	while (cur != NULL) {
		Cloth* cloth = &cur->cloth;
		fprintf(fp, "%s\n", cloth->name);
		fprintf(fp, "%d %d %d %d %s %s\n", cloth->date.year, cloth->date.month, cloth->date.day, cloth->price, cloth->w_time, cloth->type);

		cur = cur->next;
	}
	fclose(fp);
}

int main() {
	Node head;
	head.next = NULL;

	int n;

	while (1)
	{
		printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
		printf("옷장 관리 프로그램\n");
		printf("1. 옷 목록 출력\n");
		printf("2. 옷 추가\n");
		printf("3. 옷 제거\n");
		printf("4. 옷 검색\n");
		printf("5. 저장된 목록 불러오기\n");
		printf("6. 현재 목록 저장\n");
		printf("7. 프로그램 종료\n");
		printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
		printf("번호를 입력해주세요 : ");
		scanf("%d", &n);
		clearInputBuffer();

		if (n == 1)
		{
			Print_List(&head, Clothinfo);
		}

		else if (n == 2)
		{
			addCloth(&head, addNode);
		}

		else if (n == 3)
		{
			del_Cloth(&head, deleteNode);
		}

		else if (n == 4)
		{
			printf("옷 이름을 입력하시오 : ");
			find_Cloth(&head, searchname, Clothinfo);
		}

		else if (n == 5)
		{
			readCloth(&head, addNode);
		}

		else if (n == 6)
		{
			saveCloth(&head);
		}

		else if (n == 7)
		{
			break;
		}

		else
		{
			printf("제대로 입력하시오\n");
			n = 0;
		}
	}

	return 0;
}

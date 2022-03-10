#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LENGTH 52
#define CARDPOS 13

typedef struct card {
    char name;
    char color;
    int rank;
    int value;
    
} Card;

typedef struct node {
    Card value;
    struct node* next;
} TNode, *TList, **GameStacks;

typedef struct colorlist{
    TList cards;
    struct colorlist* next;
} Color, *ColorList;

typedef struct playercards{
    int playernumber;
    TList cards;
    struct playercards* next;
    struct playercards* prev;
} Player, *PlayerCards;

char cards[] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
int Values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15};
char colors[] = {'d','c','h','s'};

Card makeCard(char name, char color, int value);
TList create(Card card);
TList insertFront(TList head, Card card);
void printStack(TList head, FILE * out);
TList createstack();
TList freeTList(TList head);
TList insertFrontNode(TList head, TList node);
TList insertEndNode(TList head, TList node);
TList invert(TList head);
int length(TList head);
TList concatenate(TList dest, TList source);
TList NPiles(TList head, int N, int* order);
TList intercalare(TList head);
TList mongean(TList head);
GameStacks splitcardsrazboi(TList stack);
int nrCarti(GameStacks p);
void razboiRound(GameStacks p, FILE* out, int *contor100);
GameStacks freeStacks(GameStacks p, int n);
void frecv(TList stack, int *frecvArr);
void winnerchar(GameStacks p, FILE* out);
GameStacks splitcardsfour(TList cards);
ColorList createC();
ColorList sortcolors(TList stack);
int isWinner(ColorList* sortedcards);
void movetonextcolor(ColorList* sortedcards);
int rollinground(ColorList* sortedcards, int startpoint, FILE* out);
ColorList* freeSortedCards(ColorList* cards);
void rollingstone(TList stack, FILE* out);
int isScarabeu(char c);
int isSpecial(char c);
PlayerCards createPlayercards(TList cards);
PlayerCards playerset(TList stack);
PlayerCards freePlayer(PlayerCards p);
void scarabei(TList stack, FILE* out);

Card makeCard(char name, char color, int value)
{   
    Card c;
    c.name = name;
    c.color = color;
    c.value = value;
    return c;
}

TList create(Card card)
{
    TNode* head = (TNode*) malloc(sizeof(TNode));
    head->value = card;
    head->next = NULL;
   
    return head;
}

TList insertFront(TList head, Card card)
{
    if(head == NULL){
        head = create(card);
        return head;
    }

    TNode* c = create(card);
    c->next = head;
    head = c;

    return head;
}

TList createstack()
{
    TList cardstack = NULL;
    int i, j;
    

    for( i = 0; i < 13; i++){
        for(j = 0; j < 4; j++){
            Card c = makeCard(cards[i],colors[j],Values[i]);
            cardstack = insertFront(cardstack, c);
        }
    }
    return cardstack;
}

void printStack(TList head, FILE* out)
{
    if(head == NULL){
        return;
    } 
    while(head->next != NULL){
        fprintf(out,"%c%c ",head->value.name, head->value.color);
        head = head->next;
    }
    fprintf(out,"%c%c\n", head->value.name, head->value.color);
}

TList freeTList(TList head)
{    
    TList p;
    while(head != NULL){
        p = head;
        head = head->next;
        free(p);
    }
    free(head);

    return NULL;
}

TList insertFrontNode(TList head, TList node)
{
    if(head == NULL){
        node->next = NULL;
        head = node;
        return head;
    }
    node->next = head;
    head = node;

    return head;
}

TList insertEndNode(TList head, TList node)
{
    if(head == NULL){
        node->next = NULL;
        head = node;
        return head;
    }

    TList parse = head;
    while(parse->next != NULL){
        parse = parse->next;
    }
    node->next = NULL;
    parse->next = node;

    return head;
}

TList invert(TList head)
{
    if(head == NULL){
        return NULL;
    }

    if(head->next == NULL){
        return head;
    }

    TList extra = NULL;
    while(head != NULL){
        TList node = head;
        head = head->next;
        extra = insertFrontNode(extra,node);
    }

    return extra;
}

int length(TList head)
{
    if(head == NULL){
        return 0;
    }
    return 1 + length(head->next);
}

TList concatenate(TList dest, TList source)
{
    if(dest == NULL){
        dest = source;
        return dest;
    }
    TList cpy = dest;
    while(cpy->next != NULL){
        cpy = cpy->next; 
    }
    cpy->next = source;
    return dest;
}

TList NPiles(TList head, int N, int* order)
{
    TList* parts = (TList*) calloc(N,sizeof(TList));
    int len = length(head), i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < len/N && head != NULL; j++){
            TList node = head;
            head = head->next;
            parts[i] = insertEndNode(parts[i],node);
        }
    }
    
    for(i = 0; i < N; i++){
        head = concatenate(head, parts[order[i] - 1]);
    }

    free(parts);
    return head;
}

TList intercalare(TList head)
{
    TList* parts = (TList*) calloc(2,sizeof(TList));
    
    int len = length(head), i;
    
    for(i = 0; i < len/2; i++){
        TList node = head;
        head = head->next;
        parts[0] = insertEndNode(parts[0],node);
    }

    while(head != NULL){
        TList node = head;
        head = head->next;
        parts[1] = insertEndNode(parts[1],node);
    }
    
    for(i = 0; i < len/2; i++){
        TList node1 = parts[0], node2 = parts[1];
        parts[0] = parts[0]->next;
        parts[1] = parts[1]->next;
        head = insertEndNode(head, node1);
        head = insertEndNode(head, node2);
    }

    free(parts);
    return head;
}

TList mongean(TList head)
{
    int i = 0;
    TList result = NULL;
    TList node = head;
    head = head->next;
    result = insertFrontNode(result, node);
    while(head != NULL){
        if(i % 2 == 0){
            TList node1 = head;
            head = head->next;
            result = insertFrontNode(result,node1);
        } else {
            TList node1 = head;
            head = head->next;
            result = insertEndNode(result,node1);
        }
        i++;
    }
    return result;
}

GameStacks splitcardsrazboi(TList stack)
{
    GameStacks p = (GameStacks) calloc(2,sizeof(TList));
    TList cpy = stack;
    int i = 0, len = length(stack);
    while(i < len/2 - 1){
        cpy = cpy->next;
        i++;
    }
    p[1] = cpy->next;
    cpy->next = NULL;
    p[0] = stack;
    return p;
}

int nrCarti(GameStacks p)
{
    int min1, min2; 
    if(length(p[0]) - 1 <= p[0]->value.value){
        min1 = length(p[0]) - 1;
    } else min1 = p[0]->value.value;

    if(length(p[1]) - 1 <= p[0]->value.value){
        min2 = length(p[1]) - 1;
    } else min2 = p[0]->value.value;

    if(min1 <= min2){
        return min1;
    } else return min2;
}

GameStacks freeStacks(GameStacks p, int n)
{
    for(int i = 0; i < n; i++){
        freeTList(p[i]);
    }
    free(p);

    return NULL;
}

void frecv(TList stack, int *frecvArr)
{
    while(stack != NULL){
        for(int i = 0; i < CARDPOS; i++){
            if(stack->value.name == cards[i]){
                frecvArr[i] ++;
                break;
            }
        }
        stack = stack->next;
    }
}

void winnerchar(GameStacks p, FILE* out)
{
    int *arr1 = calloc(CARDPOS,sizeof(int));
    int *arr2 = calloc(CARDPOS,sizeof(int));
    frecv(p[0],arr1);
    frecv(p[1],arr2);
    for(int i = CARDPOS - 1; i >= 0; i--){
        if(arr1[i] > arr2[i]){
            fprintf(out,"1 %c",cards[i]);
            break;
        }
        if(arr1[i] < arr2[i]){
            fprintf(out,"2 %c",cards[i]);
            break;
        }
    }
    free(arr1);
    free(arr2);
}

void razboiRound(GameStacks p, FILE* out, int *contor100)
{
    if(*contor100 < 100){
        
        if(p[0]->value.value > p[1]->value.value){
            TList card1 = p[0];
            TList card2 = p[1];
            p[0] = p[0]->next;
            p[1] = p[1]->next;
            p[0] = insertEndNode(p[0],card1);
            p[0] = insertEndNode(p[0],card2);
        }else if(p[0]->value.value < p[1]->value.value){
            TList card1 = p[0];
            TList card2 = p[1];
            p[0] = p[0]->next;
            p[1] = p[1]->next;
            p[1] = insertEndNode(p[1],card2);
            p[1] = insertEndNode(p[1],card1);
        } else {
            int n = nrCarti(p);
            TList card1 = p[0];
            TList card2 = p[1];
            
            p[0] = p[0]->next;
            p[1] = p[1]->next;
            card1->next = NULL;
            card2->next = NULL;
            
            for(int i = 0; i < n; i++){
                TList extra1 = p[0], extra2 = p[1];
                p[0] = p[0]->next;
                p[1] = p[1]->next;
                card1 = insertFrontNode(card1,extra1);
                card2 = insertFrontNode(card2,extra2);
            }
            if(card1->value.value > card2->value.value){
                p[0] = concatenate(p[0],card1);
                p[0] = concatenate(p[0],card2);
            } else {
                p[1] = concatenate(p[1],card2);
                p[1] = concatenate(p[1],card1);
            }
        }
        if(p[0] == NULL){
            fprintf(out,"2");
            freeStacks(p,2);
            return;
        } 
        if(p[1] == NULL){
            fprintf(out,"1");
            freeStacks(p,2);
            return;
        }

        (*contor100)++;

    } else {
        winnerchar(p,out);
        freeStacks(p,2);
        return;
    }
    
    razboiRound(p,out,contor100);
}

GameStacks splitcardsfour(TList cards)
{
    GameStacks p2,buffer1, buffer2;
    GameStacks p4 = (GameStacks) calloc(4, sizeof(TList));
    p2 = splitcardsrazboi(cards);
    buffer1 = splitcardsrazboi(p2[0]);
    p4[0] = buffer1[0];
    p4[1] = buffer1[1];
    buffer2 = splitcardsrazboi(p2[1]);
    p4[2] = buffer2[0];
    p4[3] = buffer2[1];

    free(p2);
    free(buffer1);
    free(buffer2);
    return p4;
}

ColorList createC()
{
    ColorList c = (ColorList) malloc(sizeof(Color));
    c->cards = NULL;
    c->next = NULL;
    return c;
}

ColorList sortcolors(TList stack)
{
    ColorList s = createC(), h = createC(), c = createC(), d = createC();
    s->next = h;
    h->next = c;
    c->next = d;
    d->next = s;
    while(stack != NULL){
        TList node = stack;
        stack = stack->next;
        if(node->value.color == 's'){
            s->cards = insertEndNode(s->cards, node);
        } else if(node->value.color == 'h'){
            h->cards = insertEndNode(h->cards, node);
        } else if(node->value.color == 'c'){
            c->cards = insertEndNode(c->cards, node);
        } else {
            d->cards = insertEndNode(d->cards, node);
        }
    }

    return s;
}

int isWinner(ColorList* sortedcards)
{
    int i, result = 0;
    for(i = 0; i < 4; i++){
        result = ((sortedcards[i]->cards == NULL) && (sortedcards[i]->next->cards == NULL) &&
                (sortedcards[i]->next->next->cards == NULL) && (sortedcards[i]->next->next->next->cards == NULL));
        if(result == 1){
            return i + 1;
        }
    } 

    return 0;
}

void movetonextcolor(ColorList* sortedcards)
{
    for(int i = 0; i < 4; i++){
        sortedcards[i] = sortedcards[i]->next;
    }
}

int rollinground(ColorList* sortedcards, int startpoint, FILE* out)
{
    int lost = 0;
    TList table = NULL;
    while(sortedcards[startpoint]->cards == NULL){
        movetonextcolor(sortedcards);
    }
    
    while(lost == 0){
        if(sortedcards[startpoint]->cards != NULL && isWinner(sortedcards) == 0){
            
            TList card = sortedcards[startpoint]->cards;
            sortedcards[startpoint]->cards = sortedcards[startpoint]->cards->next;
            table = insertEndNode(table, card);
            
        } else lost = 1;
        startpoint = (startpoint + 1) % 4;
    }

    if(startpoint == 0){
        startpoint = 3;
    } else startpoint--;

    sortedcards[startpoint]->cards = concatenate(sortedcards[startpoint]->cards,table);
    return startpoint;
}

ColorList* freeSortedCards(ColorList* cards)
{
    for(int i = 0; i < 4; i++){
        ColorList c, head = cards[i];
        cards[i] = cards[i]->next;
        while(cards[i] != head){
            freeTList(cards[i]->cards);
            c = cards[i];
            cards[i] = cards[i]->next;
            free(c);
        }
        freeTList(head->cards);
        free(head);
    }
    free(cards);
    return NULL;
}

void rollingstone(TList stack, FILE* out)
{
    GameStacks carti = splitcardsfour(stack);
    ColorList* sortedcards = (ColorList*) calloc(4,sizeof(ColorList));
    for(int i = 0; i < 4; i++){
        sortedcards[i] = sortcolors(carti[i]);
    }
    int winner = 0;
    int loser = rollinground(sortedcards,0,out);
    movetonextcolor(sortedcards);
    while(winner == 0){
        loser = rollinground(sortedcards,loser,out);
        winner = isWinner(sortedcards);
        movetonextcolor(sortedcards);
    }
    fprintf(out,"%d",winner);

    freeSortedCards(sortedcards);
    free(carti);
}

int isScarabeu(char c)
{
    return (c == 'A' || c == 'J' || c == 'Q' || c == 'K');
}

int isSpecial(char c)
{
    return (c == '8');
}

PlayerCards createPlayercards(TList cards)
{
    PlayerCards p = (PlayerCards) calloc(1,sizeof(Player));
    p->cards = cards;
    return p;
}

PlayerCards playerset(TList stack)
{
    GameStacks carti = splitcardsfour(stack);
    PlayerCards p1 = createPlayercards(carti[0]), p2 = createPlayercards(carti[1]);
    PlayerCards p3 = createPlayercards(carti[2]), p4 = createPlayercards(carti[3]);
    p1->playernumber = 1;
    p2->playernumber = 2;
    p3->playernumber = 3;
    p4->playernumber = 4;
    p1->next = p2;
    p2->next = p3;
    p3->next = p4;
    p4->next = p1;
    p1->prev = p4;
    p2->prev = p1;
    p3->prev = p2;
    p4->prev = p3;
    free(carti);
    return p1;
}

PlayerCards freePlayer(PlayerCards p)
{
    freeTList(p->cards);
    free(p);
    return NULL;
}

void scarabei(TList stack, FILE* out)
{
    PlayerCards p = playerset(stack);
    int order = 1;                  //va avea valoarea 1 pt ordine normala si -1 pt ordine inversa
    
    while(p->next != p){                     //pana cand ramane un singur jucator in joc

        int okay = 1, must = 0;     //okay va contoriza daca inca se pun carti jos sau au fost luate, must va contoriza daca s-a pus vreun scarabeu
        PlayerCards last;                       //contorizeaza ultimul jucator care ia cartile in cazul terminarii rundei
        TList table = NULL;               //pachetul cu cartile de pe masa

        while(okay == 1){
            
            while(p->cards == NULL && last != p){                             //se elimina jucatorul daca nu mai are carti
                PlayerCards cpy = p;
                p->prev->next = p->next;
                p->next->prev = p->prev;
                if(order == 1){
                    p = p->next;
                } else p = p->prev;
                free(cpy);
            }
            
            TList card  = p->cards;
            p->cards = p->cards->next;
            table = insertFrontNode(table,card);

            if(isScarabeu(table->value.name)){
                must = 1;
                last = p;
            }

            if(isSpecial(table->value.name)){
                if(must == 0){
                    order = order*(-1);
                } 
            }

            if(order == 1){
                p = p->next;
            } else p = p->prev;

            if(isScarabeu(table->value.name) == 0 && isSpecial(table->value.name) == 0 && must == 1){
                
                last->cards = concatenate(last->cards,table);
                okay = 0;
                p = last;
                last = NULL;
            }
        }
    }

    fprintf(out,"%d",p->playernumber);
    freePlayer(p);
}
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_MAX 100
#define STACK_MAX 100

#define N 8

int graph[N][N] = {
{0, 1, 1, 1, 0, 0, 0, 0},
{1, 0, 0, 0, 1, 1, 0, 0},
{1, 0, 0, 0, 0, 0, 0, 0},
{1, 0, 0, 0, 0, 0, 0 ,0},
{0, 1, 0, 0, 0, 0, 0, 0},
{0, 1, 0, 0, 0, 0, 1, 1},
{0, 0, 0, 0, 0, 1, 0, 0},
{0, 0, 0, 0, 0, 1, 0, 0}
};



struct queue {
    int qu[QUEUE_MAX];
    int end, begin;
};
//Инициализация очереди
void queue_init(struct queue *q) {
    q->begin = 1;
    q->end = 0;
    return;
}
//Добавление элемента в очередь
void insert(struct queue *q, int x) {
    if(q->end < QUEUE_MAX - 1) {
        q->end++;
        q->qu[q->end] = x;
    }
    else
        printf("Очередь полна!\n");
    return;
}
//Проверка пустоты очереди
int queue_isempty(struct queue *q) {
    if(q->end < q->begin)    return(1);
    else  return(0);
}
//Вывод элементов очереди
void print(struct queue *q) {
    int h;
    if(queue_isempty(q) == 1) {
        printf("Очередь пуста!\n");
        return;
    }
    for(h = q->begin; h <= q->end; h++)
        printf("%d ", q->qu[h]);
    return;
}
//Удаление элемента из очереди
int queue_remove(struct queue *q) {
    int x;
    if(queue_isempty(q) == 1) {
        printf("Очередь пуста!\n");
        return(0);
    }
    x = q->qu[q->begin];
    q->begin++;
    return(x);
}  


struct stack {
    float elem[STACK_MAX];
    int top;
};
//Инициализация стека
//Индекс элемента, находящегося в вершине стека, равен 0.
void stack_init(struct stack *stk) {
    stk->top = 0;
}
//Помещение элемента в стек
void push(struct stack *stk, float f) {
    if(stk->top < STACK_MAX) {
        stk->elem[stk->top] = f;
        stk->top++;
    } else
        printf("Стек полон, количество элементов: %d !\n", stk->top);
}
//Удаление элемента из стека
float pop(struct stack *stk) {
    float elem;
    if((stk->top) > 0) {
        stk->top--;
        elem = stk->elem[stk->top];
        return(elem);
    } else {
        printf("Стек пуст!\n");
        return(0);
    }
}
//Извлечение вершины стека
float sktTop(struct stack *stk) {
    if((stk->top) > 0) {
        return(stk->elem[stk->top-1]);
    } else {
        printf("Стек пуст!\n");
        return(0);
    }
}
//Получение верхнего элемента стека без его удаления
int gettop(struct stack *stk) {
    return(stk->top);}
//Определение пустоты стека
int stack_isempty(struct stack *stk) {
    if((stk->top) == 0)    return 1;
    else return 0;
}
//Вывод элементов стека
void stkPrint(struct stack *stk) {
    int i;
    i = stk->top;
    if(stack_isempty(stk) == 1) return;
    do {
        i--;
        printf("%f\n", stk->elem[i]);
    } while (i > 0);
}

void DFS(struct stack * s, int dir) {
    int i;
    if (0)
        return;
    else {
        for (i = dir; i < N; i++)
        {
            if ( graph[dir][i] ) {
              /*
               * FIXIT:
               * вы либо используете свою структуру стека, либо используете рекурсию. в данном коде от s ничего не зависит и её можно просто удалить. 
               */
                push(s, i + 1);
                DFS(s, i);
            }
        }
    }
}

void BFS(struct queue *queue)
{
    int dir;
    int i;
    while (!queue_isempty(queue))
    {
        /*
         * есть стандартные названия для операций со стеком и с очередью. лучше придерживаться их.
         */
        dir = queue_remove(queue);
        printf("%d ", dir + 1);
        for (i = dir; i < N; i++)
        {
            if (graph[dir][i]) 
            {
                insert(queue, i);
            }
        }
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    struct stack *stk;
    stk = (struct stack*)malloc(sizeof(struct stack));
    stack_init(stk);
    int first_elem = 1;
    DFS(stk, first_elem);
    stkPrint(stk);
    printf("\n");
    struct queue *q;
    q = (struct queue*)malloc(sizeof(struct queue));
    queue_init(q);
    BFS(q);
    return 0;
}


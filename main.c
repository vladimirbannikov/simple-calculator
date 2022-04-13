#include <stdio.h> //Для работы ввода и вывода в консоль
#include <stdlib.h> 
#include <string.h> 
#include <locale.h> 
typedef struct stack_char//Символьный стек, стек операций
{
    char data;
    struct stack_char* next;
}STACK_CHAR;

typedef struct stack_int//Численный стек
{
    int data;
    struct stack_int* next;
}STACK_INT;

void push_char(STACK_CHAR** stack, const int new_element) //Помещает новую операцию в стек
{
    STACK_CHAR* new_part = NULL;
    new_part = (STACK_CHAR*)malloc(sizeof(STACK_CHAR));
    if (new_part != NULL)
    {
        memset(new_part, 0x00, sizeof(STACK_CHAR));
        new_part->data = new_element;
        new_part->next = *stack;
        *stack = new_part;
    }
}

void push_int(STACK_INT** stack, int new_element)//Помещает новый элемент в стек
{
    STACK_INT* new_part = NULL;
    new_part = (STACK_INT*)malloc(sizeof(STACK_INT));
    if (new_part != NULL)
    {
        memset(new_part, 0x00, sizeof(STACK_INT));
        new_part->data = new_element;
        new_part->next = *stack;
        *stack = new_part;
    }
}

int pop_int(STACK_INT** stack)//Достает последний элемент из стека
{
    int pop = 0;

    if (stack == NULL)
        return EXIT_SUCCESS;

    pop = (*stack)->data;
    *stack = (*stack)->next;

    return pop;
}

int pop_char(STACK_CHAR** stack)//Достает последнюю операцию из стека
{
    char pop = '\0';

    if (stack == NULL)
        return EXIT_SUCCESS;

    pop = (*stack)->data;
    *stack = (*stack)->next;

    return pop;
}

int peek_int_check(STACK_INT** stack)//Чекает последнюю операцию в стеке
{
    if (stack == NULL)
        return NULL;
    else return (*stack)->data;
}

char peek_char_check(STACK_CHAR** stack)//Чекает последний элемент в стеке
{
    if (stack == NULL)
        return NULL;
    else return (*stack)->data;
}

void print_stack_int(STACK_INT* start)//Выводит стек чисел
{
    printf("Numbers stack: ");
    if(start == NULL)
        printf("Stack is NULL\n");
    else
    {
        while (start->next != NULL)
        {
            printf("%d —> ", start->data);
            start = start->next;
        }
    }
    printf("NULL\n");
}

void print_stack_char(STACK_CHAR* start)//Выводит стек операций
{
    printf("Operations stack: ");
    if (start == NULL)
        printf("Stack is NULL\n");
    else
    {
        while (start != NULL)
        {
            if(start->data != NULL)
                printf("%c —> ", start->data);
            else
                printf("NULL", start->data);
            start = start->next;
        }
    }
    printf("\n");
}

int main()
{
    STACK_CHAR* stack_char = NULL;
    STACK_INT* stack_int = NULL;
    char symbol = '\0'; //Символ, отвечающий за операцию только поступившую из строки
    int in_stack = 0; //Число, составленное из символов, которое заносится в стек
    int from_int1 = 0, from_int2 = 0; //Два верхних числа в стеке
    char from_char = '\0'; //Верхняя операция в стеке
    int flag = 0, check_correct = 0, flag1 = 0;
    setlocale(LC_ALL, "RUS");

    push_char(&stack_char, NULL); //Кладем первые NULL-овые значения в стеки
    push_int(&stack_int, NULL);

    INPUT: //Цикл чтения символов из строки
    while ((symbol = getc(stdin)) != '\n')//Пока не достигли конца строки считываем новый символ
    {
        if ((int)symbol >= 48 && (int)symbol < 58)//Запись числа
        {
            while ((int)symbol >= 48 && (int)symbol < 58)//Чтение многозначного числа
            {
                in_stack = in_stack * 10 + ((int)symbol - 48);
                symbol = getc(stdin);
            }
            check_correct = 1; //Флаг, отвечающий за корректность ввода последнего символа
        }
        if (flag == 1)
        {
            printf("Flag = %d,  %d to %d\n", flag, peek_int_check(&stack_int), in_stack);
            pop_int(&stack_int); //Достаем 0 из стека и ставим на его место число
            pop_int(&stack_int);
            push_int(&stack_int, in_stack);
            in_stack = 0;
            flag = 0;
        }
        else
        {
            push_int(&stack_int, in_stack);
            in_stack = 0;
        }

        if (symbol != '\n' && symbol != '=') //Проверка на последний символ
        {
            if(symbol != ')')
                check_correct = 0;
            goto CIRCLE; //Переход к проверкам
        }
        else
            break;
    }

    CIRCLE: //Проверки на наличие операций лежащих в стеке с большим приоритетом чем операция постуившая из строки
    if (flag1 == 3 && peek_int_check(&stack_int) == 0)
    {
        flag1 = 0;
        printf("Get symbol\n");
        pop_int(&stack_int);
    }
    print_stack_char(stack_char);
    print_stack_int(stack_int);
    if (symbol == ')')
    {
        flag1 = 3;
        printf("\nClose bracket\n");
        while (peek_char_check(&stack_char) != '(' && peek_char_check(&stack_char) != NULL)//Выполнение оставщихся операций до открывающейся скобочки или до конца стека
        {
            if (peek_char_check(&stack_char) == NULL) //Если не встретили открывающуюся скобочку, то выражение записано некорректно
            {
                printf("Incorrect!\n");
                return -1;
            }

            from_char = (char)pop_char(&stack_char); //Достаем поседнюю операцию и два последних числа
            from_int2 = pop_int(&stack_int);
            from_int1 = pop_int(&stack_int);

            int p = from_int1;//Параметр для возведения в степень

            switch (from_char)//Определение операции
            {
                case '-':
                    push_int(&stack_int, from_int1 - from_int2);
                    break;
                case '+':
                    push_int(&stack_int, from_int1 + from_int2);
                    break;
                case '*':
                    push_int(&stack_int, from_int1 * from_int2);
                    break;
                case '/':
                    if (from_int2 != 0) //Проверка на корректную запись, делить на ноль нельзя
                        push_int(&stack_int, from_int1 / from_int2);
                    else
                    {
                        printf("Incorrect!\n");
                        return -1;
                    }
                    break;
                case '%':
                    if (from_int2 != 0)//Проверка на корректную запись, остаток от деления на ноль не существует
                        push_int(&stack_int, from_int1 % from_int2);
                    else
                    {
                        printf("Incorrect!\n");
                        return -1;
                    }
                    break;
                case '^':
                    if (from_int2 <= 0 && from_int1 != 0) //В отрицательную или равную нулю
                    {
                        for (int i = from_int2 + 1; i > 0; i--)
                            p /= from_int1;
                    }
                    else
                    {
                        for (int i = 0; i < from_int2 - 1; i++) //В положительную
                            p *= from_int1;
                    }
                    push_int(&stack_int, p);
                    break;
                case '(':
                    break;
                default://Если встречается символ, не отвечающий за какую-либо операцию, программы заканчивает работу с ошибкой
                    printf("Invalid operation!\n");
                    return -2;
                    break;
            }
        }
        from_char = (char)pop_char(&stack_char); //Удаляем открывающуюся скобочку
        symbol = (char)pop_char(&stack_char); //И достаем следующую операцию
//flag = 1;
//pop_int(&stack_int); //Достаем 0 из стека и ставим на его место число
    }

    if (symbol == '-' || symbol == '+')
    {
        printf("\n1 priority\n");
        while (peek_char_check(&stack_char) != '(' && peek_char_check(&stack_char) != NULL)//Выполнение оставщихся операций до более низкого приоритета
        {
            from_char = (char)pop_char(&stack_char);
            from_int2 = pop_int(&stack_int);
            from_int1 = pop_int(&stack_int);

            int p = from_int1;//Параметр для возведения в степень

            switch (from_char)
            {
                case '-':
                    push_int(&stack_int, from_int1 - from_int2);
                    break;
                case '+':
                    push_int(&stack_int, from_int1 + from_int2);
                    break;
                case '*':
                    push_int(&stack_int, from_int1 * from_int2);
                    break;
                case '/':
                    if (from_int2 != 0)
                        push_int(&stack_int, from_int1 / from_int2);
                    else
                    {
                        printf("Incorrect!\n");
                        return -1;
                    }
                    break;
                case '%':
                    if (from_int2 != 0)
                        push_int(&stack_int, from_int1 % from_int2);
                    else
                    {
                        printf("Incorrect!\n");
                        return -1;
                    }
                    break;
                case '^':
                    if (from_int2 <= 0 && from_int1 != 0)
                    {
                        for (int i = from_int2 + 1; i > 0; i--)
                            p /= from_int1;
                    }
                    else
                    {
                        for (int i = 0; i < from_int2 - 1; i++)
                            p *= from_int1;
                    }
                    push_int(&stack_int, p);
                    break;
                case '(':
                    break;
                default:
                    printf("Invalid operation!\n");
                    return -2;
                    break;
            }
        }
    }

    if (symbol == '/' || symbol == '*' || symbol == '%')
    {
        printf("\n2 priority\n");
        while (peek_char_check(&stack_char) != '(' && peek_char_check(&stack_char) != NULL && peek_char_check(&stack_char) != '+' && peek_char_check(&stack_char) !='-')//Выполнение оставщихся операций до более низщего приоритета
        {
            from_char = (char)pop_char(&stack_char);
            from_int2 = pop_int(&stack_int);
            from_int1 = pop_int(&stack_int);

            int p = from_int1;//Параметр для возведения в степень

            switch (from_char)
            {
                case '-':
                    break;
                case '+':
                    break;
                case '*':
                    push_int(&stack_int, from_int1 * from_int2);
                    break;
                case '/':
                    if (from_int2 != 0)
                        push_int(&stack_int, from_int1 / from_int2);
                    else
                    {
                        printf("Incorrect!\n");
                        return -1;
                    }
                    break;
                case '%':
                    if (from_int2 != 0)
                        push_int(&stack_int, from_int1 % from_int2);
                    else
                    {
                        printf("Incorrect!\n");
                        return -1;
                    }
                    break;
                case '^':
                    if (from_int2 <= 0 && from_int1 != 0)
                    {
                        for (int i = from_int2 + 1; i > 0; i--)
                            p /= from_int1;
                    }
                    else
                    {
                        for (int i = 0; i < from_int2 - 1; i++)
                            p *= from_int1;
                    }
                    push_int(&stack_int, p);
                    break;
                case '(':
                    break;
                default:
                    printf("Invalid operation!\n");
                    return -2;
                    break;
            }
        }
    }

    if (symbol == '^')
    {
        printf("\n3 priority\n");
        while (peek_char_check(&stack_char) != '(' && peek_char_check(&stack_char) != NULL && peek_char_check(&stack_char) != '+' && peek_char_check(&stack_char) != '-'
               && peek_char_check(&stack_char) != '*' && peek_char_check(&stack_char) != '/' && peek_char_check(&stack_char) != '%')//Выполнение оставщихся операций до более низкого приоритета
        {
            from_char = (char)pop_char(&stack_char);
            from_int2 = pop_int(&stack_int);
            from_int1 = pop_int(&stack_int);

            int p = from_int1;//Параметр для возведения в степень

            switch (from_char)
            {
                case '-':
                    break;
                case '+':
                    break;
                case '*':
                    break;
                case '/':
                    break;
                case '%':
                    break;
                case '^':
                    if (from_int2 <= 0 && from_int1 != 0)
                    {
                        for (int i = from_int2 + 1; i > 0; i--)
                            p /= from_int1;
                    }
                    else
                    {
                        for (int i = 0; i < from_int2 - 1; i++)
                            p *= from_int1;
                    }
                    push_int(&stack_int, p);
                    break;
                case '(':
                    break;
                default:
                    printf("Invalid operation!\n");
                    return -2;
                    break;
            }
        }
    }

    if (symbol == '(')
    {
        printf("\nOpen bracket\n");
        push_int(&stack_int, 0); //Вставляем в стек ноль для вида (-4) = (0-4)
        flag = 1; //Отмечаем эту вставку
    }

    if (symbol != '\n' && symbol != ')' && symbol != '=')//Операции, которые не кладутся в стек
    {
        push_char(&stack_char, symbol);
        goto INPUT; //Возврат к вводу символов
    }
    printf("\n");

    while (peek_char_check(&stack_char) != NULL)//Выполнение оставшихся операций в стеке
    {
        from_char = (char)pop_char(&stack_char);
        from_int2 = pop_int(&stack_int);
        from_int1 = pop_int(&stack_int);

        int p = from_int1;//Параметр для возведения в степень

        switch (from_char)
        {
            case '-':
                push_int(&stack_int, from_int1 - from_int2);
                break;
            case '+':
                push_int(&stack_int, from_int1 + from_int2);
                break;
            case '*':
                push_int(&stack_int, from_int1 * from_int2);
                break;
            case '/':
                if (from_int2 != 0)
                    push_int(&stack_int, from_int1 / from_int2);
                else
                {
                    printf("Incorrect!\n");
                    return -1;
                }
                break;
            case '%':
                if (from_int2 != 0)
                    push_int(&stack_int, from_int1 % from_int2);
                else
                {
                    printf("Incorrect!\n");
                    return -1;
                }
                break;
            case '^':
                if (from_int2 <= 0 && from_int1 != 0)
                {
                    for (int i = from_int2 + 1; i > 0; i--)
                        p /= from_int1;
                }
                else
                {
                    for (int i = 0; i < from_int2 - 1; i++)
                        p *= from_int1;
                }
                push_int(&stack_int, p);
                break;
            case '(':
                break;
            default:
                printf("Invalid operation!\n");
                return -2;
                break;
        }
    }
    print_stack_char(stack_char);
    print_stack_int(stack_int);

    if (check_correct == 0)
    {
        printf("\nIncorrect!\n");
        return -1;
    }
    else
        printf("\n= %d", pop_int(&stack_int));//Вывод результата

    return EXIT_SUCCESS;

}

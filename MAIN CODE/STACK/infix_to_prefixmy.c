#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

// Stack structure
struct Stack {
    int top;
    unsigned capacity;
    char* array;
};

// Function to create a stack
struct Stack* createStack(unsigned capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (char*)malloc(stack->capacity * sizeof(char));
    return stack;
}

// Stack operations
int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

char peek(struct Stack* stack) {
    return stack->array[stack->top];
}

char pop(struct Stack* stack) {
    if (!isEmpty(stack))
        return stack->array[stack->top--];
    return '$'; // Special character indicating empty stack
}

void push(struct Stack* stack, char op) {
    stack->array[++stack->top] = op;
}

int isOperand(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

int precedence(char ch) {
    switch (ch) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    }
    return -1;
}

// Function to convert infix to prefix
void infixToPrefix(char* infix) {
    int length = strlen(infix);

    // Reverse the infix expression
    for (int i = 0; i < length / 2; i++) {
        char temp = infix[i];
        infix[i] = infix[length - i - 1];
        infix[length - i - 1] = temp;
    }

    struct Stack* stack = createStack(MAX_SIZE);
    char prefix[MAX_SIZE];
    int k = 0;

    // Iterate through the reversed infix expression
    for (int i = 0; infix[i]; ++i) {
        if (infix[i] == '(')
            infix[i] = ')';
        else if (infix[i] == ')')
            infix[i] = '(';
    }

    for (int i = 0; i < length; i++) {
        if (isOperand(infix[i])) {
            prefix[k++] = infix[i];
        } else if (infix[i] == '(') {
            push(stack, infix[i]);
        } else if (infix[i] == ')') {
            while (!isEmpty(stack) && peek(stack) != '(')
                prefix[k++] = pop(stack);
            if (!isEmpty(stack) && peek(stack) != '(')
                return; // Invalid expression
            else
                pop(stack);
        } else { // Operator
            while (!isEmpty(stack) && precedence(infix[i]) <= precedence(peek(stack))) {
                prefix[k++] = pop(stack);
            }
            push(stack, infix[i]);
        }
    }

    // Pop remaining operators from stack
    while (!isEmpty(stack)) {
        prefix[k++] = pop(stack);
    }

    prefix[k] = '\0';

    // Reverse the final prefix expression
    length = strlen(prefix);
    for (int i = 0; i < length / 2; i++) {
        char temp = prefix[i];
        prefix[i] = prefix[length - i - 1];
        prefix[length - i - 1] = temp;
    }

    printf("Prefix expression: %s\n", prefix);
}

int main() {
    char infix[MAX_SIZE];

    printf("Enter infix expression: ");
    fgets(infix, MAX_SIZE, stdin);
    infix[strcspn(infix, "\n")] = '\0'; // Remove newline character

    infixToPrefix(infix);

    return 0;
}

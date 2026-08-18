#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    double* numStack;
    char* opStack;
    int numTop;
    int opTop;
    int maxSize;
} Stack;

double evaluate(const char* expr, int* pos, Stack* s);

void initStack(Stack* s, int maxSize) {
    s->numStack = (double*)malloc(sizeof(double) * maxSize);
    s->opStack = (char*)malloc(sizeof(char) * maxSize);
    s->numTop = -1;
    s->opTop = -1;
    s->maxSize = maxSize;

    if (s->numStack == NULL || s->opStack == NULL) {
        printf("Error: Memory allocation failed for the stack!\n");
        exit(1);
    }
}

void freeStack(Stack* s) {
    free(s->numStack);
    free(s->opStack);
}

void pushNum(Stack* s, double value) {
    s->numStack[++s->numTop] = value;
}

void pushOp(Stack* s, char op) {
    s->opStack[++s->opTop] = op;
}

double popNum(Stack* s) {
    return s->numStack[s->numTop--];
}

char popOp(Stack* s) {
    return s->opStack[s->opTop--];
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1; // Low precedence
    if (op == '*' || op == '/' || op == '%') return 2; // Medium precedence
    if (op == '^') return 3; // High precedence
    return 0; // Default
}

double calculate(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                printf("Error: Division by zero\n");
                exit(1);
            }
            return a / b;
        case '%':
            if ((int)b == 0) {
                printf("Error: Modulus by zero\n");
                exit(1);
            }
            return (int)a % (int)b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

double processFunction(const char* func, double value) {
    if (strcmp(func, "sqrt") == 0) return sqrt(value);
    if (strcmp(func, "cbrt") == 0) return cbrt(value);
    if (strcmp(func, "sin") == 0) return sin(value * M_PI / 180.0); // Degrees to radians
    if (strcmp(func, "cos") == 0) return cos(value * M_PI / 180.0);
    if (strcmp(func, "tan") == 0) return tan(value * M_PI / 180.0);
    printf("Error: Unknown function '%s'\n", func);
    exit(1);
}

// Helper function to handle numbers in the expression
double handleNumber(const char* expr, int* i) {
    double num = 0.0, fraction = 0.1;
    int isFraction = 0;

    // Handle the number or decimal
    while (isdigit(expr[*i]) || expr[*i] == '.') {
        if (expr[*i] == '.') {
            isFraction = 1;
        } else if (isFraction) {
            num += (expr[*i] - '0') * fraction;
            fraction *= 0.1;
        } else {
            num = num * 10 + (expr[*i] - '0');
        }
        (*i)++;
    }
    return num;
}

void handleFunction(const char* expr, int* i, Stack* s, const char* func) {
    if (expr[*i] == '(') {
        (*i)++; // Skip '('
        double value = evaluate(expr, i, s); // Recursively evaluate inner expression
        double result = processFunction(func, value);
        pushNum(s, result);
    }
}

// Evaluate the expression with implicit multiplication handling
double evaluate(const char* expr, int* pos, Stack* s) {
    char func[10]; // To store function names like sqrt, sin
    int i = *pos;
    int lastWasOperator = 1; // To track if the previous character was an operator

    while (expr[i] != '\0' && expr[i] != ')') {
        if (isspace(expr[i])) {
            i++; // Skip spaces
            continue;
        }

        // Implicit multiplication handling (e.g., "3(4+5)" should be treated as "3*(4+5)")
        if (isdigit(expr[i]) && !lastWasOperator) {
            pushOp(s, '*');
        }

        // If it's a number
        if (isdigit(expr[i]) || expr[i] == '.') {
            double num = handleNumber(expr, &i);
            pushNum(s, num);
            lastWasOperator = 0;
        }
        // If it's a function like sqrt, sin, cos
        else if (isalpha(expr[i])) {
            int j = 0;
            while (isalpha(expr[i])) {
                func[j++] = expr[i++];
            }
            func[j] = '\0'; // Null-terminate function name
            handleFunction(expr, &i, s, func);
            lastWasOperator = 0;
        }
        // If it's a parenthesis
        else if (expr[i] == '(') {
            i++; // Skip '('
            pushNum(s, evaluate(expr, &i, s));
            lastWasOperator = 0;
        }
        else if (expr[i] == ')') {
            i++; // Skip ')'
            break;
        }
        // If it's an operator
        else {
            while (s->opTop != -1 && precedence(s->opStack[s->opTop]) >= precedence(expr[i])) {
                double b = popNum(s);
                double a = popNum(s);
                char op = popOp(s);
                pushNum(s, calculate(a, b, op));
            }
            pushOp(s, expr[i]);
            i++;
            lastWasOperator = 1;
        }
    }

    // Process for remaining operators
    while (s->opTop != -1) {
        double b = popNum(s);
        double a = popNum(s);
        char op = popOp(s);
        pushNum(s, calculate(a, b, op));
    }

    *pos = i; // Update position for recursion
    return popNum(s);
}


int main() {
    char expr[256];
    printf("Enter a string (supports +, -, *, /, %, ^, sqrt, cbrt, sin, cos, tan): ");
    fgets(expr, sizeof(expr), stdin);
    expr[strcspn(expr, "\n")] = '\0'; // Remove newline character

    Stack s;
    initStack(&s, 200);

    int start = 0;
    double result = evaluate(expr, &start, &s);
    printf("Result: %.6lf\n", result);

    freeStack(&s);

    return 0;
}

#ifndef TYPES_H
#define TYPES_H
#define COLOR_RED "\033[31m"   // Red color
#define COLOR_GREEN "\033[32m" // Green color
#define COLOR_RESET "\033[0m"  // Reset to default color


/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success, // Using enumerated values for success and failure
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif

#define uchar_t   unsigned char
#define uint_t    unsigned int
#define lint_t    long int
#define ulint_t   unsigned long int
#define llint_t   long long int
#define ullint_t  unsigned long long int
#define ldouble_t long double

#ifdef INFO
    #define PRINT_INFO(str ...) printf(str);
#else
    #define PRINT_INFO(str ...) {};
#endif


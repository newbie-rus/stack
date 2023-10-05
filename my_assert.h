#define MY_ASSERT

#ifdef MY_ASSERT

  const char ERROR_ISFINITE[] = "Бесконечное число или NAN";
  const      char PATH_NULL[] = "Нулевой указатель";
  const     char SAME_PATHS[] = "Одинаковые пути на разные массивы";
  const   char UNREAL_VALUE[] = "Невозможное значение переменной";

  const      int ERROR_OUTPUT = -1;

#define my_assert(condition, error_code)                \
  if ((condition) != 1)                                 \
  {                                                     \
    fprintf(stderr,"Сбой в программе ошибка: %s\n"      \
           "файл: %s, функция: %s, строка: %d\n",       \
           error_code, __FILE__, __func__, __LINE__);   \
    return ERROR_OUTPUT;                                \
  }

#else
  #define my_assert(condition, error_code)
#endif
  
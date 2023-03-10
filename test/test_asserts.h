#ifndef KINETIC_NOTATION_TEST_ASSERTS_H
#define KINETIC_NOTATION_TEST_ASSERTS_H

#define ASSERT_EQ(lhs, rhs)                                                    \
  {                                                                            \
    if ((lhs) != (rhs)) {                                                      \
      fprintf(stderr, "[file: %s][line: %d]\n\t%s != %s\n", __FILE__,          \
              __LINE__, #lhs, #rhs);                                           \
      return 1;                                                                \
    }                                                                          \
  }

#define ASSERT_NE(lhs, rhs)                                                    \
  {                                                                            \
    if ((lhs) == (rhs)) {                                                      \
      fprintf(stderr, "[file: %s][line: %d]\n\t%s == %s\n", __FILE__,          \
              __LINE__, #lhs, #rhs);                                           \
      return 1;                                                                \
    }                                                                          \
  }

#endif // KINETIC_NOTATION_TEST_ASSERTS_H

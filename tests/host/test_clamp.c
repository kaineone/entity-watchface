#include "../../src/c/logic/clamp.h"
#include "test.h"

int main(void) {
  CHECK_EQ_INT(clamp_int(-5, 0, 10), 0);   /* below */
  CHECK_EQ_INT(clamp_int(15, 0, 10), 10);  /* above */
  CHECK_EQ_INT(clamp_int(5, 0, 10), 5);    /* inside */
  CHECK_EQ_INT(clamp_int(0, 0, 10), 0);    /* equal to lo */
  CHECK_EQ_INT(clamp_int(10, 0, 10), 10);  /* equal to hi */
  CHECK_EQ_INT(clamp_int(3, 5, 5), 5);     /* lo == hi */

  TEST_MAIN_END();
}

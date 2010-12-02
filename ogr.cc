
#include "php.h"
#include <ogrsf_frmts.h>

PHP_FUNCTION(ogrregisterall)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }
  OGRRegisterAll();
}

// not present in C++ API
// PHP_FUNCTION(ogrcleanupall)
// {
//   if (ZEND_NUM_ARGS() != 0) {
//     WRONG_PARAM_COUNT;
//   }
//   OGRCleanupAll();
// }


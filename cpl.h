
#ifndef CPL_H_
#define CPL_H_

#include "php.h"

PHP_FUNCTION(cplerrorreset);
PHP_FUNCTION(cplgetlasterrorno);
PHP_FUNCTION(cplgetlasterrortype);
PHP_FUNCTION(cplgetlasterrormsg);
PHP_FUNCTION(cplgetconfigoption);
PHP_FUNCTION(cplsetconfigoption);

void php_gdal_cpl_startup(INIT_FUNC_ARGS);

#endif // CPL_H_

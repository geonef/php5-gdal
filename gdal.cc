#include "php_gdal.h"

PHP_MINIT_FUNCTION(gdal)
{
  return SUCCESS;
}

zend_module_entry gdal_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
  STANDARD_MODULE_HEADER,
#endif
  PHP_GDAL_EXTNAME,
  NULL,                  /* Functions */
  PHP_MINIT(gdal),
  NULL,                  /* MSHUTDOWN */
  NULL,                  /* RINIT */
  NULL,                  /* RSHUTDOWN */
  NULL,                  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
  PHP_GDAL_EXTVER,
#endif
  STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_VEHICLES
extern "C" {
  ZEND_GET_MODULE(gdal)
}
#endif

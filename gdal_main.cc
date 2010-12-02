
#include "php.h"
#include <gdal.h>
#include "gdal_main.h"

#ifndef GDAL_H_INCLUDED
#error "grrr"
#endif

/**
 *
 * "VERSION_NUM", "RELEASE_DATE", "RELEASE_NAME", "--version", "LICENCE"
 */
PHP_FUNCTION(gdalversioninfo)
{
  char *request;
  int request_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &request, &request_len) == FAILURE) {
    WRONG_PARAM_COUNT;
  }
  const char *info = GDALVersionInfo(request);
  if (info) {
    RETURN_STRING((char*)info, 1);
  } else {
    RETURN_NULL();
  }
}



#include "php.h"
#include "ext/standard/info.h"
//#include "cpl_error.h"

#include "php_gdal.h"
#include "cpl.h"
#include "ogr.h"
#include "ogrsfdriverregistrar.h"
#include "ogrlayer.h"

PHP_MINIT_FUNCTION(gdal)
{
  //printf("Minit!\n");
  gdal_cpl_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrsfdriverregistrar_startup(INIT_FUNC_ARGS_PASSTHRU);
  gdal_ogrlayer_startup(INIT_FUNC_ARGS_PASSTHRU);
  // REGISTER_LONG_CONSTANT("CPLE_None", CPLE_None,
  //                        CONST_CS | CONST_PERSISTENT);
  // REGISTER_LONG_CONSTANT("CPLE_AppDefined", CPLE_AppDefined,
  //                        CONST_CS | CONST_PERSISTENT);
  // REGISTER_LONG_CONSTANT("CPLE_OutOfMemory", CPLE_OutOfMemory,
  //                        CONST_CS | CONST_PERSISTENT);
  // REGISTER_LONG_CONSTANT("CPLE_FileIO", CPLE_FileIO,
  //                        CONST_CS | CONST_PERSISTENT);
  // REGISTER_LONG_CONSTANT("CPLE_OpenFailed", CPLE_OpenFailed,
  //                        CONST_CS | CONST_PERSISTENT);
  // REGISTER_LONG_CONSTANT("CPLE_IllegalArg", CPLE_IllegalArg,
  //                        CONST_CS | CONST_PERSISTENT);
  // REGISTER_LONG_CONSTANT("CPLE_NotSupported", CPLE_NotSupported,
  //                        CONST_CS | CONST_PERSISTENT);
  // REGISTER_LONG_CONSTANT("CPLE_AssertionFailed", CPLE_AssertionFailed,
  //                        CONST_CS | CONST_PERSISTENT);
  // REGISTER_LONG_CONSTANT("CPLE_NoWriteAccess", CPLE_NoWriteAccess,
  //                        CONST_CS | CONST_PERSISTENT);
  // REGISTER_LONG_CONSTANT("CPLE_UserInterrupt", CPLE_UserInterrupt,
  //                        CONST_CS | CONST_PERSISTENT);
  // REGISTER_LONG_CONSTANT("CPLE_ObjectNull", CPLE_ObjectNull,
  //                        CONST_CS | CONST_PERSISTENT);
  return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(gdal)
{
  //printf("Mshutdown!\n");
  /* uncomment this line if you have INI entries
     UNREGISTER_INI_ENTRIES();
  */
  return SUCCESS;
}

/* Remove if there's nothing to do at request start */
PHP_RINIT_FUNCTION(gdal)
{
  //printf("Rinit!\n");
  return SUCCESS;
}

/* Remove if there's nothing to do at request end */
PHP_RSHUTDOWN_FUNCTION(gdal)
{
  //printf("Rshutdown!\n");
  return SUCCESS;
}

PHP_MINFO_FUNCTION(gdal)
{
  php_info_print_table_start();
  php_info_print_table_header(2, "OGR support", "enabled");
  php_info_print_table_end();

  /* Remove comments if you have entries in php.ini
     DISPLAY_INI_ENTRIES();
  */
}

static function_entry gdal_functions[] = {
  PHP_FE(gdal_test, NULL)
  PHP_FE(cplerrorreset, NULL)
  PHP_FE(cplgetlasterrorno, NULL)
  PHP_FE(cplgetlasterrortype, NULL)
  PHP_FE(cplgetlasterrormsg, NULL)
  PHP_FE(cplgetconfigoption, NULL)
  PHP_FE(cplsetconfigoption, NULL)
  PHP_FE(ogrregisterall, NULL)
  //PHP_FE(ogrcleanupall, NULL)
  {NULL, NULL, NULL}
};

zend_module_entry gdal_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
  STANDARD_MODULE_HEADER,
#endif
  PHP_GDAL_EXTNAME,
  gdal_functions,        /* Functions */
  PHP_MINIT(gdal),
  PHP_MSHUTDOWN(gdal),   /* MSHUTDOWN */
  PHP_RINIT(gdal),       /* RINIT */
  PHP_RSHUTDOWN(gdal),   /* RSHUTDOWN */
  PHP_MINFO(gdal),       /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
  PHP_GDAL_EXTVER,
#endif
  STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_GDAL
extern "C" {
  ZEND_GET_MODULE(gdal)
}
#endif

PHP_FUNCTION(gdal_test)
{
  RETURN_STRING("Test GDAL :)", 1);
}

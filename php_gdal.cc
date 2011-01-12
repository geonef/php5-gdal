
#include <gdal.h>
#include <gdal_priv.h>
#include <ogrsf_frmts.h>
#include "php.h"
#include "ext/standard/info.h"
//#include "cpl_error.h"

#include "php_gdal.h"
#include "cpl.h"
#include "gdal.h"
#include "ogr.h"
#include "ogrsfdriverregistrar.h"
#include "ogrsfdriver.h"
#include "ogrdatasource.h"
#include "ogrlayer.h"
#include "ogrfeaturedefn.h"
#include "ogrfielddefn.h"
#include "ogrenvelope.h"
#include "ogrspatialreference.h"
#include "ogrfeature.h"

PHP_MINIT_FUNCTION(gdal)
{
  php_gdal_cpl_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrsfdriverregistrar_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrsfdriver_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrdatasource_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrlayer_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrfeaturedefn_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrfielddefn_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrenvelope_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrspatialreference_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrfeature_startup(INIT_FUNC_ARGS_PASSTHRU);

# define OGR_CONST_FLAG CONST_CS | CONST_PERSISTENT
  // REGISTER_LONG_CONSTANT("CPLE_None", CPLE_None, OGR_CONST_FLAG);
  // REGISTER_LONG_CONSTANT("CPLE_AppDefined", CPLE_AppDefined, OGR_CONST_FLAG);
  // REGISTER_LONG_CONSTANT("CPLE_OutOfMemory", CPLE_OutOfMemory, OGR_CONST_FLAG);
  // REGISTER_LONG_CONSTANT("CPLE_FileIO", CPLE_FileIO, OGR_CONST_FLAG);
  // REGISTER_LONG_CONSTANT("CPLE_OpenFailed", CPLE_OpenFailed, OGR_CONST_FLAG);
  // REGISTER_LONG_CONSTANT("CPLE_IllegalArg", CPLE_IllegalArg, OGR_CONST_FLAG);
  // REGISTER_LONG_CONSTANT("CPLE_NotSupported", CPLE_NotSupported, OGR_CONST_FLAG);
  // REGISTER_LONG_CONSTANT("CPLE_AssertionFailed", CPLE_AssertionFailed, OGR_CONST_FLAG);
  // REGISTER_LONG_CONSTANT("CPLE_NoWriteAccess", CPLE_NoWriteAccess, OGR_CONST_FLAG);
  // REGISTER_LONG_CONSTANT("CPLE_UserInterrupt", CPLE_UserInterrupt, OGR_CONST_FLAG);
  // REGISTER_LONG_CONSTANT("CPLE_ObjectNull", CPLE_ObjectNull, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OGRERR_NONE", OGRERR_NONE, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OGRERR_NOT_ENOUGH_DATA", OGRERR_NOT_ENOUGH_DATA, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OGRERR_NOT_ENOUGH_MEMORY", OGRERR_NOT_ENOUGH_MEMORY, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OGRERR_UNSUPPORTED_GEOMETRY_TYPE", OGRERR_UNSUPPORTED_GEOMETRY_TYPE, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OGRERR_UNSUPPORTED_OPERATION", OGRERR_UNSUPPORTED_OPERATION, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OGRERR_CORRUPT_DATA", OGRERR_CORRUPT_DATA, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OGRERR_FAILURE", OGRERR_FAILURE, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OGRERR_UNSUPPORTED_SRS", OGRERR_UNSUPPORTED_SRS, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkb25DBit", wkb25DBit, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("ogrZMarker", ogrZMarker, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbXDR", wkbXDR, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbNDR", wkbNDR, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbUnknown", wkbUnknown, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbPoint", wkbPoint, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbLineString", wkbLineString, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbPolygon", wkbPolygon, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbMultiPoint", wkbMultiPoint, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbMultiLineString", wkbMultiLineString, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbMultiPolygon", wkbMultiPolygon, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbGeometryCollection", wkbGeometryCollection, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbNone", wkbNone, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbLinearRing", wkbLinearRing, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbPoint25D", wkbPoint25D, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbLineString25D", wkbLineString25D, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbPolygon25D", wkbPolygon25D, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbMultiPoint25D", wkbMultiPoint25D, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbMultiLineString25D", wkbMultiLineString25D, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbMultiPolygon25D", wkbMultiPolygon25D, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("wkbGeometryCollection25D", wkbGeometryCollection25D, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTInteger", OFTInteger, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTIntegerList", OFTIntegerList, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTReal", OFTReal, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTRealList", OFTRealList, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTString", OFTString, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTStringList", OFTStringList, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTWideString", OFTWideString, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTWideStringList", OFTWideStringList, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTBinary", OFTBinary, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTDate", OFTDate, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTTime", OFTTime, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OFTDateTime", OFTDateTime, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OJUndefined", OJUndefined, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OJLeft", OJLeft, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OJRight", OJRight, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OGRNullFID", OGRNullFID, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("OGRUnsetMarker", OGRUnsetMarker, OGR_CONST_FLAG);
  REGISTER_STRING_CONSTANT("OLCRandomRead", (char*)OLCRandomRead, OGR_CONST_FLAG);
  REGISTER_STRING_CONSTANT("OLCSequentialWrite", (char*)OLCSequentialWrite, OGR_CONST_FLAG);
  REGISTER_STRING_CONSTANT("OLCRandomWrite", (char*)OLCRandomWrite, OGR_CONST_FLAG);
  REGISTER_STRING_CONSTANT("OLCFastSpatialFilter", (char*)OLCFastSpatialFilter, OGR_CONST_FLAG);
  REGISTER_STRING_CONSTANT("OLCFastFeatureCount", (char*)OLCFastFeatureCount, OGR_CONST_FLAG);
  REGISTER_STRING_CONSTANT("OLCFastGetExtent", (char*)OLCFastGetExtent, OGR_CONST_FLAG);
  REGISTER_STRING_CONSTANT("OLCCreateField", (char*)OLCCreateField, OGR_CONST_FLAG);
  REGISTER_STRING_CONSTANT("OLCTransactions", (char*)OLCTransactions, OGR_CONST_FLAG);
  REGISTER_STRING_CONSTANT("ODsCCreateLayer", (char*)ODsCCreateLayer, OGR_CONST_FLAG);
  REGISTER_STRING_CONSTANT("ODrCCreateDataSource", (char*)ODrCCreateDataSource, OGR_CONST_FLAG);
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
  char *str = NULL;
  const char *name;
  const char *sep = " ";
  OGRSFDriverRegistrar *registrar;
  OGRSFDriver *ogrDriver;
  GDALDriverManager *manager;
  GDALDriver *gdalDriver;
  int i;

  php_info_print_table_start();
  php_info_print_table_row(2, "GDAL support", "enabled");
  php_info_print_table_row(2, "OGR support", "enabled");
  php_info_print_table_row(2, "GDAL version", GDALVersionInfo("--version"));
  // list GDAL drivers
  GDALAllRegister();
  manager = GetGDALDriverManager();
  for (i = 0; i < manager->GetDriverCount(); ++i) {
    gdalDriver = manager->GetDriver(i);
    name = gdalDriver->GetDescription();
    if (str) {
      str = (char*)erealloc(str, strlen(str) + strlen(sep) + strlen(name) + 1);
      strcpy(str + strlen(str), sep);
      strcpy(str + strlen(str), name);
    } else {
      str = estrdup(name);
    }
  }
  php_info_print_table_row(2, "GDAL drivers", str);
  efree(str);
  str = NULL;
  // list OGR drivers
  OGRRegisterAll();
  registrar = OGRSFDriverRegistrar::GetRegistrar();
  for (i = 0; i < registrar->GetDriverCount(); ++i) {
    ogrDriver = registrar->GetDriver(i);
    name = ogrDriver->GetName();
    if (str) {
      str = (char*)erealloc(str, strlen(str) + strlen(sep) + strlen(name) + 1);
      strcpy(str + strlen(str), sep);
      strcpy(str + strlen(str), name);
    } else {
      str = estrdup(name);
    }
  }
  php_info_print_table_row(2, "OGR drivers", str);
  efree(str);
  str = NULL;
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
  (char*)PHP_GDAL_EXTNAME,
  gdal_functions,        /* Functions */
  PHP_MINIT(gdal),
  PHP_MSHUTDOWN(gdal),   /* MSHUTDOWN */
  PHP_RINIT(gdal),       /* RINIT */
  PHP_RSHUTDOWN(gdal),   /* RSHUTDOWN */
  PHP_MINFO(gdal),       /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
  (char*)PHP_GDAL_EXTVER,
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

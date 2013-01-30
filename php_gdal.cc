//
// php_gdal.cc
//
//
// Copyright (c) 2011, JF Gigand
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

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
#include "ogr_api.h"
#include "gdal_main.h"
#include "gdaldrivermanager.h"
#include "gdaldriver.h"
#include "gdaldataset.h"
#include "ogrsfdriverregistrar.h"
#include "ogrsfdriver.h"
#include "ogrdatasource.h"
#include "ogrlayer.h"
#include "ogrfeaturedefn.h"
#include "ogrfielddefn.h"
#include "ogrenvelope.h"
#include "ogrgeometry.h"
#include "ogrspatialreference.h"
#include "ogrcoordtransform.h"
#include "ogrfeature.h"
#include "ogrexception.h"

static void errorHandler(CPLErr eErrClass, int err_no, const char *msg)
{
  // types are CPLE_* constants, classes are CPLErr values, both from cpl_error.h
  char const * const types[] = { "None", "AppDefined", "OutOfMemory", "FileIO",
                                 "OpenFailed", "IllegalArg", "NotSupported",
                                 "AssertionFailed", "NoWriteAccess",
                                 "UserInterrupt", "ObjectNull", "UNKNOWN" };
  char const * const classes[] = { "NONE", "DEBUG", "WARNING", "FAILURE", "FATAL", "UNKNOWN" };
  if (err_no > 11) {
    err_no = 11;
  }
  if (eErrClass > 5) {
    eErrClass = (CPLErr) 5;
  }
  //
  // char str[1000];
  // sprintf(str, "GDAL/OGR %s: [%s] %s", classes[eErrClass], types[err_no], msg);
  // php_log_err(str);
}

static PHP_INI_MH(onIniChangeGdalData)
{
  // php_log_err((char *) "gdal data change");
  // php_log_err(new_value);
  ////
  // char msg[500];
  // sprintf(msg, "php5-gdal: onIniChangeGdalData: stage=%d [%d, %d, %d]",
  //         stage, PHP_INI_STAGE_STARTUP, PHP_INI_STAGE_SHUTDOWN, PHP_INI_STAGE_RUNTIME);
  // php_log_err(msg);
  // sprintf(msg, "php5-gdal: onIniChangeGdalData: %s", new_value);
  // php_log_err(msg);
  ////
  char const *dataPath = new_value;
  //if (dataPath && dataPath[0]) {
  // CPLSetConfigOption will copy the value string
  CPLSetConfigOption("GDAL_DATA", dataPath && dataPath[0] ? dataPath : NULL);
  //}

  return (SUCCESS);
}

static PHP_INI_MH(onIniChangeErrorHandler)
{
  // char msg[500];
  // sprintf(msg, "php5-gdal: onIniChangeErrorHandler: stage=%d [%d, %d, %d]",
  //         stage, PHP_INI_STAGE_STARTUP, PHP_INI_STAGE_SHUTDOWN, PHP_INI_STAGE_RUNTIME);
  // php_log_err(msg);
  // sprintf(msg, "php5-gdal: onIniChangeErrorHandler: %s", new_value);
  // php_log_err(msg);

  CPLSetErrorHandler(atoi(new_value) ? errorHandler : NULL);

  return (SUCCESS);
}

static PHP_INI_MH(onIniChangeCplDebug)
{
  ////
  // char msg[500];
  // sprintf(msg, "php5-gdal: onIniChangeCplDebug: stage=%d [%d, %d, %d]",
  //         stage, PHP_INI_STAGE_STARTUP, PHP_INI_STAGE_SHUTDOWN, PHP_INI_STAGE_RUNTIME);
  // php_log_err(msg);
  // sprintf(msg, "php5-gdal: onIniChangeCplDebug: %s", new_value);
  // php_log_err(msg);
  ////
  // CPLSetConfigOption will copy the value string
  CPLSetConfigOption("CPL_DEBUG", atoi(new_value) ? "ON" : "OFF");

  return (SUCCESS);
}

// about INI: http://docstore.mik.ua/orelly/webprog/php/ch14_12.htm
// GDAL options: http://trac.osgeo.org/gdal/wiki/ConfigOptions

PHP_INI_BEGIN()
PHP_INI_ENTRY((char*)"gdal.gdal_data", (char*)"", PHP_INI_ALL, onIniChangeGdalData)
PHP_INI_ENTRY((char*)"gdal.cpl_debug", (char*)"Off", PHP_INI_ALL, onIniChangeCplDebug)
PHP_INI_ENTRY((char*)"gdal.enable_error_handler", (char*)"On", PHP_INI_ALL, onIniChangeErrorHandler)
PHP_INI_END()

#define PHP_GDAL_OGRDATASOURCE_PTR_DTOR (void (*)(void *))php_gdal_ogrdatasource_ptr_dtor


//////////////////////////////////////////////////////////////////////
// GLOBALS

ZEND_DECLARE_MODULE_GLOBALS(gdal)

static void php_gdal_globals_ctor(zend_gdal_globals *gdal_globals)
{
  //gdal_globals->some_integer = 0;
  //gdal_globals->some_string = NULL;
  //int zend_hash_init_ex ( HashTable* ht, uint nSize, hash_func_t pHashFunction, dtor_func_t pDestructor, zend_bool persistent, zend_bool bApplyProtection )
  //php_log_err("php5-gdal: globals_ctor");
  zend_hash_init_ex(&gdal_globals->ogrDataSources, 0, NULL, PHP_GDAL_OGRDATASOURCE_PTR_DTOR, 1 /* 1 */, 0);
}

static void php_gdal_globals_dtor(zend_gdal_globals *gdal_globals)
{
  //php_log_err("php5-gdal: globals_dtor");
  zend_hash_destroy(&gdal_globals->ogrDataSources);
}


//////////////////////////////////////////////////////////////////////
// MODULE WORKFLOW

PHP_MINIT_FUNCTION(gdal)
{
  ZEND_INIT_MODULE_GLOBALS(gdal, php_gdal_globals_ctor,
                           php_gdal_globals_dtor);

  php_gdal_cpl_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_gdaldrivermanager_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_gdaldriver_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_gdaldataset_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrsfdriverregistrar_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrsfdriver_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrdatasource_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrlayer_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrfeaturedefn_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrfielddefn_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrenvelope_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrgeometry_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrspatialreference_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrcoordtransform_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrfeature_startup(INIT_FUNC_ARGS_PASSTHRU);
  php_gdal_ogrexception_startup(INIT_FUNC_ARGS_PASSTHRU);

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
  REGISTER_LONG_CONSTANT("GA_ReadOnly", GA_ReadOnly, OGR_CONST_FLAG);
  REGISTER_LONG_CONSTANT("GA_Update", GA_Update, OGR_CONST_FLAG);

  REGISTER_INI_ENTRIES();


  // char const *dataPath = INI_STR((char *) "gdal.gdal_data");
  // if (dataPath && dataPath[0]) {
  //   CPLSetConfigOption("GDAL_DATA", dataPath);
  // }
  return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(gdal)
{
  //printf("Mshutdown!\n");
  /* uncomment this line if you have INI entries
     UNREGISTER_INI_ENTRIES();
  */
  CPLSetErrorHandler(NULL);
  UNREGISTER_INI_ENTRIES();
  //zend_hash_destroy(&GDAL_G(ogrDataSources));
  return SUCCESS;
}

/* Remove if there's nothing to do at request start */
PHP_RINIT_FUNCTION(gdal)
{
  //CPLError(CE_Warning, CPLE_AppDefined, "Test!!");
  return SUCCESS;
}

/* Remove if there's nothing to do at request end */
PHP_RSHUTDOWN_FUNCTION(gdal)
{
  // //--DEBUG START
  // char *msg;
  // int i, i1, i2, c;
  // OGRSFDriverRegistrar *registrar = OGRSFDriverRegistrar::GetRegistrar();
  // OGRDataSource *ds;
  // c = registrar->GetOpenDSCount();
  // asprintf(&msg, "RSHUTDOWN openDS count=%d", c);
  // php_log_err(msg);
  // free(msg);
  // for (i = 0; i < c; ++i) {
  //   ds = registrar->GetOpenDS(i);
  //   i1 = ds->GetRefCount();
  //   asprintf(&msg, "RSHUTDOWN openDS[%d]: refC=%d", i, i1);
  //   php_log_err(msg);
  //   free(msg);
  // }
  // //--DEBUG END
  ////
  //char *msg;
  ////
  //try//OGRCleanupAll(); -> makes segfault when MapScript is used as well
  ////
  // asprintf(&msg, "php5-gdal: after OGRCleanupAll");
  // php_log_err(msg);
  // free(msg);
  ////
  //try//zend_hash_clean(&GDAL_G(ogrDataSources));
  //ogrdatasource_destroy_all();
  ////
  // asprintf(&msg, "php5-gdal: after zend_hash_clean");
  // php_log_err(msg);
  // free(msg);
  ////
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

  DISPLAY_INI_ENTRIES();
}

static function_entry gdal_functions[] = {
  PHP_FE(cplerrorreset, NULL)
  PHP_FE(cplgetlasterrorno, NULL)
  PHP_FE(cplgetlasterrortype, NULL)
  PHP_FE(cplgetlasterrormsg, NULL)
  PHP_FE(cplgetconfigoption, NULL)
  PHP_FE(cplsetconfigoption, NULL)
  PHP_FE(ogrregisterall, NULL)
  //PHP_FE(ogrcleanupall, NULL)
  PHP_FE(gdalversioninfo, NULL)
  PHP_FE(gdalopen, NULL)
  PHP_FE(gdalopenshared, NULL)
  PHP_FE(getgdaldrivermanager, NULL)
  PHP_FE(gdalallregister, NULL)
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

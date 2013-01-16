#include "php.h"
#include "php_gdal.h"
#include <ogr_core.h>
#include <cpl_conv.h>
#include <cpl_string.h>
#include "ogrexception.h"
#include "ogrgeometry.h"

zend_class_entry *gdal_ogrgeometry_ce;
zend_object_handlers ogrgeometry_object_handlers;


void ogrgeometry_free_storage(void *object TSRMLS_DC)
{
  php_ogrgeometry_object *obj = (php_ogrgeometry_object *)object;
  //delete obj->geometry; // TODO
  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}


zend_object_value ogrgeometry_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_ogrgeometry_object *obj =
    (php_ogrgeometry_object *)emalloc(sizeof(php_ogrgeometry_object));
  memset(obj, 0, sizeof(php_ogrgeometry_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

  retval.handle = zend_objects_store_put(obj, NULL,
                                         ogrgeometry_free_storage, NULL TSRMLS_CC);
  retval.handlers = &ogrgeometry_object_handlers;

  return retval;
}


PHP_METHOD(OGRGeometry, IsValid)
{
  OGRGeometry *geometry;
  php_ogrgeometry_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrgeometry_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
  geometry = obj->geometry;
  RETURN_BOOL(geometry->IsValid());
}


PHP_METHOD(OGRGeometry, ExportToWkt)
{
  OGRGeometry *geometry;
  php_ogrgeometry_object *obj;
  char *ppszDstText;
  char *ret;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrgeometry_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
  geometry = obj->geometry;

  if (geometry->exportToWkt(&ppszDstText) != OGRERR_NONE) {
    php_gdal_ogr_throw("Failed to convert geometry to WKT");
    RETURN_EMPTY_STRING();
  }

  ret = estrdup(ppszDstText);
  OGRFree(ppszDstText);
  RETURN_STRING(ret, 0);
}


PHP_METHOD(OGRGeometry, ExportToWkb)
{
  OGRGeometry *geometry;
  php_ogrgeometry_object *obj;
  int wkbSize;
  char * buffer;
  int wkbByteOrder = wkbNDR; // use as default when no byteOrder is specified

  if (ZEND_NUM_ARGS() != 0) {
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
          &wkbByteOrder) == FAILURE) {
      php_gdal_ogr_throw("Illegal value for byteOrder");
      RETURN_EMPTY_STRING();
    }
  }

  if ((wkbByteOrder) != wkbNDR && (wkbByteOrder != wkbXDR)) {
      php_gdal_ogr_throw("Illegal value for byteOrder. Has to be either wkbNDR or wkbXDR");
      RETURN_EMPTY_STRING();
  }

  obj = (php_ogrgeometry_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
  geometry = obj->geometry;

  wkbSize = geometry->WkbSize();
  buffer = (char*)ecalloc(sizeof(char), wkbSize+1);

  if (geometry->exportToWkb(wkbNDR, (unsigned char*)buffer) != OGRERR_NONE) {
    php_gdal_ogr_throw("Failed to convert geometry to WKB");
    RETURN_EMPTY_STRING();
  }

  RETURN_STRINGL(buffer, wkbSize+1, 0);
}


PHP_METHOD(OGRGeometry, ExportToJson)
{
  OGRGeometry *geometry;
  php_ogrgeometry_object *obj;
  char *jsonText;
  char *ret;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrgeometry_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
  geometry = obj->geometry;

  jsonText = geometry->exportToJson();
  if (jsonText == NULL) {
    php_gdal_ogr_throw("Failed to convert geometry to JSON");
    RETURN_EMPTY_STRING();
  }

  ret = estrdup(jsonText);
  CPLFree(jsonText);
  RETURN_STRING(ret, 0);
}


PHP_METHOD(OGRGeometry, ExportToKML)
{
  OGRGeometry *geometry;
  php_ogrgeometry_object *obj;
  char *kmlText;
  char *ret;

  if (ZEND_NUM_ARGS() != 0) {
    return;
  }

  obj = (php_ogrgeometry_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
  geometry = obj->geometry;

  kmlText = geometry->exportToKML();
  if (kmlText == NULL) {
    php_gdal_ogr_throw("Failed to convert geometry to KML");
    RETURN_EMPTY_STRING();
  }

  ret = estrdup(kmlText);
  CPLFree(kmlText);
  RETURN_STRING(ret, 0);
}


PHP_METHOD(OGRGeometry, ExportToGML)
{
  OGRGeometry *geometry;
  php_ogrgeometry_object *obj;
  char *gmlText;
  char *ret;
  char *gmlOptions = NULL;
  int gmlOptionsLen;
  char **papszOptions = NULL;

  if (ZEND_NUM_ARGS() != 0) {
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &gmlOptions, &gmlOptionsLen) == FAILURE) {
      php_gdal_ogr_throw("Illegal value for GML format options");
      RETURN_EMPTY_STRING();
    }
  }

  obj = (php_ogrgeometry_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
  geometry = obj->geometry;

  if (gmlOptionsLen != 0) {
    papszOptions = CSLAddString(papszOptions, gmlOptions);
  } 

  gmlText = geometry->exportToGML(papszOptions);
  if (papszOptions) {
    CSLDestroy(papszOptions);
  }
  if (gmlText == NULL) {
    php_gdal_ogr_throw("Failed to convert geometry to GML");
    RETURN_EMPTY_STRING();
  }

  ret = estrdup(gmlText);
  CPLFree(gmlText);
  RETURN_STRING(ret, 0);
}






function_entry ogrgeometry_methods[] = {
  PHP_ME(OGRGeometry, IsValid, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRGeometry, ExportToWkt, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRGeometry, ExportToWkb, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRGeometry, ExportToJson, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRGeometry, ExportToKML, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(OGRGeometry, ExportToGML, NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};


void php_gdal_ogrgeometry_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "OGRGeometry", ogrgeometry_methods);
  gdal_ogrgeometry_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_ogrgeometry_ce->create_object = ogrgeometry_create_handler;
  memcpy(&ogrgeometry_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  ogrgeometry_object_handlers.clone_obj = NULL;
}

/* VIM settings */
/* ex: set tabstop=2 expandtab shiftwidth=2 smartindent */

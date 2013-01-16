
#include "php.h"
#include "php_gdal.h"
#include <ogr_core.h>
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


function_entry ogrgeometry_methods[] = {
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

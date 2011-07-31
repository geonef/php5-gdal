//
// php_gdal.h
//
//
// Copyright (c) 2011, Geonef SAS
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef PHP_GDAL_H
#define PHP_GDAL_H

#define PHP_GDAL_EXTNAME  "gdal"
#define PHP_GDAL_EXTVER   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

PHP_FUNCTION(gdal_test);

extern zend_module_entry gdal_module_entry;
#define phpext_gdal_ptr &gdal_module_entry;

ZEND_BEGIN_MODULE_GLOBALS(gdal)
  HashTable ogrDataSources;
ZEND_END_MODULE_GLOBALS(gdal)

ZEND_EXTERN_MODULE_GLOBALS(gdal)

#ifdef ZTS
#define GDAL_G(v) TSRMG(gdal_globals_id, zend_gdal_globals *, v)
#else
#define GDAL_G(v) (gdal_globals.v)
#endif

#endif /* PHP_GDAL_H */

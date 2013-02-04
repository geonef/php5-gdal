//
// php_gdal.h
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
#ifndef PHP_GDAL_H
#define PHP_GDAL_H

#define PHP_GDAL_EXTNAME  "gdal"
#define PHP_GDAL_EXTVER   "0.3"

// these two macros convert macro values to strings
#define STRINGIFY2(x)   #x
#define STRINGIFY(x)    STRINGIFY2(x)

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

//
// cpl.cc
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

#include "php.h"
#include "cpl_error.h"
#include "cpl_conv.h"
#include "cpl_string.h"
#include "cpl_port.h"
#include "cpl.h"

PHP_FUNCTION(cplerrorreset)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  php_log_err("php5-gdal: CPLErrorReset");
  CPLErrorReset();
}


PHP_FUNCTION(cplgetlasterrorno)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  php_log_err("php5-gdal: CPLGetLastErrorNo");
  RETURN_LONG(CPLGetLastErrorNo());
}


PHP_FUNCTION(cplgetlasterrortype)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }
  php_log_err("php5-gdal: CPLGetLastErrorType");
  RETURN_LONG(CPLGetLastErrorType());
}


PHP_FUNCTION(cplgetlasterrormsg)
{
  const char *pszMsg;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  php_log_err("php5-gdal: CPLGetLastErrorMsg");
  if ((pszMsg = CPLGetLastErrorMsg()) != NULL) {
    RETURN_STRING((char *)pszMsg, 1);
  } else {
    RETURN_NULL();
  }
}


PHP_FUNCTION(cplsetconfigoption)
{
  char *key = NULL;
  char *value = NULL;
  int key_len;
  int value_len;
  int argc = ZEND_NUM_ARGS();

  if (zend_parse_parameters(argc TSRMLS_CC, (char*)"ss", &key, &key_len,
                            &value, &value_len) == FAILURE)
    return;

  php_log_err("php5-gdal: CPLSetConfigOption");
  CPLSetConfigOption(key, value);
}


PHP_FUNCTION(cplgetconfigoption)
{
  char *key = NULL;
  char *_default = NULL;
  int key_len;
  int _default_len;
  const char *value;
  int argc = ZEND_NUM_ARGS();

  if (zend_parse_parameters(argc TSRMLS_CC, (char*)"ss", &key, &key_len,
                            &_default, &_default_len) == FAILURE)
    return;

  php_log_err("php5-gdal: CPLGetConfigOption");
  value = CPLGetConfigOption(key, _default);
  if (value) {
    RETURN_STRING((char *)value, 1);
  } else {
    RETURN_NULL();
  }
}

# define CPLE_CONST_FLAG CONST_CS | CONST_PERSISTENT
void php_gdal_cpl_startup(INIT_FUNC_ARGS)
{
  REGISTER_LONG_CONSTANT("CPLE_None", CPLE_None, CPLE_CONST_FLAG);
  REGISTER_LONG_CONSTANT("CPLE_AppDefined", CPLE_AppDefined, CPLE_CONST_FLAG);
  REGISTER_LONG_CONSTANT("CPLE_OutOfMemory", CPLE_OutOfMemory, CPLE_CONST_FLAG);
  REGISTER_LONG_CONSTANT("CPLE_FileIO", CPLE_FileIO, CPLE_CONST_FLAG);
  REGISTER_LONG_CONSTANT("CPLE_OpenFailed", CPLE_OpenFailed, CPLE_CONST_FLAG);
  REGISTER_LONG_CONSTANT("CPLE_IllegalArg", CPLE_IllegalArg, CPLE_CONST_FLAG);
  REGISTER_LONG_CONSTANT("CPLE_NotSupported", CPLE_NotSupported, CPLE_CONST_FLAG);
  REGISTER_LONG_CONSTANT("CPLE_AssertionFailed", CPLE_AssertionFailed, CPLE_CONST_FLAG);
  REGISTER_LONG_CONSTANT("CPLE_NoWriteAccess", CPLE_NoWriteAccess, CPLE_CONST_FLAG);
  REGISTER_LONG_CONSTANT("CPLE_UserInterrupt", CPLE_UserInterrupt, CPLE_CONST_FLAG);
  REGISTER_LONG_CONSTANT("CPLE_ObjectNull", CPLE_ObjectNull, CPLE_CONST_FLAG);
}

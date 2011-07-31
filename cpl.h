//
// cpl.h
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

#ifndef CPL_H_
#define CPL_H_

#include "php.h"

PHP_FUNCTION(cplerrorreset);
PHP_FUNCTION(cplgetlasterrorno);
PHP_FUNCTION(cplgetlasterrortype);
PHP_FUNCTION(cplgetlasterrormsg);
PHP_FUNCTION(cplgetconfigoption);
PHP_FUNCTION(cplsetconfigoption);

void php_gdal_cpl_startup(INIT_FUNC_ARGS);

#endif // CPL_H_

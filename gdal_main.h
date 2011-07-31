//
// gdal_main.h
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


#ifndef PHP_GDAL_MAIN_H_
#define PHP_GDAL_MAIN_H_

PHP_FUNCTION(gdalversioninfo);
PHP_FUNCTION(gdalopen);
PHP_FUNCTION(gdalopenshared);
PHP_FUNCTION(getgdaldrivermanager);
PHP_FUNCTION(gdalallregister);

#endif // PHP_GDAL_MAIN_H_

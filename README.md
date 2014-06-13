
PACKAGE
-------

php5-gdal is a PHP 5 module that provides PHP bindings to the GDAL/OGR library.
It was written and is maintained by the Geonef company, http://www.geonef.fr/.

This package is released under an X11/MIT style Open Source license. See LICENSE.txt file.

The build requires GDAL 1.7 or above.


This module is INCOMPLETE : roughly only half of the OGR library is supported
and very few classes of GDAL. Adding bindings is a bit repetitive, not so
difficult, so feel free to add the ones you need.

This module is UNSTABLE. Don't use it on production environment as it is.


BUILD INSTRUCTIONS
------------------

On a Debian GNU/Linux:
```
# apt-get install g++ php5-dev libgdal1-dev
# git clone https://github.com/geonef/php5-gdal.git && cd php5-gdal
# phpize
```

For other distributions/OS, please adapt the instruction (the "apt-get install", mainly), feel free to share them through a GitHub pull request or email to to maintainer jf@geonef.fr. Thanks!


COMMUNITY
---------

The source is hosted on GitHub : https://github.com/geonef/php5-gdal
It was added recently and basic documentation is yet to be written.

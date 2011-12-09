<?php
//
// In this example, GDAL is used to get basic information about a particular
// raster dataset given in argument.
//
// Note: this is a minimal example which has nothing to do
// with the 'gdalinfo' program.
//

function showInfo(GDALDataSet $dataset)
{
  echo "driver: ".$dataset->GetDriver()->GetDescription()."\n";
  echo "width: ".$dataset->getRasterXSize()."\n";
  echo "height: ".$dataset->getRasterYSize()."\n";
  echo "number of bands: ".$dataset->GetRasterCount()."\n";
  echo "projection: ".$dataset->GetProjectionRef()."\n";
}


$path =& $_SERVER['argv'][1];
if (!isset($path)) {
  throw new Exception("Missing argument: path to raster dataset");
}

GDALAllRegister();

$dataset = GDALOpen($path, GA_ReadOnly);
if (!$dataset) {
  throw new Exception("Could not open raster dataset: ".$path);
}

header("Content-type: text/plain");
showInfo($dataset);

<?php
//
// In this example, OGR is used to get basic information about a particular
// vector datasource given in argument.
//
// Note: this is a minimal example which has nothing to do
// with the 'ogrinfo' program.
//

function showInfo(OGRDataSource $dataset)
{
  echo "Name: ".$dataset->GetName()."\n";
  $count = $dataset->GetLayerCount();

  echo "Layer count: ".$count."\n";

  for ($i = 0; $i < $count; $i++) {
    echo "\n= LAYER ".$i." =\n";
    showLayerInfo($dataset->GetLayer($i));
  }
}

function showLayerInfo(OGRLayer $layer)
{
  $defn = $layer->GetLayerDefn();

  echo "Name: ".$defn->GetName()."\n";
  echo "Feature count: ".$layer->GetFeatureCount()."\n";
  echo "Field count: ".$defn->GetFieldCount()."\n";
  echo "Geometry type: ".$defn->GetGeomType()." (constant to match)\n";
}

$path =& $_SERVER['argv'][1];
if (!isset($path)) {
  throw new Exception("Missing argument: name/path of vector datasource");
}

OGRRegisterAll();

$datasource = OGRSFDriverRegistrar::Open($path, false /* readonly */);
if (!$datasource) {
  throw new Exception("Could not open vector datasource: ".$path);
}

header("Content-type: text/plain");
showInfo($datasource);

<?php

// Get GDAL drivers

GDALAllRegister();

$gdal = GetGDALDriverManager();
$count = $gdal->GetDriverCount();
$drivers = array();
for ($i = 0; $i < $count; $i++) {
  $driver = $gdal->GetDriver($i);
  $drivers[] = $driver->GetDescription();
}
echo "GDAL drivers : ".implode(", ", $drivers)."<br/><br/>\n";


// Get OGR drivers

OGRRegisterAll();

$registrar = OGRSFDriverRegistrar::getRegistrar();
$count = $registrar->GetDriverCount();
$drivers = array();
for ($i = 0; $i < $count; $i++) {
  $driver = $gdal->GetDriver($i);
  $drivers[] = $driver->GetDescription();
}
echo "OGR drivers : ".implode(", ", $drivers)."<br/><br/>\n";

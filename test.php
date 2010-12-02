<?php

echo "bonjour!\n";

OGRRegisterAll();
$path ="PG:dbname='aire-v1-prod' user='aire' password='59k6b_Ak' host='localhost' port='5432'";
$ds = OGRSFDriverRegistrar::Open($path);
var_dump($ds);
echo "au revoir!\n";


<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

$filename = 'products.txt';
$eachlines = file($filename, FILE_IGNORE_NEW_LINES);

foreach ($eachlines as $lines) { //add php code here
    echo "<option value='" . $lines . "'>$lines</option>";
}


<?php 
$latMasterArray = array();
$file_content = file_get_contents('queryResults.txt');
$i=0;
foreach(array_filter(explode("\n",$file_content))as $value){
	$latlogArray = explode(',',$value);
	$latMasterArray[$i][0]= $latlogArray[0];
	$latMasterArray[$i][1]= $latlogArray[1];
	$i++;
}
echo json_encode($latMasterArray);

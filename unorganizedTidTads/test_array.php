<?PHP
$host  = $_SERVER['HTTP_HOST'];
$uri   = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
$extra = 'mypage.php';

echo $host.'<br/>';
echo $uri.'<br/>';
echo $extra.'<br/>';

$regErr = '';
$my_arr['fn'] = 'Shawn';
$my_arr['ln'] = 'Stark';
$my_arr['mn'] = 'Nicholas';
//$my_arr = 'Shawn';

if(is_array($my_arr)){
$regErr = '?';
foreach($my_arr as $key=>$value){
$regErr = $regErr.$key.'='.$value.'&';

}

}else{echo $my_arr;}
echo $regErr;
?>
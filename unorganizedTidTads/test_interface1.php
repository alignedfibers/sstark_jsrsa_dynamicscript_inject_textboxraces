<?PHP
echo 'Its broken';
include 'includes/app_bundle.php';
$pageDetails = new pageInfo;
$pageDetails->newOverride('Shawn Stark', 'login_tool', 'Configured to except a login from a form post', 'v0.1', 'rev0', 'today', 'today');
$securePage = new logControl;
if($securePage->barracadeIt($pageDetails)){
echo 'page is secure';
}else{echo 'page is not secure';}
$securePage->logout();
if(isset($_POST['un']) && isset($_POST['pass'])){
if($securePage->login($_POST['un'],$_POST['pass')){
echo 'successful login';
}else{echo 'login not successful, redirecting to login';}

}else{echo 'Please provide a user name and password, redirecting to login';}




?>
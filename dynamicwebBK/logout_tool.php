<?PHP
/********************************************
This page coordinates the logout process.
upon completion, user is redirected to the helpcenter
where they will be able to navigate the help menus
and guest approved forums plus an advertisement to
become a member because this is a great site and 
view current number of regular members, upgraded 
members, moderators, and administrators a link to
download this application will also be provided 
there 
*********************************************/
	$pageStatus = false;
	
	/********************************************
	Include the whole application library
	*********************************************/
	require_once('../includes/transport_objects.php');
	require_once('../includes/appSettings.php');
	require_once('../includes/dbAdapt.php');
	require_once('../includes/security_suite.php');
	require_once('../includes/manipulators.php');
	require_once('../includes/testinggrandparent.php');
	
	/********************************************
	Create the objects needed by this page
	*********************************************/
	$pageDetails = new pageInfo;
	$securePage = new logControl;
	//$securePage->logout();
	//print_r($_SESSION);
	/********************************************
	Initialize the info this page will be referenced
	by and logged as.
	*********************************************/
	$pageDetails->newOverride('Shawn Stark', 'logout_tool.php', 'Configured to logout a user' 
	,'v0.1', 'rev0', 'today', 'today');
	
	/********************************************
	Run a few security measures and register this 
	page info to the logging system to track what 
	is and isn't being run and any suspiciouse 
	activities that may be occuring
	*********************************************/
	
	if($securePage->barracadeIt($pageDetails)){
		$pageStatus = true;
		////echo 'page is secure';
	}else{$pageStatus = false; 
	header('Location: '.$securePage->getSecurityErrURL());
	//echo "YOU CAN NOT LOGOUT";
	}
	
	if($pageStatus){
	$securePage->logout();
	//echo "YOU ARE LOGGED OUT";
	
	header('location: '.$securePage->getUserLoginPageURL());
	}

	
	?>
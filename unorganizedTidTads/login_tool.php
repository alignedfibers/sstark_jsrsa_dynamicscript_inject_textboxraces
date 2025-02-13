<?PHP
/********************************************
This page coordinates the login process from a 
form post, upon successful completion, user is
redirected to the user control panel where their
information will be loaded and all options 
available to them will be accessable.  
*********************************************/
	$pageStatus = false;
	
	/********************************************
	Include the whole application library
	*********************************************/
	include 'includes/app_bundle.php';
	
	/********************************************
	Create the objects needed by this page
	*********************************************/
	$pageDetails = new pageInfo;
	$securePage = new logControl;
	
	/********************************************
	Initialize the info this page will be referenced
	by and logged as.
	*********************************************/
	$pageDetails->newOverride('Shawn Stark', 'login_tool.php', 'Configured to except a login from a form post' 
	,'v0.1', 'rev0', 'today', 'today');
	
	/********************************************
	Run a few security measures and register this 
	page info to the logging system to track what 
	is and isn't being run and any suspiciouse 
	activities that may be occuring
	*********************************************/
	if($securePage->barracadeIt($pageDetails)){
		$pageStatus = true;
		echo 'page is secure';
	}else{$pageStatus = false; echo 'page is not secure';}
	
	//$securePage->logout(); Login will return false if user is already logged in
	if($pageStatus && isset($_POST['un']) && isset($_POST['pass'])){
		if($securePage->login($_POST['un'],$_POST['pass'])){
		header('Location: http://localhost/helpcenter.html?success=true');
			
		}else{/*Catch dual log attempt, Catch invalid user or pass*/
			echo 'login not successful, redirecting to login';
		}

	}else{
		header('Location: http://localhost/helpcenter.html');
		
	}




?>
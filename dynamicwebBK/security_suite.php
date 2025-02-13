<?PHP

/*
class view extends dbAdapter {
public function construct($slug){}
include viewFromDb;
$view = createView($slug);

rs.name
new model





}

class loadInfo(){
	$view;
	$parramList;
	
	function setView($viewNam){
		
	}
	
	function setParrams($urlParramChop){
		
	}
	
	function getView(){
	
	}
	
	function getParrams(){
	
	}
}

{

includeModel()
$modelClass = 'cc';
$modelObj = new $modelClass; // outputs "hi!"

class cc {
    function __construct() {
        echo 'hi!';
    }
}


myModel = new 

}
*/

class runDatum extends dbAdapter{
private $host; //Where the page being run is hosted 
private $namespace = 'default'; //The directory of the page being run
private $extra; //The file name of the page being run
private $parrams; //The params passed to the runnable
private $userAgent;
private $referer;
private $requestingIP;
private $isLoaded;
private $ownedBySess;
private $subDirectories = 'none';
private $includeAddress = 'none';
private $fileID = 'none';
/*
private $fileType;
private $fileSize;
private 
*/


public function __construct(){

			$this->requestingIP = $_SERVER['REMOTE_ADDR'];
			$url_apart = explode('/', $_SERVER['REQUEST_URI']);
			echo $_SERVER['REQUEST_URI'];
			if(count($url_apart) > 1){
				//First get the namespace
				array_shift($url_apart);
				$this->namespace = $url_apart[0];
				array_shift($url_apart);
				echo $this->namespace;
				//unset($url_apart[0]);				
			}else{}
			$url_apart_reversed = array_reverse($url_apart);
						echo $url_apart_reversed[0];
			//unset($url_apart[0]);
			//array_shift($url_apart);
			//echo $url_apart[0]."URLAPART";
			//$url_apart_reversed = array_reverse($url_apart);
			//echo $url_apart_reversed[0];
			
			$extra_parrams = explode('?', $url_apart_reversed[0]);
			$extra = $extra_parrams[0];
			echo "extra".$extra;
			//unset($url_apart_reversed[0]);
			array_shift($url_apart_reversed);
            echo count($url_apart_reversed);
			
			echo $url_apart_reversed[0]; //these are the subdirectories.
			//echo $url_apart_reversed[1];
			echo count($url_apart_reversed);			
			$subDirectories = $url_apart_reversed; //Always be sure to test if is array.
			/*Now that the namespace, sub directories and script are set 
			we can step through and make our sql request to see if it exists
			and create a direct load statement that can be run at any time.*/
			print($subDirectories[0]);
			//$subDirectories[1] = "nowhere";
			
			foreach($subDirectories as $key => $value){
				echo $value;
			}
			$tblAliasBaseNam = "deferredBy";
			$qryStr = "SELECT ";
			$whrClause = " WHERE ";
			$whrClauseSecHalf = "";
			$idSwitch = 0;
    $maxDepth = count($subDirectories)-1;
	
	echo $maxDepth."THISDEEP";
				echo $whrClause;
				echo '<br/>';
				echo $qryStr;

				//Create the select from build_items for the desired script before looping //through it's parent directories. This is the initial prime.
				$qryStr = $qryStr.$tblAliasBaseNam."0".".ITEM_REQ_NAME, ".$tblAliasBaseNam."0".".ITEM_INCLUDE_PATH FROM build_items AS ".$tblAliasBaseNam."0";
				
				/* A prime where clause
				$whrClause = $whrClause.$tblAliasBaseNam."0".".itemID AND ".$tblAliasBaseNam."0".".itemNam = ".$value." AND ".$tblAliasBaseNam.$useKey.".fk_ITEM_PARENT_DIR = ".$subdirectories[0]." AND ".$tblAliasBaseNam."0".".fk_ITEM_PARENT_ID = "; */
				
				//Initial prime where clause
				$whrClause = $whrClause.$tblAliasBaseNam."0".".ITEM_REQ_NAME = '".$extra."' AND ".$tblAliasBaseNam."0".".fk_ITEM_PARENT_DIR = '".$subDirectories[0]."' AND ".$tblAliasBaseNam."0".".fk_ITEM_NAMESPACE = '".$this->namespace."' AND ".$tblAliasBaseNam."0".".fk_ITEM_PARENT_ID = ";
				
				
		if(count($subDirectories)>0){
		//Now to add the discriminating directory structure to the where clause
		foreach($subDirectories as $key => $value){
			echo "foreach".$key;
				$useKey = $key+1;
			$qryStr = $qryStr.", build_items AS ".$tblAliasBaseNam.$useKey;
			if($key != $maxDepth){
				echo "not equal to max depth";
				
				$whrClause = $whrClause.$tblAliasBaseNam.$useKey.".pk_ITEM_ID AND ".$tblAliasBaseNam.$useKey.".ITEM_REQ_NAME = ".$value." AND ".$tblAliasBaseNam.$useKey.".fk_ITEM_PARENT_DIR = ".$subDirectories[$useKey+1]." AND ".$tblAliasBaseNam.$useKey.".fk_ITEM_PARENT_ID = ";
			
				}
		
			if($key == $maxDepth){
				echo "equal to max depth";
				$lastTable = $key+2;
				echo "baby1".$qryStr;
				$qryStr = $qryStr.", build_items AS ".$tblAliasBaseNam.$lastTable;
				$useKey = $key+1;
				$whrClause = $whrClause.$tblAliasBaseNam.$useKey.".pk_ITEM_ID AND ".$tblAliasBaseNam.$useKey.".ITEM_REQ_NAME = ".$value." AND ".$tblAliasBaseNam.$useKey.".fk_ITEM_PARENT_DIR = '".$this->namespace."' AND ".$tblAliasBaseNam.$useKey.".fk_ITEM_PARENT_ID = ".$tblAliasBaseNam.$lastTable.".pk_ITEM_ID AND ".$tblAliasBaseNam.$lastTable.".ITEM_REQ_NAME = ".$this->namespace;
			 
				}
				
			}	
		}


	echo '<br/>';
	echo "baby".$qryStr;
	echo '<br/>';
	echo $whrClause;		
			
			

	}
			
			/*
			appSettings::setUri($_SERVER['REQUEST_URI']);
			appSettings::setUserAgent($_SERVER['HTTP_USER_AGENT']);
			appSettings::setHost($_SERVER['HTTP_HOST']);
			appSettings::setSessionID(session_id());*/

}





class runLogger extends dbAdapter{
/***********************************************************************
 This class is instantiated in logControl and assigned to appSettings
 to be used by all child classes. appSettings is the grandparent to this
 class. Please do not instantiate it anywhere else as it is already done 
 for you when you run the barracadeIt method *
 
 This class is used to send detailed information about the run to database
 It uses the user and page information from appSettings. I will log the process
 of each run on the fly, but it is noted for performance reasons that the
 log information should be retained and committed to database upon run completion.
 
 
FunctionLst 
-logFuncRun Public logs the function, class, userID, Date, and error msg.
	
Commentby	
-starkSn -04/29/2009
************************************************************************/
	public function construct(){}
	
	
    public function addRunEntry(){
	$returnValue = false;
	
	$referer = appSettings::getReferer();
	$usrID = appSettings::getUsrID();
	if(!isset($referer)||$referer == ''){
	$referer = 'none';
	}
	if(!isset($usrID)||$usrID == ''){
	$usrID = 0;
	}
	
     //$this->writToLog('This is the referer '.$referer);
	 
	 echo appSettings::getUserAgent();
	
			$qStr = "INSERT INTO run_log(RUN_USER_ID, RUN_USER_ISLOGGED, RUN_PAGE_DEVELOPER, RUN_PAGE_VERSION, RUN_PAGE_REV, RUN_PAGE_FILE_NAME, RUN_PAGE_ADD_DATE, RUN_PAGE_UPDATED_DATE, RUN_REQUEST_IP, RUN_REQUEST_URI, RUN_USER_AGENT, RUN_PAGE_HOST, fk_RUN_REGISTERED_APPS, RUN_PAGE_REFERER, RUN_DOCUMENT_ROOT, RUN_SESSION_ID) VALUES(".parent::clnQry($usrID).",".parent::clnQry(appSettings::getUsrLogStat()).",'".parent::clnQry(appSettings::$pageInfo->getDeveloper())."','".parent::clnQry(appSettings::$pageInfo->getPageVersion())."','".parent::clnQry(appSettings::$pageInfo->getPageRev())."','".parent::clnQry(appSettings::$pageInfo->getPageNam())."','".parent::clnQry(appSettings::$pageInfo->getDateAdded())."','".parent::clnQry(appSettings::$pageInfo->getDateUpdated())."','".parent::clnQry(appSettings::getRequestingIP())."','".parent::clnQry(appSettings::getUri())."','".parent::clnQry(appSettings::getUserAgent())."','".parent::clnQry(appSettings::getHost())."','none yet, but I will register clientside apps in another table that I can join to this one','".parent::clnQry($referer)."','".parent::clnQry(appSettings::getDocumentRoot())."','".parent::clnQry(appSettings::getSessionID())."')";
		
		//$this->writToLog($qStr);
	////echo $qStr;	
		parent::opnDb();
		
			if(parent::exeQuery($qStr)){
				appSettings::setRunID(mysql_insert_id());
				$returnValue = true;
			}else{
				//$this->writToLog('Insert into run log failed '.mysql_error());
				echo mysql_error()."it is an echo";
				$returnValue = false;
			}
		return $returnValue;
	}
	/************************************************
	OTHER LOGS TO ADD
	usageLog
	*************************************************/
	
	}


class logControl extends dbAdapter{
/***********************************************************************
This class is grandchild of appSettings and child of dbAdapter.
It controls user login and validation also sets appSettings, Session,
and is the main system controller. It is not recomended to bypass this
class as it ensures protection against different possible threats. 
It will eventualy be used to filter out banned IP addresses users and more. 

Commentby	
-starkSn -04/29/2009
***********************************************************************/

	public function __construct(){}

	public function barracadeIt($pageInfo){
	

		
		$returnValue = true;
		echo 'in barracade';
		if (session_id() == ''){
			session_start();
		}		
		$mySess = new sessMan;
		echo "before session start";
		$mySess->start();
		echo "after session start";
		
		if(!$this->verifySession()){
			$returnValue = $this->setGuestUsrLog();
		}
		if($returnValue){
			$runLog = new runLogger;
			appSettings::$log = $runLog;
			appSettings::setUsrID($_SESSION['jtUsr']['ID']);
			appSettings::setUsrDspName($_SESSION['jtUsr']['UN']);
			appSettings::setUsrType($_SESSION['jtUsr']['TYPE']);
			appSettings::setUsrDir($_SESSION['jtUsr']['DIR']);
			appSettings::setUsrLogStat($_SESSION['jtUsr']['IS_LOGGED']);
			appSettings::setUsrActiveStatus($_SESSION['jtUsr']['ACTIVE']);
			appSettings::setPageInfo($pageInfo);
			appSettings::setRequestingIP($_SERVER['REMOTE_ADDR']);
			appSettings::setUri($_SERVER['REQUEST_URI']);
			appSettings::setUserAgent($_SERVER['HTTP_USER_AGENT']);
			appSettings::setHost($_SERVER['HTTP_HOST']);
			appSettings::setSessionID(session_id());
			
			if(isset($_SERVER['HTTP_REFERER'])){
			appSettings::setReferer($_SERVER['HTTP_REFERER']);
			}else{
			appSettings::setReferer('none');
			}
			$uri = $_SERVER['REQUEST_URI'];
			
			echo "at expression";
			
			$pattern = '/\/{2,}/';
			preg_match($pattern, $uri, $matches, PREG_OFFSET_CAPTURE, 3);
			print_r($matches);
			
			
			
			$uriArray = split("/",$uri);
			//$this->writToLog(count($uriArray));
			$loopControl = count($uriArray)-2;
			$cnt = 0;
			$relativeRoot = "";
			while($cnt<$loopControl){
			$relativeRoot = $relativeRoot."../";
			$cnt++;
			}
			//$this->writToLog($relativeRoot);
			appSettings::setRelativeWebRoot($relativeRoot);
			
			appSettings::setDocumentRoot(getenv("DOCUMENT_ROOT"));
			
			/*********************************************************************
			I might like to verify that the previouse properties were successfuly
			set prior to setting secure run to true. A true value in secure run
			meens all identifing information is loaded and passes all filters.
			A standard for creating filters and handling them is in process of
			being created. I believe the filters will be loaded and placed in 
			an array from database xml or other method of persistent storage.
			**********************************************************************/
			echo 'appsettings are set';
			if(appSettings::$runID = appSettings::$log->addRunEntry()){
			appSettings::setSecureRun(true);
			}else{
			appSettings::setSecureRun(false);
			}
			echo 'run entry entered';
			/*//echo appSettings::$pageInfo->getDeveloper();
			//echo appSettings::getUsrID();
			//echo appSettings::getUsrDspName();
			//echo appSettings::getUsrType();
			//echo appSettings::getUsrDir();
			//echo appSettings::getUsrLogStat();
			//echo appSettings::getUsrActiveStatus();
			
			print_r(appSettings::getPageInfo());*/
						
		}else{
		appSettings::setSecureRun(false);
		
		}
			
			if(appSettings::getSecureRun()){
			$returnValue = true;
			}else{
			$returnValue = false;
			}
			


		/*print_r($this);
		print_r($this->getPageInfo());*/
		//$this->writToLog($relativeRoot);
		return $returnValue;
		// log page access, $curPage: doubles as a DB test
		// 


		
		//$returnValue = true;
		//return $returnValue;
	}
	
		private function verifySession(){
	//BOOLEAN
		$returnValue = false;
		
		if(isset($_COOKIE['SESSINF']) && isset($_COOKIE['SESSINF']['sessID']) && isset($_COOKIE['SESSINF']['tbID']) && isset($_COOKIE['SESSINF']['UID'])){
			$returnValue = true;
		}else{$returnValue = false;}

		return $returnValue;
	}
	
	
	public function getUsrLogType(){
	//Possible Types: admin, geust, user, poweruser, machine
	//logType returns an empty string if this function fails.
		$returnValue = '';
		if(isset($_SESSION['jtUsr'])){

			if(isset($_SESSION['jtUsr']['TYPE'])){
				$returnValue = $_SESSION['jtUsr']['TYPE'];
			}
	
	
		}
		return $returnValue;
	}
	
	
	protected function setGuestUsrLog(){
	//Should create a guest record that is retained in session

		$_SESSION['jtUsr']['ID'] = '';
		$_SESSION['jtUsr']['UN'] = 'Guest';
		$_SESSION['jtUsr']['DIR'] = '';
		$_SESSION['jtUsr']['TYPE'] = 'guest';
		$_SESSION['jtUsr']['IS_LOGGED'] = true;
		$_SESSION['jtUsr']['LOG_TIME'] = time();
		$_SESSION['jtUsr']['ACTIVE'] = false;
		//header("location:".$_SERVER['REQUEST_URI']);
	return true;
	
	

	}
	
	public function logout(){
	$this->setGuestUsrLog();
	}
	private function setUserSession($row){
		$_SESSION['jtUsr']['ID'] = $row['pk_UID'];
		$_SESSION['jtUsr']['UN'] = $row['UDSPNAM'];
		$_SESSION['jtUsr']['DIR'] = $row['UDSPNAM']."_".$row['pk_UID'];
		$_SESSION['jtUsr']['TYPE'] = $row['UTYP'];
		$_SESSION['jtUsr']['IS_LOGGED'] = true;
		$_SESSION['jtUsr']['LOG_TIME'] = time();
		$_SESSION['jtUsr']['ACTIVE'] = $row['USTAT'];
	}
	public function login($un, $pass){
	/******************************************************
	Public function login validates, retrieves a user, then
	rewrites it to session and appSettings. By running barracade
	again it gets placed in appSettings. Cookies must be enabled, 
	However the only way to test if the session cookie was created
	is to check if isset on the next loaded page, thus barracade runs 
	again on next page.

	*******************************************************/ 
	//$this->writToLog('entered login function');
			$errMsg = '';
			$returnValue = 'CLEAR';
			$isGoodLog = true;
			
		if($this->getUsrLogType() == 'guest' || $this->verifySession() == false){
			
			
			$qStr = "SELECT pk_UID,UDSPNAM,UTYP,USTAT,UPREPPED FROM xusr WHERE UDSPNAM='".parent::clnQry($un)."' AND UPASS='".parent::clnQry($pass)."'";
			
			parent::opnDb();
			$rs = parent::exeQuery($qStr);
			
			//print_r($rs);

			if(mysql_num_rows($rs) == 1){
			//write session
			////echo 'you have data';
			//$this->writToLog("Class: logControl, Function: Login request returned one row");
				$row = mysql_fetch_array($rs, MYSQL_ASSOC);
				print_r($row);
/************************
$id = "ID";
$un = "UN";
$dir = "DIR";           
$type = "TYPE";
$islogged = "IS_LOGGED";
$logtime = "LOG_TIME";
$active = "ACTIVE";    
*************************/

		if($row['USTAT']==0){
		//In future release this will be thrown and
		//need to be handled.
		$isGoodLog = false;
		$returnValue = 'ERR_NOT_ACTIVE';
		echo $row['USTAT'];
		}elseif($row['USTAT']==0 && $row['UPREPPED']==0){
		//In future release this will be thrown and
		//need to be handled.
		$isGoodLog = false;
		$returnValue = 'ERR_CONFIRM_NO_SEND';
		}
		else{
		$isGoodLog = true;
		$returnValue = 'CLEAR';
		}

		if($isGoodLog){
		$this->setUserSession($row);
		}

	}

			if(mysql_num_rows($rs) > 1){
				//log integrity issue
				//throw new usrIntegExc($un);
				$errMsg = 'Integrity Issue in Users Table';
				$isGoodLog = false;
				$returnValue = 'NOT_CLEAR';
			}
			if(mysql_num_rows($rs) < 1){
				//Incorrect USERNAME or PASSWORD
				//throw new invalidUsrExc($un);
				$errMsg = 'invalid user or password';
				$isGoodLog = false;
				$returnValue = 'ERR_DONT_EXIST';
			}
			
		}else{
			//log dual login attempt
			//throw new usrLoggedExc($un, $pass);
			////$this->writToLog('Dual log attempt');
			$errMsg = 'Dual log attempt not allowed';
			$isGoodLog = false;
			$returnValue = 'ERR_DUAL_LOG_ATTEMPT';
		}
		return $returnValue;
	}
	/************************************************************
	Methods to retrieve URL of different error pages and system tools
	pages. As this grows it will load urls from an xml file of registered
	pages. These pages will be catagorized by type. Current types
	being considered are: error_messages, system_tools and registered_pages.
	pages may automaticaly register themselves on first run and remove
	themselves during system maintenance run. Developer must do a run of
	all uploaded pages before they are registered. Upon the development of
	a page adding utility which is foreseeable pages will automaticaly be
	registered when they are added and thus will need a special form of 
	identification or have a series of forms that ask for the identifing
	information when page is uploaded or submitted. Doing this makes the
	application installable to any directory without having to change the
	url for every page.
	*************************************************************/
	
	/***************************************************************
	Hardcode write a comment porting problems. BUILD URLS BASED ON
	THE APP FOLDER location. cuz if it is '' then '/'.''.'/' will happen
	thus url will look like example.com//page.php this is bad. not extremely, just ugly
	******************************************************************/
	public function getContactAdminURL(){
		$contactAdminUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/error_pages/err_contact_admin.php';
	return $contactAdminUrl;
	}
	public function getSecurityErrURL(){ // I believe this will be a 404 error
	$zeroAccessUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/error_pages/err_zero_access.html';
	return $zeroAccessUrl;
	}
	public function getUserLogOutToolURL(){
	$logOutToolUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/system_tools/logout_tool.php';
	return $logOutToolUrl;
	}
	public function getUserLogInToolURL(){
	$logInToolUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/system_tools/login_tool2.php'; 
	return $logInToolUrl;
	}
	public function getUserLogInPageURL(){
	$logInPageUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/base_pages/user_login.php'; 
	return $logInPageUrl;
	}
	public function getDualLogErrURL(){
	$dualLogErrUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/system_errors/err_dual_log_attempt.php'; 
	return $dualLogErrUrl;
	}
	public function getNonActiveUserErrURL(){
   $createEnvUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/system_errors/err_user_not_active.php'; 
	return $createEnvUrl;
	}
	public function getConfirmNoSendErrURL(){
	$chngInctvUsrEmlUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/system_errors/err_confirm_no_send.php';
	return $chngInctvUsrEmlUrl;
	}
	public function getUserProfilePageURL(){
	$profileHomeUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/base_pages/user_profile.php';
	return $profileHomeUrl;
	}
	public function getHelpCenterPageURL(){
	$helpCenterUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/extended_pages/help_center.php';
	return $helpCenterUrl;
	}
	public function getUserRegisterPageURL(){
	$userRegisterUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/base_pages/user_register.php';
	return $userRegisterUrl;
	}
	public function getUserRegisterToolURL(){
	$userRegisterUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/system_tools/register_tool.php';
	return $userRegisterUrl;
	}
	public function getSystemToolsBaseURL(){
	$baseSystemToolsUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/system_tools/';
	return $baseSystemToolsUrl;
	}
	public function getSystemErrBaseURL(){
	$baseSystemErrUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/system_errors/';
	return $baseSystemErrUrl;
	}
	public function getBasePagesBaseURL(){
	$basePagesUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/base_pages/';
	return $basePagesUrl;
	}
	public function getExtendedPagesBaseURL(){
	$baseExtendedPagesUrl = 'http://'.appSettings::getHost().appSettings::getAppFolder().'/extended_pages/';
	return $baseExtendedPagesUrl;
	}




}
class resourceControl{
//makes sure style sheets and stuff can only
//be viewed when referer url is valid
}
?>
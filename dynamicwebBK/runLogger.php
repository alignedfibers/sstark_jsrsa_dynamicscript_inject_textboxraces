<?PHP
class runLogger extends dbAdapter{
/***********************************************************************
 This class is instantiated in logControl and assigned to appSettings
 to be used by all child classes. appSettings is the grandparent to this
 class. Please do not instantiate it anywhere else as it is already done 
 for you when you run the barracadeIt method *
 
 This class is used to send detailed information about the run to database
 It uses the user and page information from appSettings. I will log the process
 of each run on the fly.
 
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
	 
	 //echo appSettings::getUserAgent();
	
			$qStr = "INSERT INTO run_log(RUN_USER_ID, RUN_USER_ISLOGGED, RUN_PAGE_DEVELOPER, RUN_PAGE_VERSION, RUN_PAGE_REV, RUN_PAGE_FILE_NAME, RUN_PAGE_ADD_DATE, RUN_PAGE_UPDATED_DATE, RUN_REQUEST_IP, RUN_REQUEST_URI, RUN_USER_AGENT, RUN_PAGE_HOST, fk_RUN_REGISTERED_APPS, RUN_PAGE_REFERER, RUN_DOCUMENT_ROOT, RUN_SESSION_ID, RUN_DISCRIMINANT, RUN_PAGE_IN_LOAD, RUN_ENTRY_TIME, RUN_LOAD_COMPLETION_TIME, RUN_SUCCESSFUL) VALUES(".parent::clnQry($usrID).",".parent::clnQry(appSettings::getUsrLogStat()).",'".parent::clnQry(appSettings::$pageInfo->getDeveloper())."','".parent::clnQry(appSettings::$pageInfo->getPageVersion())."','".parent::clnQry(appSettings::$pageInfo->getPageRev())."','".parent::clnQry(appSettings::$pageInfo->getPageNam())."','".parent::clnQry(appSettings::$pageInfo->getDateAdded())."','".parent::clnQry(appSettings::$pageInfo->getDateUpdated())."','".parent::clnQry(appSettings::getRequestingIP())."','".parent::clnQry(appSettings::getUri())."','".parent::clnQry(appSettings::getUserAgent())."','".parent::clnQry(appSettings::getHost())."','none yet, but I will register clientside apps in another table that I can join to this one','".parent::clnQry($referer)."','".parent::clnQry(appSettings::getDocumentRoot())."','".parent::clnQry(appSettings::getSessionID())."','".parent::clnQry(appSettings::getDiscriminant())."','".parent::clnQry(appSettings::getHumanInterface())."',null,null,0)";
		
		//$this->writToLog($qStr);
	//echo $qStr;	
		parent::opnDb();
		
			if(parent::exeQuery($qStr)){
				appSettings::setRunID(mysql_insert_id());
				$returnValue = true;
			}else{
				//$this->writToLog('Insert into run log failed '.mysql_error());
				//echo mysql_error()."it is an //echo";
				$returnValue = false;
			}
		return $returnValue;
	}
	/************************************************
	OTHER LOGS TO ADD
	usageLog
	*************************************************/
	
	public function completeRun(){
	
		$qStr = "UPDATE run_log SET RUN_SUCCESSFUL = 0, RUN_HAS_ERRORS = 0 WHERE pk_RUN_ID = ".appSettings::getRunID();
	//echo $qStr;
	
		    if(parent::exeQuery($qStr)){
				$returnValue = true;
			}else{
				//$this->writToLog('Insert into run log failed '.mysql_error());
				//echo mysql_error()."it is an //echo";
				$returnValue = false;
			}
		return $returnValue;
	
	}
	
	}
	
?>
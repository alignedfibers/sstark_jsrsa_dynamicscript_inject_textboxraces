<?php

class sessMan extends dbAdapter{

	/*****************************************************************************
	Written by: Shawn Nicholas Stark
	
	Variables in this class are used when NOT NECEASARY~ FOR CLARITY DURING DEVELOPMENT, 
	and will be removed when I tune the code for efficiency, memory usage, run time
	redundancy, and stuff like that.
	******************************************************************************/
	

	//Declare the public variables, that will be used outside instance of this class
	private $identifier;
	public $flatUserName; // used to specificaly address client by user name
	public $flatLocCode;  // used to taylor site specificaly to location
	public $runInfo;      // used to load page and discrepate what session remnants to retrieve
	public $timeSessLastUpd;   // keep track of session activity
	public $timeElapsedSinceSessLastUpd;
	
	//Declare session identification variables used and assigned within an instance of this class
	private $trackBackId; 
	public $sessId;
	
	//Declare encryption keys used and assigned within an instance of this class
	private $sessPublicKey;
	private $sessPrivateKey;
	private $sessModuloKey;
	private $encryptionKeyForIdentity;
	
	//hmmm still a work in progress
	private $theSessRow;
	
	//A few needed bells and whistles, including cookie switches and userAgent switches
	public $cookiesEnabled = false; //BOOLEAN
	public $isSecure;
	
	
	//Declare encryption objects used and assigned within this class
	private $rsaCrypt;
	private $singleKeyCrypt;
	
	//Declare remnant arrays
	/**/

	public function __construct(){
		//Assign all known objects and variables when a new object is created
		
		$this->encryptionKeyForIdentity = "98jkrt:hv7p9l.ks 8";
		$this->rsaCrypt = new rsaEncryption();
		$this->singleKeyCrypt = new encryption_class;
		/*$this->runInfo= new runDatum;*/
	}

	public function start(){
		//Start session
	
			$this->blockNoCookies();
			$this->loadSession();
			
	}

	
	private function blockNoCookies(){
	/*****************************************************
	This function sets an error if user has cookies disabled
	If an error is set the loader will search the error database 
	and if nothing found will deliver default
	******************************************************/
	
		if(isset($_GET['verifycookie'])){
			if($_GET['verifycookie'] == 'true'){
				if(!isset($_COOKIE['SESSINF']['COOKIESENABLED'])){
					//Go ahead and create the garbage session
					//Handle the err in loader based on nocookie set to true in info
					//this is where we do our switch for google.
					// include_functions
					// include_classes
					// include_routine
					$this->cookiesEnabled = false;
					//special error, as it exists prior to session creation,
					//we can't just do a call back to load in this instance.
					//We will have to create the guest session first, even
					//though it will not be retained. I prefer not to start makeing
					//a bunch of callbacks to different functions "gotos" the err
					//callback should follow the same path every time, this is the
					//description of a non fatal error where processing can continue
					//load will just have to check the cached errors, we are not in the
					//middle of a load where we have to clear the output before a new load
					//thus why It is a must and should be enforced that session is started
					//before any headers are sent. Long explanation I know, but is neccesary
					//more about error types will be explained in the error module... kinda
					//circumventing the use for redirects in a way. don't get me wrong they 
					//still have a purpose but I plan to handle errors on the load that gen
					//them without counting on the browser to redirect with the right uri.
					//putting validation and verification in the power of the server for
					//client programs without local copies and processing of errors
				}else{ 
				$this->cookiesEnabled = true;
				
				}
			}
		}elseif(!isset($_COOKIE['SESSINF']['COOKIESENABLED'])){ 
		setcookie("SESSINF[COOKIESENABLED]","GARBAGE",time()+3600*24,"/");
		header("location:".$_SERVER['REQUEST_URI']."?verifycookie=true");
		exit;
		}
	}
	
	private function loadSession(){ 
	//CHECK THAT ALL REQUIRED COOKIES ARE SET
	
		if($this->requiredCookiesExist()){
		
				//copy userName decrypted from cookie to local member 
				$this->decryptIdentifier(); 
				
				//copy public key from cookie to local member
				$this->loadSessPublicKey();
				
				//load privKey from active sess identified by decrypted userName
				$this->loadSessPrivateKey(); 
				
				//copy sessId decrypted with sessPrivKey from cookie to local member
				$this->decryptSessionID(); 
				
				//copy decrypted region, verified from external i.e.(loaded from external service)
				//region list to local member,  *****TRIAL TESTING*****
				//$this->decryptUsrLocation(); 
				
				
				//verify all the previousely set variables successfully reference active session
				if($this->verifyClient()){
				
					//Assign a new trackback and set sess record update time
					$this->updateSession();
					
					//Load the variables associated with this session and available
					//to the url that has been requested to the array of there accessor
					$this->loadRemnants();
				}
				
				
		
		}else{
				$this->createGuestSession();//encrypts everything and sets cookies
		
		}
	
	
				
	}

	private function verifySession(){
	//set decrypted identifier
	$identifier = $this->singleKeyCrypt->decrypt($this->encryptionKeyForIdentity, $_COOKIE['SESSINF']['IDENTIFIER']);
	
	$sqlString = "SELECT SESS_IS_ACTIVE, SESS_MODULO_CRYPT_KEY, SESS_PRIV_CRYPT_KEY, k_SESS_BACK_TRACK, k_SESS_JARBLE_ID FROM sessions WHERE k_SESS_UNAME = '$identifier' AND SESS_IS_ACTIVE = 1";
	
		parent::opnDb();
			
			if($rs = parent::exeQuery($sqlString)){
				
				if(mysql_num_rows($rs) == 1){
					$row = mysql_fetch_array($rs, MYSQL_ASSOC);
					$this->theSessRow = $row;
				}else{
					//error
					echo mysql_error()."it is an echo";
				}
				
			}else{
			
				//error
				echo mysql_error()."it is an echo";
				
			}
			
		$sessIdFromClient = $this->rsaCrypt->rsa_decrypt($_COOKIE['SESSINF']['SESSID'],$row['SESS_PRIV_CRYPT_KEY'],$row['SESS_MODULO_CRYPT_KEY']);
		
		if($sessIdFromClient == $row['k_SESS_JARBLE_ID'] && $row['k_SESS_BACK_TRACK'] == $_COOKIE['SESSINF']['TRACKBACK']  ){
		
		$this->isSecure = true;
		$this->sessId = $sessIdFromClient;
		$this->sessPrivateKey = $row['SESS_PRIV_CRYPT_KEY'];
		$this->trackBackId = $row['k_SESS_BACK_TRACK'];
		$this->sessModuloKey = $row['SESS_MODULO_CRYPT_KEY'];
		}elseif($row['k_SESS_BACK_TRACK'] != $_COOKIE['SESSINF']['TRACKBACK']){
		//error
		}elseif($sessIdFromClient != $row['k_SESS_JARBLE_ID']){
		//error
		}

	
	}
	
	private function decryptIdentifier(){
	
	$returnVal = $this->singleKeyCrypt->decrypt($this->encryptionKeyForIdentity, $_COOKIE['SESSINF']['IDENTIFIER']);
	
	echo 'decrypted identity'.$returnVal;
	
	$this->flatUserName = $returnVal;
	
	return;
	
	}
	
	private function decryptSessionID(){
		$theVal = $_COOKIE['SESSINF']['SESSID'];
		echo "PUB".$this->sessPublicKey;
		echo "PRIV".$this->sessPrivateKey;
		echo "MOD".$this->sessModuloKey;
		$this->flatSessId = $this->rsaCrypt->rsa_decrypt($_COOKIE['SESSINF']['SESSID'],$this->sessPrivateKey,$this->sessModuloKey);

		return;
	}
	
	private function loadSessPublicKey(){
	$this->sessPublicKey = $_COOKIE['SESSINF']['SESSPUBKEY'];
	echo "XXXX".$_COOKIE['SESSINF']['SESSPUBKEY']."XXXX";
	return;
	
	}
	
	private function loadSessPrivateKey(){
	
	$sqlString = "SELECT SESS_IS_ACTIVE, SESS_MODULO_CRYPT_KEY, SESS_PRIV_CRYPT_KEY, k_SESS_BACK_TRACK, k_SESS_JARBLE_ID FROM sessions WHERE k_SESS_UNAME = '$this->flatUserName' AND SESS_IS_ACTIVE = 1";
	//echo $sqlString;
	parent::opnDb();
		
			if($rs = parent::exeQuery($sqlString)){
				if(mysql_num_rows($rs) == 1){
					$row = mysql_fetch_array($rs, MYSQL_ASSOC);
					$this->theSessRow = $row;
					$this->sessPrivateKey = $row['SESS_PRIV_CRYPT_KEY'];
					//echo "assign private KEY".$row['SESS_PRIV_CRYPT_KEY'];
					$this->sessModuloKey = $row['SESS_MODULO_CRYPT_KEY'];
					$this->trackBackId = $row['k_SESS_BACK_TRACK'];
				}else{
					//error
					echo mysql_error()."it is an echo";
				}
			
			}

	return;
	
	}
	
	
	private function verifyClient(){
	$returnVal = false;
	if($this->theSessRow['k_SESS_JARBLE_ID'] == $this->flatSessId && $_COOKIE['SESSINF']['TRACKBACK'] == $this->theSessRow['k_SESS_BACK_TRACK']){ $returnVal = true; echo 'XXXXXXsession is verifiedXXXXXX';}
	
	return $returnVal;
	}
	/*Add remanant allows you to add an element with a scope/accessor of global, namespace, local, enherit or private*/
	public function addRemnant(){
	}
	public function removeRemnant(){
	}
	public function valueOfRemnant(){
	}
	public function loadRemnants(){
	}
	public function listLoadedRemnantNamesAndAccessors(){
	}
	public function listAllRemnantNamesAndAccessors(){
	}
	
	/*REMNANT RETRIEVAL METHODS DEFINED BELOW - Connecting scripts to only the
	                                            session info allowed them.
	*NOTE    build items are uniquely named within their containing directory
	         accessed uniquily with full uri. Will be working on an inteligent
	         lookup, to find all items of a specific name, globaly or within
	         a namespace or below a certain directory.*/
	
	public function getGlobalRemnants(){
		return $this->globalRem;
	}
	public function getNameSpaceRemnants(){
		return $this->namespaceRem;
	}
	public function getLocalRemnants($ns){
		//returns an array of remnants shared locally
		//in the current directory, these are
		//shown in db with an accesor of local
		if($ns === null){
		$ns = $this->namespace;
		}
		return;
	}
	public function getHeritageRemnants($buildItem){
		//returns an array of remnants enherited
		//from the parent directories, these are
		//shown in db with an accesor of inherit

		return;
	}
	public function getTimeElapsedSinceLastRequest(){
		//returns the current time minus the last
		//update time from session record
		
		return;
	}
	
	public function changeUser(){
	//Updates session with new user name and encrypts it for the matching cookie
	
	return;
	
	}
	
	
	
	private function createTrackBackId(){
	//Creates a random 7 digit number, to reference and follow a users
	//navigation through the site. Creates a new one every visit,
	
	//subject to improved efficiency by eliminating loop and using
	//a modified current time, most likely combined with user_agent
	//any way to make this function faster will help + a problem
	//arises when a user clicks out of a page before the last one has 
	//loaded, so we archive 5 trackbacks moving them fifo for the look up
	//and they expire pretty quik. Should reference stats and surveys on 
	//how long a person stays at one page.
	
		$tbID = '';
		$cnt = 0;
		while($cnt < 7){
		$tbID = $tbID.(string)rand(0,9);
		$cnt++;
		}
		$tbID = (int)$tbID;
	
	return $tbID;
	}
	
	private function createSessId(){
		/*$sessID = '';
		$cnt =0;
		while($cnt<7){
			$sessID = $sessID.$this->crypt->getChar(rand(0,71));
		}*/
		//Check if session exists with created id
		return md5(uniqid(microtime()));
	}
	
	private function createGuestName(){
	$sqlString = "SELECT count_integer FROM counters WHERE k_count_name = 'guestSessions'";
	
	parent::opnDb();
		
			if($rs = parent::exeQuery($sqlString)){
				if(mysql_num_rows($rs) == 1){
					$row = mysql_fetch_array($rs, MYSQL_ASSOC);
					$updatedCount = $row['count_integer']+1;
		$updateString = "UPDATE counters SET count_integer = ".$updatedCount." WHERE k_count_name = 'guestSessions'";
		parent::exeQuery($updateString);
				}else{
					//error
				}
			
			}
	//UPDATE counters SET count_integer = 2 WHERE k_count_name = 'guestSessions';
	return "guestHandle".$updatedCount;
	}
	
	
	private function requiredCookiesExist(){
		$returnValue = false;
		if(isset($_COOKIE['SESSINF']) && isset($_COOKIE['SESSINF']['IDENTIFIER']) && isset($_COOKIE['SESSINF']['SESSPUBKEY']) && isset($_COOKIE['SESSINF']['SESSID'])){
			$returnValue = true;
		}else{$returnValue = false;}
		
		return $returnValue;
		
	}
	
	
	private function createGuestSession(){
		$encryptionKeyForIdentity = "98jkrt:hv7p9l.ks 8";
		$this->trackbackId = $this->createTrackBackId();
		$keys = $this->rsaCrypt->generate_keys();
		//$sessID = $this->createSessId();

		$this->sessPubKey = $keys[1];
		$this->sessPrivKey = $keys[2];
		$this->sessModuloKey = $keys[0];
		$this->flatUserName = $this->createGuestName();
		$this->flatUserId = '0';
		$this->trackBackId = $this->createTrackBackId();
		$this->flatSessId = $this->createSessId();
		$rsaEncryptedSessID = $this->rsaCrypt->rsa_encrypt($this->flatSessId,$this->sessPubKey, $this->sessModuloKey);
		$singleEncUserName;
		$rsaEncSessId;
		
		$identityLen = strlen($this->flatUserName);
		$singleEncUserName = $this->singleKeyCrypt->encrypt($this->encryptionKeyForIdentity, $this->flatUserName, $identityLen);

		$rsaEncSessId = $this->rsaCrypt->rsa_encrypt ($this->flatSessId,  $this->sessPubKey,  $this->sessModuloKey);


		
		$sessActive = 1;
		//$uID = "0";
		//$qryTest = "SELECT * FROM run_log";
		if(!$this->sessExists()){
		
		$insertQry = "INSERT INTO sessions (SESS_PUB_CRYPT_KEY,SESS_PRIV_CRYPT_KEY,SESS_MODULO_CRYPT_KEY,k_SESS_UNAME,k_SESS_JARBLE_ID,k_SESS_BACK_TRACK,SESS_START_TIME,SESS_UPDATE_TIME,SESS_IS_ACTIVE,SESS_IS_GUEST) VALUES($this->sessPubKey,$this->sessPrivKey,$this->sessModuloKey,'$this->flatUserName','$this->flatSessId',$this->trackBackId,null,null,$sessActive,1)";
		
		echo $insertQry;
		
		parent::opnDb();
		
			if(parent::exeQuery($insertQry)){
				echo '###############query success##########';
				$sessID2 = mysql_insert_id();
			}else{
				//$this->writToLog('Insert into run log failed '.mysql_error());
				echo mysql_error()."it is an echo";
			}
		}
		setcookie("SESSINF[IDENTIFIER]",$singleEncUserName,time()+3600*24,"/");
		setcookie("SESSINF[SESSPUBKEY]",$this->sessPubKey,time()+3600*24,"/");
	    setcookie("SESSINF[SESSID]",$rsaEncSessId,time()+3600*24,"/") ;
		setcookie("SESSINF[TRACKBACK]",$this->trackbackId,time()+3600*24,"/") ;

	
	}
	
	private function sessExists(){
		return false;
	}
	
	
	
}
	?>


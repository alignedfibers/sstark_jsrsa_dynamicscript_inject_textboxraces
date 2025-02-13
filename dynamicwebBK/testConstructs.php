<?PHP
	ini_set('display_errors',true);

	class theParent{
	
		protected static $MyName;
		protected $instanceTest;
	
		public function __construct(){
		
		echo 'parent';
		
		 $this->MyName = 'Shawn';
		}
		
		public function printInstance(){
			echo $this->instanceTest;
		}
	
	}

	class theChild extends theParent{
	
		protected static $MyLastName;
		
		public function __construct(){
		
		echo 'child';
		
			parent::__construct();
			
			$this->MyLastName = 'Stark';
			
			
		}
		
		public function getFullName(){  
		

		   return $this->MyName.' '.$this->MyLastName;  
		} 
	}

	
	class grandChild extends theChild{
	
		public function __construct($instTst){
			echo 'grandKid';
			parent::__construct();
			
			$this->instanceTest = $instTst;
			
		}
	}
setcookie("garbage","trash",time()+3600*24,"/");

//header("location:http://localhost/java_tools2/testbed/blah.php?callpage=".$_SERVER['REQUEST_URI']); 

echo 'before the world began';

echo 'md5 test';

echo md5('ShawnXStark');
//Use multiples and squares for random uppercase out of a 3 digit number, and actually calculate the possiblities
//and draw the edges for system entry and response including theoretical Xsite and clientside boots.... 
	
$myTestObj = new grandChild('someVal1');
$myTestObj2 = new grandChild('someVal2');

echo $myTestObj->getFullName();
$myTestObj->printInstance();
echo $myTestObj2->getFullName();
$myTestObj2->printInstance();

echo 'NOW FOR THE QUERY BUILD';

//common:: List global libraries, list system libraries, list local libraries, list local modules, list views, 
	
//The path we will use for this experiment
$path = "/systemadmin/maintainance/removesystems.php?shawn=me?thomasbell=scientist";
$path4 = "/systemadmin/maintainance/tools/createNewSystem";
$path3 = "/systemadmin/maintainance/tools/addPrivateLibrary";
$path5 = "/systemadmin/maintainance/tools/addSharedLibrary";
$path6 = "/systemadmin/maintainance/views/normalInterface";
$path7 = "/systemadmin/maintainance/views/ajaxInterface";
$path8 = "/systemadmin/maintainance/tools/addModule";
$path9 =  "/systemadmin/maintainance/tools/addView";

//

echo "THE URL: ".$_SERVER['REQUEST_URI'];
//Another possible path not considered yet
$path2 = "/systemadmin/statistics/graphs";
//Another possible path yet
$path3 = "/systemadmin/maintainance/addlibrary";
$path4 = "/systemadmin/maintainance/createNewSystem";

$url_apart = explode('/', $path);

$discriminant = $url_apart[0];

//$url_apart_no_discriminant = array_shift($url_apart);
//print_r($url_apart_no_discriminant);

unset($url_apart[0]);

array_shift($url_apart);

echo $url_apart[0];

//reset ($url_apart);
print_r($url_apart);
 

$url_apart = array_values($url_apart);

$url_apart_reversed = array_reverse($url_apart);

			$extra_parrams = explode('?', $url_apart_reversed[0]);
			echo "EXTRA PARRAMS ".$extra_parrams[0];

print_r($url_apart);
print_r($url_apart_reversed);

$maxDepth = count($url_apart_reversed)-1;

echo $maxDepth;

$tblAliasBaseNam = "deferredBy";
$tblNam = "items";

/*
SELECT po.OrderID, p.LastName, p.FirstName
FROM Persons AS p,
Product_Orders AS po
WHERE p.LastName='Hansen' AND p.FirstName='Ola'
*/

$qryStr = "SELECT ";
$whrClause = " WHERE ";
$whrClauseSecHalf = "";
$idSwitch = 0;
    
	echo $whrClause;
	echo '<br/>';
	echo $qryStr;

foreach($url_apart_reversed as $key => $value){
/*
What the query should look like
SELECT deferredBy0.BUILD_ITEM_REQ_NAME, deferredBy0.fk_BUILD_ITEM_PARENT_DIR FROM build_items AS deferredBy0, build_items AS deferredBy1 WHERE deferredBy0.BUILD_ITEM_REQ_NAME = 'nothing' AND deferredBy0.fk_BUILD_ITEM_PARENT_DIR = 'maintainance' AND deferredBy0.fk_BUILD_ITEM_PARENT_ID = deferredBy1.pk_BUILD_ITEM_ID AND deferredBy1.BUILD_ITEM_REQ_NAME = 'maintainance' AND deferredBy1.fk_BUILD_ITEM_PARENT_ID IS NULL
*/
     if($key == 0 && $maxDepth >= 1){
	 /*echo 'inside first if';
	 $qryStr = $qryStr." some val to test";
	 echo $qryStr;*/
$qryStr = $qryStr.$tblAliasBaseNam.$key.".ITEM_REQ_NAME, ".$tblAliasBaseNam.$key.".ITEM_INCLUDE_PATH FROM build_items AS ".$tblAliasBaseNam.$key;
//echo $qryStr;
$whrClause = $whrClause.$tblAliasBaseNam.$key.".ITEM_REQ_NAME = '".$value."' AND ".$tblAliasBaseNam.$key.".fk_ITEM_PARENT_DIR = '".$url_apart_reversed[$key+1]."' AND ".$tblAliasBaseNam.$key.".fk_ITEM_PARENT_ID = ";
	
	}
     if($key > 0){
	$qryStr = $qryStr.", items AS ".$tblAliasBaseNam.$key;
		if($key != $maxDepth){
			$whrClause = $whrClause.$tblAliasBaseNam.$key.".pk_ITEM_ID AND ".$tblAliasBaseNam.$key.".ITEM_REQ_NAME = ".$value." AND ".$tblAliasBaseNam.$key.".parentNam = ".$url_apart_reversed[$key+1]." AND ".$tblAliasBaseNam.$key.".parentID = ";
		
		}
		if($key == $maxDepth){
		$whrClause = $whrClause.$tblAliasBaseNam.$key.".itemID AND ".$tblAliasBaseNam.$key.".itemNam = ".$value." AND ".$tblAliasBaseNam.$key.".parentID = 'none'";
			
		}
	}
	

}


	echo '<br/>';
	echo $qryStr;
	echo '<br/>';
	echo $whrClause;
	echo '<br/>';
	echo '<br/>';
	echo 'What the query should look like';
	echo '<br/>';
	echo "SELECT deferredBy0.BUILD_ITEM_REQ_NAME, deferredBy0.fk_BUILD_ITEM_PARENT_DIR FROM build_items AS deferredBy0, build_items AS deferredBy1 WHERE deferredBy0.BUILD_ITEM_REQ_NAME = 'nothing' AND deferredBy0.fk_BUILD_ITEM_PARENT_DIR = 'maintainance' AND deferredBy0.fk_BUILD_ITEM_PARENT_ID = deferredBy1.pk_BUILD_ITEM_ID AND deferredBy1.BUILD_ITEM_REQ_NAME = 'maintainance' AND deferredBy1.fk_BUILD_ITEM_PARENT_ID IS NULL";
		
		if (session_id() == ''){
			session_start();
		} 
		
		
		$sessID = "9832767899569";
		$sessUID = "8765321";
		$tbID = "27";
		$sessActive = 1;
		$uID = "0";
		
		if(!isset($_COOKIE['SESSINF']) || !isset($_COOKIE['SESSINF']['sessID']) || !isset($_COOKIE['SESSINF']['tbID']) || !isset($_COOKIE['SESSINF']['UID'])){
		setcookie("SESSINF[sessID]",$sessID,time()+3600*24,"/");
	    setcookie("SESSINF[UID]",$sessUID,time()+3600*24,"/") ;
		setcookie("SESSINF[tbID]",$tbID,time()+3600*24,"/") ;
		setcookie("SESSINF[DOGGY]",$tbID,time()+3600*24,"/") ;
		setcookie("SESSINF[COUNT]",1,time()+3600*24,"/") ;
		}
		
		if(isset($_COOKIE['SESSINF']['COUNT'])){
		
			$newCookVal = $_COOKIE['SESSINF']['COUNT']+1;
			setcookie("SESSINF[COUNT]",$newCookVal,time()+3600*24,"/") ;
			echo $newCookVal."WHATSUP";
		
		}
		
	echo strlen($sessID); 

	echo '<br/>';
	






?>

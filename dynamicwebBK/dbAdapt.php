<?PHP

class dbAdapter extends appSettings{

protected static $conn='';
protected static $connOpen = false;
protected static $fh;
protected static $logOpen = false;

	public function __construct(){
	}
	
	protected function opnDb(){
	/********************************************
	Opens the database and sets it to a static
	variable that is accessable to all children 
	of this class.
	*********************************************/

		if(self::$connOpen == false){
		self::$conn = mysql_connect(info::$dbServer, info::$dbUser,info::$dbPass);
		//self::$conn = mysql_connect(appSettings::getDbSrv(), appSettings::getDbUsr(),appSettings::getDbPass());
		self::$connOpen =  true;
		return true;
		}else{
		
		return true;
		}
	}
	
	protected function clsDb(){
	}
	
	protected function clnQry($string){
	/************************************************
	Cleans one variable at a time for entry
	into database
	*************************************************/
	  if(get_magic_quotes_gpc()){
		$string = stripslashes($string);
	  }
	  if (phpversion() >= '4.3.0'){
		$string = mysql_escape_string($string);
	  }
	  else{
		$string = mysql_escape_string($string);
	  }
	  return $string;
	}
	
	protected function exeQuery($qStr){
		mysql_select_db("web_tools", self::$conn);
		mysql_select_db(info::dbSchema, self::$conn);
		$rs = mysql_query($qStr);
		return $rs;
	}


}


?>
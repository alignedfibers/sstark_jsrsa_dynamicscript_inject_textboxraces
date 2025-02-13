<?PHP
	crtDir();
	$target_path = "";
	if ($_FILES["hiddenUpFile"]["error"] > 0){
		echo "Error: " . $_FILES["hiddenUpFile"]["error"];
		}else{ 
			echo "Upload: " . $_FILES["hiddenUpFile"]["name"] . "<br />";
	  		echo "Type: " . $_FILES["hiddenUpFile"]["type"] . "<br />";
	  		echo "Size: " . ($_FILES["hiddenUpFile"]["size"] / 1024) . " Kb<br />";
	  		echo "Stored in: " . $_FILES["hiddenUpFile"]["tmp_name"];
			
		}
		if (file_exists(getenv("DOCUMENT_ROOT")."data10/".$_FILES["hiddenUpFile"]["name"]))
		  {
		  echo $_FILES["hiddenUpFile"]["name"] . " already exists. ";
		  }
		else
		  {
		  move_uploaded_file($_FILES["hiddenUpFile"]["tmp_name"],
		  getenv("DOCUMENT_ROOT")."data10/" . $_FILES["hiddenUpFile"]["name"]);
		  echo "Stored in: " . "" . $_FILES["hiddenUpFile"]["name"];
		  }



function crtDir(){ 

			$errMsg = '';
			$returnValue = false;
			if(file_exists('../../data10/data12/')){
				
				$errMsg = "Directory Exists";
				$returnValue = false;
			}else{
				$holdit = "../../data10/data12/";
					mkdir($holdit, 0777);
				$returnValue = true;
echo $holdit;
			}
		return $returnValue;
		}
		
		
?>
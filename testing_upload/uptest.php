<?php
//Sample dataz

/*$usr =  new my_user;
if(!$usr->load()){
redirect
}*/
$userRoot = getenv("DOCUMENT_ROOT").$user->getRoot();
echo getenv("DOCUMENT_ROOT");
print_r($_FILES);

if ($_FILES["hiddenUpFile"]["error"] > 0)
  {
  echo "Error: " . $_FILES["hiddenUpFile"]["error"] . "<br />";
  }
else
  {
  echo "Upload: " . $_FILES["hiddenUpFile"]["name"] . "<br />";
  echo "Type: " . $_FILES["hiddenUpFile"]["type"] . "<br />";
  echo "Size: " . ($_FILES["hiddenUpFile"]["size"] / 1024) . " Kb<br />";
  echo "Stored in: " . $_FILES["hiddenUpFile"]["tmp_name"];

    if (file_exists("/" . $_FILES["hiddenUpFile"]["name"]))
      {
      echo $_FILES["hiddenUpFile"]["name"] . " already exists. ";
      }
    else
      {
      move_uploaded_file($_FILES["hiddenUpFile"]["tmp_name"],
      "/" . $_FILES["hiddenUpFile"]["name"]);
      echo "Stored in: " . "/" . $_FILES["hiddenUpFile"]["name"];
      }
}
?>

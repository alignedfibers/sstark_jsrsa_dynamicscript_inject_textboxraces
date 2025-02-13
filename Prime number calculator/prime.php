<?

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                         //
// COPYRIGHT                                                                                               //
//                                                                                                         //
//  This software is developed and copyrighted by Xantus WEBdevelopment.                                   //
//                                                                                                         //
//                                                                                                         //
// TERMS OF USE                                                                                            //
//                                                                                                         //
//  This script may be used free for personal use. The copyright MUST remain intact.                       //
//  Please contact us if you want to have the copyright removed.                                           //
//                                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

?>

<h3 align="center">Prime number calculator</h3><form name="form" method="post" action="prime.php" enctype="multipart/form-data">
<table border="0" width="100%" bordercolor="#000000" style="border-style: solid; border:  #449944; border-width: 1">
<tr>
<td width="40%" align="right"><b>Calculate prime number until:</b></td>
<td width="60%">
<input type="text" name="count" size="20" value="10000"></td>
</tr>
<tr>
<td width="100%" colspan="2">
<p align="center">
<input type="submit" value="Submit" name="B1">
</td>
</tr>
</table></form>
<div align=center><a style="color: green; font-size: 11px; text-decoration: none;" href="http://www.xantus-webdevelopment.com">Tool by - &copy; Xantus Webdesign </a>
</div>
<?
if ($_POST['count'] != ""){
 if (is_numeric($_POST['count'])){
  $time_start = microtime(1);
  $x = $_POST['count'];
  for($i=1;$i<=$x;$i++) { 
   $f=sqrt($i);
   for($z=1;$z<=$f;$z++) { 
    $y=$i%$z;
    if($y==0){
     $l++;
    }
    if($l>=2){
     break;
    }
   }
   if ($l<=1){ 
    echo "$i, ";
    $b++; 
   }
   $l=0;
  }
  $time_end = microtime(1);
  $time = $time_end - $time_start;
  echo "<br><b>[ Found first $x prime numbers in $time seconds ]</b>";
 }
 else
 {
  echo "<br><b>Please enter a number!</b>";
 }
}
?>
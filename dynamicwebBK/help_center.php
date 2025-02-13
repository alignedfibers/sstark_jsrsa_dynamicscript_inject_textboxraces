<?PHP
/********************************************
The helpcenter is where users will be able to 
navigate the help menus and guest approved forums 
plus view an advertisement to become a member 
if they arn't logged in, if they are logged in a
random advertisement of content on this site will
be provided based on the users view history after
the table for it gets implimented. The stats will
result from category views and views of user profiles.
This page will also provide current number of regular 
members, upgraded members, moderators, and administrators 
a link to download this application will also be provided 
here. When a user is logged in they will 
*********************************************/
	$pageStatus = false;
	
	/********************************************
	Include the whole application library
	*********************************************/
	require_once('../includes/config.php');
	require_once('../includes/transport_objects.php');
	require_once('../includes/appSettings.php');
	require_once('../includes/dbAdapt.php');
	require_once('../includes/rsa.php');
	require_once('../includes/encryptionclass.php');
	require_once('../includes/sessMan.php');
	require_once('../includes/security_suite.php');
	require_once('../includes/manipulators.php');
	require_once('../includes/testinggrandparent.php');
	//require_once('../testbed/rsa.php');
	
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
	$pageDetails->newOverride('Shawn Stark', 'help_center.php', 'Just some help information will eventually use db', 'v0.1', 'rev0', 'today', 'today');
	
	/********************************************
	Run a few security measures and register this 
	page info to the logging system to track what 
	is and isn't being run and any suspiciouse 
	activities that may be occuring
	*********************************************/
	
	if($securePage->barracadeIt($pageDetails)){
		$pageStatus = true;
		//echo 'page is secure';
	}else{$pageStatus = false; 
	header('Location: '.$securePage->getZeroAccessURL());}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" dir="ltr" lang="en-US">
<!--Copy Right Shawn Nicholas Stark -->
<head>
<title>OJT Help Center</title>
	<style type="text/css" media="screen">
		@import url(../style_it.css);
	</style>

<script type="text/javascript">
<!-- //Not currently used but should run onload to add corners to every container
function wrtCrnDivs(){
document.write('<div id="tlCorner"/><div id="blCorner"/><div id="trCorner"/><div id="brCorner"/>')
}
-->
</script>
</head>
<body>
<div id="pgCntnr">

<div id="manifest" class="frstLvlCntnr">

    <!--###########Mnfst Corners#################-->
                 <!--I can't wait until css3 is supported.-->
                <div id="topBg"></div>
	<div id="leftBg"></div><div id="tlCorner"></div><div id="blCorner"></div>
                <div id="rightBg"></div><div id="trCorner"></div><div id="brCorner"></div>
                <div id="btmBg"></div>
<div class="clearBoth" style="clear:both;"></div>
    <!--############Mnfst Row 1##################-->
	<div id="mnfstRowOne">
<?PHP
echo $_SESSION['jtUsr']['ACTIVE'];
	if(!$pageStatus || $_SESSION['jtUsr']['TYPE'] != 'guest'){
	
	?>
	           <div style="float:right; position:absolute; right:0px; bottom:0px; color:#008787;"><strong>Welcome:<?PHP echo ' '.$_SESSION['jtUsr']['UN'].' '; ?>&nbsp;&nbsp;|<a href="<?php echo $securePage->getUserLogOutToolURL(); ?>">Logout</a></strong></div>     
	
	<?PHP }else{ ?> 
    
			<div style="float:right; position:absolute; right:0px; bottom:0px; color:#008787;"><strong>Welcome:<?PHP echo ' '.$_SESSION['jtUsr']['UN'].' '; ?>&nbsp;&nbsp;Please:<a href="<?PHP echo $securePage->getUserLoginPageURL(); ?>">Login</a>&nbsp;OR&nbsp;<a href="<?PHP echo $securePage->getUserRegisterPageURL(); ?>">Register</a></strong></div> 
<?PHP } ?>       
	

	</div>
<div class="clearBoth" style="clear:both;"></div>
   <!--############Mnfst Row 2##################-->
	<div id="mnfstRowTwo">
		
		<strong>Online Java Tools</strong>

	</div>
<div class="clearBoth" style="clear:both;"></div>
   <!--############Mnfst Row 3##################-->

		<div id="mnfstRowThree">
		<div id="mainNavList">
			<li><a href="memberlist.php">Member Listing</a></li>
			<li><a href="mylibrary.php">Builds&nbsp;&amp;&nbsp;Management  </a></li>
			<li><a href="myaccount.php">Profile & Settings</a></li>
			<li><a href="helpcenter.php">Help Center</a></li>
			<li><a href="analystsuite.php">Analyst Suite</a></li>
<div class="clearBoth" style="clear:both;"></div>
		</div>
		<div class="clearBoth" style="clear:both;"></div>
		</div>
		<div id='page_title'>
		<center>The Help Center</center>
		</div>
		<div id="mnfstRowFour">
		<strong>
		
			<a href="install_instructions.php">Install Instructions</a>&nbsp;|
			<a href="register_instructions.php">How do I Register</a>&nbsp;|
			<a href="about.php">About OJT</a>&nbsp;|
			<a href="ojtblog.php">OJT Blog</a>
		
			</strong>
		</div>
		
	
     <!--########End of Mnfst Content##############-->

</div>






<div id="kate" class="frstLvlCntnr">







<div id="hlpIndx" class="scndLvlCntnrLeft">
<!--<div id="tlCorner"></div><div id="blCorner"></div><div id="trCorner"></div><div id="brCorner"></div>-->
</div>



<div id="entryCntnr" class="2ndLvlCntnrMdl">

<div class="eBreak"> Just A space between entries</div>
<div class="entry slvCntnr"><!--Slave container: sizes horizon to daddy-->
<div class="entryTitle">Please become a member: written by Shawn Stark Last Update March 3rd 2009</div>
<div class="entryContent">Become involved and have access to libraries of java code while creating 
                          your own libraries. If you choose not to become a member you are SOL as any
						  attempt to access features of this site will fail. Current features include
						  browsing member profiles, setting up your own profile, organizing and building 
						  your own java libraries, java source upload options, java source and class file 
						  downloads. Each member gets a default library that includes a hello world java 
						  build, and library of standard PHP examples. I hope to make the php examples entirely 
						  runable. Due to some security issues you will not be able to upload PHP files. 
						  There are many new oportunities and content management systems that may derive 
						  from this software. The more involvment the stronger the development will get.
						  The goal is to make the new developers life easier, safe, and secure, With online 
						  accessable compiler facilities, Data organization and storage, by creating a single 
						  entry point for retrieving files or html documents tracking usage, and enforcing 
						  security becomes an easier accomplishment. This also allows the site to provide
						  specifically to the user. So please become a member, your input is important.
						  </div>


</div>
<div class="eBreak"> Just A space between entries</div>
<div class="entry slvCntnr"><!--Slave container: sizes horizon to daddy-->
<div class="entryTitle">About OJT: written by Shawn Stark Last Update March 3rd 2009</div>
<div class="entryContent">OJT stands for Online Java Tools, As a recomendation I wouldn't suggest that
                          anyone allows web scripts to run executables on the server. Which is exactly 
						  what this does, allows you to compile javacode. Amongst other things includeing
						  organization and such. The original intent was to allow users to edit and compile java
						  beans, jsp, php and java applications. .NET, ASP or other languages have not yet been
						  taken into consideration. The benifits of this are direct access to needed programing
						  facilities from anywhere, from mobile phones to computers with restricted access or 
						  systems without the needed development environments installed.</div>


</div>

</div>


</div> 








</div>
</body>
</html>

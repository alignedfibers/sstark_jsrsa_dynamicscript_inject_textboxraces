
<html>

<center>
<?PHP echo 'server directory'.getenv("DOCUMENT_ROOT"); ?>

<form enctype="multipart/form-data" action="uploader.php" method="POST">
<input type="hidden" name="MAX_FILE_SIZE" value="100000" />
Choose a file to upload: <input name="hiddenUpFile" type="file" /><br />
<input type="submit" value="Upload File" />
</form>
</center>
</html>


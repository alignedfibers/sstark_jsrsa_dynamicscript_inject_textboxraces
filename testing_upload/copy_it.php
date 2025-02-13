<?php
$file = 'Student_Time_Proposal.doc';
$newfile = '../student_time.doc';

if (!copy($file, $newfile)) {
    echo "failed to copy $file...\n";
}
?> 
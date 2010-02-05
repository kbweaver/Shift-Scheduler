<h2>My Sub Request Statistics</h2>

<?php

    $sub_req_query = @mysql_query("SELECT count(*) AS requested FROM Sub WHERE requestor='$username'");
    $q = @mysql_fetch_array($sub_req_query);
    
    $sub_taken_query = @mysql_query("SELECT count(*) AS taken FROM Sub WHERE filled_by='$username'");
    $r = @mysql_fetch_array($sub_taken_query);
    
    $requested = (float)$q['requested'] * (float)$resolution;
    $taken = (float)$r['taken'] * (float)$resolution;
    $ratio = ($requested == 0) ? (($taken == 0) ? "undefined" : "&#8734;") : ($taken / $requested);
    
    echo "Taken: $taken hours<br />";
    echo "Requested: $requested hours<br />";
    echo "Ratio: $ratio";

?>

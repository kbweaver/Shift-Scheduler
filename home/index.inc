<h2>Upcoming Shifts and Sub Requests</h2>

<form action="./index.php?p=subs" method="post">
<input type="submit" name="take" value="Take Shift" />

<?php

    function foo($row) {
        return $row['username'];
    }
    /*function bar($array, $context) {
        foreach ($array as $element) {
            $sub_result = @mysql_query("SELECT * FROM Sub WHERE start_datetime='$context' AND requestor='$element'");
            if ($row = @mysql_fetch_array($sub_result)) {
                if ($row['filled_by'] != null) {
                    $return_array[] = "<span class=\"sub_taken\">" . get_shortname($row['filled_by']) . "</span>";
                } else {
                    $return_array[] = "<span class=\"sub_req\">" . get_shortname($element) . "</span>";
                }
            } else {
                $return_array[] = get_shortname($element);
            }
        }
        return implode(" / ", $return_array);
    }*/
    
    function bar($array, $context) {
        global $username;
        foreach ($array as $element) {
            $sub_result = @mysql_query("SELECT * FROM Sub WHERE start_datetime='$context' AND requestor='$element'");
            if ($row = @mysql_fetch_array($sub_result)) {
                if ($row['filled_by'] != null) {
                    $return_array[] = "<label><span class=\"sub_taken\">" . get_shortname($row['filled_by']) . "</span></label>";
                } else {
                    $return_array[] = "<label><span class=\"sub_req\" title=\"{$row['reason']}\"><input type=\"checkbox\" name=\"sub[]\" value=\"$element $context\"/>" . get_shortname($element) . "</span></label>";
                }
            } else {
                $return_array[] = get_shortname($element);
            }
        }
        return implode(" / ", $return_array);
    }
    print_rolling_week(date("Y-m-d H:i:00"), "", "SELECT * FROM Shift", 'foo', 'bar');

?>

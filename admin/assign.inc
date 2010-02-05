<?php
    if (!$is_admin) {
        echo "You should not be here.";
        exit (1);
    }
?>

<?php

    $query = "SELECT * FROM Shift";
    $column = "username";
    
    //TODO: constrain by capacity
    if (isset($_POST['add'])) {
        foreach ($_POST['s_dayofweek'] as $dayofweek) {
            foreach ($_POST['s_starttime'] as $starttime) {
                @mysql_query("INSERT INTO Shift VALUES ('{$_POST['s_username']}', '$dayofweek', '$starttime')");
            }
        }
    }
    
    if (isset($_POST['delete'])) {
        foreach ($_POST['s_dayofweek'] as $dayofweek) {
            foreach ($_POST['s_starttime'] as $starttime) {
                @mysql_query("DELETE FROM Shift WHERE username='{$_POST['s_username']}' AND day_of_week='$dayofweek' AND start_time='$starttime'");
            }
        }
    }
    
    if (isset($_POST['view_request'])) {
        if (isset($_POST['s_dayofweek']) && isset($_POST['s_starttime'])) {
            $r_days = implode("' OR day_of_week='", $_POST['s_dayofweek']);
            $r_times = implode("' OR start_time='", $_POST['s_starttime']);
            $query = "SELECT * FROM Shift_Request WHERE preference!='Unavailable' AND (day_of_week='$r_days') AND (start_time='$r_times')";
            $column = "username preference";
        } else {
            $query = "SELECT * FROM Shift_Request WHERE username='{$_POST['s_username']}'";
            $column = "preference";
        }
    }
    
?>

<form action="index.php?p=admin/assign" method="post">

<select multiple name="s_dayofweek[]">
<?php
    foreach ($days as $day) {
        echo "<option value=\"$day\">$day</option>";
    }
?>
</select>

<select multiple size="7" name="s_starttime[]" />
<?php
    foreach ($times as $time) {
        $formatted_time = format_time($time);
        echo "<option value=\"$time\">$formatted_time</option>";
    }
?>
</select>

<select name="s_username">
<?php
    foreach ($emps as $emp_username=>$emp_shortname) {
        echo "<option value=\"$emp_username\">$emp_shortname</option>";
    }
?>
</select>

<input type="submit" name="add" value="Add Shift" />
<input type="submit" name="delete" value="Delete Shift" />
<input type="submit" name="view_request" value="View Requests" />

<?php

    if ($column == "preference") {
        $shortname = get_shortname($_POST['s_username']);
        echo "<h3>$shortname's Requests</h3>";
    }

    function foo($row) {
        global $column;
        if ($column == "username preference") {
            return get_shortname($row['username']) . "({$row['preference']})";
        }
        return $row[$column];
    }
    function bar($array) {
        global $column;
        if ($column == "username") {
            return implode('/', array_map('get_shortname', $array));
        } elseif ($column == "username preference") {
            return implode('/', $array);
        } else {
            return $array[0];
        }
    }
    print_weekly_calendar("", $query, 'foo', 'bar', "");
?>

<table border="1">
    <tr>
        <th>Name</th>
        <th>Hours</th>
    </tr>
<?php
    $shift_result = @mysql_query("SELECT username, count(username) AS hours FROM Shift GROUP BY username ORDER BY hours DESC");
    while ($row = @mysql_fetch_array($shift_result)) {
        echo "<tr>";
        echo "<td>";
        echo get_shortname($row['username']);
        echo "</td>";
        echo "<td>";
        echo (int)$row['hours'] * $resolution;
        echo "</td>";
        echo "</tr>";
    }
    echo "<tr>";
    $shift_result = @mysql_query("SELECT count(username) AS hours FROM Shift");
    $row = @mysql_fetch_array($shift_result);
    echo "<th>Total</th>";
    echo "<td>";
    echo (int)$row['hours'] * $resolution;
    echo "</td>";
    echo "</tr>";
?>
</table>

</form>

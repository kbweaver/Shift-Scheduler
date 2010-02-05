<?php

    if (isset($_POST['add'])) {
        foreach ($_POST['rs_dayofweek'] as $dayofweek) {
            foreach ($_POST['rs_starttime'] as $starttime) {
                @mysql_query("INSERT INTO Shift_Request VALUES ('$username', '$dayofweek', '$starttime', '{$_POST['rs_preference']}')");
            }
        }
    }
    
    if (isset($_POST['delete'])) {
        foreach ($_POST['rs_dayofweek'] as $dayofweek) {
            foreach ($_POST['rs_starttime'] as $starttime) {
                @mysql_query("DELETE FROM Shift_Request WHERE username='$username' AND day_of_week='$dayofweek' AND start_time='$starttime'");
            }
        }
    }
    
    if (isset($_POST['update'])) {
        foreach ($_POST['rs_dayofweek'] as $dayofweek) {
            foreach ($_POST['rs_starttime'] as $starttime) {
                @mysql_query("UPDATE Shift_Request SET preference='{$_POST['rs_preference']}' WHERE username='$username' AND day_of_week='$dayofweek' AND start_time='$starttime'");
            }
        }
    }
?>

<form action="index.php?p=schedule/request" method="post">

<select multiple name="rs_dayofweek[]">
<?php
    foreach ($days as $day) {
        echo "<option value=\"$day\">$day</option>";
    }
?>
</select>

<select multiple size="7" name="rs_starttime[]" />
<?php
    foreach ($times as $time) {
        $formatted_time = format_time($time);
        echo "<option value=\"$time\">$formatted_time</option>";
    }
?>
</select>
Preference:
<select name="rs_preference">
    <option>High</option>
    <option>Medium</option>
    <option>Low</option>
    <option>Unavailable</option>
</select>

<input type="submit" name="add" value="Add Shift" />
<input type="submit" name="delete" value="Delete Shift" />
<input type="submit" name="update" value="Update Preference" />

<?php
    function foo($row) {
        return $row['preference'];
    }
    function bar($array) {
        foreach ($array as $element) {
            return "$element";
        }
    }
    print_weekly_calendar("", "SELECT * FROM Shift_Request WHERE username='$username'", 'foo', 'bar', "");
?>

</form>

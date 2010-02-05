<?php
    if (!$is_admin) {
        echo "You should not be here.";
        exit (1);
    }
?>

<?php

    if (isset($_POST['add'])) {
        foreach ($_POST['ws_dayofweek'] as $dayofweek) {
            foreach ($_POST['ws_starttime'] as $starttime) {
                @mysql_query("INSERT INTO Weekly_Shift VALUES ('$dayofweek', '$starttime', {$_POST['ws_capacity']})");
            }
        }
    }
    
    if (isset($_POST['delete'])) {
        foreach ($_POST['ws_dayofweek'] as $dayofweek) {
            foreach ($_POST['ws_starttime'] as $starttime) {
                @mysql_query("DELETE FROM Weekly_Shift WHERE day_of_week='$dayofweek' AND start_time='$starttime:00'");
            }
        }
    }
    
    if (isset($_POST['update'])) {
        foreach ($_POST['ws_dayofweek'] as $dayofweek) {
            foreach ($_POST['ws_starttime'] as $starttime) {
                @mysql_query("UPDATE Weekly_Shift SET capacity={$_POST['ws_capacity']} WHERE day_of_week='$dayofweek' AND start_time='$starttime:00'");
            }
        }
    }
?>

<form action="index.php?p=admin/weekly_shifts" method="post">

<select multiple name="ws_dayofweek[]">
    <option>Monday</option>
    <option>Tuesday</option>
    <option>Wednesday</option>
    <option>Thursday</option>
    <option>Friday</option>
    <option>Saturday</option>
    <option>Sunday</option>
</select>

<select multiple size="7" name="ws_starttime[]" />
<?php
    $hour = 0;
    $minute = 0;
    while ($hour < 24) {
        echo "<option value=\"$hour:$minute\">" . date("g:i A", mktime($hour, $minute)) . "</option>";
        $minute += $resolution * 60;
        if ($minute >= 60) {
            $hour++;
            $minute = 0;
        }
    }
?>
</select>

Capacity: <input type="text" name="ws_capacity" />

<input type="submit" name="add" value="Add Shift" />
<input type="submit" name="delete" value="Delete Shift" />
<input type="submit" name="update" value="Update Capacity" />

<?php
    function foo($row) {
        return $row['capacity'];
    }
    function bar($array) {
        foreach ($array as $element) {
            return $element;
        }
    }
    print_weekly_calendar("", "SELECT * FROM Weekly_Shift", 'foo', 'bar', "");
?>

</form>

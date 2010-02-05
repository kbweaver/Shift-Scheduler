<h2>Schedule Appointments</h2>

<?php

    if (!isset($_POST['start_date'])) {
        $start = date("Y-m-d H:i:00");
    } else {
        $start = $_POST['start_date'];
    }
    
    if (isset($_POST['prev_week'])) {
        $start = add_date($start, 0, 0, 0, 0, -7, 0);
    }
    
    if (isset($_POST['next_week'])) {
        $start = add_date($start, 0, 0, 0, 0, 7, 0);
    }
    
    if (isset($_POST['goto_today'])) {
        $start = date("Y-m-d H:i:00");
    }
    
    if (isset($_POST['schedule'])) {
        foreach ($_POST['appt'] as $appt) {
            $appt_array = explode(" ", $appt);
            @mysql_query("INSERT INTO Appointment VALUES ('{$appt_array[1]} {$appt_array[2]}', '{$appt_array[0]}', '{$_POST['reason']}')");
        }
    }
    
    if (isset($_POST['remove'])) {
        foreach ($_POST['appt'] as $appt) {
            $appt_array = explode(" ", $appt);
            @mysql_query("DELETE FROM Appointment WHERE start_datetime='{$appt_array[1]} {$appt_array[2]}' AND username='{$appt_array[0]}'");
        }
    }
?>

<form action="./index.php?p=appointments" method="post">
Reason: <input type="text" name="reason" size="60" maxlength="255" />
<input type="submit" name="schedule" value="Schedule Appointment" />
<input type="submit" name="remove" value="Remove Appointment" />

<p>Mouse over an appointment to view the reason.</p>

<input type="submit" name="prev_week" value="<<" />
<input type="submit" name="next_week" value=">>" />
<input type="submit" name="goto_today" value="Today" />
<input type="hidden" name="start_date" value="<?php echo $start; ?>" />

<?php

    function foo($row) {
        return $row['username'];
    }
    function bar($array, $context) {
        global $username;
        foreach ($array as $element) {
            $appt_result = @mysql_query("SELECT * FROM Appointment WHERE start_datetime='$context' AND username='$element'");
            if ($row = @mysql_fetch_array($appt_result)) {
                $return_array[] = "<span class=\"appt\" title=\"{$row['reason']}\"><label><input type=\"checkbox\" name=\"appt[]\" value=\"$element $context\"/>" . get_shortname($element) . "</label></span>";
            } else {
                $return_array[] = "<label><input type=\"checkbox\" name=\"appt[]\" value=\"$element $context\"/>" . get_shortname($element) . "</label>";
            }
        }
        return implode(" / ", $return_array);
    }
    print_rolling_week($start, "", "SELECT * FROM Shift", 'foo', 'bar');

?>

</form>

<h2>Sub Requests</h2>

<?php

    if (!isset($_POST['start_date'])) {
        $start = date("Y-m-d H:i:00");
    } else {
        $start = $_POST['start_date'];
    }
        
    $email_footer = "To take action on this shift, go to http://helpdesk.unet.brandeis.edu/index.php?p=subs";

    if (isset($_POST['request']) && !empty($_POST['reason'])) {
        foreach ($_POST['sub'] as $sub) {
            $sub_array = explode(" ", $sub);
            if ($sub_array[0] == $username) {
                if (@mysql_query("INSERT INTO Sub VALUES ('{$sub_array[1]} {$sub_array[2]}', '$username', '{$_POST['reason']}', null)")) {
                    $date = strtotime("{$sub_array[1]}");
                    if (!in_array($date, $dates)) {
                        $dates[] = $date;
                        $formatted_dates[] = date("m/d", strtotime("{$sub_array[1]} {$sub_array[2]}"));
                    }
                } else {
                    die("Error: request failed");
                }
                $formatted_datetimes[] = date("D m/d g:i A", strtotime("{$sub_array[1]} {$sub_array[2]}"));
            }
        }
        $dateslist = implode(", ", $formatted_dates);
        $datetimeslist = implode("\n", $formatted_datetimes);
        $shortname = get_shortname($username);
        mail($sub_email, "$shortname needs shift coverage on $dateslist", "$shortname needs the following shifts covered:\n\n$datetimeslist\n\nReason: {$_POST['reason']}\n\n\n$email_footer", "From: $username@brandeis.edu");
    }
    
    if (isset($_POST['take'])) {
        foreach ($_POST['sub'] as $sub) {
            $sub_array = explode(" ", $sub);
            if ($sub_array[0] != $username) {
				if (!in_array($sub_array[0], $requestors)) {
					$requestors[] = $sub_array[0];
				}
				$result = @mysql_query("SELECT * FROM Sub WHERE start_datetime='{$sub_array[1]} {$sub_array[2]}' AND requestor='{$sub_array[0]}'");
				$row = @mysql_fetch_array($result);
				if ($row['filled_by'] == null) {
                    @mysql_query("UPDATE Sub SET filled_by='$username' WHERE start_datetime='{$sub_array[1]} {$sub_array[2]}' AND requestor='{$sub_array[0]}'");
                }
            } else {
                @mysql_query("DELETE FROM Sub WHERE start_datetime='{$sub_array[1]} {$sub_array[2]}' AND requestor='$username'");
            }
        }
        foreach ($requestors as $requestor) {
            $shortname = get_shortname($username);
            mail("$requestor@brandeis.edu", "$shortname has taken one or more of your shifts", "Please go to http://helpdesk.unet.brandeis.edu/index.php?p=subs to review which shifts have been filled.", "From: $username@brandeis.edu");
        }
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
?>

<div style="float: left; display: inline; width: 80%;">

<form action="./index.php?p=subs" method="post">
Reason: <input type="text" name="reason" size="60" maxlength="255" />
<input type="submit" name="request" value="Request Sub" />
<input type="submit" name="take" value="Take Shift" />

<p>Mouse over a sub request to view the reason.</p>

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
            $sub_result = @mysql_query("SELECT * FROM Sub WHERE start_datetime='$context' AND requestor='$element'");
            if ($row = @mysql_fetch_array($sub_result)) {
                if ($row['filled_by'] != null) {
                    $return_array[] = "<span class=\"sub_taken\"><input type=\"checkbox\" name=\"sub[]\" value=\"$element $context\"/>" . get_shortname($row['filled_by']) . "</span>";
                } else {
                    $return_array[] = "<span class=\"sub_req\" title=\"{$row['reason']}\"><input type=\"checkbox\" name=\"sub[]\" value=\"$element $context\"/>" . get_shortname($element) . "</span>";
                }
            } else if ($element == $username) {
                $return_array[] = "<input type=\"checkbox\" name=\"sub[]\" value=\"$element $context\"/>" . get_shortname($element);
            }
        }
        return implode(" / ", $return_array);
    }
    print_rolling_week($start, "", "SELECT * FROM Shift WHERE username='$username' OR username IN (SELECT DISTINCT requestor FROM Sub)", 'foo', 'bar');

?>

</form>

</div>
<div style="float: right; clear: right; width: 18%;">

<h3>Future Requests</h3>

<?php

    echo "<table border=\"1\">";
    echo "<tr>";
    echo "<th>Requestor</th>";
    echo "<th>Date</th>";
    echo "</tr>";
    $next_week = add_date($start, 0, 0, 0, 0, 7, 0);
    $sub_result = @mysql_query("SELECT DISTINCT date(start_datetime) AS date, requestor FROM Sub WHERE filled_by IS NULL AND start_datetime > '$next_week' ORDER BY start_datetime");
    while ($sub = @mysql_fetch_array($sub_result)) {
        echo "<tr>";
        echo "<form action=\"./index.php?p=subs\" method=\"POST\">";
        echo "<td>" . get_shortname($sub['requestor']) . "</td>";
        $formatted_date = date("D, M j", strtotime("{$sub['date']} 00:00:00"));
        echo "<td>$formatted_date</td>";
        echo "<td><input type=\"submit\" name=\"goto\" value=\"Go\"/></td>";
        echo "<input type=\"hidden\" name=\"start_date\" value=\"{$sub['date']} 00:00:00\" />";
        echo "</form>";
        echo "</tr>";
    }
    echo "</table>";

?>

</div>

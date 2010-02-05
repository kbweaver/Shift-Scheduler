<ul class="subnav">
<li><a href="./index.php?p=subs/stats">My Stats</a></li>
</ul>

<h2>Sub Requests</h2>

<?php

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
                @mysql_query("UPDATE Sub SET filled_by='$username' WHERE start_datetime='{$sub_array[1]} {$sub_array[2]}' AND requestor='{$sub_array[0]}'");
            } else {
                @mysql_query("DELETE FROM Sub WHERE start_datetime='{$sub_array[1]} {$sub_array[2]}' AND requestor='$username'");
            }
        }
        foreach ($requestors as $requestor) {
            $shortname = get_shortname($username);
            mail("$requestor@brandeis.edu", "$shortname has taken one or more of your shifts", "Please go to http://helpdesk.unet.brandeis.edu/index.php?p=subs to review which shifts have been filled.", "From: $username@brandeis.edu");
        }
    }
?>

<form action="./index.php?p=subs" method="post">
Reason: <input type="text" name="reason" size="60" maxlength="255" />
<input type="submit" name="request" value="Request Sub" />
<input type="submit" name="take" value="Take Shift" />

<p>Mouse over a sub request to view the reason.</p>

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
    print_rolling_week(date("Y-m-d H:i:00"), "", "SELECT * FROM Shift WHERE username='$username' OR username IN (SELECT DISTINCT requestor FROM Sub)", 'foo', 'bar');

?>

</form>

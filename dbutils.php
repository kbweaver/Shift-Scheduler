<?php

    require_once('./config.php');

	ini_set('display_errors', 1);//'stderr');

	if (!@mysql_connect ($db_host, $db_username, $db_passwd)) {
		die ('<p>Could not connect to the database because: <b>' . mysql_error() . "</b></p>\n");
	} elseif (!@mysql_select_db ($db_name)) {
		die ('<p>Could not select the database because: <b>' . mysql_error() . "</b></p>\n");
	}
	
	$username = $_SERVER['REMOTE_USER'];

	$emp_result = @mysql_query ("SELECT * FROM Employee WHERE username='$username'");
	if (!$emp_result) {
	    die ('You are not in the database. Please contact an administrator.');
	}
	
    $employee = @mysql_fetch_array($emp_result);
    $emp_name = $employee['shortname'];
    $emp_role = $employee['role'];
    $role_result = @mysql_query ("SELECT is_admin FROM Role WHERE name='$emp_role'");
    $role = @mysql_fetch_array($role_result);
    $is_admin = $role['is_admin'];
    
    $emp_result = @mysql_query ("SELECT * FROM Employee");
    while ($emp = @mysql_fetch_array($emp_result)) {
        $emps[$emp['username']] = $emp['shortname'];
    }
    $days_result = @mysql_query("SELECT DISTINCT day_of_week FROM Weekly_Shift ORDER BY day_of_week");
    while ($day = @mysql_fetch_array($days_result)) {
        $days[] = $day['day_of_week'];
    }
    $times_result = @mysql_query("SELECT DISTINCT start_time FROM Weekly_Shift ORDER BY start_time");
    while ($time = @mysql_fetch_array($times_result)) {
        $times[] = $time['start_time'];
    }
    
    function get_shortname($username) {
        global $emps;
        return $emps[$username];
    }
    
    function format_time($time) {
        $time_array = explode(':', $time);
        return date('g:i A', mktime((int)$time_array[0], (int)$time_array[1]));
    }
    
    function add_date($givendate, $hr=0, $min=0, $sec=0, $mth=0, $day=0, $yr=0) {
        $cd = strtotime($givendate);
        return date('Y-m-d H:i:s', mktime(date('H',$cd) + $hr,
                                          date('i',$cd) + $min, 
                                          date('s',$cd) + $sec, 
                                          date('m',$cd) + $mth,
                                          date('d',$cd) + $day, 
                                          date('Y',$cd) + $yr));
    }
    
    function date_diff($date1, $date2) {
        return strtotime($date1) - strtotime($date2);
    }
    
    /* Prints a weekly calendar from any table containing "day_of_week" and "start_time" columns.
     * $where_clause - what rows of the full weekly shift calendar to return
     * $query - content of table, must contain "day_of_week" and "start_time" columns
     * $f - function that takes a database row and returns a single element (to be inserted into an array that is passed to $g)
     * $g - function that takes an array and returns the contents of the table cell
     */
    function print_weekly_calendar($where_clause, $query, $f, $g, $empty) {
        $days_result = @mysql_query("SELECT DISTINCT day_of_week FROM Weekly_Shift $where_clause ORDER BY day_of_week");
        while ($day = @mysql_fetch_array($days_result)) {
            $days[] = $day['day_of_week'];
        }
        $times_result = @mysql_query("SELECT DISTINCT start_time FROM Weekly_Shift $where_clause ORDER BY start_time");
        while ($time = @mysql_fetch_array($times_result)) {
            $times[] = $time['start_time'];
        }
        $query_result = @mysql_query($query);
        while ($row = @mysql_fetch_array($query_result)) {
            $calendar[$row['start_time']][$row['day_of_week']][] = $f($row);
        }
        echo "<table class=\"calendar\">";
        echo "<tr>";
        echo "<th class=\"time\"><span>Time</span></th>";
        foreach ($days as $day) {
            echo "<th>$day</th>";
        }
        echo "</tr>";
        $alt = "alt";
        foreach ($times as $time) {
            $alt = empty($alt) ? "alt" : "";
            $time_array = explode(":", $time);
            $is_hour = ($time_array[1] == "00") ? "hour" : "not_hour";
            $formatted_time = format_time($time);
            echo "<tr>";
            echo "<td class=\"time\"><span class=\"$is_hour\">$formatted_time</span></td>";
            foreach ($days as $day) {
                $q_result = @mysql_query("SELECT capacity FROM Weekly_Shift WHERE day_of_week='$day' AND start_time='$time'");
                $q = @mysql_fetch_array($q_result);
                $capacity = $q[0] ? $q[0] : 0;
                $r_result = @mysql_query("SELECT count(username) AS num_emps FROM Shift WHERE day_of_week='$day' AND start_time='$time'");
                $r = @mysql_fetch_array($r_result);
                $is_full = ((int)$r['num_emps'] == (int)$q[0]) ? "" : "not_full";
                echo "<td class=\"cap$capacity$alt $is_full\">";
                if ($cell = $calendar[$time][$day]) {
                    echo $g($cell);
                } else {
                    echo $empty;
                }
                echo "</td>";
            }
            echo "</tr>";
        }
        echo "</tr>";
        echo "</table>";
    }
    
    /* Prints a rolling week starting from today's date, including all closures and sub requests.
     * $start - the date on which to begin the week
     * $where_clause - what rows of the full weekly shift calendar to return
     * $query - content of table, must contain "day_of_week" and "start_time" columns
     * $f - function that takes a database row and returns a single element (to be inserted into an array that is passed to $g)
     * $g - function that takes an array and a context (datetime for the cell) and returns the contents of the table cell
     */
    function print_rolling_week($start, $where_clause, $query, $f, $g, $empty) {
        /***************************************************
         * Create calendar array (start time, day of week) *
         ***************************************************/
        global $resolution;
        $days_result = @mysql_query("SELECT DISTINCT day_of_week FROM Weekly_Shift $where_clause ORDER BY day_of_week");
        while ($day = @mysql_fetch_array($days_result)) {
            $days[] = $day['day_of_week'];
        }
        $times_result = @mysql_query("SELECT DISTINCT start_time FROM Weekly_Shift $where_clause ORDER BY start_time");
        while ($time = @mysql_fetch_array($times_result)) {
            $times[] = $time['start_time'];
        }
        $query_result = @mysql_query($query);
        while ($row = @mysql_fetch_array($query_result)) {
            $calendar[$row['start_time']][$row['day_of_week']][] = $f($row);
        }
        /********************
         * Fill in closures *
         ********************/
        $closures_result = @mysql_query("SELECT * FROM Closure WHERE (start_datetime BETWEEN '$start' AND '$start' + INTERVAL 7 DAY) OR (end_datetime BETWEEN '$start' AND '$start' + INTERVAL 7 DAY)");
        $is_new_day = true;
        $cur_day = "";
        while ($row = @mysql_fetch_array($closures_result)) {
            $datetime = $row['start_datetime'];
            while ($datetime != $row['end_datetime'] && date_diff($datetime, date("Y-m-d 00:00:00", strtotime(add_date($start, 0, 0, 0, 0, 7, 0)))) < 0) {
                while (date_diff($datetime, date("Y-m-d 00:00:00", strtotime($start))) < 0) {
                    $datetime = add_date($datetime, 0, $resolution * 60, 0, 0, 0, 0);
                }
                $day_of_week = date("l", strtotime($datetime));
                $datetime_array = explode(" ", $datetime);
                $date = $datetime_array[0];
                $time = $datetime_array[1];
                
                if ($day_of_week != $cur_day) {
                    if (isset($newcol_start_time) && isset($newcol_start_day)) {
                        $calendar[$newcol_start_time][$newcol_start_day][1] = $count;
                    }
                    $newcol_start_time = $time;
                    $newcol_start_day = $day_of_week;
                    $is_new_day = true;
                }
                
                if ($is_new_day && in_array($time, $times)) {
                    $calendar[$time][$day_of_week][0] = "Closed: {$row['reason']}";
                    $is_new_day = false;
                    $cur_day = $day_of_week;
                    $count = 1;
                } elseif (in_array($time, $times)) {
                    $calendar[$time][$day_of_week][0] = "empty";
                    $count++;
                }
                $datetime = add_date($datetime, 0, $resolution * 60, 0, 0, 0, 0);
            }
            if (isset($newcol_start_time) && isset($newcol_start_day)) {
                $calendar[$newcol_start_time][$newcol_start_day][1] = $count;
            }
        }
        /****************
         * Render table *
         ****************/
        echo "<table class=\"calendar\">";
        echo "<tr>";
        echo "<th class=\"time\"><span>Time</span></th>";
        $date = date("Y-m-d H:i:00", strtotime($start));
        for ($offset = 0; $offset < 7; $offset++) {
            $day_of_week = date("l", strtotime($date));
            if (in_array($day_of_week, $days)) {
                $ordered_dates[] = $date;
                $formatted_date = date("D, M j", strtotime($date));
                echo "<th>$formatted_date</th>";
            }
            $date = add_date($date, 0, 0, 0, 0, 1, 0);
        }
        echo "</tr>";
        $alt = "alt";
        foreach ($times as $time) {
            $alt = empty($alt) ? "alt" : "";
            $time_array = explode(":", $time);
            $is_hour = ($time_array[1] == "00") ? "hour" : "not_hour";
            $formatted_time = format_time($time);
            echo "<tr>";
            echo "<td class=\"time\"><span class=\"$is_hour\">$formatted_time</span></td>";
            foreach ($ordered_dates as $datetime) {
                $day = date("l", strtotime($datetime));
                if (strpos($calendar[$time][$day][0], "Closed") !== false) {
                    echo "<td class=\"closed$alt $is_hour\" rowspan=\"{$calendar[$time][$day][1]}\">";
                    echo $calendar[$time][$day][0];
                    echo "</td>";
                } else if (strpos($calendar[$time][$day][0], "empty") !== false) {
                    continue; // multicolumn closure, make this one blank
                } else {
                    $q_result = @mysql_query("SELECT capacity FROM Weekly_Shift WHERE day_of_week='$day' AND start_time='$time'");
                    $q = @mysql_fetch_array($q_result);
                    $capacity = $q[0] ? $q[0] : 0;
                    echo "<td class=\"cap$capacity$alt $is_hour\">";
                    if ($cell = $calendar[$time][$day]) {
                        $datetime_array = explode(" ", $datetime);
                        echo $g($cell, "{$datetime_array[0]} $time");
                    } else {
                        echo $empty;
                    }
                    echo "</td>";
                }
            }
            echo "</tr>";
        }
        echo "</tr>";
        echo "</table>";
    }
    
?>

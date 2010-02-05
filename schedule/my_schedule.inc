<h2>My Schedule</h2>

<?php
    $result = @mysql_query("SELECT username, count(username) AS hours FROM Shift WHERE username='$username' GROUP BY username");
    $row = @mysql_fetch_array($result);
    $hours_per_week = (int)$row['hours'] * $resolution;
?>

<p>Hours per week: <?php echo $hours_per_week; ?></p>

<?php

    function foo($row) {
        return $row['username'];
    }
    function bar($array) {
        foreach ($array as $element) {
            return get_shortname($element);
        }
    }
    print_weekly_calendar("", "SELECT * FROM Shift WHERE username='$username'", 'foo', 'bar');

?>

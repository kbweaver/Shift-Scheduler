<h2>Schedule</h2>

<?php
    function foo($row) {
        return $row['username'];
    }
    function bar($array) {
        return implode(" / ", array_map('get_shortname', $array));
    }
    print_weekly_calendar("", "SELECT * FROM Shift", 'foo', 'bar');
?>

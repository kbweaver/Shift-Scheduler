<?php
    if (!$is_admin) {
        echo "You should not be here.";
        exit (1);
    }
?>

<?php

    if (isset($_POST['update'])) {
        @mysql_query("UPDATE Closure SET start_datetime='{$_POST['c_start']}', end_datetime='{$_POST['c_end']}', reason='{$_POST['c_reason']}' WHERE start_datetime='{$_POST['c_oldstart']}'");
    }
    
    if (isset($_POST['delete'])) {
        @mysql_query("DELETE FROM Closure WHERE start_datetime='{$_POST['c_start']}' AND end_datetime='{$_POST['c_end']}'");
    }
    
    if (isset($_POST['add'])) {
        @mysql_query("INSERT INTO Closure VALUES ('{$_POST['c_start']}', '{$_POST['c_end']}', '{$_POST['c_reason']}')");
    }
?>

<table border="1">
    <tr>
        <th>Start</th>
        <th>End</th>
        <th>Reason</th>
    </tr>
<?php
    $closure_result = @mysql_query("SELECT * FROM Closure");
    while ($closure = @mysql_fetch_array($closure_result)) {
        echo "<form action=\"index.php?p=admin/closures\" method=\"post\">";
        echo "<input type=\"hidden\" name=\"c_oldstart\" value=\"{$closure['start_datetime']}\" />";
        echo "<tr>
                  <td><input type=\"text\" name=\"c_start\" value=\"{$closure['start_datetime']}\" /></td>
                  <td><input type=\"text\" name=\"c_end\" value=\"{$closure['end_datetime']}\" /></td>
                  <td><input type=\"text\" name=\"c_reason\" value=\"{$closure['reason']}\" /></td>
                  <td><input type=\"submit\" name=\"update\" value=\"Update Closure\" /></td>
                  <td><input type=\"submit\" name=\"delete\" value=\"Delete Closure\" /></td>
              </tr>";
        echo "</form>";
    }
?>
    <form action="index.php?p=admin/closures" method="post">
    <tr>
        <td><input type="text" name="c_start" /></td>
        <td><input type="text" name="c_end" /></td>
        <td><input type="text" name="c_reason" /></td>
        <td><input type="submit" name="add" value="Add Closure" /></td>
    </tr>
    </form>
</table>

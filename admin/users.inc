<?php
    if (!$is_admin) {
        echo "You should not be here.";
        exit (1);
    }
?>

<?php
   
    if (isset($_POST['update'])) {
        @mysql_query("UPDATE Employee SET username='{$_POST['emp_name']}', shortname='{$_POST['emp_shortname']}', role='{$_POST['emp_role']}' WHERE username='{$_POST['emp']}'");
    }
    
    if (isset($_POST['delete'])) {
        @mysql_query("DELETE FROM Employee WHERE username='{$_POST['emp_name']}'");
    }
    
    if (isset($_POST['add'])) {
        @mysql_query("INSERT INTO Employee VALUES ('{$_POST['emp_name']}', '{$_POST['emp_shortname']}', '{$_POST['emp_role']}')");
    }

?>

<table border="1">
    <tr>
        <th>Username</th>
        <th>Short Name</th>
        <th>Group</th>
    </tr>
    <?php
        $emp_result = @mysql_query("SELECT * FROM Employee");
        while ($emp = @mysql_fetch_array($emp_result)) {
            echo "<form action=\"index.php?p=admin/users\" method=\"post\">";
            echo "<input type=\"hidden\" name=\"emp\" value=\"{$emp['username']}\" />";
            echo "<tr>
                      <td><input type=\"text\" name=\"emp_name\" value=\"{$emp['username']}\" /></td>
                      <td><input type=\"text\" name=\"emp_shortname\" value=\"{$emp['shortname']}\" /></td>
                      <td>
                          <select name=\"emp_role\">";
            $role_result = @mysql_query("SELECT * FROM Role");
            while ($role = @mysql_fetch_array($role_result)) {
                $selected = ($role['name'] == $emp['role']) ? "selected" : "";
                echo "        <option $selected>{$role['name']}</option>";
            }
            echo "        </select>
                      </td>
                      <td><input type=\"submit\" name=\"update\" value=\"Update User\" /></td>
                      <td><input type=\"submit\" name=\"delete\" value=\"Delete User\" /></td>
                  </tr>";
            echo "</form>";
        }
    ?>
    <form action="index.php?p=admin/users" method="post">
    <tr>
        <td><input type="text" name="emp_name" /></td>
        <td><input type="text" name="emp_shortname" /></td>
        <td>
            <select name="emp_role">
            <?php
                $role_result = @mysql_query("SELECT * FROM Role");
                while ($role = @mysql_fetch_array($role_result)) {
                    echo "<option>{$role['name']}</option>";
                }
            ?>
            </select>
        </td>
        <td><input type="submit" name="add" value="Add User" /></td>
    </tr>
    </form>
</table>

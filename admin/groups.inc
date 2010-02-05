<?php
    if (!$is_admin) {
        echo "You should not be here.";
        exit (1);
    }
?>

<?php
    
    if (isset($_POST['update'])) {
        $admin = isset($_POST['role_admin']) ? 1 : 0;
        @mysql_query("UPDATE Role SET name='{$_POST['role_name']}', is_admin=$admin WHERE name='{$_POST['role']}'");
    }
    
    if (isset($_POST['delete'])) {
        @mysql_query("DELETE FROM Role WHERE name='{$_POST['role_name']}'");
    }
    
    if (isset($_POST['add'])) {
        $admin = isset($_POST['role_admin']) ? 1 : 0;
        @mysql_query("INSERT INTO Role VALUES ('{$_POST['role_name']}', $admin)");
    }

?>

<!--<form action="index.php?p=admin/groups" method="post">-->

<table border="1">
    <tr>
        <th>Group</th>
        <th>Admin?</th>
    </tr>
    <?php
        $role_result = @mysql_query("SELECT * FROM Role");
        while ($role = @mysql_fetch_array($role_result)) {
            $admin = $role['is_admin'] ? "checked=\"checked\"" : null;
            echo "<form action=\"index.php?p=admin/groups\" method=\"post\">";
            echo "<input type=\"hidden\" name=\"role\" value=\"{$role['name']}\" />";
            echo "<tr>
                      <td><input type=\"text\" name=\"role_name\" value=\"{$role['name']}\" /></td>
                      <td><input type=\"checkbox\" name=\"role_admin\" value=\"{$role['is_admin']}\" $admin /></td>
                      <td><input type=\"submit\" name=\"update\" value=\"Update Role\" /></td>
                      <td><input type=\"submit\" name=\"delete\" value=\"Delete Role\" /></td>
                  </tr>";
            echo "</form>";
        }
    ?>
    <form action="index.php?p=admin/groups" method="post">
    <tr>
        <td><input type="text" name="role_name" /></td>
        <td><input type="checkbox" name="role_admin" value="0" /></td>
        <td><input type="submit" name="add" value="Add Role" /></td>
    </tr>
    </form>
</table>

<!--<input type="submit" name="update_all" value="Update All"/>-->

<!--</form>-->

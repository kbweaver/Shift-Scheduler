<?php
    if (!$is_admin) {
        echo "You should not be here.";
        exit (1);
    }
?>

<ul>
<li><a href="./index.php?p=admin">Admin</a></li>
<li><a href="./index.php?p=admin/groups">Manage Groups</a></li>
<li><a href="./index.php?p=admin/users">Manage Users</a></li>
<li><a href="./index.php?p=admin/weekly_shifts">Manage Weekly Shifts</a></li>
<li><a href="./index.php?p=admin/assign">Assign Shifts</a></li>
<li><a href="./index.php?p=admin/closures">Manage Closures</a></li>
</ul>

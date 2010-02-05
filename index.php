<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<?php

    require_once ('./dbutils.php');
    
    if (!isset($_GET['p'])) {
        $tab = "home";
        $page = "index";
    } else {
        $url = explode("/", $_GET['p']);
        $tab = $url[0];
        if (count($url) < 2 || $url[1] == false) {
            $page = "index";
        } else {
            $page = $url[1];
        }
    }
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">

<head>

<link rel="stylesheet" type="text/css" href="./style.css" />

<title>Help Desk Options : <?php echo $tab; ?></title>

</head>

<body>

<div id="header">
    <h1>Help Desk Scheduler</h1><img src="BETA.jpg" height="100px" width="100px" style="display: inline; float: left;" />
    <p>
    <?php echo "Hello $emp_name!\nYou are logged in as $username [$emp_role]"; ?>
    </p>
</div>

<div class="main" <?php echo "id=\"$tab\""; ?>>
<ul id="tabnav">
<li class="home"><a href="./index.php?p=home">Home</a></li>
<li class="schedule"><a href="./index.php?p=schedule">Schedule</a></li>
<li class="subs"><a href="./index.php?p=subs">Subs</a></li>
<?php
    if ($is_admin) {
        echo "<li class=\"admin\"><a href=\"./index.php?p=admin\">Admin</a></li>\n";
    }
?>
<li class="help" style="position: absolute; right: 20px;"><a href="./index.php?p=help">Help</a></li>
</ul>
</div>

<div class="subnav">
<?php include("./$tab/subnav.inc"); ?>
</div>

<div class="main">
<?php
    if (!@include("./$tab/$page.inc")) {
        echo "Stop playing with my variables.";
    }
?>
</div>

<div id="footer">
    <p>This scheduling application is maintained by <?php echo "$contact_name ($contact_email)"; ?>.</p>
    <p>If it breaks, call the Help Desk.</p>
</div>

</body>

</html>

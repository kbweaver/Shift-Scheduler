<?php
    $config = parse_ini_file("./config.ini");
    
    $db_host = $config['db_host'];
    $db_username = $config['db_username'];
    $db_passwd = $config['db_passwd'];
    $db_name = $config['db_name'];
    
    //TODO: these should all be moved eventually to a single Config table in the database
    $contact_name = "Help Desk Managers";
    $contact_email = "unet_managers@lists.brandeis.edu";
    $sub_email = "unet_subs@lists.brandeis.edu";
    $resolution = 0.5;
?>

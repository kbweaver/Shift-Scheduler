DROP TABLE IF EXISTS Sub, Closure, Shift, Shift_Request, Weekly_Shift, Employee, Role;

CREATE TABLE Role (
    name varchar(255) NOT NULL,
    is_admin boolean DEFAULT false,
    PRIMARY KEY (name)
) ENGINE=INNODB;

CREATE TABLE Employee (
    username varchar(8) NOT NULL,
    shortname varchar(255) UNIQUE NOT NULL,
    role varchar(255),
    PRIMARY KEY (username),
    FOREIGN KEY (role) REFERENCES Role(name) ON UPDATE CASCADE ON DELETE CASCADE
) ENGINE=INNODB;

CREATE TABLE Weekly_Shift (
    day_of_week ENUM('Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday') NOT NULL,
    start_time time NOT NULL,
    capacity int NOT NULL,
    PRIMARY KEY (day_of_week, start_time)
) ENGINE=INNODB;

CREATE TABLE Shift_Request (
    username varchar(8) NOT NULL,
    day_of_week ENUM('Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday') NOT NULL,
    start_time time NOT NULL,
    preference ENUM('High', 'Medium', 'Low', 'Unavailable'),
    PRIMARY KEY (username, day_of_week, start_time),
    FOREIGN KEY (username) REFERENCES Employee(username) ON UPDATE CASCADE ON DELETE CASCADE,
    FOREIGN KEY (day_of_week, start_time) REFERENCES Weekly_Shift(day_of_week, start_time) ON UPDATE CASCADE ON DELETE CASCADE
) ENGINE=INNODB;

CREATE TABLE Shift (
    username varchar(8) NOT NULL,
    day_of_week ENUM('Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday') NOT NULL,
    start_time time NOT NULL,
    PRIMARY KEY (username, day_of_week, start_time),
    FOREIGN KEY (username) REFERENCES Employee(username) ON UPDATE CASCADE ON DELETE CASCADE,
    FOREIGN KEY (day_of_week, start_time) REFERENCES Weekly_Shift(day_of_week, start_time)
) ENGINE=INNODB;

CREATE TABLE Closure (
    start_datetime datetime UNIQUE NOT NULL,
    end_datetime datetime UNIQUE NOT NULL,
    reason varchar(255) NOT NULL,
    PRIMARY KEY (start_datetime, end_datetime),
    CHECK (start_datetime < end_datetime)
) ENGINE=INNODB;

CREATE TABLE Sub (
    start_datetime datetime NOT NULL,
    requestor varchar(8),
    reason varchar(255) NOT NULL,
    filled_by varchar(8) DEFAULT NULL,
    PRIMARY KEY (start_datetime, requestor),
    FOREIGN KEY (requestor) REFERENCES Employee(username) ON UPDATE CASCADE ON DELETE NO ACTION,
    FOREIGN KEY (filled_by) REFERENCES Employee(username) ON UPDATE CASCADE ON DELETE NO ACTION,
    CHECK (requestor != filled_by)
) ENGINE=INNODB;

CREATE TABLE Appointment (
    start_datetime datetime NOT NULL,
    username varchar(8) NOT NULL,
    reason varchar(255) NOT NULL,
    PRIMARY KEY (start_datetime, username),
    FOREIGN KEY (username) REFERENCES Employee(username) ON UPDATE CASCADE ON DELETE CASCADE
) ENGINE=INNODB;

CREATE TABLE Config (
    k varchar(255) NOT NULL,
    v varchar(255),
    PRIMARY KEY (k)
) ENGINE=INNODB;

INSERT INTO Role VALUES ('Student Manager', true);
INSERT INTO Employee VALUES ('kweave10', 'Kevin', 'Student Manager');

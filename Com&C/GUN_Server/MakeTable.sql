USE GUN;
CREATE TABLE GUNHP
(
	id INT PRIMARY KEY auto_increment,
    fset INT DEFAULT 0,
    hp INT DEFAULT 200,
    gauge INT DEFAULT 0,
    maxblit INT DEFAULT 0,
    blit INT DEFAULT 0,
	totblit INT DEFAULT 0,
    armor INT DEFAULT 0,
    silencer INT DEFAULT 0
);

CREATE TABLE Deal
(
	id INT PRIMARY KEY auto_increment,
    deal INT DEFAULT 0,
    damage INT DEFAULT 0
);

CREATE TABLE Killlog
(
	id INT PRIMARY KEY auto_increment,
    killer INT DEFAULT 0,
    dead INT DEFAULT 0
);
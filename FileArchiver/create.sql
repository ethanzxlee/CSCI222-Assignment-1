DROP SCHEMA IF EXISTS `FileArchiver` ;
CREATE SCHEMA IF NOT EXISTS `FileArchiver` DEFAULT CHARACTER SET latin1 COLLATE
latin1_swedish_ci ;
USE `FileArchiver` ;

-- -----------------------------------------------------
-- Table `FileArchiver`.`filerec`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `FileArchiver`.`filerec` (
`filename` VARCHAR(255) NOT NULL ,
`curlhash` INT(24) UNSIGNED NOT NULL ,
`ovhash` INT(24) UNSIGNED NOT NULL,
`currentversion` INT(11) NOT NULL ,
`nversion` INT(11) NOT NULL ,
`length` INT(11) NOT NULL ,
`mtsec` INT(11) NOT NULL ,
`filedata` MEDIUMBLOB NOT NULL,
PRIMARY KEY (`filename`))
ENGINE = InnoDB;
-- -----------------------------------------------------
-- Table `FileArchiver`.`fileblkhashes`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `FileArchiver`.`fileblkhashes` (
`idfileblkhashes` INT(24) NOT NULL AUTO_INCREMENT,
`fileref` VARCHAR(255) NOT NULL,
`blknum` VARCHAR(33) NOT NULL,
`hashval` INT(24) UNSIGNED NOT NULL,
PRIMARY KEY (`idfileblkhashes`),
FOREIGN KEY (`fileref`)
	REFERENCES filerec(`filename`))
ENGINE = InnoDB;
-- -----------------------------------------------------
-- Table `FileArchiver`.`versionrec`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `FileArchiver`.`versionrec` (
`idversionrec` INT(11) NOT NULL AUTO_INCREMENT,
`fileref` VARCHAR(255) NOT NULL,
`versionnum` INT(11) NOT NULL,
`length` INT(11) NOT NULL,
`mtsec` INT(11) NOT NULL,
`ovhash` INT(24) UNSIGNED NOT NULL,
`commenttxt` MEDIUMTEXT NULL,
PRIMARY KEY (`idversionrec`),
FOREIGN KEY (`fileref`) 
	REFERENCES filerec(`filename`))
ENGINE = InnoDB;
-- -----------------------------------------------------
-- Table `FileArchiver`.`blktable`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `FileArchiver`.`blktable` (
`idblktable` INT(11) NOT NULL AUTO_INCREMENT,
`version` INT(11) NOT NULL,
`length` INT(11) NOT NULL,
`blknum` INT(11) NOT NULL,
`hash` INT(24) UNSIGNED NOT NULL,
`data` MEDIUMBLOB NOT NULL,
PRIMARY KEY (`idblktable`),
FOREIGN KEY (`version`) 
	REFERENCES versionrec(`idversionrec`))
ENGINE = InnoDB;


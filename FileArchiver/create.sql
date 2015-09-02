DROP SCHEMA IF EXISTS `ass2` ;
CREATE SCHEMA IF NOT EXISTS `ass2` DEFAULT CHARACTER SET latin1 COLLATE
latin1_swedish_ci ;
USE `ass2` ;

-- -----------------------------------------------------
-- Table `ass2`.`blobtable`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `ass2`.`blobtable` (
`tempname` VARCHAR(45) NOT NULL ,
`filedata` MEDIUMBLOB NULL,
PRIMARY KEY (`tempname`) )
ENGINE = InnoDB;
-- -----------------------------------------------------
-- Table `ass2`.`filerec`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `ass2`.`filerec` (
`filename` VARCHAR(255) NOT NULL ,
`curlhash` VARCHAR(33) NOT NULL ,	-- TODO: pretty sure this should be a varchar if were going to use md5
`ovhash` VARCHAR(33) NOT NULL,
`currentversion` INT(11) NULL ,
`nversion` INT(11) NULL ,
`length` INT(11) NULL ,
`mtnsec` INT(11) NULL ,
`mtsec` INT(11) NULL ,
`tempname` VARCHAR(45) NOT NULL,
`blobtable_tempname` VARCHAR(45) NOT NULL,
PRIMARY KEY (`filename`),
FOREIGN KEY (`blobtable_tempname`)
	REFERENCES blobtable(`tempname`))
ENGINE = InnoDB;
-- -----------------------------------------------------
-- Table `ass2`.`commentstable`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `ass2`.`commentstable` (
`idcommentstable` INT(11) NOT NULL ,
`filerec` VARCHAR(255) NOT NULL,
`commentnum` INT(11) NOT NULL,
`commenttxt` MEDIUMTEXT NULL,
PRIMARY KEY (`idcommentstable`),
FOREIGN KEY (`filerec`) 
	REFERENCES filerec(`filename`))
ENGINE = InnoDB;
-- -----------------------------------------------------
-- Table `ass2`.`fileblkhashes`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `ass2`.`fileblkhashes` (
`idfileblkhashes` INT(11) NOT NULL ,
`fileref` VARCHAR(255) NOT NULL,
`blknum` VARCHAR(33) NOT NULL,
`hashval` VARCHAR(33) NOT NULL,
PRIMARY KEY (`idfileblkhashes`),
FOREIGN KEY (`fileref`)
	REFERENCES filerec(`filename`))
ENGINE = InnoDB;
-- -----------------------------------------------------
-- Table `ass2`.`versionrec`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `ass2`.`versionrec` (
`idversionrec` INT(11) NOT NULL ,
`fileref` VARCHAR(255) NOT NULL,
`versionnum` INT(11) NOT NULL,
`length` INT(11) NOT NULL,
`mtsec` INT(11) NOT NULL,
`mtnsec` INT(11) NOT NULL,
`ovhash` VARCHAR(33) NOT NULL,
PRIMARY KEY (`idversionrec`),
FOREIGN KEY (`fileref`) 
	REFERENCES filerec(`filename`))
ENGINE = InnoDB;
-- -----------------------------------------------------
-- Table `ass2`.`blktable`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `ass2`.`blktable` (
`idblktable` INT(11) NOT NULL ,
`version` INT(11) NOT NULL,
`length` INT(11) NOT NULL,
`blknum` INT(11) NOT NULL,
`hash` VARCHAR(33) NOT NULL,
`data` MEDIUMBLOB NOT NULL,
PRIMARY KEY (`idblktable`),
FOREIGN KEY (`version`) 
	REFERENCES versionrec(`idversionrec`))
ENGINE = InnoDB;


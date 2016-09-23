DROP TABLE IF EXISTS security_list;
CREATE TABLE security_list
(
  `Symbol`          VARCHAR(7)    NOT NULL                  COMMENT "55",
  `DecPlaces_Str`   INT UNSIGNED  NOT NULL  DEFAULT 0       COMMENT "871=16, 872",
  `DecPlaces_Forex` INT UNSIGNED  NOT NULL  DEFAULT 0       COMMENT "871=18, 872",
  `ReqID`           VARCHAR(64)   NULL                      COMMENT "322",
  `Created_at`      DATETIME      NOT NULL  DEFAULT CURRENT_TIMESTAMP,
  `Updated_at`      DATETIME      NOT NULL  DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `Deleted_at`      DATETIME      NULL,
  PRIMARY KEY (`Symbol`)
)

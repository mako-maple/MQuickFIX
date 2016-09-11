DROP TABLE IF EXISTS security_list_request;
CREATE TABLE security_list_request
(
  `ReqID`           VARCHAR(64)   NOT NULL                  COMMENT "320",
  `RespDateTime`    VARCHAR(22)   NULL                      COMMENT "52",
  `RespID`          VARCHAR(64)   NULL                      COMMENT "322",
  `Result`          INT           NULL                      COMMENT "560",
  `SymbolCount`     INT           NULL                      COMMENT "140",
  `Created_at`      DATETIME      NOT NULL  DEFAULT CURRENT_TIMESTAMP,
  `Updated_at`      DATETIME      NOT NULL  DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `Deleted_at`      DATETIME      NULL,
  PRIMARY KEY (`ReqID`)
)

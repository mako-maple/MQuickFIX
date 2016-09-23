DROP TABLE IF EXISTS test_request;
CREATE TABLE test_request
(
  `id`              INT UNSIGNED  NOT NULL  AUTO_INCREMENT,
  `TestReqID`       VARCHAR(64)   NOT NULL                  COMMENT "112",
  `SenderCompID`    VARCHAR(64)   NOT NULL                  COMMENT "49",
  `Created_at`      DATETIME      NOT NULL  DEFAULT CURRENT_TIMESTAMP,
  `Updated_at`      DATETIME      NOT NULL  DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `Deleted_at`      DATETIME      NULL,
  PRIMARY KEY (`id`),
  UNIQUE `idxTestRequest_TestReqID` (`TestReqID`)
)

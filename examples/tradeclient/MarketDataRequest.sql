DROP TABLE IF EXISTS market_data_request;
CREATE TABLE market_data_request
(
  `ReqID`            VARCHAR(64)   NOT NULL                  COMMENT '262',
  `ReqType`          TINYINT(1)    NOT NULL                  COMMENT '263 1=Start,2=Stop',
  `Symbol`           VARCHAR(7)    NOT NULL                  COMMENT '55',
  `Depth`            TINYINT(1)    NOT NULL                  COMMENT '264 0=Full,1=Top',
  `Aggregate`        CHAR(1)       NULL                      COMMENT '266 N=Non,Y=Aggr',
  `EntryTypeSum`     TINYINT(1)    NOT NULL DEFAULT 0        COMMENT '267',    
  `RespDateTime`     CHAR(21)      NULL                      COMMENT '52',
  `Entries`          INT UNSIGNED  NULL                      COMMENT '268',
  `RejectReason`     INT UNSIGNED  NULL                      COMMENT '281',
  `RejectText`       VARCHAR(256)  NULL                      COMMENT '58',
  `StartTime`        DATETIME      NULL                      COMMENT '-- Start Subscription',
  `StopTime`         DATETIME      NULL                      COMMENT '-- Stop  Subscription',
  `Created_at`       DATETIME      NOT NULL  DEFAULT CURRENT_TIMESTAMP,
  `Updated_at`       DATETIME      NOT NULL  DEFAULT CURRENT_TIMESTAMP  ON UPDATE CURRENT_TIMESTAMP,
  `Deleted_at`       DATETIME      NULL,
  PRIMARY KEY (`ReqID`),
  INDEX  `idxMDReq_Symbol`   (`Symbol`),
  INDEX  `idxMDReq_ReqID_Type` (`ReqID`, `ReqType`)
);

DROP TABLE IF EXISTS market_data_request_hist;
CREATE TABLE market_data_request_hist
(
  `id`               INT UNSIGNED  NOT NULL  AUTO_INCREMENT,
  `opeType`          CHAR(1)       NOT NULL                  COMMENT '-- I=Ins,U=Upd,D=Del',
  `ReqID`            VARCHAR(64)   NULL                      COMMENT '262',
  `ReqType`          TINYINT(1)    NULL                      COMMENT '263 1=Start,2=Stop',
  `Symbol`           VARCHAR(7)    NULL                      COMMENT '55',
  `Depth`            TINYINT(1)    NULL                      COMMENT '264 0=Full,1=Top',
  `Aggregate`        CHAR(1)       NULL                      COMMENT '266 N=Non,Y=Aggr',
  `EntryTypeSum`     TINYINT(1)    NULL                      COMMENT '267',
  `RespDateTime`     CHAR(21)      NULL                      COMMENT '52',
  `Entries`          INT UNSIGNED  NULL                      COMMENT '268',
  `RejectReason`     INT UNSIGNED  NULL                      COMMENT '281',
  `RejectText`       VARCHAR(256)  NULL                      COMMENT '58',
  `StartTime`        DATETIME      NULL                      COMMENT '-- Start Subscription',
  `StopTime`         DATETIME      NULL                      COMMENT '-- Stop  Subscription',
  `Created_at`       DATETIME      NOT NULL  DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
);


DROP TRIGGER IF EXISTS trg_af_i_market_data_request;
DELIMITER //
CREATE TRIGGER trg_af_i_market_data_request
  AFTER INSERT ON market_data_request FOR EACH ROW
BEGIN
  INSERT INTO market_data_request_hist SET 
    `opeType`     = 'I'
  , `ReqID`       = NEW.`ReqID`       
  , `ReqType`     = NEW.`ReqType`     
  , `Symbol`      = NEW.`Symbol`      
  , `Depth`       = NEW.`Depth`       
  , `Aggregate`   = NEW.`Aggregate`   
  , `EntryTypeSum`= NEW.`EntryTypeSum`
  , `RespDateTime`= NEW.`RespDateTime`
  , `Entries`     = NEW.`Entries`     
  , `RejectReason`= NEW.`RejectReason`
  , `RejectText`  = NEW.`RejectText`  
  , `StartTime`   = NEW.`StartTime`   
  , `StopTime`    = NEW.`StopTime`    
  ;
END;
//
DELIMITER ;

DROP TRIGGER IF EXISTS trg_af_u_market_data_request;
DELIMITER //
CREATE TRIGGER trg_af_u_market_data_request
  AFTER UPDATE ON market_data_request FOR EACH ROW
BEGIN
  INSERT INTO market_data_request_hist SET 
    `opeType`     = 'U'
  , `ReqID`       = NEW.`ReqID`       
  , `ReqType`     = NEW.`ReqType`     
  , `Symbol`      = NEW.`Symbol`      
  , `Depth`       = NEW.`Depth`       
  , `Aggregate`   = NEW.`Aggregate`   
  , `EntryTypeSum`= NEW.`EntryTypeSum`
  , `RespDateTime`= NEW.`RespDateTime`
  , `Entries`     = NEW.`Entries`     
  , `RejectReason`= NEW.`RejectReason`
  , `RejectText`  = NEW.`RejectText`  
  , `StartTime`   = NEW.`StartTime`   
  , `StopTime`    = NEW.`StopTime`    
  ;
END;
//
DELIMITER ;

DROP TRIGGER IF EXISTS trg_bf_d_market_data_request;
DELIMITER //
CREATE TRIGGER trg_bf_d_market_data_request
  BEFORE DELETE ON market_data_request FOR EACH ROW
BEGIN
  INSERT INTO market_data_request_hist SET 
    `opeType`     = 'D'
  , `ReqID`       = OLD.`ReqID`       
  , `ReqType`     = OLD.`ReqType`     
  , `Symbol`      = OLD.`Symbol`      
  , `Depth`       = OLD.`Depth`       
  , `Aggregate`   = OLD.`Aggregate`   
  , `EntryTypeSum`= OLD.`EntryTypeSum`
  , `RespDateTime`= OLD.`RespDateTime`
  , `Entries`     = OLD.`Entries`     
  , `RejectReason`= OLD.`RejectReason`
  , `RejectText`  = OLD.`RejectText`  
  , `StartTime`   = OLD.`StartTime`   
  , `StopTime`    = OLD.`StopTime`    
  ;
END;
//
DELIMITER ;

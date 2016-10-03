DROP TABLE IF EXISTS market_data;
CREATE TABLE market_data
(
  `Symbol`           VARCHAR(7)    NOT NULL                  COMMENT '55',
  `Type`             TINYINT(1)    NOT NULL                  COMMENT '269 0=Bid,1=Offer..',
  `Action`           TINYINT(1)    NOT NULL                  COMMENT '279 0=New,2=Del',
  `Px`               REAL          NULL                      COMMENT '270',
  `Size`             INT UNSIGNED  NULL                      COMMENT '271',
  `Orders`           INT UNSIGNED  NULL                      COMMENT '346',
  `RespDateTime`     CHAR(21)      NULL                      COMMENT '52',
  PRIMARY KEY (`Symbol`, `Type`)
)
COMMENT 'MarketData Depth=TOP';


DROP TABLE IF EXISTS market_data_hist;
CREATE TABLE market_data_hist
(
  `id`            BIGINT UNSIGNED  NOT NULL  AUTO_INCREMENT,
  `Symbol`           VARCHAR(7)    NOT NULL                  COMMENT '55',
  `Type`             TINYINT(1)    NOT NULL                  COMMENT '269 0=Bid,1=Offer..',
  `Action`           TINYINT(1)    NOT NULL                  COMMENT '279 0=New,2=Del',
  `Px`               REAL          NULL                      COMMENT '270',
  `Size`             INT UNSIGNED  NULL                      COMMENT '271',
  `Orders`           INT UNSIGNED  NULL                      COMMENT '346',
  `Created_at`       DATETIME      NOT NULL  DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX idx_MD_hist_Sym_Type (`Symbol`, `Type`),
  INDEX idx_MD_hist_Crete (`Created_at`)
)
COMMENT 'MarketData Depth=TOP';


DROP TRIGGER IF EXISTS trg_bf_u_market_data;
DELIMITER //
CREATE TRIGGER trg_bf_u_market_data
  BEFORE UPDATE ON market_data FOR EACH ROW
BEGIN
  INSERT INTO market_data_hist SET 
    `Symbol`       = NEW.`Symbol`  
  , `Type`         = NEW.`Type`    
  , `Action`       = NEW.`Action`  
  , `Px`           = NEW.`Px`      
  , `Size`         = NEW.`Size`    
  , `Orders`       = NEW.`Orders`  
  ;
END;
//
DELIMITER ;

DROP TABLE IF EXISTS MD;
CREATE TABLE MD
(
  `Symbol`           VARCHAR(7)    NOT NULL                  COMMENT '55',
  `BidOrders`        INT UNSIGNED  NULL                      COMMENT '346 269=0',
  `BidSize`          INT UNSIGNED  NULL                      COMMENT '271 269=0',
  `Bid`              REAL          NULL                      COMMENT '270 269=0',
  `Spred`            REAL          NULL                      COMMENT '270 269=9',
  `Ask`              REAL          NULL                      COMMENT '270 269=1',
  `AskSize`          INT UNSIGNED  NULL                      COMMENT '271 269=1',
  `AskOrders`        INT UNSIGNED  NULL                      COMMENT '346 269=1',
  `High`             REAL          NULL                      COMMENT '270 269=7',
  `Log`              REAL          NULL                      COMMENT '270 269=8',
  `RespDateTime`     CHAR(21)      NULL                      COMMENT '52',
  PRIMARY KEY (`Symbol`)
)
COMMENT 'MarketData Depth=TOP';


DROP TABLE IF EXISTS MD_hist;
CREATE TABLE MD_hist
(
  `id`            BIGINT UNSIGNED  NOT NULL  AUTO_INCREMENT,
  `Symbol`           VARCHAR(7)    NOT NULL                  COMMENT '55',
  `BidOrders`        INT UNSIGNED  NULL                      COMMENT '346 269=0',
  `BidSize`          INT UNSIGNED  NULL                      COMMENT '271 269=0',
  `Bid`              REAL          NULL                      COMMENT '270 269=0',
  `Spred`            REAL          NULL                      COMMENT '270 269=9',
  `Ask`              REAL          NULL                      COMMENT '270 269=1',
  `AskSize`          INT UNSIGNED  NULL                      COMMENT '271 269=1',
  `AskOrders`        INT UNSIGNED  NULL                      COMMENT '346 269=1',
  `High`             REAL          NULL                      COMMENT '270 269=7',
  `Log`              REAL          NULL                      COMMENT '270 269=8',
  `RespDateTime`     CHAR(21)      NULL                      COMMENT '52',
  PRIMARY KEY (`id`),
  INDEX idx_MD_hist_Symbol (`Symbol`),
  INDEX idx_MD_hist_DateTime (`RespDateTime`)
)
COMMENT 'MarketData Depth=TOP';


DROP TRIGGER IF EXISTS trg_bf_u_MD;
DELIMITER //
CREATE TRIGGER trg_bf_u_MD
  BEFORE UPDATE ON MD FOR EACH ROW
BEGIN
  INSERT INTO MD_hist SET 
    `Symbol`       = NEW.`Symbol`
  , `BidOrders`    = NEW.`BidOrders`
  , `BidSize`      = NEW.`BidSize`
  , `Bid`          = NEW.`Bid`
  , `Spred`        = NEW.`Spred`
  , `Ask`          = NEW.`Ask`
  , `AskSize`      = NEW.`AskSize`
  , `AskOrders`    = NEW.`AskOrders`
  , `High`         = NEW.`High`
  , `Log`          = NEW.`Log`
  , `RespDateTime` = NEW.`RespDateTime`
  ;
END;
//
DELIMITER ;

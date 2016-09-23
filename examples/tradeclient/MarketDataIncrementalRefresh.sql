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

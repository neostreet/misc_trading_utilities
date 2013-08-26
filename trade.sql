use trading

drop table if exists trade;

create table trade (
  id smallint auto_increment primary key,
  type varchar(1) not null,
  entry_entered varchar(20) not null,
  entry_filled varchar(20) not null,
  entry_filled_price smallint not null,
  exit_entered varchar(20) not null,
  exit_filled varchar(20) not null,
  exit_filled_price smallint not null,
  delta double not null
);

quit

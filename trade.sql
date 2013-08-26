use trading

drop table if exists trade;

create table trade (
  id smallint auto_increment primary key,
  type varchar(1) not null,
  entry_price smallint not null,
  exit_price smallint not null,
  delta double not null
);

quit

use trading

drop table if exists trade;

create table trade (
  id smallint auto_increment primary key,
  type varchar(1) not null,
  entry_order_type varchar(11) not null,
  entry_entered varchar(20) not null,
  entry_stop integer null,
  entry_order_limit integer not null,
  entry_filled varchar(20) not null,
  entry_filled_price integer not null,
  exit_order_type varchar(11) not null,
  exit_entered varchar(20) not null,
  exit_stop integer null,
  exit_order_limit integer not null,
  exit_filled varchar(20) not null,
  exit_filled_price integer not null,
  ticks smallint not null,
  delta double not null
);

quit

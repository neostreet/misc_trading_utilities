use trading

drop table if exists tradestation_order;

create table tradestation_order (
  id smallint auto_increment primary key,
  entered varchar(20) not null,
  filled varchar(20) not null,
  symbol varchar(10) not null,
  type varchar(4) not null,
  quantity smallint not null,
  qty_filled smallint null,
  qty_left smallint not null,
  stop integer null,
  order_limit varchar(10) not null,
  filled_price integer null,
  order_status varchar(10) not null,
  commission float not null
);

quit

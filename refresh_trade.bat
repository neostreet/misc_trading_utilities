@echo off
call gen_insert_trade_it futures1_orders.txt%1
call tomysql < zap_trade.sql
call tomysql < futures1_orders.txt%1.gen_insert_trade.sql
mysql_grab trades
mysql_grab trades2
mysql_grab position
mysql_grab type
mysql_grab type_delta
mysql_grab sum_delta3
mysql_grab composite_trade_type2

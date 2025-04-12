/*system
/*1. Получите список всех файлов табличных пространств (перманентных  и временных).*/
select * from dba_data_files;
select * from dba_temp_files;

/*2. Создайте табличное пространство с именем XXX_QDATA (10m). При создании установите его в состояние offline. 
Затем переведите табличное пространство в состояние online. Выделите пользователю XXX квоту 2m в пространстве XXX_QDATA. 
От имени XXX в  пространстве XXX_T1создайте таблицу из двух столбцов, один из которых будет являться первичным ключом. В таблицу добавьте 3 строки.*/
--alter session set container = XEPDB1;
--alter session set container = CDB$ROOT;
--drop tablespace EVP_QDATA
create tablespace EVP_QDATA
datafile 'EVP_QDATA'
size 10m
extent management local
offline;

alter tablespace EVP_QDATA online;

select * from dba_tablespaces where tablespace_name like '%EVP%';

drop user EVP cascade;

create user EVP identified by 12345
default tablespace  EVP_QDATA
quota unlimited on EVP_QDATA;

grant create session, 
    create table, 
    create view, 
    create procedure to EVP;
    
alter user EVP quota 2m on EVP_QDATA;

create table EVP_T1 (
    id int PRIMARY KEY,
    name varchar(50)
) tablespace EVP_QDATA;

insert into EVP_T1 (id, name) values (1, 'bosskfc');
insert into EVP_T1 (id, name) values (2, 'plakiplaki');
insert into EVP_T1 (id, name) values (3, 'normaldaki');
select * from EVP_T1;

/*system
3. Получите список сегментов табличного пространства  XXX_QDATA. Определите сегмент таблицы XXX_T1. Определите остальные сегменты.*/
select distinct * from dba_segments where tablespace_name = 'EVP_QDATA';
select segment_name, segment_type from dba_segments where tablespace_name='EVP_QDATA';

/*EVP
4. Удалите (DROP) таблицу XXX_T1. Получите список сегментов табличного пространства  XXX_QDATA. 
Определите сегмент таблицы XXX_T1. Выполните SELECT-запрос к представлению USER_RECYCLEBIN, поясните результат.*/
drop table EVP_T1;
/*system*/
select distinct * from dba_segments where tablespace_name = 'EVP_QDATA';

/*EVP*/
select * from user_recyclebin;

/*5. Восстановите (FLASHBACK) удаленную таблицу. EVP */
flashback table EVP_T1 to before drop;
select * from EVP_T1;

/*6. Выполните PL/SQL-скрипт, заполняющий таблицу XXX_T1 данными (10000 строк). EVP */
begin
for k in 10..10000
loop
insert into EVP_T1 values(k, k);
end loop;
commit;
end;
select * from EVP_T1;

/*7. Определите сколько в сегменте таблицы XXX_T1 экстентов, их размер в блоках и байтах. Получите перечень всех экстентов. EVP*/
select extents, bytes, blocks from dba_segments where segment_name = 'EVP_T1';
select * from dba_extents where segment_name = 'EVP_T1';

/*8. Удалите табличное пространство XXX_QDATA и его файл system*/
drop tablespace EVP_QDATA including contents and datafiles;

/*9. Получите перечень всех групп журналов повтора. Определите текущую группу журналов повтора.*/
select GROUP# from v$log;
select GROUP# from v$log where STATUS = 'CURRENT';
    
/*10. Получите перечень файлов всех журналов повтора инстанса*/  
select MEMBER from v$logfile;
    
/*11. С помощью переключения журналов повтора пройдите полный цикл переключений. 
Запишите серверное время в момент вашего первого переключения (оно понадобится для выполнения следующих заданий).*/    
--alter session set container = CDB$ROOT;
select group#, status from v$log;
alter system switch logfile;
select TO_CHAR(SYSDATE, 'HH24:MI DD MONTH YYYY') as current_date from DUAL;
--19:53 21окт 2024

/*12. Создайте дополнительную группу журналов повтора с тремя файлами журнала. Убедитесь в наличии группы и файлов, \
а также в работоспособности группы (переключением). Проследите последовательность SCN. */
alter database add logfile group 4 '/opt/oracle/oradata/XE/redo04_11.log' size 50m blocksize 512;
alter database add logfile member '/opt/oracle/oradata/XE/redo04_22.log' to group 4;
alter database add logfile member '/opt/oracle/oradata/XE/redo04_33.log' to group 4;
select * from V$LOG;
select * from v$logfile



/*13. Удалите созданную группу журналов повтора. Удалите созданные вами файлы журналов на сервере.*/
alter database drop logfile member '/opt/oracle/oradata/XE/redo04.log';
alter database drop logfile member '/opt/oracle/oradata/XE/redo04_1.log';
alter database drop logfile member '/opt/oracle/oradata/XE/redo04_2.log';
alter database drop logfile group 4;
select * from v$logfile

--14
-- (архивирование должно быть отключено, иначе дождитесь, пока другой студент выполнит задание и отключит).
SHUTDOWN IMMEDIATE;
STARTUP MOUNT;
ALTER DATABASE ARCHIVELOG;
SELECT status FROM v$instance;

-- Должны быть значения: LOG_MODE = NOARCHIVELOG; ARCHIVER = STOPPED
select DBID, NAME, LOG_MODE from V$DATABASE;
select INSTANCE_NAME, ARCHIVER, ACTIVE_STATE from V$INSTANCE;

--15
select * from V$ARCHIVED_LOG;

--16
ALTER SYSTEM ARCHIVE LOG CURRENT;
select * from V$ARCHIVED_LOG;
select DBID, NAME, LOG_MODE from V$DATABASE;
select INSTANCE_NAME, ARCHIVER, ACTIVE_STATE from V$INSTANCE;

--17
alter system switch logfile;
select group# from V$LOG where status = 'CURRENT';
select * from V$ARCHIVED_LOG;
ALTER SYSTEM ARCHIVE LOG CURRENT;


SELECT GROUP#, ARCHIVED,  STATUS FROM V$LOG;

--18
select * from V$CONTROLFILE;

--19
select * from V$CONTROLFILE;
show parameter control_files; 
SELECT dbid, name, created, log_mode FROM v$database;


--20
SHOW PARAMETER spfile;
select NAME, DESCRIPTION from V$PARAMETER;

--21
SELECT value FROM V$PARAMETER WHERE name = 'user_dump_dest';

create pfile = 'EVP_PFILE.ora' from spfile;
show parameter pfile;
select NAME, DESCRIPTION from V$PARAMETER;

--22
-- Путь /opt/oracle/dbs/
SHOW PARAMETER passwordfile;
SHOW PARAMETER spfile;

select NAME, DESCRIPTION from V$PARAMETER;
--23
SELECT * FROM V$DIAG_INFO;

--24
SELECT VALUE FROM V$DIAG_INFO WHERE NAME = 'Diag Trace';
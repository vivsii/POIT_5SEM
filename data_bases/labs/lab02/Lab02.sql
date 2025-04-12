create tablespace TS_EVP
    datafile 'TS_EVP1.dbf'
    size 7 m
    autoextend on next 5 m
    maxsize 20 m
    extent management local;

create temporary tablespace TS_EVP_TEMP 
    tempfile 'TS_EVP_TEMP.dbf'
    size 5 m
    autoextend on next 3 m 
    maxsize 30 m
    extent management local;

drop tablespace TS_EVP;
drop tablespace TS_EVP_TEMP;

select tablespace_name, status, contents logging from sys.dba_tablespaces;
select file_name, tablespace_name, status, maxbytes, user_bytes from dba_data_files
union
select file_name, tablespace_name, status, maxbytes, user_bytes from dba_temp_files;

------------------------------------------

create role RL_EVPCORE;

drop role RL_EVPCORE;

grant create session, 
    create table, 
    create view, 
    create procedure to RL_EVPCORE;
    
revoke create session, 
    create table, 
    create view, 
    create procedure from RL_EVPCORE;

select role from dba_roles;
select * from dba_sys_privs where grantee = 'RL_EVPCORE';

------------------------------------------

create profile PF_EVPCORE limit
    password_life_time 180
    sessions_per_user 3
    failed_login_attempts 7
    password_lock_time 1
    password_reuse_time 10
    password_grace_time default 
    connect_time 180
    idle_time 30;
    
drop profile PF_EVPCORE;

select profile from dba_profiles;
select * from dba_profiles where profile = 'PF_EVPCORE';
select * from dba_profiles where profile = 'DEFAULT';

-----------------------------------------

create user EVPCORE identified by qwer123
    default tablespace TS_EVP quota unlimited on TS_EVP
    temporary tablespace TS_EVP_TEMP
    profile PF_EVPCORE
    account unlock
    password expire;

grant RL_EVPCORE to EVPCORE;

DROP USER EVPCORE CASCADE;
alter session set "_ORACLE_SCRIPT"=true;

------------------------------------------
create tablespace EVP_QDATA
datafile 'EVP_QDATA.dbf'
size 10 m
offline;

alter tablespace EVP_QDATA online;
alter user EVPCORE QUOTA 2 m on EVP_QDATA;

-----------------------------------------------

create table pickme(
    id int primary key,
    name varchar(50));
    
insert into pickme(id, name) values (1, 'shchavel');
insert into pickme(id, name) values (2, 'normis');

create view pickme_bosses AS
SELECT id, name
FROM pickme;

select * from pickme_bosses;

create table EVP_T1 (
    id int PRIMARY KEY,
    name varchar(50)
) tablespace EVP_QDATA;

INSERT INTO EVP_T1 (id, name) VALUES (1, 'bosskfc');
INSERT INTO EVP_T1 (id, name) VALUES (2, 'plakiplaki');
INSERT INTO EVP_T1 (id, name) VALUES (3, 'normaldaki');

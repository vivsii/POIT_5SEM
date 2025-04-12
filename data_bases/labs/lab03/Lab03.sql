----1

select * from dba_pdbs;

----2

select * from v$instance;

----3

select COMP_NAME, version, STATUS from DBA_REGISTRY;

----6
create tablespace TS_EVP_PDB
datafile  'TS_PDB_EVP.dbf'
size 9M
autoextend on next 4M
maxsize 50M;

create temporary tablespace TS_EVP_PDB_TEMP
tempfile 'TS_PDB_EVP_TEMP.dbf'
size 5M
autoextend on next 2M
maxsize 20M;

select * from dba_tablespaces where TABLESPACE_NAME like '%TS%';
drop tablespace TS_EVP_PDB including contents and datafiles;
drop tablespace TS_EVP_PDB_TEMP including contents and datafiles;

drop role RL_PDB_EVPCORE; 
create role RL_PDB_EVPCORE;

grant connect,
    create session,
    create table,
    create view, 
    create procedure to U1_EVP_PDB;

drop profile PF_PDB_EVP;
create profile PF_PDB_EVP limit
password_life_time 180
sessions_per_user 5
failed_login_attempts 7
password_lock_time 1
password_reuse_time 10
password_grace_time default 
connect_time 180
idle_time 30;

create user U1_EVP_PDB identified by 12345
default tablespace TS_EVP_PDB
quota unlimited on TS_EVP_PDB
temporary tablespace TS_EVP_PDB_TEMP
profile PF_PDB_EVP
account unlock
password expire;

grant RL_PDB_EVPCORE to U1_EVP_PDB;


drop user U1_EVP_PDB CASCADE;
alter session set "_ORACLE_SCRIPT"=true;

----7
create table EVP_table ( x number(2), y varchar(5));
insert into EVP_table values (8, 'HELLO');
insert into EVP_table values (6, 'POKA');
commit;

DROP TABLE EVP_table
select * from EVP_table;

----8
select * from dba_tablespaces;
select * from dba_tablespaces where tablespace_name like 'TS%';
select * from DBA_DATA_FILES;
select * from DBA_TEMP_FILES;
select * from dba_roles;
select * from dba_roles where ROLE like 'RL%';
select * from dba_sys_privs where GRANTEE like 'RL%';
select * from dba_profiles;
select * from dba_users;
select * from dba_profiles where PROFILE like 'PF%';

----9
create user C##EVP identified by 123;
grant create session to C##EVP;

alter session set container=CDB$ROOT;
alter session set container = EVP_PDB;

----12
alter pluggable database EVP_PDB close;
drop pluggable database EVP_PDB including datafiles;
select file_name from dba_data_files where tablespace_name = 'TS_EVP_PDB';
drop user C##EVP cascade;
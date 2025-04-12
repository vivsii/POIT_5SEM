--1 
create user EVP
identified by 123
account unlock;

grant create session,
    create table,
    create view,
    create procedure,
    create sequence,
    create cluster,
    drop any table,
    drop any view,
    drop any procedure,
    drop any sequence,
    drop any cluster
to EVP;

grant all privileges to EVP;

select * from  dba_users;

drop user EVP cascade;
-- 2 
create sequence S1
start with 1000
increment by 10
nominvalue
nomaxvalue
nocycle
nocache
noorder;

select S1.nextval from dual;
select S1.nextval from dual;
select S1.nextval from dual;

select S1.currval from dual;

drop sequence S1;
--3, 4 
create sequence S2
start with 10
increment by 10
nominvalue
maxvalue 100
nocycle;

select S2.nextval from dual;
select S2.nextval from dual;
select S2.nextval from dual;
select S2.nextval from dual;
select S2.nextval from dual;
select S2.nextval from dual;
select S2.nextval from dual;
select S2.nextval from dual;
select S2.nextval from dual;
select S2.nextval from dual;

select S2.nextval from dual;

drop sequence S2;
--5
create sequence S3
start with 10
increment by -10
maxvalue 10
minvalue -100
nocycle
order
cache 2;

select S3.nextval from dual;
select S3.nextval from dual;
select S3.nextval from dual;
select S3.nextval from dual;
select S3.nextval from dual;
select S3.nextval from dual;
select S3.nextval from dual;
select S3.nextval from dual;
select S3.nextval from dual;
select S3.nextval from dual;
select S3.nextval from dual;
select S3.nextval from dual;

select S3.nextval from dual;

drop sequence S3;
--6

create sequence S4
start with 1
increment by 1
maxvalue 10
cycle
cache 5
noorder;

select S4.nextval from dual;
select S4.nextval from dual;
select S4.nextval from dual;
select S4.nextval from dual;
select S4.nextval from dual;
select S4.nextval from dual;
select S4.nextval from dual;
select S4.nextval from dual;
select S4.nextval from dual;
select S4.nextval from dual;

select S4.nextval from dual;

drop sequence S4; 
--7

select * from user_sequences;

--8

create table T1(
    N1 number(20),
    N2 number(20),
    N3 number(20),
    N4 number(20)
) storage (buffer_pool KEEP);

insert into T1 values(S1.nextval,S2.nextval,S3.nextval,S4.nextval);
insert into T1 values(S1.nextval,S2.nextval,S3.nextval,S4.nextval);
insert into T1 values(S1.nextval,S2.nextval,S3.nextval,S4.nextval);
insert into T1 values(S1.nextval,S2.nextval,S3.nextval,S4.nextval);
insert into T1 values(S1.nextval,S2.nextval,S3.nextval,S4.nextval);
insert into T1 values(S1.nextval,S2.nextval,S3.nextval,S4.nextval);
insert into T1 values(S1.nextval,S2.nextval,S3.nextval,S4.nextval);

select * from T1;

drop table T1;

--9

create cluster ABC(
   X number(10),
   V varchar(12)
) hashkeys 200;
   
drop cluster ABC;

--10

create table A (
    XA number(10),
    VA varchar2(12),
    additional_column varchar2(20)
) cluster ABC (XA,VA);

drop table A;
--11

create table B (
    XB number(10),
    VB varchar2(12),
    additional_column varchar2(20)
) cluster ABC (XB,VB);

drop table B;

--12

create table C (
    XC number(10),
    VC varchar2(12),
    additional_column varchar2(20)
) cluster ABC (XC,VC);

drop table C;


--13

select * from user_tables;

select * from user_clusters;



--14

create synonym private_synonym_c for EVP.C;

select * from private_synonym_c;

drop synonym private_synonym_c;

--15

create public synonym public_synonym_b for EVP.B;

select * from public_synonym_b;

drop public synonym public_synonym_b;

--16

create table A16 (
    id number primary key,
    name varchar2(50)
);

 create table B16 (
    id number primary key,
    A_ID number,
    description varchar2(100),
    constraint FK_A_ID foreign key (A_ID) references A16(id)
);

drop table A16;
drop table B16;

insert into A16 (id, name) values (1, 'A1');
insert into A16 (id, name) values (2, 'A2');
insert into A16 (id, name) values (3, 'A3');

insert into B16 (id, A_ID, description) values (1, 1, 'B1');
insert into B16 (id, A_ID, description) values (2, 2, 'B2');
insert into B16 (id, A_ID, description) values (3, 3, 'B3');

create view V1 as
select A16.id as A_ID, A16.name, B16.id as B_ID, B16.description
from A16
inner join B16 on A16.id = B16.A_ID;

select * from V1;

--17

create materialized view MV
refresh complete
start with sysdate
next sysdate + interval '2' minute
as
select A16.id as A_ID, A16.name, B16.id as B_ID, B16.description
from A16
inner join B16 on A16.id = B16.A_ID;

select * from MV;

drop materialized view MV;

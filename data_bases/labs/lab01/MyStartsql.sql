create table EVP_t(x number(3), s varchar2(50) primary key);

insert into EVP_t values(1, 'вивси1');
insert into EVP_t values(22, 'вивси22');
insert into EVP_t values(333, 'вивси333');
 commit;
 
 update EVP_t 
 set s = 'не вивси' 
 where x = 22;
 commit;
 select * from EVP_t;
 select x,s from EVP_t where x > 2;
 select max(x) from EVP_t;
 
 delete from EVP_t where x = 1;
 commit;

 create table EVP_t1(name varchar2(50), age number(3), smth varchar2(50), foreign key(name) references EVP_t(s));
 insert into EVP_t1 values('вивси333',19,'м€у м€у м€у');
 insert into EVP_t1 values('не вивси',19,'м€у м€у м€у');    
 
select a.x, a.s, b.age, b.smth
from EVP_t a left join EVP_t1 b on a.s = b.name;

select a.x, a.s, b.age, b.smth from EVP_t a
right join EVP_t1 b on a.s = b.name;

select a.x, a.s, b.age, b.smth from EVP_t a
inner join EVP_t1 b on a.s = b.name;

drop table EVP_t1;
drop table EVP_t;

insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('206-1',   '206-1', 'ЛБ-К', 15);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
                       values  ('301-1',   '301-1', 'ЛБ-К', 15);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('236-1',   '236-1', 'ЛК',   60);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('313-1',   '313-1', 'ЛК',   60);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('324-1',   '324-1', 'ЛК',   50);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('413-1',   '413-1', 'ЛБ-К', 15);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('423-1',   '423-1', 'ЛБ-К', 90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('408-2',   '408-2', 'ЛК',  90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('103-4',   '103-4', 'ЛК',  90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('105-4',   '105-4', 'ЛК',  90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('107-4',   '107-4', 'ЛК',  90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('110-4',   '110-4', 'ЛК',  30);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('111-4',   '111-4', 'ЛК',  30);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                      values  ('114-4',   '114-4', 'ЛК-К',  90 );
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values ('132-4',   '132-4', 'ЛК',   90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values ('02Б-4',   '02Б-4', 'ЛК',   90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values ('229-4',   '229-4', 'ЛК',   90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('304-4',   '304-4','ЛБ-К', 90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('314-4',   '314-4', 'ЛК',  90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('320-4',   '320-4', 'ЛК',  90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                       values  ('429-4',   '429-4', 'ЛК',  90);
insert into  AUDITORIUM   (AUDITORIUM,   AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY )
                        values  ('311-1',   '311-1', 'ЛК',  90);
-------- 1 --------
declare 
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from FACULTY where rownum = 1;
    dbms_output.put_line(faculty_rec.faculty ||' '|| faculty_rec.faculty_name);
    exception 
        when others
        then dbms_output.put_line(sqlerrm);
end;
/
-------- 2 --------
declare
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from FACULTY;
    dbms_output.put_line(faculty_rec.faculty ||' '|| faculty_rec.faculty_name);
    exception 
        when others
        then dbms_output.put_line(sqlerrm);
end;
/
-------- 3 --------
declare
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from faculty;
    dbms_output.put_line(faculty_rec.faculty || ': ' || faculty_rec.faculty_name);
exception
    when too_many_rows
        then dbms_output.put_line('error too_many_rows: ' || sqlerrm || sqlcode);
end;
-------- 4 --------

declare
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from faculty where faculty = 'ВВ';
    dbms_output.put_line(rtrim(faculty_rec.faculty) || ': ' || faculty_rec.faculty_name);
exception
    when no_data_found then
        dbms_output.put_line('error no_data_found: ' || sqlerrm || ' - ' || sqlcode);
    when others then
        dbms_output.put_line(sqlerrm);
end;



declare
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from faculty where faculty = 'ТОВ';
    dbms_output.put_line(rtrim(faculty_rec.faculty) || ': ' || faculty_rec.faculty_name);

    if sql%found then
        dbms_output.put_line('%found:     true');
    else
        dbms_output.put_line('%found:     false');
    end if;

    if sql%isopen then
        dbms_output.put_line('$isopen:    true');
    else
        dbms_output.put_line('$isopen:    false');
    end if;

    if sql%notfound then
        dbms_output.put_line('%notfound:  true');
    else
        dbms_output.put_line('%notfound:  false');
    end if;

    dbms_output.put_line('%rowcount:  ' || sql%rowcount);
end;

-------- 5 --------
begin
    update AUDITORIUM
    set auditorium_name     = 'qq',
        auditorium_capacity = 50,
        auditorium_type     = 'ЛК'
    where auditorium_name = '206-1';

    if sql%rowcount > 0 then
        commit;
        dbms_output.put_line('Изменения успешно сохранены.');
    else
        rollback;
        dbms_output.put_line('Нет обновлений, откат изменений.');
    end if;

exception
    when others then
        rollback;
        dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

select * from auditorium;

-------- 6 --------
begin
    update auditorium
    set auditorium_type = 'room'
    where auditorium_name = 'qq';
exception
    when others then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

-------- 7 --------

begin
    insert into auditorium VALUES ('44','44', 50, 'ЛК');
    insert into auditorium VALUES ('55', '55', 50, 'ЛК');
    --rollback;
    commit;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

select * from auditorium;

-------- 8 --------

begin
    insert into auditorium VALUES ('3', '2', 50, 'ROOM');
    rollback;
    --commit;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;
select * from auditorium;

-------- 9 --------

begin
    delete from auditorium where auditorium_name = '110-4';
    --commit;
    --rollback;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

select * from auditorium;

-------- 10 -------

begin
    delete from pulpit where pulpit = 'ИСиТ';
    if (sql%rowcount = 0) then
        raise no_data_found;
    end if;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

select * from pulpit;

-------- 11 -------

declare
    cursor curs_teachers is select TEACHER_NAME, PULPIT 
                            from TEACHER;
    m_teacher_name TEACHER.TEACHER_NAME%type;
    m_pulpit       TEACHER.PULPIT%type;
begin
    open curs_teachers;
    loop
        fetch curs_teachers into m_teacher_name, m_pulpit;
        exit when curs_teachers%notfound;
        dbms_output.put_line(' ' || curs_teachers%rowcount || ' '
            || m_teacher_name || ' '
            || m_pulpit);
    end loop;
    close curs_teachers;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

select * from teacher
-------- 12 -------

declare
    cursor curs_subject is
        select subject, subject_name, pulpit
        from subject;
    rec_subject subject%rowtype;
begin
    open curs_subject;
    dbms_output.put_line('rowcount = ' || curs_subject%rowcount);
    fetch curs_subject into rec_subject;
    while (curs_subject%found)
        loop
            dbms_output.put_line(' ' || curs_subject%rowcount || ' '
                || rec_subject.subject || ' '
                || rec_subject.subject_name || ' '
                || rec_subject.pulpit );
            fetch curs_subject into rec_subject;
        end loop;
    dbms_output.put_line('rowcount = ' || curs_subject%rowcount);
    close curs_subject;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

select * from subject
-------- 13 -------

declare
    cursor cur_pulpit_teachers is
        select pulpit.pulpit, teacher.teacher_name
        from pulpit 
        join teacher 
        on pulpit.pulpit = teacher.pulpit;
    rec cur_pulpit_teachers%rowtype;
begin
    for rec in cur_pulpit_teachers
        loop
            dbms_output.put_line(' ' || cur_pulpit_teachers%rowcount || ' '
                || rec.pulpit || ' '
                || rec.teacher_name);
        end loop;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

-------- 14 -------

declare
    cursor curs (low_capacity auditorium.auditorium_capacity%type, high_capacity auditorium.auditorium_capacity%type)
        is select auditorium_name, auditorium_capacity, auditorium_type
           from auditorium
           where auditorium_capacity >= low_capacity
             and auditorium_capacity <= high_capacity;
    record curs%rowtype;
begin
    dbms_output.put_line('capacity < 20 :');
    for aum in curs(0, 20)
        loop
            dbms_output.put_line(aum.auditorium_name || ' ');
        end loop;

    dbms_output.put_line('21 < capacity < 30 :');
    open curs(21, 30);
    fetch curs into record;
    while curs%found
        loop
            dbms_output.put_line(record.auditorium_name || ' ');
            fetch curs into record;
        end loop;
    close curs;

    dbms_output.put_line('31 < capacity < 60 :');
    for aum in curs(31, 60)
        loop
            dbms_output.put_line(aum.auditorium_name || ' ');
        end loop;

    dbms_output.put_line('61 < capacity < 80 :');
    open curs(61, 80);
    fetch curs into record;
    loop
        dbms_output.put_line(record.auditorium_name || ' ');
        fetch curs into record;
        exit when curs%notfound;
    end loop;
    close curs;

    dbms_output.put_line('81 < capacity:');
    for aum in curs(81, 1000)
        loop
            dbms_output.put_line(aum.auditorium_name || ' ');
        end loop;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

-------- 15 -------

declare
    type ref_cursor_type is ref cursor;
    v_auditorium_cursor ref_cursor_type;

    v_auditorium_name auditorium.auditorium_name%type;
    v_auditorium_capacity auditorium.auditorium_capacity%type;
    v_auditorium_type auditorium.auditorium_type%type;

    procedure fetch_auditoriums(p_low_capacity number, p_high_capacity number) is
    begin
        open v_auditorium_cursor for
            select auditorium_name, auditorium_capacity, auditorium_type
            from auditorium
            where auditorium_capacity >= p_low_capacity
              and auditorium_capacity <= p_high_capacity;
    end;
begin
    dbms_output.put_line('capacity < 35:');
    fetch_auditoriums(0, 35);
    loop
        fetch v_auditorium_cursor into v_auditorium_name, v_auditorium_capacity, v_auditorium_type;
        exit when v_auditorium_cursor%notfound;
        dbms_output.put_line(v_auditorium_name || ' ');
    end loop;
    close v_auditorium_cursor;
exception
    when others then
        dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

-------- 16 -------

declare
    cursor c_auditoriums is
        select auditorium_name, auditorium_capacity
        from auditorium
        where auditorium_capacity in (
            select auditorium_capacity
            from auditorium
            where auditorium_type = 'ЛК'
        );

    v_auditorium_name auditorium.auditorium_name%type;
    v_auditorium_capacity auditorium.auditorium_capacity%type;

begin
    open c_auditoriums;
    loop
        fetch c_auditoriums into v_auditorium_name, v_auditorium_capacity;
        exit when c_auditoriums%notfound;
        dbms_output.put_line('Аудитория: ' || v_auditorium_name || ' - Вместимость: ' || v_auditorium_capacity);
    end loop;
    close c_auditoriums;

exception
    when others then
        dbms_output.put_line('Ошибка: ' || sqlerrm);
end;

select * from auditorium

-------- 17 -------

declare
    cursor c_auditoriums (low_capacity number, high_capacity number) is
        select auditorium_name, auditorium_capacity
        from auditorium
        where auditorium_capacity between low_capacity and high_capacity
        for update; -- Указываем, что курсор будет использоваться для обновления

begin
    for aum in c_auditoriums(40, 80)
    loop
        update auditorium
        set auditorium_capacity = auditorium_capacity * 0.9
        where current of c_auditoriums;

        dbms_output.put_line('Уменьшена вместимость аудитории: ' || aum.auditorium_name || 
                             ' до ' || (aum.auditorium_capacity * 0.9));
    end loop;

exception
    when others then
        dbms_output.put_line('Ошибка: ' || sqlerrm);
end;
rollback;
select * from auditorium;

-------- 18 -------

declare
    cursor curs_auditorium(capacity_low auditorium.auditorium_name%type, capacity_high auditorium.auditorium_name%type)
        is
        select auditorium_name, auditorium_capacity
        from auditorium
        where auditorium_capacity >= capacity_low
          and AUDITORIUM_CAPACITY <= capacity_high
            for update;
    aum auditorium.auditorium_name%type;
    cty auditorium.auditorium_capacity%type;
begin
    open curs_auditorium(0, 20);
    fetch curs_auditorium into aum, cty;

    while(curs_auditorium%found)
        loop
            delete auditorium
            where current of curs_auditorium;
            dbms_output.put_line(' ' || aum || ' ' || cty);
            fetch curs_auditorium into aum, cty;
        end loop;

    close curs_auditorium;
    --commit;
    rollback;
exception
    when others then
        dbms_output.put_line(sqlerrm);
end;

INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY, AUDITORIUM_TYPE) VALUES ('555', '555', 10, 'ЛК');
commit;
select * from auditorium;
-------- 19 -------

declare
    v_rowid_row rowid;
begin
    select rowid
    into v_rowid_row
    from auditorium
    where auditorium_capacity < 50
    and rownum = 1;

    update auditorium
    set auditorium_capacity = auditorium_capacity * 1.1
    where rowid = v_rowid_row;

    dbms_output.put_line('Обновлена аудитория с ROWID: ' || v_rowid_row);

    delete from auditorium
    where rowid = v_rowid_row;

    dbms_output.put_line('Удалена аудитория с ROWID: ' || v_rowid_row);

exception
    when others then
        dbms_output.put_line('Ошибка: ' || sqlerrm);
end;
select * from auditorium;
rollback;
-------- 20 -------

declare
    cursor cur is
        select *
        from teacher;
    rec teacher%rowtype;
begin
    open cur;
    loop
        fetch cur into rec;
        exit when cur%notfound;
        
         dbms_output.put_line(' ' || rec.teacher_name || ' ' || rec.pulpit);
         if (mod(cur%rowcount, 3) = 0) then
            dbms_output.put_line('-----------------------');
        end if;
    end loop;
    close cur;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

-- 1
ALTER TABLE TEACHER ADD SALARY NUMBER(10,2);
ALTER TABLE TEACHER ADD BIRTHDAY DATE;

BEGIN
  FOR rec IN (SELECT TEACHER FROM TEACHER) LOOP
    UPDATE TEACHER
    SET
      TEACHER.SALARY = ROUND(DBMS_RANDOM.VALUE(500, 5000)),
      TEACHER.BIRTHDAY = DATE '1960-01-01' + TRUNC(DBMS_RANDOM.VALUE(0, (DATE '1995-12-31' - DATE '1960-01-01')))
    WHERE
      TEACHER = rec.TEACHER;

  END LOOP;
  COMMIT;
END;

SELECT * FROM TEACHER;



-- 2
DECLARE
   v_teacher_name TEACHER.TEACHER_NAME%TYPE;
   v_firstName VARCHAR2(100);
   v_secondName VARCHAR2(100);
   v_thirdName VARCHAR2(100);
BEGIN
   FOR rec IN (SELECT TEACHER_NAME FROM TEACHER) LOOP
      v_firstName := REGEXP_SUBSTR(rec.TEACHER_NAME, '[^ ]+', 1, 1);
      v_secondName := REGEXP_SUBSTR(REC.TEACHER_NAME, '[^ ]+', 1, 2);
      v_thirdName := REGEXP_SUBSTR(REC.TEACHER_NAME, '[^ ]+', 1, 3);

      DBMS_OUTPUT.PUT_LINE(
               rec.TEACHER_NAME || '   -   ' ||
              INITCAP(v_firstName) || ' ' || UPPER(SUBSTR(v_secondName, 0 , 1)) || '.' || UPPER(SUBSTR(v_thirdName, 0 , 1)) || '.');
   END LOOP;
END;

-- 3
DECLARE
    v_teacher_name TEACHER.TEACHER%TYPE;
    v_birthday DATE;

BEGIN
    FOR rec IN (SELECT TEACHER, BIRTHDAY FROM TEACHER) LOOP
        v_teacher_name := rec.TEACHER;
        v_birthday := rec.BIRTHDAY;

        IF TO_CHAR(v_birthday, 'DY', 'NLS_DATE_LANGUAGE=RUSSIAN') = 'ом' THEN
            DBMS_OUTPUT.PUT_LINE(v_teacher_name || ' - ' || TO_CHAR(v_birthday, 'DD.MM.YYYY'));
        END IF;
    END LOOP;
END;

-- 4
CREATE OR REPLACE VIEW TEACHERS_NEXT_MONTHS_BIRTHDAY AS
SELECT TEACHER, BIRTHDAY
FROM TEACHER
WHERE EXTRACT(MONTH FROM BIRTHDAY) = EXTRACT(MONTH FROM ADD_MONTHS(SYSDATE, 1))
  AND EXTRACT(YEAR FROM BIRTHDAY) != EXTRACT(YEAR FROM SYSDATE);

SELECT * FROM TEACHERS_NEXT_MONTHS_BIRTHDAY;

-- 5
CREATE OR REPLACE VIEW TEACHER_BIRTHDAY_COUNT_BY_MONTH AS
SELECT EXTRACT(MONTH FROM BIRTHDAY) AS BIRTH_MONTH, COUNT(*) AS TEACHER_COUNT
FROM TEACHER
GROUP BY EXTRACT(MONTH FROM BIRTHDAY)
ORDER BY BIRTH_MONTH;

SELECT * FROM TEACHER_BIRTHDAY_COUNT_BY_MONTH;
-- 6
DECLARE
    CURSOR teachers_anniversary IS
        SELECT TEACHER, BIRTHDAY
        FROM TEACHER
        WHERE MOD(EXTRACT(YEAR FROM SYSDATE) + 1 - EXTRACT(YEAR FROM BIRTHDAY), 10) = 0;

    v_teacher TEACHER.TEACHER%TYPE;
    v_birthday DATE;
BEGIN
    FOR rec IN teachers_anniversary LOOP
        v_teacher := rec.TEACHER;
        v_birthday := rec.BIRTHDAY;

        DBMS_OUTPUT.PUT_LINE(v_teacher || ' - ' || TO_CHAR(v_birthday, 'DD.MM.YYYY'));
    END LOOP;
END;

-- 7
DECLARE
  CURSOR c_avg_salary_pulpit IS
    SELECT P.PULPIT, P.FACULTY, TRUNC(AVG(T.SALARY)) AS avg_salary
    FROM TEACHER T JOIN PULPIT P ON T.PULPIT = P.PULPIT
    GROUP BY P.PULPIT, P.FACULTY
    ORDER BY P.FACULTY;

  v_current_faculty VARCHAR2(50);
  v_total_avg_salary_faculty NUMBER := 0;
  v_faculty_count NUMBER := 0;
  v_total_avg_salary_all_faculties NUMBER := 0;
  v_total_pulpit_count NUMBER := 0;
BEGIN
  DBMS_OUTPUT.PUT_LINE('Average salary by department:');
  FOR rec IN c_avg_salary_pulpit LOOP
    IF v_current_faculty IS NOT NULL AND v_current_faculty != rec.FACULTY THEN
      DBMS_OUTPUT.PUT_LINE('Average salary for faculty ' || v_current_faculty || ': ' || TRUNC(v_total_avg_salary_faculty / v_faculty_count));
      v_total_avg_salary_faculty := 0;
      v_faculty_count := 0;
    END IF;
    DBMS_OUTPUT.PUT_LINE('Pulpit: ' || rec.PULPIT || ', Faculty: ' || rec.FACULTY || ', Avg Salary: ' || rec.avg_salary);

    v_total_avg_salary_faculty := v_total_avg_salary_faculty + rec.avg_salary;
    v_faculty_count := v_faculty_count + 1;
    v_total_avg_salary_all_faculties := v_total_avg_salary_all_faculties + rec.avg_salary;
    v_total_pulpit_count := v_total_pulpit_count + 1;
    v_current_faculty := rec.FACULTY;
  END LOOP;
  IF v_faculty_count > 0 THEN
    DBMS_OUTPUT.PUT_LINE('Average salary for faculty ' || v_current_faculty || ': ' || TRUNC(v_total_avg_salary_faculty / v_faculty_count));
  END IF;
  DBMS_OUTPUT.PUT_LINE('Average salary for all faculties: ' || TRUNC(v_total_avg_salary_all_faculties / v_total_pulpit_count));
END;

-- 8
DECLARE
  TYPE teacher_rec IS RECORD (
    teacher_id   NUMBER,
    teacher_name VARCHAR2(50),
    salary       NUMBER(7,2),
    birthday     DATE
  );
  TYPE pulpit_rec IS RECORD (
    pulpit_name  VARCHAR2(50),
    teacher_info teacher_rec
  );
  v_teacher teacher_rec;
  v_pulpit  pulpit_rec;
BEGIN
  v_teacher.teacher_id := 101;
  v_teacher.teacher_name := 'John Doe';
  v_teacher.salary := 5000.75;
  v_teacher.birthday := TO_DATE('1980-03-15', 'YYYY-MM-DD');
  DBMS_OUTPUT.PUT_LINE('Teacher ID: ' || v_teacher.teacher_id);
  DBMS_OUTPUT.PUT_LINE('Teacher Name: ' || v_teacher.teacher_name);
  DBMS_OUTPUT.PUT_LINE('Salary: ' || v_teacher.salary);
  DBMS_OUTPUT.PUT_LINE('Birthday: ' || TO_CHAR(v_teacher.birthday, 'DD-MM-YYYY'));

  v_pulpit.pulpit_name := 'Mathematics';
  v_pulpit.teacher_info := v_teacher;
  DBMS_OUTPUT.PUT_LINE('Pulpit Name: ' || v_pulpit.pulpit_name);
  DBMS_OUTPUT.PUT_LINE('Teacher Name in Pulpit: ' || v_pulpit.teacher_info.teacher_name);

  v_teacher := v_pulpit.teacher_info;
  DBMS_OUTPUT.PUT_LINE('After Assignment - Teacher Name: ' || v_teacher.teacher_name);
END;
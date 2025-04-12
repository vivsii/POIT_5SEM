-- 1
BEGIN
  NULL; 
END;

-- 2
BEGIN
  DBMS_OUTPUT.PUT_LINE('Hello World!');
END;

-- 3
BEGIN
  DECLARE
    v_num NUMBER := 1;
    v_den NUMBER := 0;
    v_result NUMBER;
  BEGIN
    v_result := v_num / v_den;
  EXCEPTION
    WHEN OTHERS THEN
      DBMS_OUTPUT.PUT_LINE('Код ошибки: ' || SQLCODE);
      DBMS_OUTPUT.PUT_LINE('Сообщение ошибки: ' || SQLERRM);
  END;
END;


-- 4
BEGIN
  DECLARE
    v_outer_var NUMBER := 10;
    
  BEGIN
    DBMS_OUTPUT.PUT_LINE('Внешний блок: Начало работы.');
    DECLARE
      v_inner_var NUMBER := 0;
      
    BEGIN
      DBMS_OUTPUT.PUT_LINE('Вложенный блок: Начало работы.');
      v_inner_var := v_outer_var / v_inner_var;
      DBMS_OUTPUT.PUT_LINE('Вложенный блок: Это сообщение не будет выведено.');
    EXCEPTION
      WHEN ZERO_DIVIDE THEN
        DBMS_OUTPUT.PUT_LINE('Вложенный блок: Ошибка деления на ноль.');
    END;

    DBMS_OUTPUT.PUT_LINE('Внешний блок: Завершение работы.');
  EXCEPTION
    WHEN OTHERS THEN
      DBMS_OUTPUT.PUT_LINE('Внешний блок: Обработано общее исключение.');
  END;
END;


-- 5
BEGIN
  DBMS_OUTPUT.PUT_LINE('Текущие настройки предупреждений:');
  DBMS_OUTPUT.PUT_LINE(DBMS_WARNING.get_warning_setting_string);
END;
/

--6

select keyword from v_$reserved_words
where length = 1 and keyword != 'A';


--7

select keyword from v_$reserved_words
where length > 1 and keyword != 'A' order by keyword;

-- 8
--docker exec -it oracle-db /bin/bash
--sqlplus sys@oracle-db as sysdba
--SHOW PARAMETERS plsql;
BEGIN
    FOR param IN (SELECT NAME, VALUE, ISMODIFIED, ISDEFAULT 
                  FROM V$PARAMETER 
                  WHERE NAME LIKE '%plsql%') LOOP
        DBMS_OUTPUT.PUT_LINE(param.NAME || ' = ' || param.VALUE || 
                             ' | Изменен: ' || param.ISMODIFIED || 
                             ' | Значение по умолчанию: ' || param.ISDEFAULT);
    END LOOP;
END;

-- 9
DECLARE
    -- 10. Объявление и инициализация целых number-переменных
    v_num1 NUMBER := 10;
    v_num2 NUMBER := 3;
    v_sum NUMBER;
    v_diff NUMBER;
    v_product NUMBER;
    v_quotient NUMBER;
    v_remainder NUMBER;

    -- 12. Объявление и инициализация number-переменных с фиксированной точкой
    v_fixed_point1 NUMBER(10, 2) := 12.34;
    v_fixed_point2 NUMBER(10, 2) := 56.78;
    v_fixed_sum NUMBER(10, 2);

    -- 13. Объявление и инициализация number-переменных с фиксированной точкой и отрицательным масштабом
    v_rounded_value NUMBER(10, -1) := 125.67; -- Будет округлено до 130

    -- 14. Объявление и инициализация BINARY_FLOAT-переменной
    v_float_value BINARY_FLOAT := 1.234567F;

    -- 15. Объявление и инициализация BINARY_DOUBLE-переменной
    v_double_value BINARY_DOUBLE := 3.14159265358979D;

    -- 16. Объявление number-переменных с точкой и применением символа E
    v_scientific1 NUMBER := 1E2;  -- 100
    v_scientific2 NUMBER := 2.5E-1; -- 0.25

    -- 17. Объявление и инициализация BOOLEAN-переменных
    v_is_valid BOOLEAN := TRUE;
    v_is_error BOOLEAN := FALSE;

BEGIN
    -- 11. Арифметические действия над двумя целыми number-переменными
    v_sum := v_num1 + v_num2;
    v_diff := v_num1 - v_num2;
    v_product := v_num1 * v_num2;
    v_quotient := v_num1 / v_num2;
    v_remainder := MOD(v_num1, v_num2); 

    DBMS_OUTPUT.PUT_LINE('Сумма: ' || TO_CHAR(v_sum));
    DBMS_OUTPUT.PUT_LINE('Разность: ' || TO_CHAR(v_diff));
    DBMS_OUTPUT.PUT_LINE('Произведение: ' || TO_CHAR(v_product));
    DBMS_OUTPUT.PUT_LINE('Частное: ' || TO_CHAR(v_quotient));
    DBMS_OUTPUT.PUT_LINE('Остаток: ' || TO_CHAR(v_remainder));

    DBMS_OUTPUT.PUT_LINE('Фиксированная точка 1: ' || TO_CHAR(v_fixed_point1));
    DBMS_OUTPUT.PUT_LINE('Фиксированная точка 2: ' || TO_CHAR(v_fixed_point2));

    v_fixed_sum := v_fixed_point1 + v_fixed_point2;
    DBMS_OUTPUT.PUT_LINE('Сумма фиксированных точек: ' || TO_CHAR(v_fixed_sum));

    DBMS_OUTPUT.PUT_LINE('Округлённое значение: ' || TO_CHAR(v_rounded_value));
    DBMS_OUTPUT.PUT_LINE('BINARY_FLOAT: ' || TO_CHAR(v_float_value));
    DBMS_OUTPUT.PUT_LINE('BINARY_DOUBLE: ' || TO_CHAR(v_double_value));
    DBMS_OUTPUT.PUT_LINE('Научное значение 1: ' || TO_CHAR(v_scientific1));
    DBMS_OUTPUT.PUT_LINE('Научное значение 2: ' || TO_CHAR(v_scientific2));

    DBMS_OUTPUT.PUT_LINE('Boolean is_valid: ' || TO_CHAR(CASE WHEN v_is_valid THEN 'TRUE' ELSE 'FALSE' END));
    DBMS_OUTPUT.PUT_LINE('Boolean is_error: ' || TO_CHAR(CASE WHEN v_is_error THEN 'TRUE' ELSE 'FALSE' END));

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Ошибка: ' || SQLERRM);
END;

-- 18
DECLARE
    c_string_constant CONSTANT VARCHAR2(50) := 'Hello, World!';
    c_char_constant CONSTANT CHAR(10) := 'PL/SQL';
    c_number_constant CONSTANT NUMBER := 100;
    v_concatenated_string VARCHAR2(100);
    v_incremented_number NUMBER;
    
BEGIN
    v_concatenated_string := c_string_constant || ' - ' || c_char_constant;
    v_incremented_number := c_number_constant + 50;
    DBMS_OUTPUT.PUT_LINE('Конкатенированная строка: ' || v_concatenated_string);
    DBMS_OUTPUT.PUT_LINE('Увеличенное число: ' || v_incremented_number);
    -- c_string_constant := 'New Value';
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Ошибка: ' || SQLERRM);
END;

-- 19
DECLARE
    v_faculty_name FACULTY.FACULTY_NAME%TYPE;
    v_teacher_name TEACHER.TEACHER_NAME%TYPE;
    v_subject_name SUBJECT.SUBJECT_NAME%TYPE;

BEGIN
    v_faculty_name := 'faculty1';
    v_teacher_name := 'teacher1';
    v_subject_name := 'subject1';

    DBMS_OUTPUT.PUT_LINE('Имя факультета: ' || v_faculty_name);
    DBMS_OUTPUT.PUT_LINE('Имя преподавателя: ' || v_teacher_name);
    DBMS_OUTPUT.PUT_LINE('Название предмета: ' || v_subject_name);
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Ошибка: ' || SQLERRM);
END;

-- 20
DECLARE
    v_faculty_record FACULTY%ROWTYPE;

BEGIN
    SELECT FACULTY, FACULTY_NAME
    INTO v_faculty_record
    FROM FACULTY
    WHERE ROWNUM = 1; 

    DBMS_OUTPUT.PUT_LINE('Код факультета: ' || v_faculty_record.FACULTY);
    DBMS_OUTPUT.PUT_LINE('Название факультета: ' || v_faculty_record.FACULTY_NAME);

EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('Нет данных в таблице FACULTY.');
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Ошибка: ' || SQLERRM);
END;


-- 21
DECLARE
    v_num1 NUMBER := 10;
    v_num2 NUMBER := 5;
    v_result VARCHAR2(50);
BEGIN
    IF v_num1 > v_num2 THEN
        DBMS_OUTPUT.PUT_LINE('v_num1 больше, чем v_num2');
    END IF;

    IF v_num1 < v_num2 THEN
        DBMS_OUTPUT.PUT_LINE('v_num1 меньше, чем v_num2');
    ELSE
        DBMS_OUTPUT.PUT_LINE('v_num1 не меньше, чем v_num2');
    END IF;

    IF v_num1 = v_num2 THEN
        DBMS_OUTPUT.PUT_LINE('v_num1 равен v_num2');
    ELSIF v_num1 > v_num2 THEN
        DBMS_OUTPUT.PUT_LINE('v_num1 больше, чем v_num2'); 
    ELSE
        DBMS_OUTPUT.PUT_LINE('v_num1 меньше, чем v_num2');
    END IF;

    IF v_num1 > v_num2 THEN
        IF v_num1 > 0 THEN
            DBMS_OUTPUT.PUT_LINE('v_num1 положительное и больше, чем v_num2');
        ELSE
            DBMS_OUTPUT.PUT_LINE('v_num1 отрицательное, но больше, чем v_num2');
        END IF;
    END IF;
END;

-- 23 
DECLARE
    v_grade CHAR(1) := 'B';
    v_description VARCHAR2(50);

    v_num NUMBER := 5; 
    v_result VARCHAR2(50);
BEGIN
    v_description := CASE v_grade
        WHEN 'A' THEN 'Отлично'
        WHEN 'B' THEN 'Хорошо'
        WHEN 'C' THEN 'Удовлетворительно'
        WHEN 'D' THEN 'Плохо'
        ELSE 'Неизвестная оценка'
    END;
    DBMS_OUTPUT.PUT_LINE('Оценка: ' || v_grade || ' - ' || v_description);

    v_result := CASE
        WHEN v_num > 0 THEN 'Положительное число'
        WHEN v_num < 0 THEN 'Отрицательное число'
        ELSE 'Число равно нулю'
    END;
    DBMS_OUTPUT.PUT_LINE('Результат: ' || v_result);
END;

-- 24
DECLARE
    v_counter NUMBER := 0;
    v_sum NUMBER := 0;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Бесконечный цикл с условием выхода:');
    LOOP
        v_counter := v_counter + 1;
        DBMS_OUTPUT.PUT_LINE('Итерация ' || v_counter);
        
        IF v_counter = 5 THEN
            EXIT;
        END IF;
    END LOOP;

    v_counter := 1;
    DBMS_OUTPUT.PUT_LINE('WHILE LOOP:');
    WHILE v_counter <= 5 LOOP
        v_sum := v_sum + v_counter;
        DBMS_OUTPUT.PUT_LINE('Счетчик: ' || v_counter || ', Сумма: ' || v_sum);
        v_counter := v_counter + 1;
    END LOOP;

    DBMS_OUTPUT.PUT_LINE('FOR LOOP:');
    FOR i IN 1..5 LOOP
        DBMS_OUTPUT.PUT_LINE('Итерация ' || i || ', Квадрат числа: ' || i*i);
    END LOOP;

END;




































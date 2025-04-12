--1. размер области SGA
    select * from V$SGA;
    select SUM(value) from v$sga;
--2. текущие размеры основных пулов SGA
    select * from v$sga_dynamic_components where current_size > 0;
--3. размеры гранулы для каждого пула
    select component, granule_size from v$sga_dynamic_components where current_size > 0;
--4. объем доступной свободной памяти в SGA
    select current_size from v$sga_dynamic_free_memory;
--5. максимальный и целевой размер области SGA
    select value from v$parameter where name = 'sga_target';
    select value from v$parameter where name = 'sga_max_size';
--6. размеры пулов КЕЕP, DEFAULT и RECYCLE буферного кэша
    select component, current_size, min_size, MAX_SIZE from v$sga_dynamic_components
    where component='KEEP buffer cache' or component='DEFAULT buffer cache' or component='RECYCLE buffer cache';
    
--7. cоздайте таблицу, которая будет помещаться в пул КЕЕP. Продемонстрируйте сегмент таблицы.
    create table vivsikeep_lab5 (
        id number,
        name varchar2(50)
    ) storage (buffer_pool keep) tablespace users;

    select segment_name, segment_type, tablespace_name, buffer_pool
    from user_segments
    where segment_name = 'VIVSIKEEP_LAB5';

    drop table vivsikeep_lab5;

--8. cоздайте таблицу, которая будет кэшироваться в пуле DEFAULT. Продемонстрируйте сегмент таблицы. 
    create table vivsidefault_lab5 (
      id number,
      name varchar2(50),
      age number
    ) cache;

    select segment_name, segment_type, tablespace_name, buffer_pool
    from user_segments
    where segment_name = 'VIVSIDEFAULT_LAB5';

    drop table cachedTableLab5;

--9. Найдите размер буфера журналов повтора
    show parameter log_buffer;

--10. Найдите размер свободной памяти в большом пуле.
    select POOL, sum(BYTES) from v$sgastat where pool = 'large pool' AND name = 'free memory'
    group by POOL;
    
--11. определите режимы текущих соединений с инстансом (dedicated, shared).
    select username, service_name, server, osuser, machine, program, STATE from v$session
    where username is not null;

--12. получите полный список работающих в настоящее время фоновых процессов.
    SELECT name, description FROM v$bgprocess;

--13. получите список работающих в настоящее время серверных процессов.
    select * from v$process where addr != '00';

--14. определите, сколько процессов DBWn работает в настоящий момент.
    select count(*) from v$process where addr!= '00' and pname like 'DBW%';

--15. определите сервисы (точки подключения экземпляра).
    select * from v$active_services;
    select * from v$services;

--16. получите известные вам параметры диспетчеров
    select * from v$dispatcher;
    show parameter dispatchers;

--17. укажите в списке Windows-сервисов сервис, реализующий процесс LISTENER
    select * from v$services;
    --OracleOraDB12Home4TNSListener
    
--18. продемонстрируйте и поясните содержимое файла LISTENER.ORA. 
    --oradata/dbconfig/XE

--19. запустите утилиту lsnrctl и поясните ее основные команды. 
    /*
    docker exec -it oracle-db /bin/bash
        1. start - Запускает слушатель баз данных Oracle.
        2. servacls - Отображает список сервисов и их доступа для подключений через слушателя.
        3. trace - Включает или отключает функцию трассировки для слушателя.
        4. show - Отображает текущие настройки слушателя или информацию о подключенных клиентах.
        5. stop - Останавливает слушатель баз данных Oracle.
        6. version - Отображает версию слушателя.
        7. quit или exit - Выходит из lsnrctl.
        8. status - Отображает текущий статус слушателя.
        9. reload - Перезагружает конфигурацию слушателя без его остановки.
        10. services - Отображает список доступных сервисов, которые могут быть запущены через слушателя.
        11. save_config - Сохраняет текущую конфигурацию слушателя в файл.
    */

--20. получите список служб инстанса, обслуживаемых процессом LISTENER
    --services
using System;
using System.Threading.Tasks;

class Program
{
    const int TaskCount = 20;               // Количество задач
    const int ThreadLifeTime = 10;          // Время работы каждой задачи (в секундах)
    const int ObservationTime = 30;         // Время наблюдения за задачами (в секундах)
    static int[,] Matrix = new int[TaskCount, ObservationTime];   // Матрица для сохранения данных
    static DateTime StartTime = DateTime.Now;                     // Время начала работы

    // Метод для работы каждой задачи
    static void Work(int id)
    {
        for (int i = 0; i < ThreadLifeTime * 20; i++)  // 20 циклов работы в 1 секунду (50 мс)
        {
            DateTime CurrentTime = DateTime.Now;
            int ElapsedSeconds = (int)Math.Round(CurrentTime.Subtract(StartTime).TotalSeconds - 0.49); // Округляем время

            // Записываем информацию о текущей секунде работы задачи
            if (ElapsedSeconds >= 0 && ElapsedSeconds < ObservationTime)
            {
                Matrix[id, ElapsedSeconds] += 50;   // Каждые 50 мс задача добавляет 50 к своей ячейке в матрице
            }

            MySleep(50);  // Имитируем вычисления (метод MySleep)
        }
    }

    static void Main(string[] args)
    {
        Task[] tasks = new Task[TaskCount];

        Console.WriteLine("A student ... is creating tasks...");

        // Создание и запуск задач
        for (int i = 0; i < TaskCount; i++)
        {
            int taskId = i;
            tasks[i] = Task.Run(() => Work(taskId));
        }

        Console.WriteLine("A student ... is waiting for tasks to finish...");

        // Ожидание завершения всех задач
        Task.WaitAll(tasks);

        // Вывод результатов работы задач в виде таблицы
        for (int s = 0; s < ObservationTime; s++)
        {
            Console.Write("{0,3}: ", s);
            for (int th = 0; th < TaskCount; th++)
            {
                Console.Write(" {0,5}", Matrix[th, s]);
            }
            Console.WriteLine();
        }
    }

    // Метод MySleep для имитации вычислений (время сна)
    static Double MySleep(int ms)
    {
        Double Sum = 0, Temp;
        for (int t = 0; t < ms; ++t)
        {
            Temp = 0.711 + (Double)t / 10000.0;
            Double a, b, c, d, e, nt;
            for (int k = 0; k < 68000; ++k) // Увеличено до 68000
            {
                nt = Temp - k / 27000.0;
                a = Math.Sin(nt);
                b = Math.Cos(nt);
                c = Math.Cos(nt / 2.0);
                d = Math.Sin(nt / 2);
                e = Math.Abs(1.0 - a * a - b * b) + Math.Abs(1.0 - c * c - d * d);
                Sum += e;
            }
        }
        return Sum;
    }
}

using System;
using System.Threading;

class Program
{
    const int ThreadCount = 20;            // Количество потоков
    const int ThreadLifeTime = 10;         // Время работы каждого потока (в секундах)
    const int ObservationTime = 30;        // Время наблюдения за потоками (в секундах)
    static int[,] Matrix = new int[ThreadCount, ObservationTime];   // Матрица для сохранения данных
    static DateTime StartTime = DateTime.Now;                      // Время начала работы

    // Метод для работы каждого потока
    static void WorkThread(object o)
    {
        int id = (int)o;   // ID потока (номер)

        // Цикл работы потока
        for (int i = 0; i < ThreadLifeTime * 20; i++)  // 20 циклов работы в 1 секунду (чтобы заполнять таблицу каждые 50 мс)
        {
            DateTime CurrentTime = DateTime.Now;
            int ElapsedSeconds = (int)Math.Round(CurrentTime.Subtract(StartTime).TotalSeconds - 0.49); // Округляем время

            // Записываем информацию о текущей секунде работы потока
            if (ElapsedSeconds >= 0 && ElapsedSeconds < ObservationTime)
            {
                Matrix[id, ElapsedSeconds] += 50;   // Каждые 50 мс поток добавляет 50 к своей ячейке в матрице
            }

            MySleep(50);  // Имитируем вычисления методом MySleep (из задания 5)
        }
    }

    static void Main(string[] args)
    {
        // Массив потоков
        Thread[] t = new Thread[ThreadCount];

        // Запускаем потоки
        for (int i = 0; i < ThreadCount; ++i)
        {
            object o = i;   // Передаем идентификатор потока в качестве параметра
            t[i] = new Thread(WorkThread);
            t[i].Start(o);
        }

        // Ожидаем завершения работы потоков
        Console.WriteLine("A student ... is waiting for the threads to finish");
        for (int i = 0; i < ThreadCount; ++i)
            t[i].Join();

        // Выводим информацию о работе потоков в виде таблицы
        for (int s = 0; s < ObservationTime; s++)
        {
            Console.Write("{0,3}: ", s);   // Выводим номер секунды
            for (int th = 0; th < ThreadCount; th++)
            {
                Console.Write(" {0,5}", Matrix[th, s]);  // Выводим данные о работе каждого потока в данной секунде
            }
            Console.WriteLine();
        }
    }

    // Метод MySleep для имитации вычислений (из задания 5)
    static Double MySleep(int ms)
    {
        Double Sum = 0, Temp;
        for (int t = 0; t < ms; ++t)
        {
            Temp = 0.711 + (Double)t / 10000.0;
            Double a, b, c, d, e, nt;
            for (int k = 0; k < 68000; ++k) 
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
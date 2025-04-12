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
        Console.WriteLine("A student ... is placing threads to the pool...");
        for (int i = 0; i < ThreadCount; ++i)
        {
            object o = i;
            ThreadPool.QueueUserWorkItem(WorkThread, o);
        }
        Console.WriteLine("A student ... is waiting for the threads to finish...");
        Thread.Sleep(1000 * ObservationTime);
        for (int s = 0; s < ObservationTime; s++)
        {
            Console.Write("{0,3}: ", s);
            for (int th = 0; th < ThreadCount; th++)
            {
                Console.Write(" {0,5}", Matrix[th, s]);
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




//using System;
//using System.Threading;

//class Program
//{
//    const int ThreadCount = 20;       // Количество потоков
//    const int ThreadLifeTime = 10;    // Время работы каждого потока (в секундах)
//    const int ObservationTime = 30;   // Время наблюдения (в секундах)
//    static int[,] Matrix = new int[ThreadCount, ObservationTime]; // Матрица для сохранения статистики работы потоков
//    static DateTime StartTime = DateTime.Now;  // Время начала работы потоков

//    static void WorkThread(object o)
//    {
//        int id = (int)o;
//        for (int i = 0; i < ThreadLifeTime * 20; i++)  // Цикл, выполняющийся в течение времени жизни потока
//        {
//            DateTime CurrentTime = DateTime.Now;
//            int ElapsedSeconds = (int)Math.Round(CurrentTime.Subtract(StartTime).TotalSeconds - 0.49);

//            // Проверяем, чтобы индекс времени не вышел за границы массива
//            if (ElapsedSeconds >= 0 && ElapsedSeconds < ObservationTime)
//            {
//                Matrix[id, ElapsedSeconds] += 50;
//            }

//            MySleep(50);  // Функция из задания 5
//        }
//    }

//    static void Main(string[] args)
//    {
//        Console.WriteLine("A student ... is placing threads to the pool...");

//        // Используем пул потоков
//        for (int i = 0; i < ThreadCount; ++i)
//        {
//            object o = i;
//            ThreadPool.QueueUserWorkItem(WorkThread, o);  // Добавляем задачи в пул потоков
//        }

//        Console.WriteLine("A student ... is waiting for the threads to finish...");

//        // Ждём завершения работы потоков (время наблюдения)
//        Thread.Sleep(1000 * ObservationTime);

//        // Выводим результаты в виде таблицы
//        for (int s = 0; s < ObservationTime; s++)
//        {
//            Console.Write("{0,3}: ", s);
//            for (int th = 0; th < ThreadCount; th++)
//            {
//                Console.Write(" {0,5}", Matrix[th, s]);
//            }
//            Console.WriteLine();
//        }
//    }

//    // Функция, симулирующая выполнение работы потока в течение определённого времени
//    static Double MySleep(int ms)
//    {
//        Double Sum = 0, Temp;
//        for (int t = 0; t < ms; ++t)
//        {
//            Temp = 0.711 + (Double)t / 10000.0;
//            Double a, b, c, d, e, nt;
//            for (int k = 0; k < 5500; ++k)
//            {
//                nt = Temp - k / 27000.0;
//                a = Math.Sin(nt);
//                b = Math.Cos(nt);
//                c = Math.Cos(nt / 2.0);
//                d = Math.Sin(nt / 2);
//                e = Math.Abs(1.0 - a * a - b * b) + Math.Abs(1.0 - c * c - d * d);
//                Sum += e;
//            }
//        }
//        return Sum;
//    }
//}

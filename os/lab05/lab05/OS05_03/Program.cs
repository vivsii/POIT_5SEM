using System;
using System.Diagnostics;
using System.Threading;

class Program
{
    // Количество потоков
    const int ThreadCount = 8;
    static Thread[] t = new Thread[ThreadCount];

    static void WorkThread(object o)
    {
        int threadNumber = (int)o;
        for (int i = 0; i < 5; i++)
        {
            Console.WriteLine($"Поток {threadNumber}: итерация {i}");
            Thread.Sleep(100); // Имитируем работу потока
        }
    }

    static void Main()
    {
        Console.WriteLine("Евсеенко Виктория");
        for (int i = 0; i < ThreadCount; ++i)
        {
            object o = i;
            t[i] = new Thread(WorkThread);
            //if (i < 2) // первые 2 потока на низком приоритете
            //    t[i].Priority = ThreadPriority.Lowest;
            //else // остальные на высоком приоритете
            //    t[i].Priority = ThreadPriority.Highest;
            if (i < 2) // первые 2 потока на ниже нормальном приоритете
                t[i].Priority = ThreadPriority.BelowNormal;
            else // остальные на нормальном приоритете
                t[i].Priority = ThreadPriority.Normal;

            t[i].Start(o);
        }

        // Ожидаем завершения всех потоков
        foreach (var thread in t)
        {
            thread.Join();
        }
    }
}











//using System;
//using System.Diagnostics;
//using System.Threading;

//class Program
//{
//    const int ThreadCount = 8;
//    static Thread[] t = new Thread[ThreadCount];

//    static void WorkThread(object o)
//    {
//        int index = (int)o;
//        for (int i = 0; i < 5; i++)
//        {
//            Console.WriteLine($"Поток {index}: итерация {i}");
//            Thread.Sleep(100); // Имитация работы потока
//        }
//    }

//    static void Main()
//    {
//        // Установка маски процессоров (например, только для первых 4 процессоров)
//        IntPtr processorMask = new IntPtr(0x0F); // 00001111 (первые 4 процессора)

//        for (int i = 0; i < ThreadCount; ++i)
//        {
//            object o = i;
//            t[i] = new Thread(WorkThread);

//            // Установка приоритета потоков
//            if (i < 2) // Здесь 2 - половина логических процессоров
//                t[i].Priority = ThreadPriority.Lowest;
//            else
//                t[i].Priority = ThreadPriority.Highest;

//            // Установка маски процессора
//            t[i].Start(o);
//        }

//        // Установка маски для текущего процесса
//        Process.GetCurrentProcess().Threads[0].ProcessorAffinity = processorMask;

//        foreach (var thread in t)
//        {
//            thread.Join(); // Ожидание завершения всех потоков
//        }
//    }
//}
